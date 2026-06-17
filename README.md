<picture align="center">
  <img alt="KEYPOINT logo" src="/Picture/KEYPOINT.svg">
</picture>


<h1 align="center">K E Y P O I N T</h1>
KEYPOINT combines a 48-key split keyboard with MX switches with a built-in TrackPoint and trackpad, helping users stay focused by eliminating hand movement between keyboard and mouse.  

- [中文说明书](README_CN.md)
  
# Key Features
- Dual mouse (trackpoint, trackpad and opt. trackball)
- Two Encoders
- Two Ultra-Low-Power memory lcd display
- Aluminium case
- 24 keys per side
# Where to buy
If you are in China: [Taobao store](https://item.taobao.com/item.htm?abbucket=2&id=1056667585740&mi_id=00009nmeTcz5pMgG1HN1dVzE6N3oAPsuea6630xyzSAb-Xc&ns=1&skuId=6097163660045&spm=a21n57.1.hoverItem.1&utparam=%7B%22aplus_abtest%22%3A%22f7d87d1a9ebf9bed4f7868155d0390e9%22%7D&xxc=taobaoSearch)
# Image gallery
<p align="center">
<img width="800"  src="https://github.com/DZT970525/KEYPOINT/blob/main/Picture/Image_Gallery_1.jpg"/>
</p>

# Keymap

# Trackpoint and mouse layer

# Change trackpoint dpi on the fly

# How to connect this keyboard with your device [🔼](#contents)
- Turn on the Bluetooth on your phone
- Find the device ```KEYPOINT``` and pair with it then you should type with the keyboard
> [!CAUTION]
> After you delete the bluetooth profile and if you want to pair with the keyboard with another device when you have already paired with some device before, make sure to enter layer 1 and press the encoder on the left half keyboard to clear the bluetooth profile on the keyboard side. Otherwise there will be error when you want to pair with another device!!! Please use the following steps to delete the bluetooth pairing
- First delete the bluetooth profile(somewhere it's called forget the device or unpair the device) on your device
- Enter layer 1 and press the encoder on the left split keyboard 
- Refresh the Bluetooth setting page on your device(You can turn on and off the bluetooth) and then you can pair with the keyboard again

# Multi device connect
By default the keyboard can be paired with 4 devices at the same time and can be switched between them seamlessly. Here are the steps how to connect with devices:
1. Assume you have already connected this keyboard with one device
2. Now enter layer 1 and rotate the encoder on the left half keyboard and the lighted number will show you which bluetooth channel(which device) the keyboard is ready to be connected

Now the keyboard is ready to be paired with another device
Similarly, you can pair this keyboard with a third and a fourth device.  

4. Once pairing is done, you can switch between the devices the same way.

> [!CAUTION]
> - The BT_CLEAR keycoode — which is triggered by pressing the encoder on the left half keyboard — is calculated independently for each device. This means that when you trigger BT_CLEAR while connected to the second device, it won’t affect any of the other devices.

# Output select [🔼](#contents)
The keyboard can be set at USB output or Bluetooth output, it can be read on the left display.  
You can toggle the output by **pressing the right encoder at layer one by default**  
This output is important when you want to update your keymap via ZMK Studio.  
<table >
    <tr>
        <td><img src="https://github.com/DZT970525/KEYPOINT/blob/main/Picture/USB_Output_image.png"  style="width:300px;"/></td>
        <td><img src="https://github.com/DZT970525/KEYPOINT/blob/main/Picture/BLE_Output.png"  style="width:300px;"/></td>
    </tr>
</table>

> [!NOTE]
> - The USB output status will only show when the keyboard is connected with a host device through a cable

# Realtime Keymap Updating [🔼](#contents)
[ZMK Studio](https://zmk.dev/docs/features/studio) provides runtime update functionality to ZMK powered devices, allowing users to change their keymap layers without flashing new firmware to their keyboards.  

You can use ZMK Studio with ```Chrome/Edge``` at [https://zmk.studio/](https://zmk.studio/).  

Before you use ZMK Studio, you need to get the keyboard plugged with you computer and set the keyboard at ``USB output mode`` [Please refer to this content](https://github.com/DZT970525/KEYPOINT/tree/main#output-select-), then you can select the port shown on the ZMK Studio page and then start the keymap updating.  

The picture below shows you how it looks like when the keyboard connects with ZMK Studio successfully.  
<p align="center">
<img width="800"  src="https://github.com/DZT970525/KEYPOINT/blob/main/Picture/zmk_studio.png"/>
</p>

## Advanced Keymap Updating [🔼](#contents)
Since the ZMK Studio is still in beta stage, There’s also a more straightforward method to edit the keymap.  
Please refer to this [page](https://github.com/DZT970525/zmk-config-KEYPOINT)

# Firmware

# How to reset the keyboard

# Build your own Firmware

# Others

# Credits
