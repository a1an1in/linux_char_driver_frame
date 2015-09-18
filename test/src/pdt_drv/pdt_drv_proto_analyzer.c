#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <math.h>
#include "libdbg/debug.h"
#include "liballoc/allocator.h"
#include "libproto_analyzer/protocol_format_set.h"
#include "libproto_analyzer/protocol_analyzer.h"
#include "pdt_drv_proto_format.h"

debugger_t *debugger_gp;
allocator_t *allocator;

int test_pdt_proto_3008(protocol_format_set_t *pfs_p,
		allocator_t *allocator)
{
	int ret = 0;
	struct protocol_analyzer_s *pa;
	uint8_t proto_data2[23] ={
		0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,
		0xb,0xc,0xd,0xe,0xf,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18
	};


	/*set and parse test*/
	pa = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3008, pfs_p, pa);
	/*
	 *pa = proto_analyer_admin_get_pa(pa_admin,0x3008);
	 */

	pa_set_value(PROTO_NUM,0x3008,pa);
	pa_set_value(LAB,0x1834,pa);
	pa_set_value(SLOT_INFO,0x1234,pa);
	pa_set_value(SYNC_INFO,0,pa);
	pa_set_value(TACT_INFO,2,pa);
	pa_set_variable_length_value(SLC_INFO,4,pa);
	pa_set_value(SLC_DATA,0xaabbccdd,pa);
	pa_set_value(SYNC_INDEX,0x24,pa);
	pa_set_value(ST_INFO,0x32,pa);
	pa_set_variable_length_value(DC_INFO,23,pa);
	pa_set_buf(DC_DATA,proto_data2,23,pa);

	dbg_str(DBG_IMPORTANT,"print list for each,orinal data");
	pfs_print_list_for_each(pa->pa_list_head_p);

	pa_generate_protocol_data(pa);

	dbg_str(DBG_DETAIL,"pa_parse_protocol_data");
	pa_parse_protocol_data(pa);

	dbg_str(DBG_IMPORTANT,"print list for each after parse");
	pfs_print_list_for_each(pa->pa_list_head_p);

	dbg_str(DBG_DETAIL,"pa_destroy_protocol_analyzer");
	pa_destroy_protocol_analyzer(pa);

	return ret;
}

int __pdt_drv_proto_init_3019(uint16_t proto_id,uint16_t lab,
		uint16_t slot_info,uint16_t sync_index,uint8_t st_info,
		uint8_t dc_info,uint8_t *dc_data,
		struct protocol_analyzer_s *pa)
{
	pa_set_value(PROTO_NUM,proto_id,pa);
	pa_set_value(LAB,lab,pa);
	pa_set_value(SLOT_INFO,slot_info,pa);
	pa_set_value(SYNC_INDEX,sync_index,pa);
	pa_set_value(ST_INFO,st_info,pa);
	pa_set_variable_length_value(DC_INFO,dc_info,pa);
	pa_set_buf(DC_DATA,dc_data,dc_info,pa);

	return 0;
}
int pdt_drv_proto_init_3019(uint16_t proto_id,uint16_t lab,
		uint16_t slot_info,uint16_t sync_index,uint8_t st_info,
		uint8_t buffer_count,buffer_t *buffer_p,
		struct protocol_analyzer_s *pa)
{
#define MAX_PROTO_BUFFER_LEN 1024
	uint8_t proto_buffer[MAX_PROTO_BUFFER_LEN];
#undef MAX_PROTO_BUFFER_LEN
	uint16_t len,os = 0;
	int i;

	for(i = 0; i < buffer_count; i++){
		__pdt_drv_proto_init_3019(proto_id,lab, slot_info + i*2,sync_index, st_info,
				buffer_p[i].len ,buffer_p[i].data_p,pa);
		pa_generate_protocol_data(pa);
		len = pa->protocol_data_len - 5;
		memcpy(proto_buffer + os,pa->protocol_data + 5, len);
		dbg_str(DBG_DETAIL,"len=%d",len);
		proto_buffer[os] = len;
		os += len;
		pa_reset_vlen_flag(pa);//the varible len may not equal the var len before,so it need be cleared
	}
	dbg_buf(DBG_DETAIL,"proto_buffer:",proto_buffer,os);
	pa->protocol_data[4] = buffer_count;
	memcpy(pa->protocol_data + 5,proto_buffer,os); 
	pa->protocol_data_len = os + 5;
	dbg_buf(DBG_DETAIL,"proto_data:",pa->protocol_data,pa->protocol_data_len);
}
typedef struct pa_array_s{
	int count;
	struct protocol_analyzer_s **pa_p2;
}pa_array_t;

