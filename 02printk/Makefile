PWD ?= $(shell pwd)
KERNELDIR:=/home/samba/tspai/kernel
obj-m += demo.o

module:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 modules
clean:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm64 clean