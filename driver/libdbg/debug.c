#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include "libdbg/debug.h"

void printk_buffer(uint8_t *buf,size_t len)
{
	size_t i;
	if(len == 0){
		return;
	}
	for(i = 0; i < len - 1; i++){
		printk("%x-",*(buf + i));
	}
	printk("%x",*(buf + i));

}

void print_dbg_level(uint32_t set_str)
{

	switch((set_str >> 8) & 0xf){
		case LV_PANIC:
			printk("[DBG_PANIC]--");
			break;
		case LV_FATAL:
			printk("[DBG_FATAL]--");
			break;
		case LV_ERROR: 			
			printk("[DBG_ERROR]--");
			break;
		case LV_WARNNING: 			
			printk("[DBG_WARNNING]--");
			break;
		case LV_SUC:
			printk("[DBG_SUC]--");
			break;
		case LV_CORRECT:
			printk("[DBG_CORRECT]--");
			break;
		case LV_VIP: 		
			printk("[DBG_VIP]--");
			break;
		case LV_FLOW:  			
			printk("[DBG_FLOW]--");
			break;
		case LV_IMPORTANT: 		
			printk("[DBG_IMPORTANT]--");
			break;
		case LV_DETAIL: 			
			printk("[DBG_DETAIL]--");
			break;
		default:
			printk("[DEFAULT]--");
			break;
	}
}
