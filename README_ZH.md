<!--
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-01-15 20:09:22
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-02-12 17:40:12
 * @FilePath: /SleepPanda/README_ZH.md
 * @Description: 
 * Copyright (c) 2023 by Chengsen Dong 1034029664@qq.com(www.github.com/xddcore), All Rights Reserved. 
-->
# SleepPanda，一个贴心睡眠管家

![Logo](./img/Sleep_Panda_LOGO.png)  

[![Open Source Love](https://badges.frapsoft.com/os/v2/open-source.svg?v=103)](https://github.com/ellerbrock/open-source-badges/)  

![GPL V3 Licence](https://img.shields.io/github/license/xddcore/SleepPanda)

![Youtube](https://img.shields.io/youtube/channel/subscribers/UCP2-6ywKLy2js1dzJQ5hR_g?style=social)

>[点我访问Youtube频道](https://www.youtube.com/@SleepPanda666)


Change README Language: [English](./README.md) ｜ [中文](./README_ZH.md)

## Auto Unit Test
|  Source   | Status  |
|  ----  | ----  |
| helloworld.cpp  | ![Helloworld cpp test](https://img.shields.io/github/actions/workflow/status/xddcore/SleepPanda/helloworld_action.yml) |
| rpi4b-qemu-env  | ![rpi4b-qemu-env](https://img.shields.io/github/actions/workflow/status/xddcore/SleepPanda/rpi4b-qemu_action.yml) |
| . | . |
| .  | . |


## 项目简介
SleepPanda是一个以树莓派4b(bcm2711)为核心的睡眠监测系统。SleepPanda使用多种传感器收集用户的睡眠数据，并通过dsp，卷积神经网络等算法对数据进行分析处理。最终将数据处理结果通过多种方式呈现给用户。

## 项目特性
1. 早睡提醒，起床闹钟
2. 体温(热成像)，心率，血氧监测
3. 睡姿监测
4. 呼吸暂停综合症预警
5. 浅度/深度睡眠分类
6. 助眠音乐播放
7. 分享与社交(不同的SleepPanda硬件将会被连接至同一服务器，这将有助于用户间分享睡眠数据，以及互相监督早睡计划)

## 项目贡献者
|  Name   | ID  |  Role  |
|  ----  | ----  | ----  |
| Chengsen Dong  | 2789825d | 嵌入式工程师 |
| Rui Liu  | 2756138L | 项目管理&宣传&软件工程师 |
| Hui Wang  | 2810919W | 硬件工程师&测试工程师 |
| Yihan Wang  | 2696441W | 项目管理&宣传&软件工程师 |
>如果你对本项目有任何建议，欢迎发邮件至xddcore@gmail.com

## 项目开发规划

>[点我查看开发会议纪要](./doc/meeting/)

#### Global
- [x] 构建Github仓库&README文档，完成基础开发环境配置，固定Linux内核源码，测试内核模块编译。 
- [x] 建立dev分支
- [x] 购买所有硬件(除触摸屏以外)
- [x] Pitch Session PPT幻灯片初稿
- [x] Pitch Session PPT幻灯片最终稿
- [x] 14:20:00, 25/01/2023 Pitch Session

#### Chengsen Dong
- [ ] 更新README文档(随着开发过程同步更新)
- [x] 蜂鸣器驱动开发
- [ ] Sound Sensor驱动开发
- [ ] MAX30101驱动开发
- [ ] MLX90640驱动开发
- [ ] Tensorflow Lite神经网络推理框架(C++版本)
- [ ] MLX90640+卷积神经网络手势识别
- [ ] WM8960驱动开发(低优先级)
- [ ] 墨水屏驱动开发(虚拟动物园)
- [ ] 4K 30FPS摄像头(opencv c++框架)
- [ ] 卷积神经网络睡姿分类
- [ ] 触摸屏(基于QT的GUI)开发
- [ ] MQTT服务器搭建(低优先级)

#### Yihan Wang
- [ ] 同步更新README_ZH.md&README.md的内容(每周一次)
- [x] 创建Youtube频道，用于推广SleepPanda项目
- [x] Pitch Session PPT幻灯片初稿
- [x] 制作成本核算&原件选行Excel表格
- [ ] To do later...

#### Rui Liu
- [ ] 同步更新README_ZH.md&README.md的内容(每周一次)
- [x] 同步更新会议记录的内容(每周一次)
- [x] 设计SleepPanda Logo
- [x] Pitch Session PPT幻灯片终稿&pitch session演讲准备
- [x] 制作成本核算&原件选行Excel表格
- [ ] To do later...

#### Hui Wang
- [x] 查询所有传感器/芯片的数据手册，并上传至github仓库
- [ ] To do later...

# 指南

## 1. 系统部署

### 1.1 软硬件版本

0. **C++11 Standard Template Library**
1. 树莓派4B(4GB/8GB)｜bcm2711
2. Linux发行版: Raspberry Pi OS(32bit)｜Ubuntu Desktop 22.04.01 LTS(64Bit)
3. 内核版本: Linux raspberrypi 5.15.84-v7l+ #1613|5.15.0-1023-raspi |检索所有Linux内核`apt-cache search linux-raspi-headers`
4. g++: `g++ version 10.2.1 20210110 (Raspbian 10.2.1-6+rpi1)`｜`g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0`
5. gcc: `gcc version 10.2.1 20210110 (Raspbian 10.2.1-6+rpi1)`｜`gcc (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0`


### 1.2 将树莓派脱离键鼠&显示器(For Ubuntu Desktop)
0. Windows电脑需要安装的软件:`1.Termius(用于SSH)` `2.Microsoft  Remote Desktop(用于远程桌面)3.Visual Studio code(万能编辑器)` `3.Visual Studio code(万能编辑器)` `4.Raspberry Pi Image(镜像烧录)` `5.Github Desktop(可选)`
1. 使用Raspberry Pi Image烧录镜像至SD卡(ubuntu镜像:`https://ubuntu.com/raspberry-pi/desktop`)
2. 使用外接显示器和键鼠完成ubuntu的初始化。系统语言English，创建用户名为`ubuntu`，密码为`ubuntu`。
3. 需要安装的软件包: ssh vim gcc g++ screen htop git make
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install ssh vim gcc g++ screen htop git make
```
>Note:注意`sudo apt-get upgrade`命令默认把内核升级为最新版本，请手动将内核版本保持至5.15.0-1023-raspi
4. 开启ubuntu远程桌面:Setting-share-开启远程桌面-修改用户名和密码为`ubuntu`

   4.1 允许hdmi热插拔，避免不插hdmi时无法访问远程桌面。｜`未解决！！！`|尝试使用HDMI伪负载解决  
   输入以下命令:`sudo vim /boot/firmware/config.txt`，并在config.txt文件中增加以下内容:  
   ```
   [hdmi]
   #allow remote desktop when no hdmi wire 
   hdmi_force_hotplug=1
   hdmi_drive=2 
   ```
   4.2固定远程桌面登陆密码，防止每次重启后密码随机生成无法访问远程桌面  
   Application-password&keys-默认密钥&login-change password-不设置密码(留空)-最后实现取消加密  
   4.3使用Microsoft Remote Desktop软件来访问树莓派桌面|树莓派的局域网IP+用户名和密码均为`ubuntu`
   >如何获得树莓派IP: 1.若在同一路由器下，可通过访问路由器后台获得。2.若无法访问路由器后台，则通过`arp -a(windows)`等方式获得树莓派的局域网IP。

5. 防止树莓派休眠:Setting-Power-Nerver sleep
6. 至此树莓派完全脱离键盘鼠标和显示器，可用其他电脑远程操作
7. 使用Termius软件SSH连接至树莓派的局域网IP。用户名:`ubuntu` 密码:`ubuntu`

### 1.3 将树莓派脱离键鼠&显示器(For Raspberry Pi OS)

0. Windows电脑需要安装的软件:`1.Termius(用于SSH)` `2.VNC Viewer(用于远程桌面,下载链接https://www.realvnc.com/en/connect/download/viewer/)` `3.Visual Studio code(万能编辑器)` `4.Raspberry Pi Image(镜像烧录) ` `5.Github Desktop(可选)`
1. 使用Raspberry Pi Image烧录`Raspberry Pi OS(32bit)`镜像至SD卡(Raspberry Pi OS镜像:`https://www.raspberrypi.com/software/operating-systems/`)
>在烧录之前，请先点击右下角齿轮按钮，进行如下配置:
> 1. 勾选设置主机名->raspberrypi.local
> 2. 勾选开启SSH服务->使用密码登录
> 3. 勾选Set username and password -> Username: pi | Password: pi
> 4. 点击保存
2. 将树莓派连接上电源，启动树莓派。等待一会儿后，通过`termius`软件的SSH连接上树莓派。
3. 输入`sudo raspi-config`命令，使用**方向键**进行如下配置:
> 1. 设置启动选项为自动登录桌面。`System Options -> Boot/Auto Login -> Desktop Autologin`
> 2. 打开VNC远程桌面。`Interface Options -> VNC - > YES`
> 3. 打开SPI端口。`Interface Options -> SPI - > YES`
> 4. 打开IIC端口。`Interface Options -> I2C - > YES`
> 5. 选择Finish保存设置。
4. 输入`reboot`命令重启树莓派。(执行完此步骤后需要等待一会儿)
5. 此时，树莓派已经可以通过`VNC Viewer`远程桌面进行访问。
>树莓派上对应的VNC软件包为 `realvnc-vnc-server`（Raspberry Pi OS已自带，不用安装)
6. 需要安装的软件包: ssh vim gcc g++ screen htop git make,执行以下命令:
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install ssh vim gcc g++ screen htop git make
```
4. 输入`reboot`命令重启树莓派。至此，已完成基础环境配置。

### 1.4 构建交叉编译环境&本地编译环境
考虑到树莓派算力不足，而使用服务器将会显著提升编译效率，版本控制，代码备份，协同工作。我们在本项目开发过程中使用一台中心化服务器(`I9-12900k+RTX3090TI+32GB DDR4+512G SSD`)，并在服务器中构建交叉编译环境。

#### 1.4.1 交叉编译环境(服务器)

1. 安装aarch64-linux-gnu-交叉编译工具链，以及其他编译linux内核所需的编译工具
2. 获取树莓派内核源码
```
apt-get source linux-image-$(uname -r) #5.15.0-1023-raspi
```
3. 编译内核源码
>Ref:https://www.raspberrypi.com/documentation/computers/linux_kernel.html
4. 编译`helloworld`模块代码
5. 测试`helloworld`模块

![helloworld_modinfo](./img/helloworld_modinfo.png)

#### 1.4.2 本地编译环境(树莓派)

1. 编译`helloworld`模块代码

```
make -C /usr/src/linux-headers-5.15.0-1023-raspi/ M=`pwd` modules
```
2. 测试`helloworld`模块
>测试结果同上

---

Q1:模块未签名问题:
>看起来你的系统供应商已经在你的内核上启用了内核模块签名验证，这意味着它不会加载供应商尚未签名的任何模块。换句话说，你的补丁模块没有（正确地）签名并且内核将拒绝加载它。
>解决方案:cmd run `CONFIG_MODULE_SIG=n`

Q2:执行`sudo apt-get upgrade`时，某些pack(例如linux内核)被保留，无法升级。
> `sudo apt-get --with-new-pkgs upgrade`


### 1.5 传感器&执行器&服务器

>[点我下载BOM](./doc/BOM/SleepPanda_BOM.xlsx)

>[点我下载datasheet](./doc/datasheet/)

**传感器**
1. 扬声器&麦克风:WM8960(Control:IIC;Audio:IIS)+Sound Sensor(电压比较器Dout)-✅
2. 体温(热成像)&手势控制:MLX90640(IIC)-✅
3. 心率&血氧:MAX30101(IIC)-✅
4. 睡姿监测:4K 30FPS摄像头(USB+OpenCV)-✅

**执行器**
1. 墨水屏：SSD1608(虚拟动物园)(SPI)-✅
2. 触摸屏(基于QT的GUI)(Video:HDMI,Touch:USB)
3. 蜂鸣器:紧急异常报警(High/Low Pin Level)-✅

**其他**

1. 杜邦线若干-✅


**服务器**
面向全球，负责存储用户数据，并承担虚拟动物园之间的参观访问/动物交换。


## 2. 系统开发

### 2.1 获取SleepPanda源码

`git clone https://github.com/xddcore/SleepPanda.git`

**Note:如果你是开发人员，记得通过以下命令切换到dev分支:**
```
git checkout dev
git branch -l
```

### 2.2 cpp单元测试框架

#### 2.2.1 cppunit

```
#install cppunit lib
sudo apt install -y libcppunit-dev
```
>#include <cppunit/TestRunner.h>

>因为:
>1. google test使用比较简单，写test case不用分为声明和定义两部分，也不用自己注册test suite.
>2. google test的assert更加丰富
>所以本项目采用google test(gtest)进行单元测试

#### 2.2.2 google test(gtest)
##### 2.2.2.1 直接加载编译好的动态链接库文件

###### 2.2.2.1.1 For Ubuntu Desktop 22.04 64bit

gtest动态链接库路径`SleepPanda/tools/gtest/ubuntu22.04_64bit/lib/`, gtest头文件路径`SleepPanda/tools/gtest/ubuntu22.04_64bit/include/`

```
# g++ build demo

# cd to workscape
cd ./SleepPanda/src/app/gtest_demo

# build code
g++ -std=c++14 ./gtest_demo.cpp -I ../../../tools/gtest/ubuntu22.04_64bit/include/ ../../../tools/gtest/ubuntu22.04_64bit/lib/libgtest.so -lpthread -o gtest_demo

# Export the gtest dynamic link library to the system environment variable (temporary)
export LD_LIBRARY_PATH=../../../tools/gtest/ubuntu22.04_64bit/lib/:$LD_LIBRARY_PATH

# run gtest demo
./gtest_demo
```

###### 2.2.2.1.2 For Raspberry Pi OS 32bit**

gtest动态链接库路径`SleepPanda/tools/gtest/rpios_32bit/lib/`, gtest头文件路径`SleepPanda/tools/gtest/rpios_32bit/include/`

* 使用g++进行编译(两种编译方式二选一，也可都尝试一下)
```
# g++ build demo

# cd to workscape
cd ./SleepPanda/src/app/gtest_demo

# build code
g++ -std=c++14 ./gtest_demo.cpp -I ../../../tools/gtest/rpios_32bit/include/ ../../../tools/gtest/rpios_32bit/lib/libgtest.so -lpthread -o gtest_demo

# Export the gtest dynamic link library to the system environment variable (temporary)
export LD_LIBRARY_PATH=../../../tools/gtest/rpios_32bit/lib/:$LD_LIBRARY_PATH

# run gtest demo
./gtest_demo
```

* 使用cmake进行编译(两种编译方式二选一，也可都尝试一下)
```
# g++ build demo

# cd to workscape
cd ./SleepPanda/src/app/gtest_demo/build

# build code
cmake .. && make

# Export the gtest dynamic link library to the system environment variable (temporary)
export LD_LIBRARY_PATH=../../../tools/gtest/rpios_32bit/lib/:$LD_LIBRARY_PATH

# run gtest demo
./gtest_demo
```
**Error:在使用cmake编译时，可能会出现如下报错:**
```
CMake Error: The source "/home/pi/xddcore/SleepPanda/src/app/gtest_demo/CMakeLists.txt" does not match the source "/home/ubuntu/xddcore/SleepPanda/src/app/gtest_demo/CMakeLists.txt" used to generate cache.  Re-run cmake with a different source directory.
```
**解决办法:**
```
# remove /build dir
rm -rf build/

# create new /build dir
mkdir build

# go to build dir
cd build

# build code
cmake .. && make

# run gtest demo code
cd .. && ./gtest_demo

```

理论上，你将获得如下运行结果:
```
./gtest_demo
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from FooTest
[ RUN      ] FooTest.test_add
[       OK ] FooTest.test_add (0 ms)
[ RUN      ] FooTest.test_minus
./gtest_demo.cpp:54: Failure
Expected equality of these values:
  foo->GetNum()
    Which is: 1
  0
[  FAILED  ] FooTest.test_minus (0 ms)
[----------] 2 tests from FooTest (1 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (2 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] FooTest.test_minus
```

##### 2.2.2.2 自己通过google test源码编译动态链接库文件
```
# install dependencies
sudo apt-get install cmake

# install google test lib

# get source
git clone https://github.com/google/googletest.git

# build
cd googletest
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/install -DBUILD_SHARED_LIBS=ON ..
make -j8
make install

# Then the corresponding dynamic library of gtest will be generated in the build/install directory
ls install/
# include lib

```
### 2.3 软件架构

![Software_Architecture](./img/Software_Architecture.png) 

|Index| 层|-|注释|
|:----:|:----:|:----:|:----:|
|7(Top)|GUI界面层(QT)|-|与用户交互|
|6|C++逻辑层|-|Opencv，TensorFlow Lite，MQTT Client等|
|5|硬件驱动层(Sensor Class)|-|硬件(传感器等)的配置/驱动|
|4|硬件隔离层(Rpi4b Class)|-|隔离软件逻辑和硬件依赖，目的是为了可以抛开底层硬件依赖(比如pigpio)，单独运行/测试上层代码。|
|3|pigpiod嵌套字通信进程|-|驱动rpi4b(bcm2711)寄存器|
|2|系统调用|-|由Linux内核提供|
|1(Bottom)|Linux内核模块|-|分配中断|

### 2.4 传感器&执行器驱动开发

>Ref:  
>https://berndporr.github.io/realtime_cpp_coding/  
>https://github.com/berndporr/realtime_cpp_coding  

**在开始之前**，你需要执行以下命令，以安装必须的依赖。在此，对pigpio库(https://github.com/joan2937/pigpio)的开发者表示由衷的感谢。

**Raspberry Pi OS安装方法**

运行如下命令，即可一键安装piopig

```
sudo apt-get install libpigpio-dev
```

**其他操作系统安装方法**

```
apt-get install cmake
# Download and install latest version(pigpio)
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install

#If the Python part of the install fails it may be because you need the setup tools.
sudo apt install python-setuptools python3-setuptools
```

最后，为了检验pigpio lib是否被正确安装，我们还**建议您运行如下测试命令**:
```
cd pigpio-master #(if you are not in this dir)
sudo ./x_pigpio # check C I/F

# To start the pigpio daemon
sudo pigpiod
./x_pigpiod_if2 # check C      I/F to daemon
# Then you should get PASS

# To stop the pigpio daemon
sudo killall pigpiod
./x_pigpiod_if2 # check C      I/F to daemon
# Then you should get 'pigpio initialisation failed (-2003).', because pigpio daemon not runing.
```
>Note: 在每次使用pigpio时，你都应先运行`sudo pigpiod`，以打开守护程序。

理论上，你将获得如下运行结果:
```
Testing pigpio C I/F
pigpio version 79.
Hardware revision 13644052.
Mode/PUD/read/write tests.
TEST  1.1  PASS (set mode, get mode: 0)
TEST  1.2  PASS (set pull up down, read: 1)
TEST  1.3  PASS (set pull up down, read: 0)
TEST  1.4  PASS (write, get mode: 1)
TEST  1.5  PASS (read: 0)
TEST  1.6  PASS (write, read: 1)
PWM dutycycle/range/frequency tests.
TEST  2.1  PASS (set PWM range, set/get PWM frequency: 10)
TEST  2.2  PASS (get PWM dutycycle: 0)
TEST  2.3  PASS (set PWM dutycycle, callback: 0)
TEST  2.4  PASS (get PWM dutycycle: 12

...etc
```

---
Q1: ERROR: **initAllocDMAMem: mbox open failed(No such device or address)**
```
:~$ sudo pigpiod
OR
:~$ ./x_pigpio
Then get
:~$ initAllocDMAMem: mbox open failed(No such device or address)
```
Fixup: Run`sudo modprobe vcio`
OR
`./x_pigpio`是一个使用mailbox分配内存的程序。如果在某种情况下(例如qemu env),你将不会有GPU内存可供分配。pigpio会使用mailbox分配DMA内存(除非您请求了一个大缓冲区)。使用使用`-a1`选项启动pigpiod时，会使用页面映射来分配DMA内存。
```
sudo pigpiod -a1 #force use PMAP allocate DMA memory
./x_pigpiod_if2 # check C      I/F to daemon
```

Q2: ERROR: **modprobe: FATAL: Module vcio not found in directory**
```
:~$ sudo modprobe vcio
:~$ modprobe: FATAL: Module vcio not found in directory /lib/modules/5.15.0-1031-azure
```
Fixup: Run`sudo apt-get install -y linux-modules-extra-$(uname -r)`  
OR
`You need install Ubuntu Desktop Image, and then you will have vcio(videocore io)`

Q3: ERROR: **initInitialise: Can't lock /var/run/pigpio.pid**
```
:~$ sudo ./x_pigpio
:~$ initInitialise: Can't lock /var/run/pigpio.pid
pigpio initialisation failed.
```
Fixup: Run`sudo killall pigpiod`

Q4: 在QEMU环境下，pigpio无法仿真
```
2023-01-27 09:02:23 initAllocDMAMem: mbox open failed(No such device or address)
Can't initialise pigpio library
pigpio initialisation failed (-2003).
```
Fixup: 因为pigpio依赖BCM2711芯片硬件功能实现超低延迟的DMA操作，而qemu的DMA无法支持这一操作，所以在QEMU环境下，无法完成pigpio仿真。

#### 2.4.1 蜂鸣器


