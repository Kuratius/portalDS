#include "common/general.h"


state_struct gameState={.init=&initGame, .frame=&gameFrame, .kill=&killGame, .vbl=&gameVBL, .id=0, .mc_id=0};
state_struct menuState={.init=&initMenu, .frame=&menuFrame, .kill=&killMenu, .vbl=&menuVBL, .id=1, .mc_id=0};
state_struct editorState={.init=&initEditor, .frame=&editorFrame, .kill=&killEditor, .vbl=&editorVBL, .id=2, .mc_id=0};


extern state_struct * current_state;

void doSPALSH()
{
    setBrightness(3,-16);
    swiWaitForVBlank();
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_5_2D);

    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankC(VRAM_C_SUB_BG);

    int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
    struct gl_texture_t* spalsh=(struct gl_texture_t *)ReadPCXFile("spalsh.pcx","");

    dmaCopy(spalsh->texels, bgGetGfxPtr(bg), 256*192);
    dmaCopy(spalsh->palette, BG_PALETTE, 256*2);

    freePCX(spalsh);

    int bg_sub = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
    struct gl_texture_t* spalsh_sub=(struct gl_texture_t *)ReadPCXFile("spalsh_bottom.pcx","");

    dmaCopy(spalsh_sub->texels, bgGetGfxPtr(bg_sub), 256*192);
    dmaCopy(spalsh_sub->palette, BG_PALETTE_SUB, 256*2);

    freePCX(spalsh_sub);

    fadeIn();
    int i;
    for(i=0;i<60;i++)swiWaitForVBlank();
    fadeOut();
}

int main(int argc, char **argv)
{
    defaultExceptionHandler();
    int ret=initFilesystem(argc, argv);
    if (!ret)
    {
        consoleDemoInit();
        printf("Failed to initalize filesystem.\n");
        printf("Press START to exit.\n");
        while (1)
        {
            swiWaitForVBlank();
            scanKeys();

            if (keysDown() & KEY_START)
                return 1;
        }
    }
    glInit();
#if McuASAN_CONFIG_IS_ENABLED
    nocashMessage("Init ASAN\n");
    McuASAN_Init();
#endif
    //initAudio();

    //doSPALSH();
    nocashMessage("scan keys\n");
    //TEMP DEBUG
    scanKeys();
    scanKeys();
    scanKeys();
    scanKeys();

    if(keysHeld() & KEY_SELECT)
    {
        nocashMessage("editorstate\n");
        changeState(&editorState);
    }
    else
    {
        nocashMessage("gamestate\n");
        changeState(&gameState);
    }
    nocashMessage("menustate\n");
    changeState(&menuState);

    nocashMessage("applystate\n");
    applyState();

    while(1)
    {
        currentState->init();
        while(currentState->used)
            currentState->frame();

        currentState->kill();
        applyState();
    }

    return 0;
}
