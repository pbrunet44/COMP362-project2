UNAME	:= $(shell uname -r)
KDIR	:= /lib/modules/$(UNAME)/build
PWD	:= $(shell pwd)

MOD_TARGET = simdev
simdev-objs := sim_dev.o disk.o

obj-m	+= $(MOD_TARGET).o

all::	test_sim_dev
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	
test_sim_dev: test_sim_dev.c
	gcc -o $@ $<

clean::
	$(RM) test_sim_dev 
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
