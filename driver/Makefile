obj-m :=  plat_driver.o
KERNEL := /lib/modules/`uname -r`/build/
#KERNEL := /root/workspace/LINUX/linux-2.6.19_good

plat_driver-objs := plat_driver.o

plat_driver-objs += libdbg/debug.o

plat_driver-objs += liballoc/allocator.o
plat_driver-objs += liballoc/allocator_sys_alloc.o
plat_driver-objs += liballoc/liballoc_register_modules.o
plat_driver-objs += liballoc/test_allocator.o

plat_driver-objs += libcre/sync_lock/sync_lock.o

plat_driver-objs += libproto_analyzer/protocol_analyzer.o
plat_driver-objs += libproto_analyzer/protocol_format_set.o
plat_driver-objs += libproto_analyzer/test/pdt_drv_proto_format.o
plat_driver-objs += libproto_analyzer/test/pdt_drv_proto_analyzer.o





all:
	make -C $(KERNEL) $(CFLAGS) M=`pwd` modules 
install:
	make -C $(KERNEL) M=`pwd` modules_install
	depmod -A
clean:
	make -C $(KERNEL) M=`pwd` clean
	rm -f Module.* modules.* .*.cmd

