obj-m += mod_firstchar.o

CFLAGS_mod_firstchar,o := -DDEBUG
VER ?= $(shell uname -r)
KDIR = /lib/modules/$(VER)/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
