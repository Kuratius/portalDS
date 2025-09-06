#include "stdafx.h"


#if 0
#include "LUTs.h"
ARM_CODE uint32_t sqrtv(uint32_t x)
{
	const uint16_t a=f32toint(x);
	const uint16_t b=x&4095;
	return (a<SQRTRANGE)?((a)?mulf32(sqrtLUT1[a],sqrtLUT2[b/(2*a)]):(sqrtLUT3[b])):(0);
}

#else 
uint32_t isqrt_asm(uint32_t);


ARM_CODE uint32_t sqrtv(uint32_t x)
{
	if (x<((1u<<20)-1 ))
    {
        return isqrt_asm(x<<12);
    } 

    return isqrt_asm(x)<<6;
}
#endif

ARM_CODE vect3D normalize(vect3D v)
{
	int32 d=sqrtv(((int64_t)v.x*v.x+(int64_t)v.y*v.y+(int64_t)v.z*v.z)>>12);
	return vect(divv16(v.x,d),divv16(v.y,d),divv16(v.z,d));
}


void transposeMatrix33(int32_t *restrict m1, int32_t *restrict m2) //3x3
{
	int i, j;
	for(i=0;i<3;i++)for(j=0;j<3;j++)m2[j+i*3]=m1[i+j*3];
    return;
}



ARM_CODE vect3D evalVectMatrix33(int32_t* m, vect3D v) //3x3
{
	return vect(((int64_t)v.x*m[0]+(int64_t)v.y*m[1]+(int64_t)v.z*m[2])>>12,
				((int64_t)v.x*m[3]+(int64_t)v.y*m[4]+(int64_t)v.z*m[5])>>12,
				((int64_t)v.x*m[6]+(int64_t)v.y*m[7]+(int64_t)v.z*m[8])>>12);
}


ARM_CODE void multMatrix33(int32* m1, int32* m2, int32 *restrict m) //3x3
{
	int i, j;
	// for(i=0;i<4;i++)for(j=0;j<4;j++)m[i+j*4]=m1[i+0*4]*m2[0+j*4]+m1[i+1*4]*m2[1+j*4]+m1[i+2*4]*m2[2+j*4]+m1[i+3*4]*m2[3+j*4];
	for(i=0;i<3;i++)for(j=0;j<3;j++)m[j+i*3]=((int64_t)m1[0+i*3]*m2[j+0*3]+(int64_t)m1[1+i*3]*m2[j+1*3]+(int64_t)m1[2+i*3]*m2[j+2*3])>>12;
}

ARM_CODE void multMatrix332(int32* m1, int32* m2, int32 *restrict m) //3x3
{
	int i, j;
	for(i=0;i<3;i++)for(j=0;j<3;j++)m[i+j*3]=((int64_t)m1[i+0*3]*m2[0+j*3]+(int64_t)m1[i+2*3]*m2[2+j*3]+(int64_t)m1[i+2*3]*m2[2+j*3])>>12;
}

ARM_CODE void rotateMatrixAxis(int32* tm, int32 x, vect3D a, bool r)
{
	int32 rm[9], m[9];

	int32 cosval=cosLerp(x);
	int32 sinval=sinLerp(x);
	int32 onemcosval=inttof32(1)-cosval;

	rm[0]=cosval+mulf32(mulf32(a.x,a.x),onemcosval);
	rm[1]=mulf32(mulf32(a.x,a.y),onemcosval)-mulf32(a.z,sinval);
	rm[2]=mulf32(mulf32(a.x,a.z),onemcosval)+mulf32(a.y,sinval);

	rm[3]=mulf32(mulf32(a.x,a.y),onemcosval)+mulf32(a.z,sinval);
	rm[4]=cosval+mulf32(mulf32(a.y,a.y),onemcosval);
	rm[5]=mulf32(mulf32(a.y,a.z),onemcosval)-mulf32(a.x,sinval);

	rm[6]=mulf32(mulf32(a.x,a.z),onemcosval)-mulf32(a.y,sinval);
	rm[7]=mulf32(mulf32(a.y,a.z),onemcosval)+mulf32(a.x,sinval);
	rm[8]=cosval+mulf32(mulf32(a.z,a.z),onemcosval);

	if(r)multMatrix33(rm,tm,m);
	else multMatrix33(tm,rm,m);
	memcpy(tm,m,9*sizeof(int32));
}

ARM_CODE vect3D vectProduct(vect3D v1, vect3D v2)
{
	return vect(((int64_t)v1.y*v2.z+(int64_t)v1.z*-v2.y)>>12,((int64_t)v1.z*v2.x+(int64_t)-v1.x*v2.z)>>12,((int64_t)-v1.x*-v2.y+(int64_t)-v1.y*v2.x)>>12);
}

ARM_CODE void fixMatrix(int32* m) //3x3
{
	if(!m)return;
	vect3D x=vect(m[0],m[3],m[6]);
	vect3D y=vect(m[1],m[4],m[7]);
	vect3D z=vect(m[2],m[5],m[8]);
	
	projectVectorPlane(&x,y);
	projectVectorPlane(&z,y);
	projectVectorPlane(&z,x);
	
	x=normalize(x);
	y=normalize(y);
	z=normalize(z);
	
	m[0]=x.x;m[3]=x.y;m[6]=x.z;
	m[1]=y.x;m[4]=y.y;m[7]=y.z;
	m[2]=z.x;m[5]=z.y;m[8]=z.z;
}