pa_array_t 
pdt_drv_proto_parse_3019(protocol_format_set_t *pfs_p,
		allocator_t *allocator,buffer_t *buffer_p)
{
	struct protocol_analyzer_s *pa;
#define MAX_PROTO_BUFFER_LEN 1024
	uint8_t proto_buffer[MAX_PROTO_BUFFER_LEN];
	uint16_t proto_buffer_len;
#undef MAX_PROTO_BUFFER_LEN
	uint8_t count,i;
	uint16_t os = 5,pdu_len;


	dbg_buf(DBG_DETAIL,"parse_3019:",buffer_p->data_p,buffer_p->len);
	pa = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3019, pfs_p, pa);

	count =	buffer_p->data_p[4];
	for(i = 0; i < count; i++){
		pdu_len = buffer_p->data_p[os];
		memcpy(pa->protocol_data,buffer_p->data_p,5);
		dbg_str(DBG_DETAIL,"os=%d",os);
		memcpy(pa->protocol_data + 5,buffer_p->data_p + os,pdu_len);
		pa->protocol_data_len = pdu_len + 5;
		dbg_buf(DBG_DETAIL,"parse data:",pa->protocol_data,pa->protocol_data_len);
		pa_parse_protocol_data(pa);
		dbg_str(DBG_IMPORTANT,"print list for each after parse");
		pfs_print_list_for_each(pa->pa_list_head_p);
		os += pdu_len;
	}
}
int test_pdt_proto_3019(protocol_format_set_t *pfs_p,
		allocator_t *allocator)
{
	int ret = 0;
	struct protocol_analyzer_s *pa;
	uint8_t proto_data2[24] ={
		0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,
		0xb,0xc,0xd,0xe,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18
	};
	uint16_t len;
	buffer_t buf[4];
	uint8_t i;
	uint8_t size = 23;
	buffer_t data;


	/*set and parse test*/
	pa = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3019, pfs_p, pa);

	
	for(i = 0; i < 4; i++){
		buf[i].data_p = allocator_mem_alloc(allocator,size);
		memcpy(buf[i].data_p,proto_data2,size);
		buf[i].len = size;
	}

	pdt_drv_proto_init_3019(0x3019,0x1234,
			0x5678,0x9012,0x34, 4,buf, pa);

	data.data_p = allocator_mem_alloc(allocator,pa->protocol_data_len);
	memcpy(data.data_p,pa->protocol_data,pa->protocol_data_len);
	data.len = pa->protocol_data_len;
	pdt_drv_proto_parse_3019(pfs_p,allocator,&data);
	/*
	 *dbg_str(DBG_DETAIL,"pa_destroy_protocol_analyzer");
	 *pa_destroy_protocol_analyzer(pa);
	 */

	return ret;

}

int __pdt_drv_proto_init_3050(
		uint8_t len, uint16_t slot_info,
		uint16_t sync_info,uint8_t tact_info,uint8_t slc_info,
		uint8_t *slc_data, uint8_t sync_index,uint8_t st_info,
		uint8_t dc_info,uint8_t *dc_data,
		struct protocol_analyzer_s *pa)
{
	pa_set_value(PROTO_NUM,0x3050,pa);
	pa_set_value(LEN,len,pa);
	pa_set_value(EMB_FLAG,0,pa);
	pa_set_value(SLOT_INFO,slot_info,pa);
	pa_set_value(SYNC_INFO,sync_info,pa);
	pa_set_value(TACT_INFO,tact_info,pa);
	pa_set_variable_length_value(SLC_INFO,slc_info,pa);
	pa_set_buf(SLC_DATA,slc_data,slc_info,pa);
	pa_set_value(SYNC_INDEX,sync_index,pa);
	pa_set_value(ST_INFO,st_info,pa);
	pa_set_variable_length_value(DC_INFO,dc_info,pa);
	pa_set_buf(DC_DATA,dc_data,dc_info,pa);

	return 0;
}
int __pdt_drv_proto_init_3052(
		uint8_t len, uint16_t slot_info,
		uint16_t sync_info,uint8_t tact_info,uint8_t slc_info,
		uint8_t *slc_data, uint8_t emb_info,uint8_t lc_info,
		uint8_t *lc_data, uint8_t dc_info,uint8_t *dc_data,
		struct protocol_analyzer_s *pa)
{
	pa_set_value(PROTO_NUM,0x3052,pa);
	pa_set_value(LEN,len,pa);
	pa_set_value(EMB_FLAG,1,pa);
	pa_set_value(SLOT_INFO,slot_info,pa);
	pa_set_value(SYNC_INFO,sync_info,pa);
	pa_set_value(TACT_INFO,tact_info,pa);
	pa_set_variable_length_value(SLC_INFO,slc_info,pa);
	pa_set_buf(SLC_DATA,slc_data,slc_info,pa);
	pa_set_value(EMB_INFO,emb_info,pa);
	pa_set_variable_length_value(LC_INFO,lc_info,pa);
	pa_set_buf(LC_DATA,lc_data,lc_info,pa);
	pa_set_variable_length_value(DC_INFO,dc_info,pa);
	pa_set_buf(DC_DATA,dc_data,dc_info,pa);

