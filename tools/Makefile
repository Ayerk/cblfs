obj-m += cblfs_nvme.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean