PWD ?= $(shell pwd)
# KERNELDIR:=/home/samba/tspai/kernel
KERNELDIR:=/lib/modules/5.4.0-150-generic/build
obj-m += main.o
ccflags-y:=-std=gnu99 -Wno-declaration-after-statement
modlue:
	make -C $(KERNELDIR) M=$(PWD)  modules
clean:
	make -C $(KERNELDIR) M=$(PWD)  clean