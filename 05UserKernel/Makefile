PWD ?= $(shell pwd)
KERNELDIR:=/home/samba/tspai/kernel
obj-m += UserKernel.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement
CROSS_COMPILE ?= /home/samba/tspai/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
CC := $(CROSS_COMPILE)gcc
module:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 modules
	$(CC) test.c -o test
cleanAll:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 clean
	rm test.c test
clean:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 clean
	rm test