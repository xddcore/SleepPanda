MODULE_NAME=helloworld

MODULE_DIR=$(shell basename `pwd`) #get driver name

obj-m:=$(MODULE_NAME).o

KERNEL_DIR=/usr/src/linux-headers-5.15.0-1023-raspi/ #../../kernel/linux-headers-5.15.0-1023-raspi/ 

CROSS=ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-

all:
	make -C $(KERNEL_DIR) M=`pwd` modules
clean:
	make -C $(KERNEL_DIR) M=`pwd` clean
cross:
	make $(CROSS) -C $(KERNEL_DIR) M=`pwd` modules

cross-clean:
	make $(CROSS) -C $(KERNEL_DIR) M=`pwd` clean