	return 0;
}
void pdt_drv_proto_parse_301b(protocol_format_set_t *pfs_p,
		allocator_t *allocator,uint8_t *data,uint32_t len)
{
	struct protocol_analyzer_s *pa50,*pa52,*pa;
#define MAX_PROTO_BUFFER_LEN 1024
	uint8_t proto_buffer[MAX_PROTO_BUFFER_LEN];
	uint16_t proto_buffer_len;
#undef MAX_PROTO_BUFFER_LEN
	uint8_t count,i;
	uint16_t os = 5,pdu_len;
	uint8_t emb_flag;


	dbg_buf(DBG_DETAIL,"parse_301b:",data,len);
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
	}
}
void test_pdt_proto_301b(protocol_format_set_t *pfs_p,
		allocator_t *allocator)
{
	int ret = 0;
	struct protocol_analyzer_s *pa50,*pa52,*pa;
	uint8_t dc_data[24] ={
		0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,
		0xb,0xc,0xd,0xe,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18
	};
	uint16_t len;
	buffer_t buf[4];
	uint8_t i;
	uint8_t size = 23;
	buffer_t data;
	uint8_t proto_buffer[1024];
	uint16_t os = 0;


	proto_buffer[0] = 0x30;
	proto_buffer[1] = 0x24;

	/*set and parse test*/
	pa50 = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3050, pfs_p, pa50);

	pa52 = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3052, pfs_p, pa52);

	pa = pa50;
	__pdt_drv_proto_init_3050(
			0,//uint8_t len,
			0x1234,//uint16_t slot_info,
			0,//uint16_t sync_info,
			0x67,//uint8_t tact_info,
			0xa,//uint8_t slc_info,
			dc_data,//uint8_t *slc_data, 
			0,//uint8_t sync_index,
			0x98,//uint8_t st_info,
			23,//uint8_t dc_info,
			dc_data,//uint8_t *dc_data,
			pa);//struct protocol_analyzer_s *pa)

	pa_generate_protocol_data(pa);
	memcpy(proto_buffer+5+os,pa->protocol_data + 2,pa->protocol_data_len - 2);
	proto_buffer[5 + os] = pa->protocol_data_len - 2;
	os += pa->protocol_data_len - 2;
	
	pa = pa52;
	__pdt_drv_proto_init_3052(
			0,//uint8_t len,
			0x1234,//uint16_t slot_info,
			0,//uint16_t sync_info,
			0x67,//uint8_t tact_info,
			0xa,//uint8_t slc_info,
			dc_data,//uint8_t *slc_data, 
			0x23,//emb_info
			5,//lc_info
			dc_data,//lc_data
			23,//uint8_t dc_info,
			dc_data,//uint8_t *dc_data,
			pa);//struct protocol_analyzer_s *pa)

	pa_generate_protocol_data(pa);
	memcpy(proto_buffer+5+os,pa->protocol_data + 2,pa->protocol_data_len - 2);
	proto_buffer[5 + os] = pa->protocol_data_len - 2;
	os += pa->protocol_data_len - 2;

	pa = pa50;
	__pdt_drv_proto_init_3050(
			0,//uint8_t len,
			0x1234,//uint16_t slot_info,
			0,//uint16_t sync_info,
			0x67,//uint8_t tact_info,
			0xa,//uint8_t slc_info,
			dc_data,//uint8_t *slc_data, 
			0,//uint8_t sync_index,
			0x98,//uint8_t st_info,
			23,//uint8_t dc_info,
			dc_data,//uint8_t *dc_data,
			pa);//struct protocol_analyzer_s *pa)

	pa_generate_protocol_data(pa);
	memcpy(proto_buffer+5+os,pa->protocol_data + 2,pa->protocol_data_len - 2);
	proto_buffer[5 + os] = pa->protocol_data_len - 2;
	os += pa->protocol_data_len - 2;


	proto_buffer[4] = 0x3;
	dbg_buf(DBG_DETAIL,"proto_buffer:",proto_buffer,os + 5);
	/*
	 *pa_parse_protocol_data(pa);
	 *dbg_str(DBG_IMPORTANT,"print list for each after parse");
	 *pfs_print_list_for_each(pa->pa_list_head_p);
	 */

	pdt_drv_proto_parse_301b(pfs_p, allocator,proto_buffer,os + 5);
}


