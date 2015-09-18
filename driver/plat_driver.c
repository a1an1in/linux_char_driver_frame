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
#include <linux/time.h>
#include <linux/delay.h>

#include <libdbg/debug.h>
#include <liballoc/allocator.h>
#include <liballoc/test_allocator.h>
#include <liballoc/liballoc_register_modules.h>
#include <libproto_analyzer/pdt_drv_proto_analyzer.h>
#include <libproto_analyzer/protocol_format_set.h>
#include <libproto_analyzer/protocol_analyzer.h>
#include <libdata_structure/test_datastructure.h>
#include <business/business.h>
#include <task_admin/task_admin.h>

#define VERSION 1.1.2.0

#define DEF_MAJOR 	90
extern void pfs_set_pdt_drv_proto_format(protocol_format_set_t *pfs_p);

struct pdev_priv {
	unsigned long phy_base;
	void __iomem *vir_base;
	unsigned long phy_size;
	int irq;
	struct cdev cdev;
	struct class *class;
	dev_t dev_id;
	struct device *class_dev;
	wait_queue_head_t writeq;
	wait_queue_head_t readq;
};
protocol_format_set_t *pfs_p;
allocator_t *allocator;

/*
 * file_operations->open
 */
static int 
device_open(struct inode *inode, struct file *filp)
{
	struct pdev_priv *pdev_priv = container_of(inode->i_cdev, struct pdev_priv, cdev);

	dbg_str(DBG_DETAIL,"device open");
	filp->private_data = pdev_priv;
	return 0;
}

/* 
 * file_operations->release
 */
static int 
device_release(struct inode *inode, struct file *filp)
{
	dbg_str(DBG_DETAIL,"device release");
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

	dbg_str(DBG_DETAIL,"device read");
	wait_event(pdev_priv->readq,0);
/*
 *    count = min(count, (size_t)(pdev_priv->phy_size - *f_pos));
 *    if (0 == count)
 *        return 0;
 *    
 *    [> read register <]
 *    tmp = (char *)kzalloc(count, GFP_KERNEL);
 *    if (NULL == tmp)
 *        return -ENOMEM;
 *    memcpy_fromio(tmp, (pdev_priv->vir_base + *f_pos), count);
 *
 *    [> copy to user <]
 *    if (copy_to_user(buf, tmp, count)) {
 *        kfree(tmp);
 *        return -EFAULT;
 *    }
 *
 *    *f_pos += count;
 */

	count = 0;
	return count;
}

/*
 *struct protocol_analyzer_s *pa;
 */

static ssize_t
device_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
#define MAX_BUF_SIZE 1024
	unsigned char data[MAX_BUF_SIZE];
	uint16_t command; 
	/*
	 *struct timeval tval1,tval2,tval3,tval4,tval5,tval6;
	 *struct timespec tspec1,tspec2,tspec3,tspec4,tspec5,tspec6;
	 */

	if(buf == NULL || count <=0){
		dbg_str(DBG_ERROR,"write err");
	}
	copy_from_user(data,buf,count);
	dbg_buf(DBG_IMPORTANT,"device write data:",data,count);

	command = data[0] << 8 | data[1];
	process_protocol(command,data,count);

	/*
	 *do_gettimeofday(&tval5);
	 *getnstimeofday(&tspec5);
	 *udelay(30);
	 *do_gettimeofday(&tval6);
	 *getnstimeofday(&tspec6);
	 *printk("1timeval:%lds -- %ldus\n",tval1.tv_sec,tval1.tv_usec); 
	 *printk("2timeval:%lds -- %ldus\n",tval2.tv_sec,tval2.tv_usec); 
	 *printk("3timeval:%lds -- %ldus\n",tval3.tv_sec,tval3.tv_usec); 
	 *printk("4timeval:%lds -- %ldus\n",tval4.tv_sec,tval4.tv_usec); 
	 *printk("5timeval:%lds -- %ldus\n",tval5.tv_sec,tval5.tv_usec); 
	 *printk("6timeval:%lds -- %ldus\n",tval6.tv_sec,tval6.tv_usec); 
	 *printk("1timeval:%lds -- %ldns\n",tspec1.tv_sec,tspec1.tv_nsec); 
	 *printk("2timeval:%lds -- %ldns\n",tspec2.tv_sec,tspec2.tv_nsec); 
	 *printk("3timeval:%lds -- %ldns\n",tspec3.tv_sec,tspec3.tv_nsec); 
	 *printk("4timeval:%lds -- %ldns\n",tspec4.tv_sec,tspec4.tv_nsec); 
	 *printk("5timeval:%lds -- %ldns\n",tspec5.tv_sec,tspec5.tv_nsec); 
	 *printk("6timeval:%lds -- %ldns\n",tspec6.tv_sec,tspec6.tv_nsec); 
	 *printk("2-1diff =%ldus\n",(tspec2.tv_nsec - tspec1.tv_nsec)/1000);
	 *printk("3-2diff =%ldus\n",(tspec3.tv_nsec - tspec2.tv_nsec)/1000);
	 *printk("4-3diff =%ldus\n",(tspec4.tv_nsec - tspec3.tv_nsec)/1000);
	 *printk("5-4diff =%ldus\n",(tspec5.tv_nsec - tspec4.tv_nsec)/1000);
	 *printk("6-5diff =%ldus\n",(tspec6.tv_nsec - tspec5.tv_nsec)/1000);
	 *printk("5-1diff =%ldus\n",(tspec5.tv_nsec - tspec1.tv_nsec)/1000);
	 */

	return 0;
