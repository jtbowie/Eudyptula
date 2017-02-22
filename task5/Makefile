CONFIG_MODULE_SIG=n

CFLAGS_mod_hello_girls.o := -DDEBUG
obj-m += mod_hello_girls.o

VER ?= $(shell uname -r)
KDIR = /lib/modules/$(VER)/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
