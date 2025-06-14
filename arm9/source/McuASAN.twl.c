/*
 * Copyright (c) 2021, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <nds.h>
#if 1

#include "McuASANconfig.h"
#if McuASAN_CONFIG_IS_ENABLED
#include "McuASAN.h"
//#include "McuLog.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine/debug.h"

#define u8 uint8_t
#define ssize_t int64_t


#ifdef __cplusplus
extern "C" {
#endif

#define McuLog_fatal(_fmt, _args...) do { char nogba_buffer[128]; snprintf(nogba_buffer, sizeof(nogba_buffer), _fmt, ##_args); nocashMessage(nogba_buffer); } while(0)


#if 1
/* hooks if using -fsanitize=address */
/* -fasan-shadow-offset=number */
/* -fsanitize=kernel-address */
static void __asan_ReportGenericError(void) {
#if 1
  nocashMessage("ASAN generic failure");
#else
  __asm volatile("bkpt #0"); /* stop application */
  for(;;){}
#endif
}

/*
void __asan_register_globals(void *globals, ssize_t size);
void __asan_unregister_globals(void *globals, ssize_t size);
void __asan_handle_no_return(void);
void __asan_alloca_poison(void *, ssize_t size);
void __asan_allocas_unpoison(void *stack_top, ssize_t stack_bottom);

void __asan_load1(void *);
void __asan_store1(void *);
void __asan_load2(void *);
void __asan_store2(void *);
void __asan_load4(void *);
void __asan_store4(void *);
void __asan_load8(void *);
void __asan_store8(void *);
void __asan_load16(void *);
void __asan_store16(void *);
void __asan_loadN(void *, ssize_t size);
void __asan_storeN(void *, ssize_t size);

void __asan_load1_noabort(void *);
void __asan_store1_noabort(void *);
void __asan_load2_noabort(void *);
void __asan_store2_noabort(void *);
void __asan_load4_noabort(void *);
void __asan_store4_noabort(void *);
void __asan_load8_noabort(void *);
void __asan_store8_noabort(void *);
void __asan_load16_noabort(void *);
void __asan_store16_noabort(void *);
void __asan_loadN_noabort(void *, ssize_t size);
void __asan_storeN_noabort(void *, ssize_t size);

void __asan_report_load1_noabort(void *);
void __asan_report_store1_noabort(void *);
void __asan_report_load2_noabort(void *);
void __asan_report_store2_noabort(void *);
void __asan_report_load4_noabort(void *);
void __asan_report_store4_noabort(void *);
void __asan_report_load8_noabort(void *);
void __asan_report_store8_noabort(void *);
void __asan_report_load16_noabort(void *);
void __asan_report_store16_noabort(void *);
void __asan_report_load_n_noabort(void *, ssize_t size);
void __asan_report_store_n_noabort(void *, ssize_t size);

void __asan_set_shadow_00(const void *addr, ssize_t size);
void __asan_set_shadow_f1(const void *addr, ssize_t size);
void __asan_set_shadow_f2(const void *addr, ssize_t size);
void __asan_set_shadow_f3(const void *addr, ssize_t size);
void __asan_set_shadow_f5(const void *addr, ssize_t size);
void __asan_set_shadow_f8(const void *addr, ssize_t size);

void *__asan_memset(void *addr, int c, ssize_t len);
void *__asan_memmove(void *dest, const void *src, ssize_t len);
void *__asan_memcpy(void *dest, const void *src, ssize_t len);

void __hwasan_load1_noabort(void *);
void __hwasan_store1_noabort(void *);
void __hwasan_load2_noabort(void *);
void __hwasan_store2_noabort(void *);
void __hwasan_load4_noabort(void *);
void __hwasan_store4_noabort(void *);
void __hwasan_load8_noabort(void *);
void __hwasan_store8_noabort(void *);
void __hwasan_load16_noabort(void *);
void __hwasan_store16_noabort(void *);
void __hwasan_loadN_noabort(void *, ssize_t size);
void __hwasan_storeN_noabort(void *, ssize_t size);

void __hwasan_tag_memory(void *, u8 tag, ssize_t size);

void *__hwasan_memset(void *addr, int c, ssize_t len);
void *__hwasan_memmove(void *dest, const void *src, ssize_t len);
void *__hwasan_memcpy(void *dest, const void *src, ssize_t len);

void kasan_tag_mismatch(void *addr, unsigned long access_info,
			unsigned long ret_ip);
*/