#undef MAX_BUF_SIZE
}
static int 
device_ioctl(struct inode *inodep,struct file *filp,unsigned int cmd,unsigned long arg)
{
	if(cmd != 2)
		dbg_str(DBG_DETAIL,"device_ioctl,cmd=%d",cmd);
	return 0;
}
static struct file_operations pdrv_fops = {
	.owner   = THIS_MODULE,
	.open    = device_open,
	.release = device_release,
	.read    = device_read,
	.write   = device_write,
	.ioctl = device_ioctl,
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
	
	dbg_str(DBG_DETAIL,"plat_probe");
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
	/*
	 *pdev_priv->vir_base = ioremap(pdev_priv->phy_base, pdev_priv->phy_size);
	 *if (NULL == pdev_priv->vir_base) {
	 *    printk("cannot ioremap: 0x%lx\n", pdev_priv->phy_base);
	 *    ret = -EIO;
	 *    goto err;
	 *}
	 *printk("ioremap 0x%lx to 0x%p\n", pdev_priv->phy_base, pdev_priv->vir_base);
	 */

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
	/*
	 *pdev_priv->class_dev = device_create(pdev_priv->class, [> class <]
	 *    &pdev->dev,	[> parent <]
	 *    pdev_priv->dev_id,	[> dev_t <]
	 *    NULL,		[> drvdata <]
	 *    "hpi_driver-%d", MINOR(pdev_priv->dev_id)); [> name <]
	 */
	pdev_priv->class_dev = class_device_create(pdev_priv->class,NULL,pdev_priv->cdev.dev,NULL,"ssc");

	if (IS_ERR(pdev_priv->class_dev)) {
		ret = PTR_ERR(pdev_priv->class_dev);
		goto err2;
	}

	/* save pdev_priv to platform_device */
	platform_set_drvdata(pdev, pdev_priv);


	dbg_str(DBG_DETAIL,"my test begin");

	init_waitqueue_head(&pdev_priv->writeq);
	init_waitqueue_head(&pdev_priv->readq);

	liballoc_register_modules();
	/*
	 *test_allocator();
	 */

	/*
	 *pdt_drv_proto_analyzer();
	 */

	/* create mem allocator */
	allocator = allocator_creator(ALLOCATOR_TYPE_SYS_MALLOC,0);
	if(allocator == NULL){
		dbg_str(DBG_ERROR,"allocator_creator");
		goto err3;
	}

	/* create protocol format set */
	pfs_p = pfs_create_proto_format_set(allocator);
	if(pfs_p == NULL){
		dbg_str(DBG_ERROR,"pfs_create_proto_format_set");
		goto err4;
	}
	init_proto_format_set(0x3000,100,pfs_p);

	pfs_set_pdt_drv_proto_format(pfs_p);


	/* create task admin */
	task_admin_slot1_gp = create_task_admin();
	task_admin_slot2_gp = create_task_admin();

	/*
	 *dbg_str(DBG_DETAIL,"pfs_destroy_protocol_format_set");
	 *pfs_destroy_protocol_format_set(pfs_p);
	 */
	 /*
      *pa = pa_create_protocol_analyzer(allocator);
      *pa_init_protocol_analyzer(0x3008, pfs_p, pa);
	  */

	return 0;

err4:
	allocator_destroy(allocator);
err3:
	device_destroy(pdev_priv->class, pdev_priv->dev_id);
err2:
	dbg_str(DBG_DETAIL,"run at here");
	cdev_del(&pdev_priv->cdev);
	/*
	 *iounmap(pdev_priv->vir_base);
	 */
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

	destroy_task_admin(task_admin_slot1_gp);
	destroy_task_admin(task_admin_slot2_gp);
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
MODULE_AUTHOR("alan lin");

