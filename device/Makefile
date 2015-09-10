obj-m :=  plat_device.o

#KERNEL := /lib/modules/`uname -r`/build/
KERNEL := /root/workspace/LINUX/linux-2.6.19_good

all:
	make -C $(KERNEL) M=`pwd` modules
install:
	#make -C $(KERNEL) M=`pwd` modules_install
	#depmod -A
	cp *.ko /var/ftp/
clean:
	make -C $(KERNEL) M=`pwd` clean
	rm -f Module.* modules.* .*.cmd

