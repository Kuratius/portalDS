#include <nds.h>
#include <stdint.h>
ARM_CODE static uint32_t sqrt_core(uint32_t x, uint32_t y)
{
    x>>=1;
    uint32_t t=x+(x>>1);
    if (t < (1u<<31)){ //first iteration is special cased
        t+=t>>1;
        if (t < (1u<<31)) {
            x=t;
            y+=y>>1;
        }
    }
    for (uint32_t i =2; i<14; i+=1){
        uint32_t t=x+(x>>i);
        t+=t>>i;
        if (t < (1u<<31)) {
            x=t;
            y+=y>>i;
        }
    }
    uint32_t hi = ((uint64_t)(x)*(y)  )>>32;
    y+=y>>1; //this can overflow
    return ((y)-(hi) ); //but then this underflows so they cancel
}

uint32_t rsqrt1616(uint32_t m)
{
//input: 16.16 number
    int clz=__builtin_clz(m);
    int ilog2=31-clz;
    ilog2>>=1;
    ilog2<<=1;
    int shift=30-ilog2;
    m<<=shift;
    int shift2=22-(shift>>1);
    uint32_t result=sqrt_core(m, 1<<30);
    return shift2>=0 ? result>>shift2 : result<<-shift2;
//output: 16.16 number
}

ARM_CODE static inline int32_t sqrt64_helper(uint64_t m, int * exp)
{
    int clz=__builtin_clzll(m);
    int ilog2=63-clz;
    ilog2>>=1;
    ilog2<<=1;
    int shift=30-ilog2;
    m= shift>=0 ? m<<shift : m>>-shift;
    int shift2=22-(shift>>1);
    *exp=shift2;
    return sqrt_core(m, 1<<30);
}

ARM_CODE void normalize_arm7(int32_t * a)
{
    uint64_t squared_magnitude=(int64_t)a[0]*a[0]+(int64_t)a[1]*a[1]+(int64_t)a[2]*a[2];
    if (squared_magnitude==0)
        return;
    int exp;
    uint32_t res=sqrt64_helper(squared_magnitude, &exp);
    exp+=12;
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
