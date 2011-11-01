#ifneq ($(KERNELRELEASE), )

obj-m := driver.o device.o
CC = gcc
#else

#KDIR := /usr/src/linux-2.6.32.2

#all:
#make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-


KERNELDIR ?=/lib/modules/$(shell uname -r)/build

PWD :=$(shell pwd)

modules:
	$(MAKE) -C  $(KERNELDIR) M=$(PWD) modules

clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers


