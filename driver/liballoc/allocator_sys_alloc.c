/*  
 * Copyright (c) 2015-2020 alan lin <a1an1in@sina.com>
 *  
 *  
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#include <linux/mm.h>
#include <libdbg/debug.h>
#include "liballoc/inc_files.h"

void *sys_alloc_alloc(allocator_t *alloc,uint32_t size)
{
	/*
	 *dbg_str(DBG_DETAIL,"sys_alloc");
	 */
	return kmalloc(size,GFP_KERNEL);
}
void sys_alloc_free(allocator_t *alloc,void *addr)
{
	kfree(addr);
}
int allocator_sys_alloc_register(){
	allocator_module_t salloc = {
		.allocator_type = ALLOCATOR_TYPE_SYS_MALLOC,
		.alloc_ops = {
			.init    = NULL,
			.alloc   = sys_alloc_alloc,
			.free    = sys_alloc_free,
			.destroy = NULL,
		}
	};
	memcpy(&allocator_modules[ALLOCATOR_TYPE_SYS_MALLOC],&salloc,sizeof(allocator_module_t));
	return 0;
}
