PWD ?= $(shell pwd)
# KERNELDIR:=/home/samba/tspai/kernel
KERNELDIR:=/lib/modules/5.4.0-150-generic/build
obj-m += main.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement
# CROSS_COMPILE ?= /home/samba/tspai/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
modlue:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean