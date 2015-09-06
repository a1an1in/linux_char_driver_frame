#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x41086e, "module_layout" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xd0d8621b, "strlen" },
	{ 0xc288f8ce, "malloc_sizes" },
	{ 0x343a1a8, "__list_add" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x2bc95bd4, "memset" },
	{ 0xb72397d5, "printk" },
	{ 0x6c2e3320, "strncmp" },
	{ 0x521445b, "list_del" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x884f145, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0x2e60bace, "memcpy" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "2CF73EF4C7B4DD810295DD2");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 6,
};
