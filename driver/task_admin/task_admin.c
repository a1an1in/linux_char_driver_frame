/*
 * =====================================================================================
 *
 *       Filename:  task admin.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/14/2015 11:33:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "task_admin/task_admin.h"
#include "libdata_structure/hash_list.h"
#include "libdata_structure/link_list.h"
#include "libdbg/debug.h"
/*
 *typedef struct task_s{
 *    uint8_t data;
 *    uint16_t type;
 *    uint16_t slot;
 *}task_t;
 *typedef struct task_admin_s{
 *    uint32_t slot;
 *    llist_t *data_sys_designate_slot;
 *    llist_t *voice_sys_designate_slot;
 *    hash_map_t *data_user_designate_slot;
 *    hash_map_t *voice_user_designate_slot;
 *}task_admin_t;
 */

task_admin_t *task_admin_slot1_gp;
task_admin_t *task_admin_slot2_gp;

task_admin_t *create_task_admin()
{
	allocator_t *allocator = NULL;
	task_admin_t *admin = NULL;
	llist_t *llist = NULL;
	hash_map_t *hmap = NULL;
	uint32_t key_size,data_size,bucket_size;
	pair_t *pair;

	allocator = allocator_creator(ALLOCATOR_TYPE_SYS_MALLOC,0);
	if(allocator == NULL){
		dbg_str(DBG_ERROR,"allocator_creator error");
		goto err1;
	}
	//some alloc need init
	//.....
	
	admin = allocator_mem_alloc(allocator,sizeof(task_admin_t));
	if(admin == NULL){
		dbg_str(DBG_ERROR,"allocator_mem_alloc");
		goto err2;
	}
	admin->allocator = allocator;

	llist = llist_create(allocator,0);
	if(llist == NULL){
		dbg_str(DBG_ERROR,"llist_create");
		goto err3;
	}
	llist_init(llist,sizeof(task_t));
	admin->data_sys_designate_slot = llist;

	
	llist = llist_create(allocator,0);
	if(llist == NULL){
		dbg_str(DBG_ERROR,"llist_create");
		goto err4;
	}
	llist_init(llist,sizeof(task_t));
	admin->voice_sys_designate_slot = llist;


	hmap = hash_map_create(allocator,0);
	if(hmap == NULL){
		dbg_str(DBG_ERROR,"hash_map_create");
		goto err5;
	}
	key_size = 2;
	data_size = sizeof(task_t);
	bucket_size = 10;
	hash_map_init(hmap, key_size,//uint32_t key_size,
			data_size + key_size,//uint32_t data_size
			bucket_size,//uint32_t bucket_size
			default_hash_func,
			default_key_cmp_func);
	admin->data_user_designate_slot = hmap;

	
	hmap = hash_map_create(allocator,0);
	if(hmap == NULL){
		dbg_str(DBG_ERROR,"hash_map_create");
		goto err6;
	}
	key_size = 2;
	data_size = sizeof(task_t);
	bucket_size = 10;
	hash_map_init(hmap, key_size,//uint32_t key_size,
			data_size + key_size,//uint32_t data_size
			bucket_size,//uint32_t bucket_size
			default_hash_func,
			default_key_cmp_func);
	admin->voice_user_designate_slot = hmap;

	pair = create_pair(key_size,data_size);
	if(pair == NULL){
		dbg_str(DBG_ERROR,"hash_map_create");
		goto err7;
	}
	admin->pair = pair;

	return admin;

err7:
	hash_map_destroy(admin->voice_user_designate_slot);
err6:
	hash_map_destroy(admin->data_user_designate_slot);
err5:
	llist_destroy(admin->voice_sys_designate_slot);
err4:
	llist_destroy(admin->data_sys_designate_slot);
err3:
	allocator_mem_free(allocator,admin);
err2:
	allocator_destroy(allocator);
err1:
	return NULL;
}

void destroy_task_admin(task_admin_t *admin)
{
	allocator_t *allocator = admin->allocator;;

	hash_map_destroy(admin->voice_user_designate_slot);
	hash_map_destroy(admin->data_user_designate_slot);
	llist_destroy(admin->voice_sys_designate_slot);
	llist_destroy(admin->data_sys_designate_slot);
	allocator_mem_free(allocator,admin);
	allocator_destroy(allocator);
}
/**
 * @brief  		add task to relevant list
 *
 * @param admin
 * @param task  task data 
 * @param flag  add to rcv or snd task list
 */
