# KEYPOINT说明书

千人QQ大群：908283136 可以加入Q群进行售后和交流  

Keypoint分体键盘固件基于ZMK开源固件 可以使用ZMK STUDIO网页或者Github action进行在线固件编译来修改键位和实现自己想要的功能。  

这是一把配置拉满的一把分体键盘，将触控板和指点杆融入键盘来实现解放双手的功能。  

## 键盘基本特征  

**自带双鼠标** ：该分体键盘左右两边各自带着鼠标指针设备 默认左边为黑莓触控板 右边为小红点指点杆。通过模块化设计可以左右手互换或者上两个小红点 还可以替换成黑莓轨迹球模块。
指点杆通过力学按压来操作鼠标 在有力的情况下鼠标可以一直移动 黑莓触控板则是根据相对滑动来输出鼠标移动的 两者的移动方式可以完成完美互补 比如小红点快速定位 还差一点距离 可以通过黑莓触控板进行位置的微调。

**双屏幕设计**：分体键盘的两边都带有0.96寸144x72像素的memory lcd超级低功耗屏幕，可以显示打字速度、连接装填、当前层或者播放gif动画的功能。<u>该屏幕不带背光 为全反射屏幕 环境光亮度越高 该屏幕看的越清晰</u>

**双旋钮设计**：左右手自带一个旋钮，可以调节音量、移动光标、放大和缩小 或者切换蓝牙配对设备等功能。  

**铝合金外壳**：键盘全系标配铝合金外壳 一套键盘一共四个铝合金件 外加一些3d打印修饰件来保证两边的无线连接信号强度。

**磁吸生态**：键盘底座下面放置有兼容magsafe生态的磁铁 可以搭配其他磁吸配件固定在桌子边上或者人体工学椅上来实现躺着办公。

**长续航**：每一边都配有750mah电池，基本情况下续航15-20天无压力。 

**其他一些特点**：小红点dpi可以在线调节 一共十档。

## 下面介绍具体用法：  

**分体键盘基本知识：**

分体键盘是分主手和副手的 副手和主手之间是采用2.4G协议通信的 **副手只负责给主手发送副手端哪个按键被按下以及鼠标的移动信息；**  
主手负责接收副手的按键信息并且通过键盘当前的层级状态来向主机发送输出内容。副手是不知道键盘处于哪个层的，在这个分体键盘中：默认固件下左手是主手；  
右手是副手。键盘的连接状态以及当前层的信息在主手的屏幕上显示，**如果想要先有线测试键盘，拿数据线连接左手然后把两个键盘开机就可以了。**

**默认固件键位图：**
键盘默认固件有四个层：一个QWER层，也就是层0，然后层1，层2，层3和层4。 其中层3是鼠标层 层4是预留层。当检测到通过小红点或者触控板进行鼠标移动的时候  键盘会自动进入鼠标层 并且在没有鼠标移动的600毫秒后自动退回默认层0。

### 层0键位图：

<p align="center">
<img width="800"  alt="image" src="https://github.com/user-attachments/assets/9f12002a-7832-49e6-ba8c-af5ed7002eac" />
</p>

### 层1键位图：

<p align="center">
<img width="800"  alt="image" src="https://github.com/user-attachments/assets/97947685-d77d-4fd5-a4f3-cf3e6b181bc2" />
</p>

### 层2键位图：
<p align="center">
<img width="800"  alt="image" src="https://github.com/user-attachments/assets/5fb5a9d1-442b-4809-9c48-ea10e44018f0" />
</p>

### 层3鼠标层键位图：
<p align="center">
<img width="800"  alt="image" src="https://github.com/user-attachments/assets/f0366dec-0d71-4d4a-9b80-729931d35190" />
</p>

### 层4预留层键位图：
<p align="center">
<img width="800" alt="image" src="https://github.com/user-attachments/assets/63ec0a5d-42fe-45ff-a47b-2159db1223f5" />
</p>


