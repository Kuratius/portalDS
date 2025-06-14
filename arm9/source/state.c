#include "common/general.h"

//static u8 state_id;

extern state_struct menuState;

state_struct *currentState= &menuState;
static state_struct *nextState= &menuState;

state_struct* getCurrentState(void)
{
	return currentState;
}

void changeState(state_struct* s)
{

    if (s==NULL)
        return;
    if (currentState==NULL)
        return;
    currentState->used=0;
    nextState=s;
    return;
}

void initHardware(void)
{
	defaultExceptionHandler();
	glInit();
}
#if 0
void createState(state_struct* s, function i, function f, function k, function vbl)
{
	s->init=(function)i;
	s->frame=(function)f;
	s->kill=(function)k;
	s->vbl=(function)vbl;

	s->id=state_id;
	s->mc_id=0;
	state_id++;
}
#endif

void setState(state_struct* s)
{
    if (!s)
        return;
    currentState=s;
    currentState->used=1;
}

void applyState()
{
	currentState=nextState;
	currentState->used=1;
	currentState->mc_id=0;
	initMalloc();
	irqSet(IRQ_VBLANK, currentState->vbl);
}