void add_task(task_admin_t *admin,task_t *task,uint8_t flag)
{
	/*
	 *SLOT_TYPE_SYS_DESIGNATE = 0,
	 *SLOT_TYPE_USER_DESIGNATE
	 */
	/*
	 *enum task_type{
	 *    TASK_TYPE_DATA = 0,
	 *    TASK_TYPE_VOICE,
	 *    TASK_TYPE_RC
	 *};
	 */
	if(flag == TASK_FLAG_RCV){
		dbg_str(DBG_WARNNING,"not support,now");

	}else if(flag == TASK_FLAG_SND){
		if(task->slot_type == SLOT_TYPE_SYS_DESIGNATE && task->type == TASK_TYPE_DATA){
			dbg_str(DBG_DETAIL,"SLOT_TYPE_SYS_DESIGNATE, TASK_TYPE_DATA");
			llist_push_back(admin->data_sys_designate_slot,task);
		}else if(task->slot_type == SLOT_TYPE_SYS_DESIGNATE && task->type == TASK_TYPE_VOICE){
			llist_push_back(admin->voice_sys_designate_slot,task);
		}else if(task->slot_type == SLOT_TYPE_USER_DESIGNATE && task->type == TASK_TYPE_DATA){
			make_pair(admin->pair,task->slot_num,task);
			hash_map_insert(admin->data_user_designate_slot,admin->pair->data);
		}else if(task->slot_type == SLOT_TYPE_USER_DESIGNATE && task->type == TASK_TYPE_VOICE){
			make_pair(admin->pair,task->slot_num,task);
			hash_map_insert(admin->voice_user_designate_slot,admin->pair->data);
		}else{
			dbg_str(DBG_WARNNING,"task type err");
		}

	}else{
		dbg_str(DBG_ERROR,"not support");
		return;
	}

}


/*
 *enum task_state{
 *    CHC_STATE_TX = 0,
 *    CHC_STATE_RX,
 *    CHC_STATE_IDLE,
 *    CHC_STATE_MAINC_CHANNEL,
 *    CHC_STATE_VOICE_CHANNEL,
 *    CHC_STATE_VOICE_TX_BEGIN,
 *    CHC_STATE_VOICE_TX_END,
 *    CHC_STATE_VOICE_RX_BEGIN,
 *    CHC_STATE_VOICE_RX_END,
 *    CHC_STATE_DATA_CHANNEL,
 *    CHC_STATE_DATA_TX_BEGIN,
 *    CHC_STATE_DATA_TX_EDN,
 *    CHC_STATE_DATA_RX_BEGIN,
 *    CHC_STATE_DATA_RX_EDN,
 *};
 */
static inline int8_t get_task_state(task_admin_t *admin,uint8_t type)
{
	return 1;
}
int get_task(task_admin_t *admin,uint8_t flag,uint16_t slot_num,task_t *task)
{
	int ret = -1;
	list_t *list;
	hash_map_pos_t pos;
	uint8_t key[2];

	if(flag == TASK_FLAG_RCV){
		dbg_str(DBG_WARNNING,"not support,now");
	}else if(flag == TASK_FLAG_SND){
		if(get_task_state(admin,CHC_STATE_MAINC_CHANNEL)){
			key[0] = (slot_num >> 8) && 0xff;
			key[1] = slot_num && 0xff;
			pos = hash_map_search(admin->data_user_designate_slot, key);
			if(pos.hlist_node_p == NULL){
			}else{
				return 1;
			}
			list= llist_detach_front(admin->data_sys_designate_slot);
			if(list == NULL){
			}else{
				return 1;
			}
			//... periodic task
		}else if(get_task_state(admin,CHC_STATE_VOICE_CHANNEL)){
		}else if(get_task_state(admin,CHC_STATE_DATA_CHANNEL)){
		}else{
			dbg_str(DBG_ERROR,"not support");
		}
	}else{
		dbg_str(DBG_ERROR,"not support");
	}

	return ret;
}