/* below are the required callbacks needed by ASAN */





void __asan_report_store1(void *address) {__asan_ReportGenericError();}
void __asan_report_store2(void *address) {__asan_ReportGenericError();}
void __asan_report_store4(void *address) {__asan_ReportGenericError();}
void __asan_report_store_n(void *address, int) {__asan_ReportGenericError();}
void __asan_report_load1(void *address) {__asan_ReportGenericError();}
void __asan_report_load2(void *address) {__asan_ReportGenericError();}
void __asan_report_load4(void *address) {__asan_ReportGenericError();}
void __asan_report_load_n(void *address,int) {__asan_ReportGenericError();}






#endif

#if 1
static void NYI(void) {
  //__asm volatile("bkpt #0"); /* stop application */

  nocashMessage("NYI ERROR\n");
  for(;;){}
}
void __asan_stack_malloc_1(size_t size, void *addr) { NYI(); }
void __asan_stack_malloc_2(size_t size, void *addr) { NYI(); }
void __asan_stack_malloc_3(size_t size, void *addr) { NYI(); }
void __asan_stack_malloc_4(size_t size, void *addr) { NYI(); }
void __asan_handle_no_return(void) { NYI(); }
void __asan_option_detect_stack_use_after_return(void) { NYI(); }

//void __asan_register_globals(void) { NYI(); }
void __asan_register_globals(void *, int) { NYI(); }
void __asan_unregister_globals(void*, int) { NYI(); }
void __asan_version_mismatch_check_v8(void) { NYI(); }
#endif

/* see https://github.com/gcc-mirror/gcc/blob/master/libsanitizer/asan/asan_interface_internal.h */
static uint8_t shadow[McuASAN_CONFIG_APP_MEM_SIZE/8]; /* one shadow byte for 8 application memory bytes. A 1 means that the memory address is poisoned */

#if McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE > 0
static void *freeQuarantineList[McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE];
/*!< list of free'd blocks in quarantine */
static int freeQuarantineListIdx; /* index in list (ring buffer), points to free element in list */
#endif

typedef enum {
  kIsWrite, /* write access */
  kIsRead,  /* read access */
} rw_mode_e;

__attribute__((no_sanitize ("kernel-address"))) uint8_t *MemToShadow(void *address) {
  address -= McuASAN_CONFIG_APP_MEM_START;
  //return shadow+(((uint32_t)address)>>3); /* divided by 8: every byte has a shadow bit */

  uint8_t * addr=shadow+(((uint32_t)address)>>3); /* divided by 8: every byte has a shadow bit */
  return addr;
}


__attribute__((no_sanitize ("kernel-address"))) uint8_t GetShadowValue(void *address) {
    return *MemToShadow(address);
}

__attribute__((no_sanitize ("kernel-address"))) static void PoisonShadowByte1Addr(void *addr) {
  if (addr>=(void*)McuASAN_CONFIG_APP_MEM_START && addr<(void*)(McuASAN_CONFIG_APP_MEM_START+McuASAN_CONFIG_APP_MEM_SIZE)) {
    *MemToShadow(addr) |= 1<<((uint32_t)addr&7); /* mark memory in shadow as poisoned with shadow bit */
  }
}

__attribute__((no_sanitize ("kernel-address"))) static void ClearShadowByte1Addr(void *addr) {
  if ((uint32_t)addr>=McuASAN_CONFIG_APP_MEM_START && (uint32_t)addr<(McuASAN_CONFIG_APP_MEM_START+McuASAN_CONFIG_APP_MEM_SIZE)) {
    *MemToShadow(addr) &= ~(1<<((uint32_t)addr&7)); /* clear shadow bit: it is a valid memory */
  }
}

__attribute__((no_sanitize ("kernel-address"))) static bool SlowPathCheck(int8_t shadow_value, void *address, size_t kAccessSize) {
  /* return true if access to address is poisoned */


  //int8_t first_accessed_byte = (((uint32_t)address) & 7);
  int8_t last_accessed_byte = (((uint32_t)address) & 7) + kAccessSize - 1;
  //return (last_accessed_byte >= shadow_value);


  shadow_value>>=last_accessed_byte;
  return shadow_value & 1;
/*
  int index=0;
  while( ! (shadow_value &1) & index<8 & index <kAccessSize){
    if (shadow_value & 1){
        nocashMessage("Slowpath access invalid!\n");
        return 1;
    } else{
        shadow_value>>=1;
        index+=1;
        
    }


   }
 */
}

