#ifndef PIC_H
#define PIC_H

#define PORTALFRACTIONX (12)
#define PORTALFRACTIONY (6)

#define PORTALSIZEX (inttof32(1)/PORTALFRACTIONX)
#define PORTALSIZEY (inttof32(1)/PORTALFRACTIONY)

#define PISIGNALDATA (6)
#define PISIGNALMASK ((1<<PISIGNALDATA)-1)

#define NUMPLATFORMS (8)
#define PLATFORMSIZE (256*3) //half

#define NUMAARS (300)

typedef enum
{
	PI_START=1,  	    //ARG : 0
	PI_PAUSE=2,  	    //ARG : 0
	PI_STOP=3,   	    //ARG : 0
	PI_RESET=4,  	    //ARG : 0
	PI_RESETALL=18,	    //ARG : 0
	PI_ADDBOX=5,  	    //ARG : 5 (id;[sizex|sizey][sizez|mass][posx][posy][posz])
	PI_APPLYFORCE=6,    //ARG : 5 (id;[posx|posy][posz][vx][vy][vz])
	PI_ADDAAR=7,  	    //ARG : 5 (id;[sizex][sizey][sizez][normal][posx][posy][posz])
	PI_MAKEGRID=8,      //ARG : 0
	PI_SETVELOCITY=9,   //ARG : 3 (id;[vx][vy][vz])
	PI_UPDATEPLAYER=10, //ARG : 3 ([vx][vy][vz])
	PI_UPDATEPORTAL=11, //ARG : 7 (id;[px][py][pz][n][p0x][p0y][p0z])
	PI_ADDPLATFORM=12,  //ARG : 3 (id;[posx][posy][posz])
	PI_UPDATEPLATFORM=13,//ARG : 3 (id;[posx][posy][posz])
	PI_TOGGLEPLATFORM=14,//ARG : 1 (id;[active])
	PI_KILLBOX=15,       //ARG : 0 (id)
	PI_RESETPORTALS=16,  //ARG : 0
	PI_TOGGLEAAR=17,  	 //ARG : 0 (id)
	// 18 taken you fool
}message_type;

#ifdef ARM7
	void generateGuidAAR(portal_struct* p);
	
	static inline void computePortalPlane(portal_struct* p)
	{
		if(!p)return;
		
		p->plane[1]=vectProduct(p->normal,p->plane[0]);
		
		//TEST
		generateGuidAAR(p);
	}
#else
	static inline void computePortalPlane(portal_struct* p)
	{
		if(!p)return;
		
		p->plane[1]=vectProduct(p->normal,p->plane[0]);
	}
#endif

vect3D warpVector(portal_struct* p, vect3D v);


void warpMatrix(portal_struct* p, int32* m); //3x3


#endif
