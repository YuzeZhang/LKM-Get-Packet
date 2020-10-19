obj-m := lkm.o lkm_mmap.o

UNAME := $(shell uname -r)
PWD := $(shell pwd)
KERNELBUILD := /lib/modules/$(UNAME)/build

default:
	$(MAKE) -C $(KERNELBUILD) M=$(PWD) modules

clean:
	rm -rf *.o *.cmd *.mod.c .tmp_versions Module.symvers modules.order