# ifeq ($(KERNELRELEASE), )
# KERNELDIR := /lib/modules/$(shell uname -r)/build
# PWD :=$(shell pwd)
# default:
# 	$(MAKE) -C $(KERNELDIR)  M=$(PWD)  
# clean:
# 	rm -rf *.mk .tmp_versions Module.symvers *.mod.c *.o *.ko .*.cmd Module.markers modules.order *.a *.mod
# load:
# 	insmod ch341.ko
# unload:
# 	rmmod ch341
# install: default
# 	rmmod ch341 || true
# 	insmod ch341.ko || true
# 	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/usb/serial/ || true
# 	cp -f ./ch341.ko /lib/modules/$(shell uname -r)/kernel/drivers/usb/serial/ || true
# 	depmod -a
# uninstall:
# 	rmmod ch341 || true
# 	rm -rf /lib/modules/$(shell uname -r)/kernel/drivers/usb/serial/ch341.ko || true
# 	depmod -a
# else
# 	obj-m := ch341.o
# endif

PWD ?= $(shell pwd)
KERNELDIR:=/home/samba/tspai/kernel
obj-m += ch341.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement
CROSS_COMPILE ?= /home/samba/tspai/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
CC := $(CROSS_COMPILE)gcc
module:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 modules
cleanAll:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 clean
clean:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 clean