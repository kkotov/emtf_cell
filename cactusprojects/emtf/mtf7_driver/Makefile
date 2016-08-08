#
# Makefile for utca_sp12 driver
#
ifneq ($(KERNELRELEASE),)
obj-m:=utca_sp12.o
utca_sp12-objs = \
	utca_sp12_fops.o \
	utca_sp12_init_drv.o \
	utca_sp12_init_pci.o \
        utca_sp12_misc.o \
        utca_sp12_ioctl.o 

else 
KDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KDIR) V=1 M=$(PWD) modules

clean:
	$(RM) .*.cmd *.mod.c *.o *.ko -r .tmp* 

endif
