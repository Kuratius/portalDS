#ifndef PI7_H
#define PI7_H

typedef struct
{
	vect3D position;
}player_struct;

typedef struct portal_struct
{
	vect3D position, normal, plane[2];
	AAR_struct guideAAR[4];
	struct portal_struct* targetPortal;
	bool used;
}portal_struct;

#include "../../common/include/PIC.h"

extern player_struct player;
extern portal_struct portal[2];

void initPI7(void);
bool getPI7Status(void);
void listenPI7(void);
void sendDataPI7(void);

#endif
