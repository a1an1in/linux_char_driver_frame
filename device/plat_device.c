/*
 * =====================================================================================
 *
 *       Filename:  plat_device.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/28/2015 02:25:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/platform_device.h>

#define BASE_CARD	0xF4802400
#define SIZE_CARD	0x100
#define	IRQ_CARD	11

static void 
plat_release(struct device *dev)
{
	struct platform_device *tmp = container_of(dev, struct platform_device, dev);

	printk("Release device %s, id is %d\n", tmp->name, tmp->id);
}


static struct resource plat_res[] = {
	[0] = {
		.start 	= BASE_CARD,
		.end 	= BASE_CARD + SIZE_CARD - 1,
		.flags	= IORESOURCE_MEM,
	},	
	[1] = {
		.start 	= IRQ_CARD,
		.end	= IRQ_CARD,
		.flags	= IORESOURCE_IRQ,
	},
};


static struct platform_device pdev = {
	.name 	= "arm_hpi",
	.id 	= 2,
	.num_resources 	= ARRAY_SIZE(plat_res),
	.resource = plat_res,
	.dev = {
		.release = plat_release,
	}
};
	

static int __init device_module_init(void)
{
	return platform_device_register(&pdev);
}

static void __exit device_module_exit(void)
{
	platform_device_unregister(&pdev);
}

module_init(device_module_init);
module_exit(device_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alan");

