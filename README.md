<!--
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-01-18 00:44:11
 * @LastEditors: Yihan Wang yihanwangg@163.com
 * @LastEditTime: 2023-02-15 13:09:57
 * @FilePath: /SleepPanda/README.md
 * @Description: 
 * Copyright (c) 2023 by Chengsen Dong 1034029664@qq.com(www.github.com/xddcore), All Rights Reserved. 
-->
# SleepPanda, a caring sleep manager

![Logo](./img/Sleep_Panda_LOGO.png)

[![Open Source Love](https://badges.frapsoft.com/os/v2/open-source.svg?v=103)](https://github.com/ellerbrock/open-source-badges/ )

![GPL V3 License](https://img.shields.io/github/license/xddcore/SleepPanda)

![Youtube](https://img.shields.io/youtube/channel/subscribers/UCP2-6ywKLy2js1dzJQ5hR_g?style=social)

>[Click me to visit the Youtube channel](https://www.youtube.com/@SleepPanda666)


Change README Language: [English](./README.md) ｜ [Chinese](./README_ZH.md)

## Auto Unit Test
| Source | Status |
| ---- | ---- |
| helloworld.cpp | ![Helloworld cpp test](https://img.shields.io/github/actions/workflow/status/xddcore/SleepPanda/helloworld_action.yml) |
| rpi4b-qemu-env | ![rpi4b-qemu-env](https://img.shields.io/github/actions/workflow/status/xddcore/SleepPanda/rpi4b-qemu_action.yml) |
| . | . |
| . | . |


## Project Description
SleepPanda is a sleep monitoring system based on Raspberry Pi 4b (bcm2711). SleepPanda uses a variety of sensors to collect sleep data of users, and analyzes and processes the data through dsp, convolutional neural network and other algorithms. Finally, the data processing results are presented to users in various ways.

## Project Features
1. Early bed reminder, wake up alarm clock
2. Body temperature (thermal imaging), heart rate, blood oxygen monitoring
3. Sleep position monitoring
4. Early warning of apnea syndrome
5. Light/deep sleep classification
6. Playing sleep aid music
7. Sharing and social networking (different SleepPanda hardware will be connected to the same server, which will help users share sleep data and monitor each other's early bedtime plans)

## Project Contributors
| Name | ID | Role |
| ---- | ---- | ---- |
| Chengsen Dong | 2789825d | Embedded Engineer |
| Rui Liu | 2756138L | Project Management & Publicity & Software Engineer |
| Hui Wang | 2810919W | Hardware Engineer & Test Engineer |
| Yihan Wang | 2696441W | Project Management & Publicity & Software Engineer |
> If you have any suggestions for this project, please email to xddcore@gmail.com

## Project Development Planning

>[Click here to view the minutes of the development meeting](./doc/meeting/)

#### Global
- [x] Build the Github warehouse & README document, complete the basic development environment configuration, fix the Linux kernel source code, and test the compilation of the kernel module.
- [x] build dev branch
- [x] Buy all hardware (except touchscreen)
- [x] Pitch Session PPT slide draft
- [x] Final draft of Pitch Session PPT slides
- [x] 14:20:00, 25/01/2023 Pitch Session

#### Chengsen Dong
- [ ] Update the README document (synchronized with the development process)
- [x] Buzzer driver development
- [ ] MAX30101 driver development
- [ ] MLX90640 driver development
- [ ] Tensorflow Lite neural network reasoning framework (C++ version)
- [ ] MLX90640+ Convolutional Neural Network Gesture Recognition
- [ ] WM8960 driver development (low priority)
- [ ] ink screen driver development (virtual zoo)
- [ ] 4K 30FPS camera (opencv c++ framework)
- [ ] Convolutional Neural Network Sleeping Position Classification
- [ ] Touch screen (QT-based GUI) development
- [ ] MQTT server setup (low priority)

#### Yihan Wang
- [ ] Synchronously update the contents of README_ZH.md & README.md (once a week)
- [ ] Simultaneous updates of the minutesmeeting (once a week)
- [x] Creation of a Youtube channel to promote the SleepPanda project
- [x] Pitch Session PPT slide draft
- [x] Make cost accounting & original row selection Excel table
- [x] Sound Sensor Driver Development
- [ ] To do later...

#### Rui Liu
- [ ] Synchronously update the contents of README_ZH.md & README.md (once a week)
- [ ] Simultaneous updates of the minutesmeeting (once a week)
- [ ] Create Gantt charts to organise tasks according to the classification of people
- [x] Design SleepPanda Logo
- [x] Pitch Session PPT slide final draft & pitch session speech preparation
- [x] Make cost accounting & original row selection Excel table
- [ ] To do later...

#### Hui Wang
- [x] Query the data sheets of all sensors/chips and upload them to the github repository
- [ ] To do later...

# guide

## 1. System deployment

### 1.1 Software and hardware version

0. **C++11 Standard Template Library**
1. Raspberry Pi 4B (4GB/8GB)｜bcm2711
2. Linux distribution: Ubuntu Desktop 22.04.01 LTS (64Bit)
3. Kernel version: 5.15.0-1023-raspi | Retrieve all Linux kernel `apt-cache search linux-raspi-headers`
4. g++: `g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0`
5. gcc: `gcc (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0`


### 1.2 Separate Raspberry Pi from keyboard, mouse & monitor
0. Software that needs to be installed on a Windows computer: `1.Termius (for SSH)` `2.Microsoft Remote Desktop (for remote desktop) 3.Visual Studio code (universal editor)`
1. Use Raspberry Pi Image to burn the image to SD card (ubuntu image: `https://ubuntu.com/raspberry-pi/desktop`)
2. Use an external monitor, keyboard and mouse to complete the initialization of ubuntu. The system language is English, the created user name is `ubuntu`, and the password is `ubuntu`.
3. Packages to be installed: ssh vim gcc g++ screen htop git make
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install ssh vim gcc g++ screen htop git make
```
>Note: Note that the `sudo apt-get upgrade` command will upgrade the kernel to the latest version by default, please manually keep the kernel version to 5.15.0-1023-raspi
4. Open ubuntu remote desktop: Setting-share-open remote desktop-change username and password to `ubuntu`

    4.1 Allow hdmi hot plugging to avoid inability to access remote desktop when hdmi is not plugged in. | `Unresolved! ! ! `|Try to use HDMI dummy load to resolve
    Enter the following command: `sudo vim /boot/firmware/config.txt`, and add the following content to the config.txt file:
    ```
    [hdmi]
    #allow remote desktop when no hdmi wire
    hdmi_force_hotplug=1
    hdmi_drive=2
    ```
    4.2 Fix the remote desktop login password to prevent the password from being randomly generated after each restart and unable to access the remote desktop
    Application-password&keys-default key&login-change password-do not set a password (leave blank)-finally cancel encryption
    4.3 Use the Microsoft Remote Desktop software to access the Raspberry Pi desktop | Raspberry Pi's LAN IP + user name and password are `ubuntu`
    >How to get Raspberry Pi IP: 1. If it is under the same router, it can be obtained by accessing the background of the router. 2. If you cannot access the router backend, obtain the Raspberry Pi’s LAN IP through `arp -a(windows)` and other methods.

5. Prevent Raspberry Pi from sleeping: Setting-Power-Nerver sleep
6. At this point, the Raspberry Pi is completely separated from the keyboard, mouse and monitor, and can be operated remotely by other computers
7. Use Termius software to SSH to connect to the LAN IP of the Raspberry Pi. Username: `ubuntu` Password: `ubuntu`

### 1.3 Take the Raspberry Pi off the keypad & monitor (For Raspberry Pi OS)

0. Software to be installed on your Windows PC: `1.Termius (for SSH)` `2.VNC Viewer (for remote desktop, download link https://www.realvnc.com/en/connect/download/viewer/)` `3.Visual Studio code (Universal Editor)` `Raspberry Pi Image (image burning) ` `5.Github Desktop(Optional)`
1. Burn the `Raspberry Pi OS(32bit)` image to the SD card using Raspberry Pi Image (Raspberry Pi OS image:`https://www.raspberrypi.com/software/operating-systems/`)
>Before burning, click on the gear button in the bottom right hand corner to configure the following:
> 1. check Set hostname -> raspberrypi.local
> 2. check Enable SSH service -> Login with password
> 3. Check Set username and password -> Username: pi | Password: pi
> 4. Click Save
2. Connect the Raspberry Pi to the power supply and start the Raspberry Pi. Wait for a while and then connect to the Raspberry Pi via SSH with `termius` software. 
3. Enter the `sudo raspi-config` command and use the **direction keys** to configure the following:
> 1. Set the boot option to automatically log into the desktop. `System Options -> Boot/Auto Login -> Desktop Autologin`
> 2. Open VNC Remote Desktop. `Interface Options -> VNC -> YES` 
> 3. Open the SPI port. ` Interface Options -> SPI -> YES` 
> 4. Open the IIC port. `Interface Options -> I2C -> YES` 
> 5. Select Finish to save the settings. 
4. Enter the `reboot` command to reboot the Raspberry Pi. (Wait a while after performing this step) 
5. At this point, the Raspberry Pi is ready to be accessed via `VNC Viewer` remote desktop.
>The corresponding VNC package on the Raspberry Pi is `realvnc-vnc-server` (Raspberry Pi OS comes with it, no need to install it)
6. packages to be installed: ssh vim gcc g++ screen htop git make, execute the following command:
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install ssh vim gcc g++ screen htop git make
```
4. Enter the `reboot` command to reboot the Raspberry Pi. At this point, the basic environment configuration is complete.

### 1.4 Build cross-compilation environment & local compilation environment
Considering that the computing power of the Raspberry Pi is insufficient, using a server will significantly improve compilation efficiency, version control, code backup, and collaborative work. We use a centralized server (`I9-12900k+RTX3090TI+32GB DDR4+512G SSD`) during the development of this project, and build a cross-compilation environment in the server.

#### 1.4.1 Cross-compilation environment (server)

1. Install the aarch64-linux-gnu-cross-compilation toolchain, and other compilation tools required to compile the linux kernel
2. Get the Raspberry Pi kernel source code
```
apt-get source linux-image-$(uname -r) #5.15.0-1023-raspi
```
3. Compile the kernel source code
>Ref: https://www.raspberrypi.com/documentation/computers/linux_kernel.html
4. Compile `helloworld` module code
5. Test the `helloworld` module

![helloworld_modinfo](./img/helloworld_modinfo.png)

#### 1.4.2 Local compilation environment (Raspberry Pi)

1. Compile `helloworld` module code

```
make -C /usr/src/linux-headers-5.15.0-1023-raspi/ M=`pwd` modules
```
2. Test the `helloworld` module
>Test result same as above

---

Q1: The module is not signed problem:
It looks like your system vendor has enabled kernel module signature verification on your kernel, which means it won't load any modules that the vendor hasn't signed. In other words, your patched module is not (correctly) signed and the kernel will refuse to load it.
> Solution: cmd run `CONFIG_MODULE_SIG=n`

Q2: When `sudo apt-get upgrade` is executed, some packs (such as the linux kernel) are reserved and cannot be upgraded.
> `sudo apt-get --with-new-pkgs upgrade`


### 1.5 Sensors & Actuators & Servers

>[Click me to download BOM](./doc/BOM/SleepPanda_BOM.xlsx)

>[Click me to download datasheet](./doc/datasheet/)

**sensor**
1. Speaker & Microphone: WM8960(Control:IIC;Audio:IIS)+Sound Sensor(Voltage Comparator Dout)-✅
2. Body temperature (thermal imaging) & gesture control: MLX90640(IIC)-✅
3. Heart rate & blood oxygen: MAX30101(IIC)-✅
4. Sleeping position monitoring: 4K 30FPS camera (USB+OpenCV)-✅

**Actuator**
1. Ink Screen:SSD1608(Virtual Zoo) (SPI)-✅
2. Touch screen (QT-based GUI) (Video: HDMI, Touch: USB)
3. Buzzer: Emergency abnormal alarm (High/Low Pin Level)-✅

**other**

1. Several DuPont lines-✅


**server**
Facing the world, it is responsible for storing user data and undertaking visits/animal exchanges between virtual zoos.


## 2. System development

### 2.1 Get the source code of SleepPanda

`git clone https://github.com/xddcore/SleepPanda.git`

**Note: If you are a developer, remember to switch to the dev branch with the following command:**
```
git checkout dev
git branch -l
```

### 2.2 cpp unit testing framework

#### 2.2.1 cppunit

```
#install cppunit-lib
sudo apt install -y libcppunit-dev
```
>#include <cppunit/TestRunner.h>

> because:
>1. Google test is relatively simple to use. Writing a test case does not need to be divided into two parts: declaration and definition, and there is no need to register the test suite yourself.
>2. The assert of google test is richer
> So this project uses google test (gtest) for unit testing

#### 2.2.2 google test(gtest)
##### 2.2.2.1 Directly load the compiled dynamic link library file

###### 2.2.2.1.1 For Ubuntu Desktop 22.04 64bit

gtest dynamic link library path `SleepPanda/tools/gtest/lib/`, gtest header file path `SleepPanda/tools/gtest/include/`

```
# g++ build demo

# cd to workscape
cd /SleepPanda/src/app/gtest_demo

# build code
g++ -std=c++14 ./gtest_demo.cpp -I ../../../tools/gtest/include/ ../../../tools/gtest/lib/libgtest.so -lpthread -o gtest_demo

# Export the gtest dynamic link library to the system environment variable (temporary)
export LD_LIBRARY_PATH=../../../tools/gtest/lib/:$LD_LIBRARY_PATH

# run gtest demo
./gtest_demo
```

###### 2.2.2.1.2 For Raspberry Pi OS 32bit**

gtest dynamic link library path`SleepPanda/tools/gtest/rpios_32bit/lib/`
gtest header file path`SleepPanda/tools/gtest/rpios_32bit/include/`

* Compile using g++ (choose one of the two compilation methods, or try both)
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

* Compile with cmake (choose one of the two compilation methods, or try both)
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
**Error: When compiling with cmake, the following error may occur:**
```
CMake Error: The source "/home/pi/xddcore/SleepPanda/src/app/gtest_demo/CMakeLists.txt" does not match the source "/home/ubuntu/xddcore/SleepPanda/src/app/gtest_demo/CMakeLists.txt" used to generate cache.  Re-run cmake with a different source directory.
```
**Solution:**
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

Theoretically, you will get the following running result:
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

##### 2.2.2.2 Compile the dynamic link library file by yourself through the google test source code
```
# install dependencies
sudo apt-get install cmake

# install google test lib

# get source
git clone https://github.com/google/googletest.git

#build
cd googletest
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/install -DBUILD_SHARED_LIBS=ON ..
make -j8
make install

# Then the corresponding dynamic library of gtest will be generated in the build/install directory
ls install /
# include lib

```

### 2.3 Hardware Architecture

![Hardware_Architecture](./img/Hardware_Architecture.png)

### 2.4 Software Architecture

![Software_Architecture](./img/Software_Architecture.png) 

|Index| Layer|-|Comments|
|:----:|:----:|:----:|:----:|
|7(Top)|GUI Interface Layer(QT)|-|Interaction with users|
|6|C++ Logic Layer|-|Opencv, TensorFlow Lite, MQTT Client, etc.|
|5|Hardware Driver Layer (Sensor Class)|-|Configuration/Driver of Hardware (Sensor, etc.)|
|~~4~~|~~Hardware Isolation Layer (Rpi4b Class)~~|-|~~Isolate software logic and hardware dependencies, the purpose is to run/test the upper layer code independently of the underlying hardware dependencies (such as pigpio).~~|
|3|Pigpiod Nested Word Communication Process|-|Drive rpi4b(bcm2711) register|
|2|System Call|-|Provided by the linux kernel|
|1(Bottom)|Linux Kernel Modules|-|Allocate interrupt|

### 2.5 Sensor & Actuator Driver Development

>Ref:
> https://berndporr.github.io/realtime_cpp_coding/
> https://github.com/berndporr/realtime_cpp_coding

**Before you start**, you need to execute the following commands to install the necessary dependencies. Here, I would like to express my sincere thanks to the developers of the pigpio library (https://github.com/joan2937/pigpio).

**How to install Raspberry Pi OS**

Run the following command to install piopig with one click

```
sudo apt-get install libpigpio-dev
```

**Other operating system installation methods**

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

Finally, in order to verify whether pigpio lib is installed correctly, we also **recommend you to run the following test command**:
```
cd pigpio-master #(if you are not in this dir)
sudo ./x_pigpio # check C I/F

# To start the pigpio daemon
sudo pigpiod
./x_pigpiod_if2 # check C I/F to daemon
# Then you should get PASS

# To stop the pigpio daemon
sudo killall pigpiod
./x_pigpiod_if2 # check C I/F to daemon
# Then you should get 'pigpio initialisation failed (-2003).', because pigpio daemon not running.
```
>Note: Every time you use pigpio, you should first run `sudo pigpiod` to start the daemon.

Theoretically, you will get the following running result:
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
then get
:~$ initAllocDMAMem: mbox open failed(No such device or address)
```
Fixup: Run `sudo modprobe vcio`
OR
`./x_pigpio` is a program that uses mailboxes to allocate memory. If under certain conditions (e.g. qemu env), you won't have GPU memory to allocate. pigpio will use the mailbox to allocate DMA memory (unless you request a large buffer). When pigpiod is started with the `-a1` option, page mapping is used to allocate DMA memory.
```
sudo pigpiod -a1 #force use PMAP allocate DMA memory
./x_pigpiod_if2 # check C I/F to daemon
```

Q2: ERROR: **modprobe: FATAL: Module vcio not found in directory**
```
:~$ sudo modprobe vcio
:~$ modprobe: FATAL: Module vcio not found in directory /lib/modules/5.15.0-1031-azure
```
Fixup: Run `sudo apt-get install -y linux-modules-extra-$(uname -r)`
OR
`You need install Ubuntu Desktop Image, and then you will have vcio(videocore io)`

Q3: ERROR: **initInitialise: Can't lock /var/run/pigpio.pid**
```
:~$ sudo ./x_pigpio
:~$ initInitialise: Can't lock /var/run/pigpio.pid
pigpio initialisation failed.
```
Fixup: Run `sudo killall pigpiod`

Q4: In the QEMU environment, pigpio cannot be emulated
```
2023-01-27 09:02:23 initAllocDMAMem: mbox open failed(No such device or address)
Can't initialize pigpio library
pigpio initialisation failed (-2003).
```
Fixup: Because pigpio relies on the BCM2711 chip hardware function to achieve ultra-low-latency DMA operations, and qemu's DMA cannot support this operation, so pigpio simulation cannot be completed in the QEMU environment.

#### 2.5.1 Buzzer

#### 2.5.2 Sound Sensor
>Author: Yihan Wang

**Specification:**
Working Voltage: DC 3.3V-5.5V  
Nominal Frequency: 40KHz  
PCB Size: 35 x 15mm / 1.38 x 0.59" (LxW)  
Fixed Hole Diameter: 3mm  
Main chip:LM393, Electret condenser microphone  
Output form: Digital and Analog Output  

**Note:**
AO outputs real-time voltage signal of the microphone.  
DO outputs low and high lever signal when the sound intensity reaches a threshold  
Sensitivity is adjustable by adjusting the potentiometer  
