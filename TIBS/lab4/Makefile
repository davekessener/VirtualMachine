DEBFLAGS = -O2

EXTRA_CFLAGS = $(DEBFLAGS)
EXTRA_CFLAGS += -I$(LDDINC)

ACFLAGS=$(EXTRA_CFLAGS) -Wall -O0 -ggdb

ifneq ($(KERNELRELEASE),)
# call from kernel build system

obj-m	:= tzm.o
tzm-objs := main.o speed.o

else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) LDDINC=$(PWD)/../include modules

endif



clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions userspace reader

userspace:
	$(CC) userspace.c terminal.c speed.c $(ACFLAGS) -o userspace -lrt

reader:
	$(CC) reader.c $(ACFLAGS) -o reader

depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif
