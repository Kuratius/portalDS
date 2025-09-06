#ifndef MATH_H
#define MATH_H

#define inttof32(n)          ((n) << (12)) /*!< \brief convert int to f32 */
#define f32toint(n)          ((n) >> (12)) /*!< \brief convert f32 to int */
#define floattof32(n)        ((int)((n) * (1 << (12)))) /*!< \brief convert float to f32 */
#define f32tofloat(n)        (((float)(n)) / (float)(1<<(12))) /*!< \brief convert f32 to float */

#define min(a,b) (((a)>(b))?(b):(a))
#define max(a,b) (((a)>(b))?(a):(b))

typedef struct
{
	int32 x, y, z;
}vect3D;

// float sqrtf(float a){return a;}
int32_t sqrtv(int32_t x);

static inline vect3D vect(int32 x, int32 y, int32 z)
{
    vect3D v;
    v.x=x;
    v.y=y;
    v.z=z;
    return v;
}

static inline int32_t mulf32(int32_t a, int32_t b)
{
	int64_t result = (int64_t)a * (int64_t)b;
	return (int32_t)(result >> (12));
}



// static inline int32 divf32(int32 a, int32 b)
// {
	// long long result = (((long long)a)<<12) / (long long)b;
	// return (int32)(result);
// }

static inline int32 divv16(int32 a, int32 b)
{
	int32 d=(((int32)a)<<12)/b;
	return (int32)d;
}

static inline int32 mulv16(int32 a, int32 b)
{
	int32 d=((int32)a)*((int32)b);
	return (int32)(d>>12);
}

static inline vect3D addVect(vect3D p1, vect3D p2)
{
	return vect(p1.x+p2.x,p1.y+p2.y,p1.z+p2.z);
}

static inline vect3D vectMult(vect3D v, int32 k)
{
	return vect(mulf32(v.x,k), mulf32(v.y,k), mulf32(v.z,k));
}

static inline vect3D vectMultv16(vect3D v, int32 k)
{
	return vect(mulv16(v.x,k), mulv16(v.y,k), mulv16(v.z,k));
}

static inline vect3D vectMultInt(vect3D v, int k)
{
	return vect((v.x*k), (v.y*k), (v.z*k));
}

static inline vect3D vectDivInt(vect3D v, int k)
{
	return vect((v.x/k), (v.y/k), (v.z/k));
}

static inline vect3D vectDifference(vect3D p1, vect3D p2)
{
	return vect(p1.x-p2.x,p1.y-p2.y,p1.z-p2.z);
}

static inline int32 dotProduct(vect3D v1, vect3D v2)
{
	return ((int64_t)v1.x*v2.x+(int64_t)v1.y*v2.y+(int64_t)v1.z*v2.z)>>12;
}

static inline vect3D normalize(vect3D v)
{
	int32 d=sqrtv(((int64_t)v.x*v.x+(int64_t)v.y*v.y+(int64_t)v.z*v.z)>>12);
	return vect(divv16(v.x,d),divv16(v.y,d),divv16(v.z,d));
}

static inline int32 magnitude(vect3D v)
{
	int32 d=sqrtv(((int64_t)v.x*v.x+(int64_t)v.y*v.y+(int64_t)v.z*v.z)>>12);
	return d;
}

static inline int32 distance(vect3D v1, vect3D v2)
{
	return magnitude(vectDifference(v2,v1));
}

static inline vect3D divideVect(vect3D v, int32 d)
{
	return vect(divv16(v.x,d),divv16(v.y,d),divv16(v.z,d));
}

vect3D vectProduct(vect3D v1, vect3D v2);

// static inline int cosLerp(int32 x){return floattof32(cos((x*PI)/16384));}
// static inline int sinLerp(int32 x){return floattof32(sin((x*PI)/16384));}

static inline int cosLerp(int32 x){return 1;}
static inline int sinLerp(int32 x){return 1;}

void multMatrix33(int32* m1, int32* m2, int32 *restrict m); //3x3

void multMatrix332(int32* m1, int32* m2, int32 *restrict m); //3x3

static inline void addMatrix33(int32* m1, int32* m2, int32* m) //3x3
{
	int i, j;
	for(i=0;i<3;i++)for(j=0;j<3;j++)m[j+i*3]=m1[j+i*3]+m2[j+i*3];
}

void transposeMatrix33(int32* m1, int32* m2); //3x3


vect3D evalVectMatrix33(int32* m, vect3D v); //3x3


static inline int32 clamp(int32 v, int32 m, int32 M)
{
	if(m<M)return max(m,min(v,M));
	else return min(m,max(v,M));
}

static inline void rotateMatrixX(int32* tm, int32 x, bool r)
{
	int i;
	int32 rm[9], m[9];
	for(i=0;i<9;i++)rm[i]=0;
	rm[0]=inttof32(1);
	rm[4]=cosLerp(x);
	rm[5]=sinLerp(x);
	rm[7]=-sinLerp(x);
	rm[8]=cosLerp(x);
	if(r)multMatrix33(rm,tm,m);
	else multMatrix33(tm,rm,m);
	memcpy(tm,m,9*sizeof(int32));
}

static inline void rotateMatrixY(int32* tm, int32 x, bool r)
{
	int i;
	int32 rm[9], m[9];
	for(i=0;i<9;i++)rm[i]=0;
	rm[0]=cosLerp(x);
	rm[2]=sinLerp(x);
	rm[4]=inttof32(1);
	rm[6]=-sinLerp(x);
	rm[8]=cosLerp(x);
	if(r)multMatrix33(rm,tm,m);
	else multMatrix33(tm,rm,m);
	memcpy(tm,m,9*sizeof(int32));
}

static inline void rotateMatrixZ(int32* tm, int32 x, bool r)
{
	int i;
	int32 rm[9], m[9];
	for(i=0;i<9;i++)rm[i]=0;
	rm[0]=cosLerp(x);
	rm[1]=sinLerp(x);
	rm[3]=-sinLerp(x);
	rm[4]=cosLerp(x);
	rm[8]=inttof32(1);
	if(r)multMatrix33(rm,tm,m);
	else multMatrix33(tm,rm,m);
	memcpy(tm,m,9*sizeof(int32));
}

void rotateMatrixAxis(int32* tm, int32 x, vect3D a, bool r);


static inline void projectVectorPlane(vect3D* v, vect3D n)
{
	if(!v)return;
	int32 r=dotProduct(*v,n);
	*v=vectDifference(*v,vectMult(n,r));
}

void fixMatrix(int32* m); //3x3


#endif
