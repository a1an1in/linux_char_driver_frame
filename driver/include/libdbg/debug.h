#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>


void printk_buffer(uint8_t *buf,size_t len);
void print_dbg_level(uint32_t set_str);

enum{
	LV_PANIC=0, 		
	LV_FATAL, 		
	LV_ERROR ,		
	LV_WARNNING,
	LV_SUC ,		
	LV_CORRECT,
	LV_VIP, 	
	LV_NON1,
	LV_NON2,
	LV_FLOW , 		
	LV_NON3,
	LV_IMPORTANT, 		
	LV_DETAIL
};

#define ERRORIP "cannot find host ip"  

#define swap16(s) ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)) 

/*color*/
#define BLACK 				0x0
#define RED 				0x1
#define GREEN 				0x2
#define YELLOW 				0x3
#define BLUE 				0x4
#define PURPLE 				0x5
#define DARK_GREEN 			0x6
#define WHITE				0x7

/*reverse backgrand and font flag*/
#define REVERSE_COLOR_FLAG  1
#define HIGHLIGHT_FLAG 	1

/*backgrand and font color*/
#define BGC(color) color << 4
#define FTC(color) color << 1

#define get_dbg_level(color) (((color) >> 8) & 0xf)

/*debug level*/
#define DBG_PANIC 			HIGHLIGHT_FLAG << 12 |	LV_PANIC << 8	  | BGC(RED)    | FTC(WHITE)  
#define DBG_ERROR 			HIGHLIGHT_FLAG << 12 |	LV_ERROR << 8	  | BGC(RED)    | FTC(WHITE) | REVERSE_COLOR_FLAG 
#define DBG_FATAL 			HIGHLIGHT_FLAG << 12 |	LV_FATAL << 8	  | BGC(PURPLE) | FTC(WHITE) 
#define DBG_NON1 			HIGHLIGHT_FLAG << 12 |	LV_WARNNING << 8  | BGC(PURPLE) | FTC(WHITE) | REVERSE_COLOR_FLAG 
#define DBG_SUC 			HIGHLIGHT_FLAG << 12 |	LV_SUC << 8		  | BGC(GREEN)  | FTC(WHITE)  
#define DBG_CORRECT 		HIGHLIGHT_FLAG << 12 |	LV_CORRECT << 8   | BGC(GREEN)  | FTC(WHITE) | REVERSE_COLOR_FLAG 
#define DBG_NON2 			HIGHLIGHT_FLAG << 12 |	LV_VIP << 8       | BGC(YELLOW) | FTC(WHITE)  
#define DBG_WARNNING 		HIGHLIGHT_FLAG << 12 |	LV_WARNNING << 8  | BGC(YELLOW) | FTC(WHITE) | REVERSE_COLOR_FLAG 
#define DBG_NON3  			HIGHLIGHT_FLAG << 12 |	LV_NON2 << 8      | BGC(BLUE)   | FTC(WHITE)  
#define DBG_FLOW  			HIGHLIGHT_FLAG << 12 |  LV_FLOW << 8      | BGC(BLUE)   | FTC(WHITE) | REVERSE_COLOR_FLAG 
#define DBG_VIP 			HIGHLIGHT_FLAG << 12 |	LV_VIP << 8       | BGC(BLACK)  | FTC(WHITE)  
#define DBG_IMPORTANT 		HIGHLIGHT_FLAG << 12 |	LV_IMPORTANT << 8 | BGC(BLACK)  | FTC(WHITE) | REVERSE_COLOR_FLAG 
#define DBG_DETAIL 							   		LV_DETAIL << 8	  | BGC(BLACK)  | FTC(WHITE) | REVERSE_COLOR_FLAG
#define DBG_CONTAINER_DETAIL DBG_DETAIL
#define DBG_CONTAINER_WARNNING  DBG_WARNNING 
#define DBG_CONTAINER_ERROR  DBG_ERROR 
#define DBG_CONTAINER_IMPORTANT  DBG_IMPORTANT 
#define DBG_ALLOC_DETAIL DBG_DETAIL
#define DBG_ALLOC_WARNNING  DBG_WARNNING 
#define DBG_ALLOC_ERROR  DBG_ERROR 
#define DBG_ALLOC_IMPORTANT  DBG_IMPORTANT 

/*debug switch*/
#define PDEBUG_LEVEL  LV_DETAIL
#define SDEBUG_LEVEL  LV_DETAIL

#define OPEN_DEBUG 
#define OPEN_PDEBUG        //打开调试信息
#define OPEN_ERR_DEBUG 		//debug packet switch


#ifdef OPEN_PDEBUG           
	#define PS(level, fmt, args...) \
		print_dbg_level(level);\
		printk("[" fmt"]--[%-20s:%d]\n", ## args,__FILE__, __LINE__)
	#define PSB(level,fmt,saddr,slen)\
		print_dbg_level(level);\
		printk("[" fmt);\
		printk_buffer(saddr,slen);\
		printk("]--[%-20s:%d]\n",__FILE__, __LINE__)

	#define PE(fmt,args...) \
		if(3 >= 1) printk("[" fmt"\terror(%d):%s]--[%s:%s:%d]\n",\
			## args,errno,strerror(errno),__FILE__,__PRETTY_FUNCTION__, __LINE__)
#else
	#define PS(level, fmt, args...) do {} while(0)  		//什么也不做
	#define PE(fmt, args...) do {} while(0)
	#define PSB(level,fmt,saddr,slen) do {} while(0)
#endif


#ifdef OPEN_DEBUG
	#define dbg_str(color,fmt,args...)\
		do{\
			if(get_dbg_level(color) <= PDEBUG_LEVEL){\
				PS(color,fmt,##args);\
			}\
		}while(0)
	#define dbg_buf(color,fmt,saddr,slen)\
		do{\
			if(get_dbg_level(color) <= PDEBUG_LEVEL){\
				PSB(color,fmt,saddr,slen);\
			}\
		}while(0)
#else
	#define dbg_str(color,fmt,args...) do{}while(0) 
	#define dbg_buf(color,fmt,saddr,slen) do{}while(0)  
#endif


#endif
