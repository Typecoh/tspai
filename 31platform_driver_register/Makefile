PWD ?= $(shell pwd)
obj-m += platform_drivers.o
KERNELDIR:=/lib/modules/5.4.0-150-generic/build
ccflags-y:=-std=gnu99 -Wno-declaration-after-statement
module:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean