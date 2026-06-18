/*
 * A320 optical sensor driver
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT avago_a320

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>

#include <math.h>
#include <stdlib.h>

#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/events/hid_indicators_changed.h>
#include <zmk/hid.h>
#include <zmk/endpoints.h>

#include "trackpad_led.h"
#include "a320.h"

LOG_MODULE_REGISTER(a320, CONFIG_A320_LOG_LEVEL);

/* =========================
 * Config
 * ========================= */

#define ARROW_KEY_THRESHOLD 4
#define ARROW_RELEASE_DELAY_MS 300

#ifndef CONFIG_A320_POLL_INTERVAL_MS
#define CONFIG_A320_POLL_INTERVAL_MS 10
#endif

/* =========================
 * HID indicators
 * ========================= */

static zmk_hid_indicators_t current_indicators;
static bool last_capslock = false;

static int16_t sum_dx = 0, sum_dy = 0;
static uint8_t sample_cnt = 0;

#define HID_INDICATORS_CAPS_LOCK (1 << 1)

/* =========================
 * Scroll residual
 * ========================= */

static float scroll_residual_x = 0;
static float scroll_residual_y = 0;
static uint32_t last_read_time = 0;

/* =========================
 * Motion GPIO
 * ========================= */

#define MOTION_GPIO_NODE DT_NODELABEL(gpio0)
#define MOTION_GPIO_PIN 8

static const struct device *motion_gpio_dev;

/* =========================
 * State flags
 * ========================= */

static bool touched = false;
static bool ctrl_pressed = false;
static bool arrow_pressed = false;

/* Arrow accumulators */
static int dx_acc = 0;
static int dy_acc = 0;

static int window_dx = 0;
static int window_dy = 0;
static int64_t window_start = 0;

#define ARROW_WINDOW_MS 300

/* =========================
 * Device global (NEW)
 * ========================= */
static const struct device *a320_dev;

/* =========================
 * Arrow repeat work
 * ========================= */

static struct k_work_delayable arrow_repeat_work;

/* =========================================================
 * INPUT KEY SEND (REPLACED VERSION)
 * ========================================================= */

static inline void send_input_key(uint16_t key) {
    if (!a320_dev) {
        return;
    }

    input_report_key(a320_dev, key, 1, true, K_FOREVER);
    input_report_key(a320_dev, key, 0, true, K_FOREVER);
}

/* Arrow mapping */
static void trigger_left_arrow(void) { send_input_key(INPUT_BTN_1); }
static void trigger_right_arrow(void) { send_input_key(INPUT_BTN_0); }
static void trigger_up_arrow(void) { send_input_key(INPUT_BTN_3); }
static void trigger_down_arrow(void) { send_input_key(INPUT_BTN_2); }

/* =========================
 * Arrow repeat task
 * ========================= */

static void arrow_repeat_work_handler(struct k_work *work) {
    int64_t now = k_uptime_get();

    if (!arrow_pressed) {
        window_dx = 0;
        window_dy = 0;
        window_start = now;

        k_work_schedule(&arrow_repeat_work, K_MSEC(ARROW_RELEASE_DELAY_MS));
        return;
    }

    window_dx += dx_acc;
    window_dy += dy_acc;

    dx_acc = 0;
    dy_acc = 0;

    if (window_start == 0)
        window_start = now;

    if (now - window_start < ARROW_WINDOW_MS) {
        k_work_schedule(&arrow_repeat_work, K_MSEC(10));
        return;
    }

    int abs_dx = abs(window_dx);
    int abs_dy = abs(window_dy);

    if (abs_dx > abs_dy && abs_dx > ARROW_KEY_THRESHOLD) {

        if (window_dx > 0)
            trigger_right_arrow();
        else
            trigger_left_arrow();

    } else if (abs_dy > abs_dx && abs_dy > ARROW_KEY_THRESHOLD) {

        if (window_dy > 0)
            trigger_down_arrow();
        else
            trigger_up_arrow();
    }

    window_dx = 0;
    window_dy = 0;
    window_start = now;

    k_work_schedule(&arrow_repeat_work, K_MSEC(ARROW_RELEASE_DELAY_MS));
}

