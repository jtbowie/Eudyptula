obj-m += mod_sysfs.o

VER ?= $(shell uname -r)
KDIR = /lib/modules/$(VER)/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
