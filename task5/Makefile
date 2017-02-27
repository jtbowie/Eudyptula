CFLAGS_mod_usb_dummy.o := -DDEBUG

obj-m += mod_usb_dumy.o

VER ?= $(shell uname -r)
KDIR = /lib/modules/$(VER)/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
