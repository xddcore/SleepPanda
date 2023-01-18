<!--
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-01-18 00:44:11
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-01-18 10:06:48
 * @FilePath: /SleepPanda/README.md
 * @Description: 
 * Copyright (c) 2023 by Chengsen Dong 1034029664@qq.com(www.github.com/xddcore), All Rights Reserved. 
-->
# SleepPanda, a caring sleep manager

[![Open Source Love](https://badges.frapsoft.com/os/v2/open-source.svg?v=103)](https://github.com/ellerbrock/open-source-badges/)
[![GPL Licence](https://badges.frapsoft.com/os/gpl/gpl.png?v=103)](https://opensource.org/licenses/GPL-3.0/)

Change README Language: [English](./README.md) ｜ [中文](./README_ZH.md)

## Description
SleepPanda is a sleep monitoring system based on Raspberry Pi 4b (bcm2711). SleepPanda uses a variety of sensors to collect sleep data of users, and analyzes and processes the data through dsp, convolutional neural network and other algorithms. Finally, the data processing results are presented to users in various ways.

## Features
1. Early bed reminder, wake up alarm clock
2. Body temperature (thermal imaging), heart rate, blood oxygen monitoring
3. Sleep position monitoring
4. Early warning of apnea syndrome
5. Light/deep sleep classification
6. Playing sleep aid music
7. Sharing and social networking (different SleepPanda hardware will be connected to the same server, which will help users share sleep data and monitor each other's early bedtime plans)

## Contributors
|  Name   | ID  |  Role  |
|  ----  | ----  | ----  |
| Chengsen Dong  | 2789825d |  |
| Rui Liu  | 2756138L |  |
| Hui Wang  | 2810919W |  |
| Yihan Wang  | 2696441W |  |
>If you have any suggestions for this project, please email to xddcore@gmail.com

## Development Planning
- [x] Build the Github warehouse & README document, complete the basic development environment configuration, fix the Linux kernel source code, and test the compilation of the kernel module.
- [ ] To do later...

# Guide

## 1. System Deployment

### 1.1 Software and hardware version

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

    4.1 Allow hdmi hot plugging to avoid inability to access remote desktop when hdmi is not plugged in. ｜Unresolved! ! ! |Try to use HDMI pseudo load to solve
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

### 1.3 Build cross-compilation environment & local compilation environment
Considering that the computing power of the Raspberry Pi is insufficient, using a server will significantly improve compilation efficiency, version control, code backup, and collaborative work. We use a centralized server (`I9-12900k+RTX3090TI+32GB DDR4+512G SSD`) during the development of this project, and build a cross-compilation environment in the server.

#### 1.3.1 Cross-compilation environment (server)

1. Install the aarch64-linux-gnu-cross-compilation toolchain, and other compilation tools required to compile the linux kernel
2. Get the Raspberry Pi kernel source code
```
apt-get source linux-image-$(uname -r) #5.15.0-1023-raspi
```
3. Compile the kernel source code
4. Compile `helloworld` module code
5. Test the `helloworld` module

![helloworld_modinfo](./img/helloworld_modinfo.png)

#### 1.3.2 Local compilation environment

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
