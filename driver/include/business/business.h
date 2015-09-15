/*
 * =====================================================================================
 *
 *       Filename:  business.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/15/2015 09:15:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  alan lin (), a1an1in@sina.com
 *   Organization:  
 *
 * =====================================================================================
 */

#define COMMAND_0X3000  0x3000 
#define COMMAND_0X3001  0x3001 
#define COMMAND_0X3002  0x3002 
#define COMMAND_0X3003  0x3003 
#define COMMAND_0X3004  0x3004 
#define COMMAND_0X3005  0x3005 
#define COMMAND_0X3006  0x3006 
#define COMMAND_0X3007  0x3007 
#define COMMAND_0X3008  0x3008 
#define COMMAND_0X3009  0x3009 
#define COMMAND_0X300A  0x300A
#define COMMAND_0X300B  0x300B
#define COMMAND_0X300C  0x300C
#define COMMAND_0X300D  0x300D
#define COMMAND_0X300E  0x300E
#define COMMAND_0X300F  0x300F

#define COMMAND_0X3010  0x3010 
#define COMMAND_0X3011  0x3011 
#define COMMAND_0X3012  0x3012 
#define COMMAND_0X3013  0x3013 
#define COMMAND_0X3014  0x3014 
#define COMMAND_0X3015  0x3015 
#define COMMAND_0X3016  0x3016 
#define COMMAND_0X3017  0x3017 
#define COMMAND_0X3018  0x3018 
#define COMMAND_0X3019  0x3019 
#define COMMAND_0X301A  0x301A
#define COMMAND_0X301B  0x301B
#define COMMAND_0X301C  0x301C
#define COMMAND_0X301D  0x301D
#define COMMAND_0X301E  0x301E
#define COMMAND_0X301F  0x301F

void process_protocol(uint16_t command,struct protocol_analyzer_s *pa);