static void ReportError(void *address, size_t kAccessSize, rw_mode_e mode) {

  if (address >= getHeapStart() && address<= (void *)0x2FF0000){

    McuLog_fatal("ASAN ptr failure: addr %p, %s, size: %d", address, mode==kIsRead?"read":"write", kAccessSize);
    NOGBA("stuck!\n");
    //volatile int* nullp=NULL;
    //*nullp=4;
    //while(1);  
  } else{
    //McuLog_fatal("ASAN static ptr failure: addr %p, %s, size: %d", address, mode==kIsRead?"read":"write", kAccessSize);
 }
  //__asm volatile("bkpt #0"); /* stop application if debugger is attached */
}

__attribute__((no_sanitize ("kernel-address")))void CheckShadow(void *address, size_t kAccessSize, rw_mode_e mode) 
{
/*  static int calls=20;
  printf("checking shadow at addr %p\n", address);
  if (calls<=0) {
        printf("stackoverflow\n");
        while(1);
    }
  calls-=1;*/
  int8_t *shadow_address;
  int8_t shadow_value;

  if ((uint32_t)address>=McuASAN_CONFIG_APP_MEM_START && (uint32_t)address<(McuASAN_CONFIG_APP_MEM_START+McuASAN_CONFIG_APP_MEM_SIZE)) 
    {
        shadow_address = (int8_t*)MemToShadow(address);
        shadow_value = *shadow_address;
        
        if (shadow_value==-1)
        {
            ReportError(address, kAccessSize, mode);
            return;
        } 
        else if (shadow_value!=0) 
        { /* fast check: poisoned! */
            if (SlowPathCheck(shadow_value, address, 1)) 
            {
                ReportError(address, kAccessSize, mode);
                return;
            }
        }

        shadow_address = (int8_t*)MemToShadow(address+kAccessSize-1);
        shadow_value = *shadow_address;
        
        if (shadow_value==-1)
        {
            ReportError(address, kAccessSize, mode);
            return;
        } 
        else if (shadow_value!=0) 
        { /* fast check: poisoned! */
            if (SlowPathCheck(shadow_value, address+kAccessSize-1, 1)) 
            {
                ReportError(address, kAccessSize, mode);
                return;
            }
        }


    }
}

void __asan_load4_noabort(void *address) {
  CheckShadow(address, 4, kIsRead); /* check if we are reading from poisoned memory */
}

void __asan_store4_noabort(void *address) {
  CheckShadow(address, 4, kIsWrite); /* check if we are writing to poisoned memory */
}

void __asan_load2_noabort(void *address) {
  CheckShadow(address, 2, kIsRead); /* check if we are reading from poisoned memory */
}

void __asan_store2_noabort(void *address) {
  CheckShadow(address, 2, kIsWrite); /* check if we are writing to poisoned memory */
}

void __asan_load1_noabort(void *address) {
  CheckShadow(address, 1, kIsRead); /* check if we are reading from poisoned memory */
}

void __asan_store1_noabort(void *address) {
  CheckShadow(address, 1, kIsWrite); /* check if we are writing to poisoned memory */
}

void __asan_load8_noabort(void * p){CheckShadow(p, 8, kIsRead);};
void __asan_store8_noabort(void * p){CheckShadow(p, 8, kIsWrite);};

void __asan_loadN_noabort(const void * p, int size){CheckShadow((void*)p, size, kIsRead);};
void __asan_storeN_noabort(const void * p, int size){CheckShadow((void*)p, size, kIsWrite);};

/*
void __asan_load2_noabort(void *);
void __asan_store2_noabort(void *);
void __asan_load4_noabort(void *);
void __asan_store4_noabort(void *);


void __asan_load16_noabort(void *);
void __asan_store16_noabort(void *);
*/




#if McuASAN_CONFIG_CHECK_MALLOC_FREE
/* undo possible defines for malloc and free */
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
  void * memcpy(void* restrict dest, const void * restrict src, size_t bytes);
