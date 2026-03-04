// SPDX-License-Identifier: Zlib
//
// Copyright (C) 2025 Dominik Kurz
#include <nds.h>
#include <stdint.h>

ARM_CODE uint32_t isqrt_asm(uint32_t r0)
{
    uint32_t r2=(3u<<30);
    uint32_t r1=(1u<<30);
    for (int i =0; i<(16*2); i+=2)
    {
        asm(".syntax unified\n\t"
            "cmp %[r0], %[r1], ror %[i]\n\t"
            "subcs %[r0], %[r0], %[r1], ror %[i]\n\t"
            "adc %[r1], %[r2], %[r1], lsl #1"
        : [r0]"+r"(r0), [r1]"+r"(r1)  //output
        : [r2]"r"(r2),[i]"Ir"(i)//input
        :"cc"//clobber
        );
    }
    return (r1<<2)>>2;
}
