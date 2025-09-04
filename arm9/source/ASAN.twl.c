//Copyright (C) 2025 Dominik Kurz

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation; either
//version 3 of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//Lesser General Public License for more details.

//You should have received a copy of the GNU Lesser General Public License
//along with this program; if not, write to the Free Software Foundation,
//Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "McuASANconfig.h"
#if McuASAN_CONFIG_IS_ENABLED
#include <nds.h>
#include "McuASAN.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define ssize_t int64_t
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(x, 0)

#define nocashPrint(_fmt, _args...) do { char nogba_buffer[128]; snprintf(nogba_buffer, sizeof(nogba_buffer), _fmt, ##_args); nocashMessage(nogba_buffer); } while(0)

#if (McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE > 0)
static struct quarantineList {
    size_t idx;
    void *list[McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE];
} quarantine;
#endif

typedef enum {
  kIsWrite, /* write access */
  kIsRead,  /* read access */
} rw_mode_e;

static uint8_t shadow[McuASAN_CONFIG_APP_MEM_SIZE>>3];

//TO DO: PROVIDE PROPER implementation for global and stack variables
static void unimplemented(void) 
{
  nocashMessage("Error: required asan function not implemented, please send a PR\n");
  while(1);
}
void __asan_stack_malloc_1(size_t size, void *addr) 
{ 
    unimplemented(); 
}

void __asan_stack_malloc_2(size_t size, void *addr) 
{ 
    unimplemented(); 
}
void __asan_stack_malloc_3(size_t size, void *addr) 
{ 
    unimplemented(); 
}

void __asan_stack_malloc_4(size_t size, void *addr) 
{ 
    unimplemented(); 
}

void __asan_handle_no_return(void) 
{ 
    unimplemented(); 
}

void __asan_option_detect_stack_use_after_return(void) 
{
    unimplemented(); 
}

void __asan_register_globals(void *, int) 
{ 
    unimplemented(); 
}

void __asan_unregister_globals(void*, int) 
{ 
    unimplemented(); 
}

void __asan_version_mismatch_check_v8(void) 
{
    unimplemented(); 
}

__attribute__((no_sanitize ("kernel-address"))) static void PoisonShadowBit(void *addr) 
{
    if ((size_t)addr>=McuASAN_CONFIG_APP_MEM_START && (size_t)addr<(McuASAN_CONFIG_APP_MEM_START+McuASAN_CONFIG_APP_MEM_SIZE)) 
        shadow[((size_t)addr-McuASAN_CONFIG_APP_MEM_START) >> 3] |= 1<<((size_t)addr&7); 
    return;
}

__attribute__((no_sanitize ("kernel-address"))) static void ClearShadowBit(void *addr) 
{
    if ((size_t)addr>=McuASAN_CONFIG_APP_MEM_START && (size_t)addr<(McuASAN_CONFIG_APP_MEM_START+McuASAN_CONFIG_APP_MEM_SIZE))
        shadow[((size_t)addr-McuASAN_CONFIG_APP_MEM_START) >> 3] &= ~(1<<((size_t)addr&7)); 
    return;
}

static void ReportError(void *address, size_t kAccessSize, rw_mode_e mode)
{
    if (address >= (void*)getHeapStart() && address<= (void*)getHeapLimit()) //do not error for stack address
    {
        nocashPrint("ASAN ptr failure: addr %p, %s, size: %d", address, mode==kIsRead?"read":"write", kAccessSize);
        if (mode==kIsWrite)
        {
            while(1);//program must halt before corrupting memory

        }
    }
    return;
}

__attribute__((no_sanitize ("kernel-address"))) void poison_range(void *address, size_t size)
{
    while( ((size_t)address & 7)  && size)
    {
        PoisonShadowBit(address);
        address++;
        size--;
    }

    memset(&shadow[((size_t)address-McuASAN_CONFIG_APP_MEM_START) >> 3], 255, size>>3);

    size_t t=(size>>3)<<3;
    size-=t;
    address+=t;

    for (int i=0;i<size; i++){
        PoisonShadowBit(address+i);
    }
    return;
}

__attribute__((no_sanitize ("kernel-address"))) void clear_range(void *address, size_t size)
{
    while( ((size_t)address & 7)  && size)
    {
        ClearShadowBit(address);
        address++;
        size--;
    }

    memset(&shadow[((size_t)address-McuASAN_CONFIG_APP_MEM_START) >> 3], 0, size>>3);

    size_t t=(size>>3)<<3;
    size-=t;
    address+=t;

    for (int i=0;i<size; i++)
    {
        ClearShadowBit(address+i);
    }
    return;
}