## 如何改键：
选择1：电脑用数据线连接左边键盘 **关掉和键盘连接着的设备的蓝牙** 比如手机，然后打开网页 [https://zmk.studio/](https://zmk.studio) 选择对应的连接COM口 可以进行改键

选择2：利用github action在线编译，首先确保你的电脑能翻墙 然后打开这个[github网页](https://github.com/DZT970525/zmk-config-KEYPOINT)，右上角 fork一下
<p align="center">
<img width="600" " alt="image" src="https://github.com/user-attachments/assets/697b5ec1-e54f-4f3c-98d1-1ed1dc777235" />
</p>

然后打开[keymap编辑网页](https://nickcoutsos.github.io/keymap-editor) 授权登录github后 可以直接可视化修改固件，然后修改完在你的github action 部分可以下载到编译好的固件
<p align="center">
<img width="600"  alt="image" src="https://github.com/user-attachments/assets/9e53d633-93fa-48e8-8580-5e18769c88c6" />
</p>
然后对键盘进行烧录 就更新好键位了。如何烧录看下面的教程。

## 如何刷新固件：

方法1：首先要用数据线把你的键盘连接到你的电脑上
默认固件下开机后进层1按一下旋钮旁边的位置  也就是这个按键
<p align="center">
<img width="400" alt="image" src="https://github.com/user-attachments/assets/d880185d-fe0a-4a42-b436-f1739a48b041" />
</p>

键盘就会进入烧录状态 你的电脑会发现一个新的U盘 名字叫nicenano，把新的.uf2固件烧录到键盘端 过大概五秒烧录会完成 U盘会消失 如果报错说找不到U盘了 不用怕 那说明烧录完成了。注意左边的固件只对左边的键盘使用，右边的固件只对右边的键盘使用。  
**注意：右手键盘只在和主手完成无线配对连接时能用这个方法进入烧录模式。**

**方法2：** 用数据线把你的键盘和你的电脑连接起来
开机后 用镊子或者什么东西双击这个小按钮 
<p align="center">
<img width="400" alt="image" src="https://github.com/user-attachments/assets/4e2ee17a-ba67-444b-9d69-ea26011d565e" />
</p>

这个小按钮是复位按钮 不受固件影响就可以进入烧录模式，双击时用鼠标双击的那种速度 然后你的电脑会发现一个新的U盘 把新的固件拖进这个U盘里 大概过五秒 固件会烧录完成。

## 如何取消和重新配对：
当你连接上一个设备后，如果想取消配对连接另一个设备 **键盘端也要删除这个配对，否则无法进行下一次配对。** 默认固件下需要进入层1往下按左边的旋钮来取消配对。**这个步骤很关键！**

## 多设备连接：
默认固件下 键盘可以同时和四个设备进行蓝牙连接
进入层一 旋转左边旋钮，可以切换你键盘所属的配对频道 同时你左手键盘的屏幕中下部分也会显示你当前配对的设备频道。
<p align="center">
<img width="250"  alt="image" src="https://github.com/user-attachments/assets/79c45a0b-294c-451f-8d1a-bdaaa2895626" />
</p>

## 如何重置键盘：
使用过程中可能碰到键盘断开某个设备后 不管怎么操作都连接不上的情况，这时候需要重置键盘的固件来清除一下之间的配对信息。需要做以下步骤：
比如左手键盘：
先给左手键盘进入烧录模式 然后把重置固件 
<img width="375" height="117" alt="image" src="https://github.com/user-attachments/assets/d152e220-bee7-463a-a887-8e65ac58c936" />

重置固件在群文件里
拖进弹出的名字叫nice！nano的硬盘里面，然后再让左手键盘进入烧录模式 
把正常固件拖进U盘里<img width="420" height="97" alt="image" src="https://github.com/user-attachments/assets/e62781a9-e54c-40f0-90be-a6b4613215a9" />
  这样左手键盘就重置完成了，同样的操作对右手键盘进行一次 这样左右手就完成了一次固件重置。

## 如何调节小红点dpi：‘
默认固件下 进入层1 再按右边键盘的左右两个小按键能进行小红点dpi的调节；
<p align="center">
<img width="300"  alt="image" src="https://github.com/user-attachments/assets/43c38334-928a-4dd9-9b92-eb456f31b0b9" />
</p>

## 如何让小红点和轨迹球进入滚轮模式：
当左边键盘按住最外侧拇指键的时候 轨迹球变成滚轮模式   
当右边键盘按住最外侧拇指键  
的时候 小红点变成滚轮模式  
<p align="center">
<img width="600" alt="image" src="https://github.com/user-attachments/assets/a54eb3c4-4991-4af2-8516-396d5b07c5d2" />
</p>

## 键盘充电指示灯：
键盘上那个电池icon下有个LED，当充满电的时候这个灯熄灭。
充电速度大概3-4小时能充满

## 关于休眠：
默认状态下 如果键盘没检测到任何输入或者鼠标移动，会在半小时后进入休眠
这时候随便按一个键 键盘就会从休眠状态中激活

## 键盘死机了怎么办：
99%情况下 重启键盘就可以让键盘恢复工作 如果没有 请联系群主
