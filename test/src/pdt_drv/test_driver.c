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


void test_pdt_hpi_driver_3008()
{
	int fd;
	allocator_t *allocator;
	protocol_format_set_t *pfs_p;
	uint32_t ret = 0;


	fd = open("/dev/ssc",O_RDWR);
	if(fd == -1){
		perror("open");
		exit(1);
	}
	allocator = allocator_creator(ALLOCATOR_TYPE_SYS_MALLOC,0);

	pfs_p = pfs_create_proto_format_set(allocator);
	init_proto_format_set(0x3000,100,pfs_p);
	pfs_set_pdt_drv_proto_format(pfs_p);

	/*
	 *test_pdt_drv_proto(pfs_p,allocator);
	 */


	struct protocol_analyzer_s *pa;
	uint8_t proto_data2[23] ={
		0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,
		0xb,0xc,0xd,0xe,0xf,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18
	};


	/*set and parse test*/
	pa = pa_create_protocol_analyzer(allocator);
	pa_init_protocol_analyzer(0x3008, pfs_p, pa);

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

	pa_generate_protocol_data(pa);

	ret = write(fd,pa->protocol_data,pa->protocol_data_len);
	if(ret < 0){
		perror("write\n");
	}

	close(fd);
}
void test_pdt_hpi_driver_3010()
{
	int fd;
	allocator_t *allocator;
	protocol_format_set_t *pfs_p;
	uint32_t ret = 0;


	fd = open("/dev/ssc",O_RDWR);
	if(fd == -1){
		perror("open");
		exit(1);
	}
	allocator = allocator_creator(ALLOCATOR_TYPE_SYS_MALLOC,0);

	pfs_p = pfs_create_proto_format_set(allocator);
	init_proto_format_set(0x3000,100,pfs_p);
	pfs_set_pdt_drv_proto_format(pfs_p);

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

	ret = write(fd,proto_buffer,os);
	if(ret < 0){
		perror("write\n");
	}

	close(fd);
}
void test_pdt_hpi_driver_301b()
{
	int fd;
	allocator_t *allocator;
	protocol_format_set_t *pfs_p;
	uint32_t ret = 0;


	fd = open("/dev/ssc",O_RDWR);
	if(fd == -1){
		perror("open");
		exit(1);
	}
	allocator = allocator_creator(ALLOCATOR_TYPE_SYS_MALLOC,0);

	pfs_p = pfs_create_proto_format_set(allocator);
	init_proto_format_set(0x3000,100,pfs_p);
	pfs_set_pdt_drv_proto_format(pfs_p);

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
	proto_buffer[1] = 0x1b;

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


	ret = write(fd,proto_buffer,os + 5);
	if(ret < 0){
		perror("write\n");
	}

	close(fd);

}

void test_hpi_driver_ioctl()
{
	int fd;
	int ret;

	fd = open("/dev/ssc",O_RDWR);
	if(fd == -1){
		perror("open");
		exit(1);
	}
	ret = ioctl(fd,1,0x1234);
	printf("ret=%d\n",ret);
	if(ret == 0){
		printf("set ok\n");
	}else{
		printf("set failed\n");
	}
	
}
void test_pdt_hpi_driver()
{
	/*
	 *test_pdt_hpi_driver_3008();
	 */
	/*
	 *test_pdt_hpi_driver_3010();
	 */
	 /*
	  *test_pdt_hpi_driver_301b();
	  */
	test_hpi_driver_ioctl();
}
