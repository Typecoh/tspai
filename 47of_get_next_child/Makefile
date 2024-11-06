# PWD ?= $(shell pwd)
# obj-m += platform_drivers.o
# KERNELDIR:=/lib/modules/5.4.0-150-generic/build
# ccflags-y:=-std=gnu99 -Wno-declaration-after-statement
# module:
# 	make -C $(KERNELDIR) M=$(PWD) modules
# clean:
# 	make -C $(KERNELDIR) M=$(PWD) clean

PWD ?= $(shell pwd)
KERNELDIR:=/home/samba/tspai/kernel
obj-m += platform_drivers.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement
CROSS_COMPILE ?= /home/samba/tspai/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
CC := $(CROSS_COMPILE)gcc
modlue:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 modules
clean:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 clean