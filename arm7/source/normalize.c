#include <nds.h>
#include <stdint.h>

uint32_t rsqrt_asm(uint32_t x);
ARM_CODE uint32_t irsqrt(uint32_t m)
{
    int clz=__builtin_clz(m);
    int ilog2=31-clz;
    ilog2>>=1;
    ilog2<<=1;
    int shift=30-ilog2;
    m<<=shift;
    int shift2=22-(shift>>1);
    return shift2>=0 ? rsqrt_asm(m)>>shift2 : rsqrt_asm(m)<<-shift2;
}



ARM_CODE static inline int32_t irsqrt3232(uint64_t m, int * exp)
{
    int clz=__builtin_clzll(m);
    int ilog2=63-clz;
    ilog2>>=1;
    ilog2<<=1;
    int shift=30-ilog2;
    m= shift>=0 ? m<<shift : m>>-shift;
    int shift2=22-(shift>>1);
    *exp=shift2;
    return rsqrt_asm(m);
}



ARM_CODE __attribute__((noinline)) void normalizef32(int32_t * a)
{
    uint64_t squared_magnitude=(int64_t)a[0]*a[0]+(int64_t)a[1]*a[1]+(int64_t)a[2]*a[2];
    if (squared_magnitude==0)
        return;
    int exp;
    uint32_t res=irsqrt3232(squared_magnitude, &exp);
    exp+=16-8+4;
    for(int i=0; i<3;i++)
    {
        int32_t t=a[i];
        uint32_t tu=t;
        if (t<0)
            tu=-tu;

        uint64_t prod=(uint64_t)tu*res;
        prod= exp >=0 ? prod>>exp : prod<<-exp;
        int32_t sprod=prod;
        if (t<0)
            sprod=-sprod;

        a[i]=sprod;
    }
    return;
}
