/*
 * =====================================================================================
 *
 *       Filename:  task.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/14/2015 11:33:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <liballoc/allocator.h>
#include <libdata_structure/link_list_struct.h>
#include <libdata_structure/hash_list_struct.h>

enum task_flag{
	TASK_FLAG_RCV = 0,
	TASK_FLAG_SND
};
enum task_type{
	TASK_TYPE_DATA = 0,
	TASK_TYPE_VOICE,
	TASK_TYPE_RC
};
enum slot_type{
	SLOT_TYPE_SYS_DESIGNATE = 0,
	SLOT_TYPE_USER_DESIGNATE
};
enum chc_state{
	CHC_STATE_TX = 0,
	CHC_STATE_RX,
	CHC_STATE_IDLE,
	CHC_STATE_MAINC_CHANNEL,
	CHC_STATE_VOICE_CHANNEL,
	CHC_STATE_VOICE_TX_BEGIN,
	CHC_STATE_VOICE_TX_END,
	CHC_STATE_VOICE_RX_BEGIN,
	CHC_STATE_VOICE_RX_END,
	CHC_STATE_DATA_CHANNEL,
	CHC_STATE_DATA_TX_BEGIN,
	CHC_STATE_DATA_TX_EDN,
	CHC_STATE_DATA_RX_BEGIN,
	CHC_STATE_DATA_RX_EDN,
};
typedef struct task_s{
	uint8_t data;
	uint8_t type;
	uint8_t slot_type;
	uint16_t slot_num;
}task_t;

typedef struct chc_admin_s{
	uint32_t slot;
	allocator_t *allocator;
	pair_t *pair;
	uint32_t chc_state;

	llist_t *data_sys_designate_slot;
	llist_t *voice_sys_designate_slot;
	hash_map_t *data_user_designate_slot;
	hash_map_t *voice_user_designate_slot;

}chc_admin_t;
