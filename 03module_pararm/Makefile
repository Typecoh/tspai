PWD ?= $(shell pwd)
KERNELDIR:=/home/samba/tspai/kernel
obj-m += ModulePararm.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement
module:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 modules
clean:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 clean
