/*---------------------------------------------------------------------------------

	default ARM7 core

		Copyright (C) 2005 - 2010
		Michael Noland (joat)
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.

	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.

	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/
#include <nds.h>
#include "stdafx.h"

uint32_t cnt=0;

void VblankHandler(void) {

}

void VcountHandler() {
	inputGetAndSend();
}

volatile bool exitflag = false;

void powerButtonCB() {
	exitflag = true;
}

//OBB_struct *testOBB, *testOBB2;
//plane_struct testPlane;
//extern u32 coll, integ, impul;
//extern u8 sleeping;
ARM_CODE __attribute__((noinline)) void mainLoop(){

    coll=0;
    impul=0;
    integ=0;
	while(!exitflag)
	{
		if(getPI7Status())
		{
            for (int i=0;i<5; i++)
            {
	            listenPI7();
			    updatePlatforms();
			    updateOBBs();
            }
			sendDataPI7();
		}
	    listenPI7();
		const uint16_t key_mask = KEY_SELECT | KEY_START | KEY_L | KEY_R;
		uint16_t keys_pressed = ~REG_KEYINPUT;
		if ((keys_pressed & key_mask) == key_mask)
			exitflag = true;

		swiWaitForVBlank();
		cnt++;
	}

}

int main() {
	enableSound();
	readUserSettings();
	touchInit();
	irqInit();
	fifoInit();
	// mmInstall(FIFO_MAXMOD);
	SetYtrigger(80);
	installSoundFIFO();
	installSystemFIFO();
	//initClockIRQ();
    initClockIRQTimer(0);
	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);

	irqEnable(IRQ_VBLANK | IRQ_VCOUNT);

	setPowerButtonCB(powerButtonCB);

	initPI7();
    mainLoop();
	return 0;
}
