<!--
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-01-15 20:09:22
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-01-16 14:52:58
 * @FilePath: /Embedded_Linux/ENG5220/README.md
 * @Description: 
 * Copyright (c) 2023 by Chengsen Dong 1034029664@qq.com(www.github.com/xddcore), All Rights Reserved. 
-->
# SleepPanda，一个贴心睡眠管家

## 1. 系统部署指南

### 1.1 软硬件版本

1. 树莓派4B(4GB/8GB)
2. Linux发行版:Ubuntu Desktop 22.04.01 LTS(64Bit)
3. 内核版本:5.15.0-1023-raspi |搜索Linux内核`apt-cache search linux-raspi-headers`
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
4. 开启ubuntu远程桌面:Setting-share-开启远程桌面-修改用户名和密码为`ubuntu`

   4.1 允许hdmi热插拔，避免不插hdmi时无法访问远程桌面。  
   输入以下命令:`sudo vim /boot/fireware/config.txt`，并在config.txt文件中增加以下内容:  
   ```
   [hdmi]
   hdmi_force_hotplug=1 #allow remote desktop when no hdmi wire 
   ```
   4.2固定远程桌面登陆密码，防止每次重启后密码随机生成无法访问远程桌面  
   Application-password&keys-默认密钥&login-change password-不设置密码(留空)-最后实现取消加密  
   4.3使用Microsoft Remote Desktop软件来访问树莓派桌面|树莓派的局域网IP+用户名和密码均为`ubuntu`

5. 防止树莓派休眠:Setting-Power-Nerver sleep
6. 至此树莓派完全脱离键盘鼠标和显示器，可用其他电脑远程操作
7. 使用Termius软件SSH连接至树莓派的局域网IP。用户名:`ubuntu` 密码:`ubuntu`