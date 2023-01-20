<!--
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-01-15 20:09:22
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-01-20 21:11:26
 * @FilePath: /SleepPanda/README_ZH.md
 * @Description: 
 * Copyright (c) 2023 by Chengsen Dong 1034029664@qq.com(www.github.com/xddcore), All Rights Reserved. 
-->
# SleepPanda，一个贴心睡眠管家

![Logo](./img/SleepPanda-logo.png)
[![Open Source Love](https://badges.frapsoft.com/os/v2/open-source.svg?v=103)](https://github.com/ellerbrock/open-source-badges/)
[![GPL Licence](https://badges.frapsoft.com/os/gpl/gpl.png?v=103)](https://opensource.org/licenses/GPL-3.0/)

Change README Language: [English](./README.md) ｜ [中文](./README_ZH.md)

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
| Chengsen Dong  | 2789825d |  |
| Rui Liu  | 2756138L |  |
| Hui Wang  | 2810919W |  |
| Yihan Wang  | 2696441W |  |
>如果你对本项目有任何建议，欢迎发邮件至xddcore@gmail.com

## 项目开发规划

#### Global
- [x] 构建Github仓库&README文档，完成基础开发环境配置，固定Linux内核源码，测试内核模块编译。 
- [x] 建立dev分支
- [ ] 购买所有硬件
- [ ] 25/01/2023 Pitch Session

#### Chengsen Dong
- [ ] 蜂鸣器驱动开发
- [ ] WM8960驱动开发
- [ ] MLX90640驱动开发
- [ ] MAX30101驱动开发
- [ ] 墨水屏驱动开发
- [ ] 4K 30FPS摄像头(opencv c++框架)
- [ ] 触摸屏(基于QT的GUI)开发
- [ ] MQTT服务器搭建

#### Yihan Wang
- [ ] To do later...

#### Rui Liu
- [ ] To do later...

#### Hui Wang
- [ ] To do later...

# 指南

## 1. 系统部署

### 1.1 软硬件版本

1. 树莓派4B(4GB/8GB)｜bcm2711
2. Linux发行版:Ubuntu Desktop 22.04.01 LTS(64Bit)
3. 内核版本:5.15.0-1023-raspi |检索所有Linux内核`apt-cache search linux-raspi-headers`
4. g++:`g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0`
5. gcc:`gcc (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0`


### 1.2 将树莓派脱离键鼠&显示器
0. Windows电脑需要安装的软件:`1.Termius(用于SSH)` `2.Microsoft  Remote Desktop(用于远程桌面)3.Visual Studio code(万能编辑器)`
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

### 1.3 构建交叉编译环境&本地编译环境
考虑到树莓派算力不足，而使用服务器将会显著提升编译效率，版本控制，代码备份，协同工作。我们在本项目开发过程中使用一台中心化服务器(`I9-12900k+RTX3090TI+32GB DDR4+512G SSD`)，并在服务器中构建交叉编译环境。

#### 1.3.1 交叉编译环境(服务器)

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

#### 1.3.2 本地编译环境(树莓派)

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

### 1.4 传感器&执行器&服务器

**传感器**
1. 扬声器&麦克风:WM8960
2. 体温(热成像)&手势控制:MLX90640
3. 心率&血氧:MAX30101
4. 睡姿监测:4K 30FPS摄像头

**执行器**
1. 墨水屏(虚拟动物园)
2. 触摸屏(基于QT的GUI)
3. 蜂鸣器:紧急异常报警

**服务器**
面向全球，负责存储用户数据，并承担虚拟动物园之间的参观访问/动物交换。
