obj-m +=  hpi_driver.o
#KERNEL := /lib/modules/`uname -r`/build/
#LINUX := ~/workspace/LINUX/linux-2.6.19_good
KERNEL := /root/workspace/LINUX/linux-2.6.19_good

hpi_driver-objs := plat_driver.o

hpi_driver-objs += libdbg/debug.o

hpi_driver-objs += liballoc/allocator.o
hpi_driver-objs += liballoc/allocator_sys_alloc.o
hpi_driver-objs += liballoc/liballoc_register_modules.o
hpi_driver-objs += liballoc/test_allocator.o

hpi_driver-objs += libcre/sync_lock/sync_lock.o

hpi_driver-objs += libproto_analyzer/protocol_analyzer.o
hpi_driver-objs += libproto_analyzer/protocol_format_set.o
hpi_driver-objs += libproto_analyzer/test/pdt_drv_proto_format.o
hpi_driver-objs += libproto_analyzer/test/pdt_drv_proto_analyzer.o





all:
	make -C $(KERNEL) $(CFLAGS) M=`pwd` modules 
install:
	#make -C $(KERNEL) M=`pwd` modules_install
	#depmod -A
	cp -rf *.ko /var/ftp
clean:
	make -C $(KERNEL) M=`pwd` clean
	rm -f Module.* modules.* .*.cmd

