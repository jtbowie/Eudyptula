CONFIG_MODULE_SIG=n

obj-m += mod_firstchar.o

all:
ifeq ($(VER),)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
else
	make -C /lib/modules/$(VER)/build M=$(PWD) modules
endif

clean:
ifeq ($(VER),)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
else
	make -C /lib/modules/$(VER)/build M=$(PWD) clean
endif