#endif


#ifdef memset
  #undef memset
  void * memset(void* ptr, int val, size_t bytes);
#endif

/*
 * rrrrrrrr  red zone border (incl. size below)
 * size
 * memory returned
 * rrrrrrrr  red zone boarder
 */

__attribute__((no_sanitize ("kernel-address"))) void * __asan_memcpy(void * restrict dest, const void * restrict src, size_t bytes){

        if (dest==NULL || src ==NULL ){

            nocashMessage("wrong memcpy\n");
        }
        __asan_loadN_noabort(src, bytes);
        __asan_storeN_noabort(dest, bytes);
        return memcpy(dest, src, bytes);
}


__attribute__((no_sanitize ("kernel-address"))) void * __asan_memset(void* ptr, int val, size_t bytes){
    if (ptr==NULL)return NULL;

    __asan_storeN_noabort(ptr, bytes);
    return memset(ptr, val, bytes);
}

__attribute__((no_sanitize ("kernel-address"))) void *__asan_malloc(size_t size) {
  /* malloc allocates the requested amount of memory with redzones around it.
   * The shadow values corresponding to the redzones are poisoned and the shadow values
   * for the memory region are cleared.
   */
  void *p = malloc(size+2*McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER); /* add size_t for the size of the block */
  void *q;

  q = p;
  /* poison red zone at the beginning */
  for(int i=0; i<McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER; i++) {
    PoisonShadowByte1Addr(q);
    q++;
  }
  *((size_t*)(q-sizeof(size_t))) = size; /* store memory size, needed for the free() part */
  /* clear valid memory */
  for(int i=0; i<size; i++) {
    ClearShadowByte1Addr(q);
    q++;
  }
  /* poison red zone at the end */
  for(int i=0; i<McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER; i++) {
    PoisonShadowByte1Addr(q);
    q++;
  }
  return p+McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER; /* return pointer to valid memory */
}
#endif

#if McuASAN_CONFIG_CHECK_MALLOC_FREE
__attribute__((no_sanitize ("kernel-address"))) void __asan_free(void *p) {
    NOGBA("entering asan free(%p)\n", p);

    if (!p) return;
  /* Poisons shadow values for the entire region and put the chunk of memory into a quarantine queue
   * (such that this chunk will not be returned again by malloc during some period of time).
   */


  size_t size = *((size_t*)(p-sizeof(size_t))); /* get size */
  void *q = p;
    NOGBA("entering for loop asan free %lu\n",size );
  for(int i=0; i<size; i++) {
    PoisonShadowByte1Addr(q);
    q++;
  }

  NOGBA("leaving for loop asan free\n");
  q = p-McuASAN_CONFIG_MALLOC_RED_ZONE_BORDER; /* calculate beginning of malloc()ed block */
#if McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE > 0
  /* put the memory block into quarantine */
  freeQuarantineList[freeQuarantineListIdx] = q;
  freeQuarantineListIdx++;
  if (freeQuarantineListIdx>=McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE) {
    freeQuarantineListIdx = 0;
  }
  if (freeQuarantineList[freeQuarantineListIdx]!=NULL) {
    free(freeQuarantineList[freeQuarantineListIdx]);
    freeQuarantineList[freeQuarantineListIdx] = NULL;
  }
#else
  free(q); /* free block */
#endif
}
#endif /* McuASAN_CONFIG_CHECK_MALLOC_FREE */

__attribute__((no_sanitize ("kernel-address"))) void McuASAN_Init(void) {
  for(int i=0; i<sizeof(shadow); i++) { /* initialize full shadow map */
    shadow[i] = -1; /* poison everything  */
  }
  /* because the shadow is part of the memory area: poison the shadow */
  for(int i=0; i<sizeof(shadow); i+=8) {
    PoisonShadowByte1Addr(&shadow[i]);
  }
#if McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE > 0
  for(int i=0; i<McuASAN_CONFIG_FREE_QUARANTINE_LIST_SIZE; i++) {
    freeQuarantineList[i] = NULL;
  }
  freeQuarantineListIdx = 0;
#endif
}

#endif /* McuASAN_CONFIG_IS_ENABLED */
#endif
