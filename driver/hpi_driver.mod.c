#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x41086e, "module_layout" },
	{ 0xbef9fe65, "cdev_del" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xf28b622f, "cdev_init" },
	{ 0xf6d62bd3, "dev_set_drvdata" },
	{ 0xc288f8ce, "malloc_sizes" },
	{ 0x96e2ed60, "device_destroy" },
	{ 0xb72397d5, "printk" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0x88329182, "platform_get_resource" },
	{ 0x173b7186, "device_create" },
	{ 0xc89104ab, "platform_driver_register" },
	{ 0x40c497a1, "cdev_add" },
	{ 0x42c8de35, "ioremap_nocache" },
	{ 0x884f145, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0xedc03953, "iounmap" },
	{ 0x85440935, "class_destroy" },
	{ 0xcf01172d, "platform_driver_unregister" },
	{ 0x8fe748ab, "__class_create" },
	{ 0x93b38b05, "dev_get_drvdata" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "140E33751F270DD578E3FD8");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 6,
};
