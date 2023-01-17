MODULE_NAME=helloworld

MODULE_DIR=$(shell basename `pwd`) #获取当前驱动文件夹名

obj-m:=$(MODULE_NAME).o

KERNEL_DIR=/home/xdd/xddcore/SleepPanda/src/kernel/linux-raspi-headers-5.15.0-1023

CROSS=ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-

all:
	make $(CROSS) -C $(KERNEL_DIR) M=`pwd` modules

clean:
	make $(CROSS) -C $(KERNEL_DIR) M=`pwd` clean