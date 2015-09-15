/*
 * =====================================================================================
 *
 *       Filename:  business.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/15/2015 09:15:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include <linux/types.h>
#include <libproto_analyzer/protocol_analyzer.h>
#include <libproto_analyzer/pdt_drv_proto_format.h>
#include <business/business.h>
#include "chc_admin/chc_admin.h"
#include <libdbg/debug.h>

/*
 *#define PROTO_NUM  "proto_num"
 *#define LAB        "lab"
 *#define LEN        "len"
 *#define COUNT      "count"
 *#define SLOT_INFO  "slot_info"
 *#define SYNC_INFO  "sync_info"
 *#define TACT_INFO  "tact_info"
 *#define SLC_INFO   "slc_info"
 *#define SLC_DATA   "slc_data"
 *#define SYNC_INDEX "sync_index"
 *#define ST_INFO    "st_info"
 *#define DC_INFO    "dc_info"
 *#define DC_DATA    "dc_data"
 */
/*
 *typedef struct task_s{
 *    uint8_t data;
 *    uint8_t type;
 *    uint8_t slot_type;
 *    uint16_t slot_num;
 *}task_t;
 */
/*
 *extern chc_admin_t *chc_admin_slot1_gp;
 *extern chc_admin_t *chc_admin_slot2_gp;
 */
void do_command_0x3008(struct protocol_analyzer_s *pa)
{
	proto_info_list_t *info_list;
	task_t task;
	uint16_t slot_num;
	
	info_list = pa_find_key(SLOT_INFO,pa);
	if(info_list == NULL){
		dbg_str(DBG_WARNNING,"not find key");
	}
	dbg_str(DBG_DETAIL,"slot info:%d",info_list->data);
	slot_num = info_list->data;

	info_list = pa_find_key(TACT_INFO,pa);
	if(info_list == NULL){
		dbg_str(DBG_WARNNING,"not find key");
	}
	dbg_str(DBG_DETAIL,"st:%d",info_list->data);
	task.tact_info = info_list->data;


	info_list = pa_find_key(SLC_DATA,pa);
	if(info_list == NULL){
		dbg_str(DBG_WARNNING,"not find key");
	}
	dbg_str(DBG_DETAIL,"slc data:%x",info_list->data);

	info_list = pa_find_key(DC_DATA,pa);
	if(info_list == NULL){
		dbg_str(DBG_WARNNING,"not find key");
	}
	dbg_buf(DBG_DETAIL,"dc data:",info_list->buf.data_p,info_list->buf.len);

	if(slot_num & 0x1000){
		task.slot_type = SLOT_TYPE_SYS_DESIGNATE;
	}else{
		task.slot_type = SLOT_TYPE_USER_DESIGNATE;
	}
	task.type = TASK_TYPE_DATA;
	if(slot_num&1){
		add_task(chc_admin_slot2_gp,&task,TASK_FLAG_SND);
	}else{
		add_task(chc_admin_slot1_gp,&task,TASK_FLAG_SND);
	}

}
void process_protocol(uint16_t command,struct protocol_analyzer_s *pa)
{
	switch(command){
		case COMMAND_0X3008:
			do_command_0x3008(pa);
			break;
		case COMMAND_0X3000:
		case COMMAND_0X3001:
		case COMMAND_0X3002:
		case COMMAND_0X3003:
		case COMMAND_0X3004:
		case COMMAND_0X3005:
		case COMMAND_0X3006:
		case COMMAND_0X3007:
		case COMMAND_0X3009:
		case COMMAND_0X300A:
		case COMMAND_0X300B:
		case COMMAND_0X300C:
		case COMMAND_0X300D:
		case COMMAND_0X300E:
		case COMMAND_0X300F:
		case COMMAND_0X3010:
		case COMMAND_0X3011:
		case COMMAND_0X3012:
		case COMMAND_0X3013:
		case COMMAND_0X3014:
		case COMMAND_0X3015:
		case COMMAND_0X3016:
		case COMMAND_0X3017:
		case COMMAND_0X3018:
		case COMMAND_0X3019:
		case COMMAND_0X301A:
		case COMMAND_0X301B:
		case COMMAND_0X301C:
		case COMMAND_0X301D:
		case COMMAND_0X301E:
		case COMMAND_0X301F:
			break;
		default:
			break;
	}
}