/**
 * @brief  procol_0x3010 internal protocol
 *
 * @param no_name1
 * @param st_info
 * @param dc_info
 * @param dc_data
 * @param pa
 *
 * @return   
 */
int __pdt_drv_proto_init_3054(
		uint8_t no_name1, uint8_t st_info, uint8_t dc_info,uint8_t *dc_data,
		struct protocol_analyzer_s *pa)
{
	pa_set_value(PROTO_NUM,0x3054,pa);
	pa_set_value(NO_NAME1,no_name1,pa);
	pa_set_value(ST_INFO,st_info,pa);
	pa_set_variable_length_value(DC_INFO,dc_info,pa);
	pa_set_buf(DC_DATA,dc_data,dc_info,pa);

	return 0;
}
void pdt_drv_proto_parse_3010(protocol_format_set_t *pfs_p,
		allocator_t *allocator,uint8_t *data,uint32_t len)
{
	struct protocol_analyzer_s *pa;
#define MAX_PROTO_BUFFER_LEN 1024
	uint8_t proto_buffer[MAX_PROTO_BUFFER_LEN];
	uint16_t proto_buffer_len;
#undef MAX_PROTO_BUFFER_LEN
	uint8_t count,i;
	uint16_t os = 6,peri_data_len;
	uint8_t emb_flag;


	dbg_buf(DBG_DETAIL,"parse_3010:",data,len);
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
	}
}

void test_pdt_proto_3010(protocol_format_set_t *pfs_p,
		allocator_t *allocator)
{

	int ret = 0;
	struct protocol_analyzer_s *pa;
	uint8_t dc_data[24] ={
		0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,
		0xb,0xc,0xd,0xe,0xf,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18
	};
	uint16_t len;
	uint8_t i;
	uint8_t size = 23;
	buffer_t data;
	uint8_t proto_buffer[1024];
	uint16_t os = 6;


	proto_buffer[0] = 0x30;
	proto_buffer[1] = 0x10;

	/*set and parse test*/
	pa = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3054, pfs_p, pa);

	__pdt_drv_proto_init_3054(
			0x11,//uint8_t no_name1,
			0x22,//uint8_t st_info,
			5,//uint8_t dc_info,
			dc_data,//uint8_t *dc_data,
			pa);//struct protocol_analyzer_s *pa)

	pa_generate_protocol_data(pa);
	memcpy(proto_buffer+os,pa->protocol_data + 2,pa->protocol_data_len - 2);
	os += pa->protocol_data_len - 2;

	__pdt_drv_proto_init_3054(
			0x11,//uint8_t no_name1,
			0x22,//uint8_t st_info,
			5,//uint8_t dc_info,
			dc_data,//uint8_t *dc_data,
			pa);//struct protocol_analyzer_s *pa)

	pa_generate_protocol_data(pa);
	memcpy(proto_buffer+os,pa->protocol_data + 2,pa->protocol_data_len - 2);
	os += pa->protocol_data_len - 2;

	__pdt_drv_proto_init_3054(
			0x11,//uint8_t no_name1,
			0x22,//uint8_t st_info,
			5,//uint8_t dc_info,
			dc_data,//uint8_t *dc_data,
			pa);//struct protocol_analyzer_s *pa)

	pa_generate_protocol_data(pa);
	memcpy(proto_buffer+os,pa->protocol_data + 2,pa->protocol_data_len - 2);
	os += pa->protocol_data_len - 2;

	proto_buffer[4] = 0x3;
	proto_buffer[5] = 0x3;
	dbg_buf(DBG_DETAIL,"proto_buffer:",proto_buffer,os);

	pdt_drv_proto_parse_3010(pfs_p, allocator,proto_buffer,os);
}

int test_pdt_drv_proto(protocol_format_set_t *pfs_p, allocator_t *allocator)
{
	/*
	 *test_pdt_proto_3008(pfs_p,allocator);
	 */
	/*
	 *test_pdt_proto_3019(pfs_p,allocator);
	 */
	/*
	 *test_pdt_proto_301b(pfs_p, allocator);
	 */
	 test_pdt_proto_3010(pfs_p, allocator);
}
int test_pdt_proto_analyzer()
{
	protocol_format_set_t *pfs_p;
	uint32_t ret = 0;

	allocator = allocator_creator(ALLOCATOR_TYPE_SYS_MALLOC,0);

	pfs_p = pfs_create_proto_format_set(allocator);
	init_proto_format_set(0x3000,100,pfs_p);
	pfs_set_pdt_drv_proto_format(pfs_p);

	test_pdt_drv_proto(pfs_p,allocator);

	dbg_str(DBG_DETAIL,"pfs_destroy_protocol_format_set");
	pfs_destroy_protocol_format_set(pfs_p);


	return ret;
}
