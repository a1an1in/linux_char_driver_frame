/*
 * =====================================================================================
 *
 *       Filename:  allocator_cds_alloc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/15/2015 09:51:15 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __ALLOCATOR_SGI_ALLOC_H__
#define __ALLOCATOR_SGI_ALLOC_H__

#include "libcre/sync_lock/sync_lock.h"
/*
 *#include "basic_types.h"
 */
/*
 *#include "liballoc/list.h"
 */
#include <linux/list.h>

#define SLAB_ARRAY_MAX_NUM 24
#define MEM_POOL_MAX_SIZE 1024*4
#define MEM_POOL_MIN_DEPTH 8

typedef struct cds_mem_pool_head_list{
	uint32_t size;
	uint32_t count;
	/*
	 *pthread_rwlock_t head_lock;
	 */
	sync_lock_t head_lock;    
	struct list_head list_head;
}cds_mempool_head_list_t;
typedef struct cds_mem_pool{
	void *start;
	uint32_t depth;
	uint32_t min_depth;
	uint32_t size;
	//pthread_rwlock_t head_lock;
	struct list_head list_head;
}cds_mempool_t;

typedef struct slab_head_list{
	uint16_t size;
	uint32_t count;
	/*
	 *pthread_rwlock_t head_lock;
	 */
	sync_lock_t head_lock;    
	struct list_head list_head;
}cds_slab_head_list_t;
typedef struct slab{
	uint16_t size;
	uint16_t data_size;
	uint16_t slab_size;
	uint8_t stat_flag;
	uint8_t *mem_addr;
	struct list_head list_head;
	uint8_t data[0];
}cds_slab_t;

typedef struct cds_alloc_s{
	uint32_t slab_array_max_num;
	uint32_t mempool_capacity;
	uint32_t data_min_size;
	struct list_head *empty_pool;
	struct list_head *pool;
	struct list_head **free_slabs;
	struct list_head **used_slabs;
}cds_alloc_t;
/*
 *int allocator_cds_alloc_register();
 */
#endif