__attribute__((no_sanitize ("kernel-address")))void CheckShadow(void *address, size_t kAccessSize, rw_mode_e mode) 
{
    uint8_t shadow_value;

    if ((size_t)address>=McuASAN_CONFIG_APP_MEM_START && (size_t)address<(McuASAN_CONFIG_APP_MEM_START+McuASAN_CONFIG_APP_MEM_SIZE)) 
    {
        shadow_value=shadow[((size_t)address-McuASAN_CONFIG_APP_MEM_START) >>3];
        if ((shadow_value >> ((size_t)address & 7)) & 1) 
        {
             ReportError(address, kAccessSize, mode);
             return;
        }

    }
    address+=kAccessSize-1;

    if ((size_t)address>=McuASAN_CONFIG_APP_MEM_START && (size_t)address<(McuASAN_CONFIG_APP_MEM_START+McuASAN_CONFIG_APP_MEM_SIZE)) 
    {
        shadow_value=shadow[((size_t)address-McuASAN_CONFIG_APP_MEM_START) >>3];
        if ((shadow_value >> ((size_t)address & 7)) & 1)
        {
             ReportError(address-kAccessSize+1, kAccessSize, mode);
             return;
        }
    }
    return;
}

void __asan_loadN_noabort(const void *p, int size)
{
    CheckShadow((void*)p, size, kIsRead);
}
void __asan_storeN_noabort(const void *p, int size)
{
    CheckShadow((void*)p, size, kIsWrite);
}

void __asan_load4_noabort(void *address)
{
    CheckShadow(address, 4, kIsRead); /* check if we are reading from poisoned memory */
}

void __asan_store4_noabort(void *address)
{
    CheckShadow(address, 4, kIsWrite); /* check if we are writing to poisoned memory */
}

void __asan_load2_noabort(void *address)
{
    CheckShadow(address, 2, kIsRead); /* check if we are reading from poisoned memory */
}

void __asan_store2_noabort(void *address)
{
    CheckShadow(address, 2, kIsWrite); /* check if we are writing to poisoned memory */
}

void __asan_load1_noabort(void *address)
{
    CheckShadow(address, 1, kIsRead); /* check if we are reading from poisoned memory */
}

void __asan_store1_noabort(void *address)
{
    CheckShadow(address, 1, kIsWrite); /* check if we are writing to poisoned memory */
}

void __asan_load8_noabort(void *p)
{
    CheckShadow(p, 8, kIsRead);
}
void __asan_store8_noabort(void *p)
{
    CheckShadow(p, 8, kIsWrite);
}

#if McuASAN_CONFIG_CHECK_MALLOC_FREE

#ifdef malloc
  #undef malloc
  void *malloc(size_t);
#endif

#ifdef free
  #undef free
  void free(void*);
#endif

#ifdef memcpy
  #undef memcpy
  void * memcpy(void *restrict dest, const void *restrict src, size_t bytes);
#endif

#ifdef memset
  #undef memset
  void * memset(void* ptr, int val, size_t bytes);
#endif

__attribute__((no_sanitize ("kernel-address"))) void * __asan_memcpy(void *restrict dest, const void *restrict src, size_t bytes)
{
        if (likely ((dest>src+bytes) || (src>dest+bytes)) ) 
        {
            __asan_loadN_noabort(src, bytes);
            __asan_storeN_noabort(dest, bytes);
            return memcpy(dest, src, bytes);
        }
        nocashPrint("Illegal memcpy to %p from %p, %zu \n", dest, src, bytes);
        while(1);
}

__attribute__((no_sanitize ("kernel-address"))) void * __asan_memset(void *ptr, int val, size_t bytes)
{
    __asan_storeN_noabort(ptr, bytes);
    return memset(ptr, val, bytes);
}

//This function will only be called once (hence always cold cache)
//and compiling as arm increases the size
THUMB_CODE __attribute__((no_sanitize ("kernel-address"))) void McuASAN_Init(void)
{
    memset(&shadow[0], 255, sizeof(shadow));
#if McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE > 0
    memset(&quarantine, 0,sizeof(quarantine));
#endif
    return;
}

__attribute__((no_sanitize ("kernel-address"))) void *__asan_malloc(size_t size) 
{

    void *p = malloc(size+2*McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER);
    if (!p)
        return NULL;

    for(size_t i=0; i<McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER; i++) 
    {
        PoisonShadowBit(p+i);
    }
    p+=McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER;

    *((size_t*)(p-McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER)) = size; 

    for(size_t i=0; i<size; i++) 
    {
        ClearShadowBit(p+i);
    }
    p+=size;
    for(size_t i=0; i<McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER; i++)
    {
        PoisonShadowBit(p+i);
    }
    return p-size; 
}

__attribute__((no_sanitize ("kernel-address"))) void __asan_free(void *p)
{
    if (!p) 
        return;
    size_t size = *((size_t*)(p-McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER));
    for(size_t i=0; i<size; i++) 
    {
        PoisonShadowBit(p+i);
    }
    p = p-McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER; 
#if McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE > 0
    quarantine.list[quarantine.idx++] = p;
    if (quarantine.idx>=McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE)
        quarantine.idx = 0;

    free(quarantine.list[quarantine.idx]);
    quarantine.list[quarantine.idx] = NULL;
#else
    free(p); 
#endif
}
#endif /* McuASAN_CONFIG_CHECK_MALLOC_FREE */

#endif /* McuASAN_CONFIG_IS_ENABLED */
