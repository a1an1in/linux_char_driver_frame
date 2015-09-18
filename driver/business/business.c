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
#include "libproto_analyzer/protocol_format_set.h"
#include <libproto_analyzer/pdt_drv_proto_format.h>
#include <business/business.h>
#include "task_admin/task_admin.h"
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
extern allocator_t *allocator;
extern protocol_format_set_t *pfs_p;

static inline void 
process_tact_info(struct protocol_analyzer_s *pa,
		task_t *task)
{
	proto_info_list_t *info_list;

	info_list = pa_find_key(TACT_INFO,pa);
	if(info_list == NULL){
		dbg_str(DBG_WARNNING,"not find key");
	}
	dbg_str(DBG_DETAIL,"st:%d",info_list->data);
	task->tact_info = info_list->data;
}
static inline void 
process_slot_info(struct protocol_analyzer_s *pa,
		task_t *task)
{
	proto_info_list_t *info_list;

	info_list = pa_find_key(SLOT_INFO,pa);
	if(info_list == NULL){
		dbg_str(DBG_WARNNING,"not find key");
	}
	dbg_str(DBG_DETAIL,"slot info:%d",info_list->data);
	task->slot_num = info_list->data;
}
static inline void 
process_slc_data(struct protocol_analyzer_s *pa,
		task_t *task)
{
	proto_info_list_t *info_list;

	info_list = pa_find_key(SLC_DATA,pa);
	if(info_list == NULL){
		dbg_str(DBG_WARNNING,"not find key");
	}
	dbg_str(DBG_DETAIL,"slc data:%x",info_list->data);
}
static inline void 
process_dc_data(struct protocol_analyzer_s *pa,
		task_t *task)
{
	proto_info_list_t *info_list;

	info_list = pa_find_key(DC_DATA,pa);
	if(info_list == NULL){
		dbg_str(DBG_WARNNING,"not find key");
	}
	dbg_buf(DBG_DETAIL,"dc data:",info_list->buf.data_p,info_list->buf.len);
}
void do_command_0x3008(uint8_t *data,uint32_t len)
{
	proto_info_list_t *info_list;
	task_t task;
	struct protocol_analyzer_s *pa;

	pa = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3008, pfs_p, pa);
	memcpy(pa->protocol_data,data,len);
	pa->protocol_data_len = len;
	pa_parse_protocol_data(pa);

	process_slot_info(pa,&task);
	process_tact_info(pa,&task);
	process_slc_data(pa,&task);
	process_dc_data(pa,&task);

	if(task.slot_num & 0x1000){
		task.slot_type = SLOT_TYPE_SYS_DESIGNATE;
	}else{
		task.slot_type = SLOT_TYPE_USER_DESIGNATE;
	}
	task.type = TASK_TYPE_DATA;
	if(task.slot_num&1){
		add_task(task_admin_slot2_gp,&task,TASK_FLAG_SND);
	}else{
		add_task(task_admin_slot1_gp,&task,TASK_FLAG_SND);
	}

}
void do_command_0x3010(uint8_t *data,uint32_t len)
{
#define MAX_PROTO_BUFFER_LEN 1024
	struct protocol_analyzer_s *pa;
	uint8_t proto_buffer[MAX_PROTO_BUFFER_LEN];
	uint16_t proto_buffer_len;
	uint8_t count,i;
	uint16_t os = 6,peri_data_len;
	uint8_t emb_flag;


	pa = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3054, pfs_p, pa);

	count = data[5];
	for(i = 0; i < count; i++){
		peri_data_len = data[os + 2];
		pa->protocol_data[0] = 0x30;
		pa->protocol_data[1] = 0x54;
		memcpy(pa->protocol_data + 2 ,data + os,peri_data_len + 3);
		pa->protocol_data_len = peri_data_len + 3 + 2;
		dbg_buf(DBG_DETAIL,"parse data:",pa->protocol_data,pa->protocol_data_len);
		pa_parse_protocol_data(pa);
		dbg_str(DBG_IMPORTANT,"print list for each after parse");
		pfs_print_list_for_each(pa->pa_list_head_p);
		os += peri_data_len + 3;
		pa_reset_vlen_flag(pa);
	}
#undef MAX_PROTO_BUFFER_LEN
}
void do_command_0x301b(uint8_t *data,uint32_t len)
{
#define MAX_PROTO_BUFFER_LEN 1024
	struct protocol_analyzer_s *pa50,*pa52,*pa;
	uint8_t proto_buffer[MAX_PROTO_BUFFER_LEN];
	uint16_t proto_buffer_len;
	uint8_t count,i;
	uint16_t os = 5,pdu_len;
	uint8_t emb_flag;

	pa50 = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3050, pfs_p, pa50);
	pa52 = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3052, pfs_p, pa52);

	count = data[4];
	for(i = 0; i < count; i++){
		pdu_len = data[os];
		emb_flag = data[os + 1];
		if(emb_flag == 0){
			pa = pa50;
			pa->protocol_data[0] = 0x30;
			pa->protocol_data[1] = 0x50;
		}else{
			pa = pa52;
			pa->protocol_data[0] = 0x30;
			pa->protocol_data[1] = 0x52;
		}
		memcpy(pa->protocol_data + 2 ,data + os,pdu_len);
		pa->protocol_data_len = pdu_len + 2;
		dbg_buf(DBG_DETAIL,"parse data:",pa->protocol_data,pa->protocol_data_len);
		pa_parse_protocol_data(pa);
		dbg_str(DBG_IMPORTANT,"print list for each after parse");
		pfs_print_list_for_each(pa->pa_list_head_p);
		os += pdu_len;
		pa_reset_vlen_flag(pa);
	}

#undef MAX_PROTO_BUFFER_LEN
}
void process_protocol(uint16_t command,uint8_t *data,uint32_t len)
{
	switch(command){
		case COMMAND_0X3008:
			do_command_0x3008(data,len);
			break;
		case COMMAND_0X3010:
			do_command_0x3010(data,len);
			break;
		case COMMAND_0X301B:
			do_command_0x301b(data,len);
			break;
		case COMMAND_0X3000:
		case COMMAND_0X3002:
		case COMMAND_0X3004:
		case COMMAND_0X3006:
		case COMMAND_0X300A:
		case COMMAND_0X300C:
		case COMMAND_0X300E:
		case COMMAND_0X3012:
		case COMMAND_0X3014:
		case COMMAND_0X3016:
		case COMMAND_0X3018:
		case COMMAND_0X301A:
		case COMMAND_0X301C:
		case COMMAND_0X301E:
			break;
		default:
			break;
	}
}