/* =========================
 * (rest unchanged)
 * ========================= */

struct a320_dev_config {
    struct i2c_dt_spec i2c;
};

typedef int (*a320_read_fn_t)(const struct device *dev, int16_t *dx, int16_t *dy);

struct a320_data {
    const struct device *dev;
    struct k_work_delayable poll_work;
    a320_read_fn_t read_motion;
};

/* =========================
 * Key listener
 * ========================= */

static int key_listener_cb(const zmk_event_t *eh) {
    const struct zmk_position_state_changed *ev = as_zmk_position_state_changed(eh);

    if (!ev)
        return 0;

    if (ev->position == 48 || ev->position == 49)
        ctrl_pressed = ev->state;

    if (ev->position == 20)
        arrow_pressed = ev->state;

    return 0;
}

ZMK_LISTENER(a320_key_listener, key_listener_cb);
ZMK_SUBSCRIPTION(a320_key_listener, zmk_position_state_changed);

/* =========================
 * HID indicator listener
 * ========================= */

static int hid_indicators_listener(const zmk_event_t *eh) {
    const struct zmk_hid_indicators_changed *ev = as_zmk_hid_indicators_changed(eh);

    if (ev)
        current_indicators = ev->indicators;

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(a320_hid_listener, hid_indicators_listener);
ZMK_SUBSCRIPTION(a320_hid_listener, zmk_hid_indicators_changed);

/* =========================
 * I2C read (unchanged)
 * ========================= */

static int a320_read_motion_3b(const struct device *dev, int16_t *dx, int16_t *dy) {
    const struct a320_dev_config *cfg = dev->config;

    uint8_t buf[3];
    uint8_t reg = 0x82;

    if (i2c_write_dt(&cfg->i2c, &reg, 1) < 0)
        return -EIO;

    if (i2c_burst_read_dt(&cfg->i2c, 0x82, buf, sizeof(buf)) < 0)
        return -EIO;

    *dx = (int8_t)buf[1];
    *dy = -(int8_t)buf[2];

    return 0;
}

static int a320_read_motion_37(const struct device *dev, int16_t *dx, int16_t *dy) {
    const struct a320_dev_config *cfg = dev->config;

    uint8_t buf[7];
    uint8_t reg = 0x0A;

    if (i2c_write_dt(&cfg->i2c, &reg, 1) < 0)
        return -EIO;

    if (i2c_burst_read_dt(&cfg->i2c, 0x0A, buf, sizeof(buf)) < 0)
        return -EIO;

    *dx = -(int8_t)buf[1];
    *dy = (int8_t)buf[3];

    return 0;
}

/* =========================
 * Poll work (UNCHANGED except arrow already removed)
 * ========================= */

static void a320_poll_work_handler(struct k_work *work) {
    struct k_work_delayable *dwork = CONTAINER_OF(work, struct k_work_delayable, work);

    struct a320_data *data = CONTAINER_OF(dwork, struct a320_data, poll_work);

    const struct device *dev = data->dev;

    int pin_state = gpio_pin_get(motion_gpio_dev, MOTION_GPIO_PIN);

    bool capslock = current_indicators & HID_INDICATORS_CAPS_LOCK;

    if (last_capslock && !capslock) {
        sum_dx = 0;
        sum_dy = 0;
        sample_cnt = 0;

        scroll_residual_x = 0;
        scroll_residual_y = 0;
        last_read_time = 0;
    }

    last_capslock = capslock;

    if (pin_state == 0) {

        int16_t dx = 0, dy = 0;

        if (data->read_motion(data->dev, &dx, &dy) == 0 && (dx || dy)) {

            if (arrow_pressed) {
                dx_acc += dx;
                dy_acc += dy;
                goto reschedule;
            }

            if (capslock || ctrl_pressed) {

                uint32_t now = k_uptime_get_32();

                if (now - last_read_time > 60) {
                    scroll_residual_x = 0;
                    scroll_residual_y = 0;
                }

                last_read_time = now;

                float speed = sqrtf((float)(dx * dx + dy * dy));

                float scale = (speed > 80)   ? 0.05f
                              : (speed > 40) ? 0.04f
                              : (speed > 20) ? 0.03f
                              : (speed > 5)  ? 0.02f
                                             : 0.015f;

                scroll_residual_x += dx * scale;
                scroll_residual_y += dy * scale;

                int16_t out_x = (int16_t)scroll_residual_x;
                int16_t out_y = (int16_t)scroll_residual_y;

                scroll_residual_x -= out_x;
                scroll_residual_y -= out_y;

                if (out_x || out_y) {
                    input_report_rel(dev, INPUT_REL_HWHEEL, out_x, false, K_FOREVER);
                    input_report_rel(dev, INPUT_REL_WHEEL, -out_y, true, K_FOREVER);
                }
            } else {

                scroll_residual_x = 0;
                scroll_residual_y = 0;

                uint8_t tp_led_brt = indicator_tp_get_last_valid_brightness();

                float tp_factor = 0.4f + 0.01f * tp_led_brt;

                dx = dx * 1 * tp_factor;
                dy = dy * 1 * tp_factor;

                input_report_rel(dev, INPUT_REL_X, dx, false, K_FOREVER);
                input_report_rel(dev, INPUT_REL_Y, dy, true, K_FOREVER);

                touched = true;
            }
        }

    } else {
        touched = false;
    }

reschedule:
    k_work_reschedule(&data->poll_work, K_MSEC(CONFIG_A320_POLL_INTERVAL_MS));
}

bool tp_is_touched(void) { return touched; }

/* =========================
 * INIT (add dev global)
 * ========================= */

static int a320_init(const struct device *dev) {
    const struct a320_dev_config *cfg = dev->config;
    struct a320_data *data = dev->data;

    a320_dev = dev; /* IMPORTANT */

    if (!device_is_ready(cfg->i2c.bus))
        return -ENODEV;

    motion_gpio_dev = DEVICE_DT_GET(MOTION_GPIO_NODE);

    if (!device_is_ready(motion_gpio_dev))
        return -ENODEV;

    gpio_pin_configure(motion_gpio_dev, MOTION_GPIO_PIN, GPIO_INPUT | GPIO_PULL_UP);

    uint8_t test_reg;
    struct i2c_dt_spec scan_spec = cfg->i2c;

    int found_addr = 0;
    const uint8_t candidates[] = {0x3B, 0x37};

    for (int i = 0; i < ARRAY_SIZE(candidates); i++) {
        scan_spec.addr = candidates[i];

        if (i2c_read_dt(&scan_spec, &test_reg, 1) == 0) {
            found_addr = candidates[i];
            break;
        }
    }

    if (!found_addr) {
        found_addr = 0x37;
        LOG_WRN("A320 I2C not detected, fallback 0x37");
    } else {
        LOG_INF("A320 detected at 0x%02X", found_addr);
    }

    ((struct a320_dev_config *)cfg)->i2c.addr = found_addr;

    if (found_addr == 0x3B)
        data->read_motion = a320_read_motion_3b;
    else
        data->read_motion = a320_read_motion_37;

    data->dev = dev;

    k_work_init_delayable(&data->poll_work, a320_poll_work_handler);
    k_work_init_delayable(&arrow_repeat_work, arrow_repeat_work_handler);

    k_work_schedule(&data->poll_work, K_MSEC(CONFIG_A320_POLL_INTERVAL_MS));

    k_work_schedule(&arrow_repeat_work, K_MSEC(ARROW_RELEASE_DELAY_MS));

    LOG_INF("A320 init OK");

    return 0;
}

/* =========================
 * DEVICE
 * ========================= */

#define A320_INIT_PRIORITY CONFIG_INPUT_A320_INIT_PRIORITY

#define A320_DEFINE(inst)                                                                          \
    static struct a320_data a320_data_##inst;                                                      \
    static struct a320_dev_config a320_cfg_##inst = {                                              \
        .i2c = I2C_DT_SPEC_INST_GET(inst),                                                         \
    };                                                                                             \
    DEVICE_DT_INST_DEFINE(inst, a320_init, NULL, &a320_data_##inst, &a320_cfg_##inst, POST_KERNEL, \
                          A320_INIT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(A320_DEFINE)