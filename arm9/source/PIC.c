#include "game/game_main.h"
#include "../../common/include/PIC.h"
ARM_CODE vect3D warpVector(portal_struct* p, vect3D v)
{
	if(!p)return vect(0,0,0);
	portal_struct* p2=p->targetPortal;
	if(!p2)return vect(0,0,0);
	
	// computePortalPlane(p2);
	
	int32 x=dotProduct(v,p->plane[0]);
	int32 y=dotProduct(v,p->plane[1]);
	int32 z=dotProduct(v,p->normal);
	
	return addVect(vectMult(p2->normal,-z),addVect(vectMult(p2->plane[0],-x),vectMult(p2->plane[1],y)));
}

void warpMatrix(portal_struct* p, int32* m) //3x3
{
	if(!m)return;
	
	vect3D x=warpVector(p,vect(m[0],m[3],m[6]));
	vect3D y=warpVector(p,vect(m[1],m[4],m[7]));
	vect3D z=warpVector(p,vect(m[2],m[5],m[8]));
	
	m[0]=x.x;m[3]=x.y;m[6]=x.z;
	m[1]=y.x;m[4]=y.y;m[7]=y.z;
	m[2]=z.x;m[5]=z.y;m[8]=z.z;
}
