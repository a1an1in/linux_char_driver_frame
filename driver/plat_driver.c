/*
 * =====================================================================================
 *
 *       Filename:  plat_driver.c
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
#include <linux/fs.h>		/* file_operations */
#include <linux/cdev.h>		/* cdev */
#include <linux/ioport.h>	/* request_mem_region */
#include <linux/io.h>		/* readb/writeb */
#include <linux/uaccess.h>
#include <linux/platform_device.h>

#include <liballoc/test_allocator.h>
/*
 *#include <libproto_analyzer/pdt_drv_proto_analyzer.h>
 */

#define DEF_MAJOR 	90

struct pdev_priv {
	unsigned long phy_base;
	void __iomem *vir_base;
	unsigned long phy_size;
	int irq;
	struct cdev cdev;
	struct class *class;
	dev_t dev_id;
	struct device *class_dev;
};

/*
 * file_operations->open
 */
static int 
device_open(struct inode *inode, struct file *filp)
{
	struct pdev_priv *pdev_priv = container_of(inode->i_cdev, struct pdev_priv, cdev);

	filp->private_data = pdev_priv;
	return 0;
}

/* 
 * file_operations->release
 */
static int 
device_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/*
 * file_operations->read
 */
static ssize_t
device_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct pdev_priv *pdev_priv = filp->private_data;
	char *tmp = NULL;

	count = min(count, (size_t)(pdev_priv->phy_size - *f_pos));
	if (0 == count)
		return 0;
	
	/* read register */
	tmp = (char *)kzalloc(count, GFP_KERNEL);
	if (NULL == tmp)
		return -ENOMEM;
	memcpy_fromio(tmp, (pdev_priv->vir_base + *f_pos), count);

	/* copy to user */
	if (copy_to_user(buf, tmp, count)) {
		kfree(tmp);
		return -EFAULT;
	}

	*f_pos += count;

	return count;
}

static ssize_t
device_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	return 0;
}

static struct file_operations pdrv_fops = {
	.owner   = THIS_MODULE,
	.open    = device_open,
	.release = device_release,
	.read    = device_read,
	.write   = device_write,
};





/* 
 * platform_driver->probe 
 */
static int 
plat_probe(struct platform_device *pdev)
{
	struct pdev_priv *pdev_priv;
	int ret;
	struct resource *res1, *res2;
	
	/* get resource from platform_device */
	res1 = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	res2 = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res1 || !res2)
		printk("Not enough resources.\n");

	/* alloc & init private struct */
	pdev_priv = (struct pdev_priv *)kzalloc(sizeof(*pdev_priv), GFP_KERNEL);
	if (NULL == pdev_priv)
		return -ENOMEM;

	pdev_priv->phy_base = res1->start;
	pdev_priv->phy_size = res1->end - res1->start + 1;
	if (res2)
		pdev_priv->irq = res2->start;

	/* ioremap */
	pdev_priv->vir_base = ioremap(pdev_priv->phy_base, pdev_priv->phy_size);
	if (NULL == pdev_priv->vir_base) {
		printk("cannot ioremap: 0x%lx\n", pdev_priv->phy_base);
		ret = -EIO;
		goto err;
	}
	printk("ioremap 0x%lx to 0x%p\n", pdev_priv->phy_base, pdev_priv->vir_base);

	/* register cdev */
	if (pdev->id == -1)
		pdev_priv->dev_id = MKDEV(DEF_MAJOR, 0);
	else 
		pdev_priv->dev_id = MKDEV(DEF_MAJOR, pdev->id);
	cdev_init(&pdev_priv->cdev, &pdrv_fops);
	pdev_priv->cdev.owner = THIS_MODULE;
	cdev_add(&pdev_priv->cdev, pdev_priv->dev_id, 1);

	/* create class */
	pdev_priv->class = class_create(THIS_MODULE, "hpi");
	if (IS_ERR(pdev_priv->class))
		return PTR_ERR(pdev_priv->class);

	/* create device file */
	pdev_priv->class_dev = device_create(pdev_priv->class, /* class */
		&pdev->dev,	/* parent */
		pdev_priv->dev_id,	/* dev_t */
		NULL,		/* drvdata */
		"hpi_driver-%d", MINOR(pdev_priv->dev_id)); /* name */

	if (IS_ERR(pdev_priv->class_dev)) {
		ret = PTR_ERR(pdev_priv->class_dev);
		goto err2;
	}

	/* save pdev_priv to platform_device */
	platform_set_drvdata(pdev, pdev_priv);


	test_allocator();

	/*
	 *pdt_drv_proto_analyzer();
	 */

	return 0;
err2:
	cdev_del(&pdev_priv->cdev);
	iounmap(pdev_priv->vir_base);
err:
	kfree(pdev_priv);
	return ret;
}

/*
 * platform_driver->remove
 */
static int 
plat_remove(struct platform_device *pdev)
{
	struct pdev_priv *pdev_priv = platform_get_drvdata(pdev);

	device_destroy(pdev_priv->class, pdev_priv->dev_id);
	class_destroy(pdev_priv->class);
	cdev_del(&pdev_priv->cdev);
	iounmap(pdev_priv->vir_base);
	kfree(pdev_priv);

	return 0;
}

static struct platform_driver pdrv = {
	.probe 	= plat_probe,
	.remove = plat_remove,
	.driver = {
		.name = "arm_hpi",
	}
};




static int __init driver_module_init(void)
{
	int ret;
	ret = platform_driver_register(&pdrv);
	return 0;
}

static void __exit driver_module_exit(void)
{
	/* unregister platform_driver */
	platform_driver_unregister(&pdrv);
}

module_init(driver_module_init);
module_exit(driver_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alan");

