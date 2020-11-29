#include "menu/menu_main.h"

char **testList=NULL;
int testListCnt, testListCnt1;
screenList_struct testScreenList;

void initMenuButtons(void)
{
	initSimpleGui();
}

void setupMenuPage(menuButton_struct* mp, u8 n)
{
	if(!mp || !n)return;

	cleanUpSimpleButtons();

	int i;
	for(i=0;i<n;i++)
	{
		createSimpleButton(vect(0,192-(i+1)*16,0), mp[i].string, mp[i].targetFunction);
	}
}

extern u8 logoAlpha;

void startMenuPlayButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[4],&cameraStates[0],48);
	setupMenuPage(mainMenuPage, mainMenuPageLength);
	logoAlpha=0;
}

menuButton_struct startMenuPage[]={(menuButton_struct){"START", (buttonTargetFunction)startMenuPlayButtonFunction}};
u8 startMenuPageLength=arrayLength(startMenuPage);

void mainMenuPlayButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[0],&cameraStates[1],48);
	setupMenuPage(playMenuPage, playMenuPageLength);
}

void mainMenuCreateButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[0],&cameraStates[2],64);
	setupMenuPage(createMenuPage, createMenuPageLength);
}

menuButton_struct mainMenuPage[]={(menuButton_struct){"Options", NULL}, (menuButton_struct){"Create", (buttonTargetFunction)mainMenuCreateButtonFunction}, (menuButton_struct){"Play", (buttonTargetFunction)mainMenuPlayButtonFunction}};
u8 mainMenuPageLength=arrayLength(mainMenuPage);

void playMenuCampaignButtonFunction(sguiButton_struct* b)
{
	setMapFilePath("maps/test1.map");
	changeState(&gameState);
}

void playMenuLoadLevelButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[1],&cameraStates[3],64);
	setupMenuPage(selectLevelMenuPage, selectLevelMenuPageLength);

	testListCnt=0;
	testListCnt1=0;

	testListCnt1=testListCnt+=listFiles("./maps", NULL);
	#ifndef FATONLY
		char str[255];
		sprintf(str,"%s/%s/maps",basePath,ROOT);
		testListCnt+=listFiles(str, NULL);
	#endif

	testList=malloc(sizeof(char*)*testListCnt);

	listFiles("./maps", testList);
	#ifndef FATONLY
		listFiles(str, &testList[testListCnt1]);
	#endif

	initScreenList(&testScreenList, "Select level", testList, testListCnt);
	updateScreenList(&testScreenList);
}

void playMenuBackButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[1],&cameraStates[0],48);
	setupMenuPage(mainMenuPage, mainMenuPageLength);
}

menuButton_struct playMenuPage[]={(menuButton_struct){"Back", (buttonTargetFunction)playMenuBackButtonFunction}, (menuButton_struct){"Select Level", playMenuLoadLevelButtonFunction}, (menuButton_struct){"Campaign", playMenuCampaignButtonFunction}};
u8 playMenuPageLength=arrayLength(playMenuPage);

void createMenuBackButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[2],&cameraStates[0],64);
	setupMenuPage(mainMenuPage, mainMenuPageLength);
}

void createMenuNewLevelButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[2],&cameraStates[3],64);
	setupMenuPage(newLevelMenuPage, newLevelMenuPageLength);

	resetSceneScreen();
	sprintf(menuScreenText[0],"Level name :");
	sprintf(menuScreenText[1],"  ");

	setupKeyboard(&menuScreenText[1][2], 10, 16, 16);
}

void createMenuLoadLevelButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[2],&cameraStates[3],64);
	setupMenuPage(loadLevelMenuPage, loadLevelMenuPageLength);

	testListCnt=0;
	testListCnt1=0;

	char str[255];
	sprintf(str,"%s/%s/maps",basePath,ROOT);
	testListCnt1=testListCnt+=listFiles(str, NULL);

	testList=malloc(sizeof(char*)*testListCnt);

	listFiles(str, testList);

	initScreenList(&testScreenList, "Load level", testList, testListCnt);
	updateScreenList(&testScreenList);
}

menuButton_struct createMenuPage[]={(menuButton_struct){"Back", (buttonTargetFunction)createMenuBackButtonFunction}, (menuButton_struct){"Load Level", createMenuLoadLevelButtonFunction}, (menuButton_struct){"New level", (buttonTargetFunction)createMenuNewLevelButtonFunction}};
u8 createMenuPageLength=arrayLength(createMenuPage);

void newLevelMenuOKButtonFunction(sguiButton_struct* b)
{
	static char str[2048];
	sprintf(str,"%s/%s/maps/%s.map",basePath,ROOT,&menuScreenText[1][2]);

	setEditorMapFilePath(str);
	changeState(&editorState);
}

void newLevelMenuBackButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[3],&cameraStates[2],64);
	setupMenuPage(createMenuPage, createMenuPageLength);
}

menuButton_struct newLevelMenuPage[]={(menuButton_struct){"Back", (buttonTargetFunction)newLevelMenuBackButtonFunction}, (menuButton_struct){"OK", (buttonTargetFunction)newLevelMenuOKButtonFunction}};
u8 newLevelMenuPageLength=arrayLength(newLevelMenuPage);

void selectLevelMenuUpButtonFunction(sguiButton_struct* b)
{
	screenListMove(&testScreenList, -1);
	updateScreenList(&testScreenList);
}

void selectLevelMenuDownButtonFunction(sguiButton_struct* b)
{
	screenListMove(&testScreenList, 1);
	updateScreenList(&testScreenList);
}

void selectLevelMenuOKButtonFunction(sguiButton_struct* b)
{
	static char str[2048];
	if(testScreenList.cursor<testListCnt1)sprintf(str,"./maps/%s",testScreenList.list[testScreenList.cursor]);
	else sprintf(str,"%s/%s/maps/%s",basePath,ROOT,testScreenList.list[testScreenList.cursor]);

	setMapFilePath(str);
	changeState(&gameState);
}

void selectLevelMenuBackButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[3],&cameraStates[1],64);
	setupMenuPage(playMenuPage, playMenuPageLength);
	freeFileList(testList, testListCnt);
	testList=NULL;
}

menuButton_struct selectLevelMenuPage[]={(menuButton_struct){"Back", (buttonTargetFunction)selectLevelMenuBackButtonFunction}, (menuButton_struct){"OK", (buttonTargetFunction)selectLevelMenuOKButtonFunction}, (menuButton_struct){"Down", (buttonTargetFunction)selectLevelMenuDownButtonFunction}, (menuButton_struct){"Up", (buttonTargetFunction)selectLevelMenuUpButtonFunction}};
u8 selectLevelMenuPageLength=arrayLength(selectLevelMenuPage);

void loadLevelMenuUpButtonFunction(sguiButton_struct* b)
{
	screenListMove(&testScreenList, -1);
	updateScreenList(&testScreenList);
}

void loadLevelMenuDownButtonFunction(sguiButton_struct* b)
{
	screenListMove(&testScreenList, 1);
	updateScreenList(&testScreenList);
}

void loadLevelMenuOKButtonFunction(sguiButton_struct* b)
{
	static char str[2048];
	sprintf(str,"%s/%s/maps/%s",basePath,ROOT,testScreenList.list[testScreenList.cursor]);

	setEditorMapFilePath(str);
	changeState(&editorState);
}

void loadLevelMenuBackButtonFunction(sguiButton_struct* b)
{
	testTransition=startCameraTransition(&cameraStates[3],&cameraStates[2],64);
	setupMenuPage(createMenuPage, createMenuPageLength);
	freeFileList(testList, testListCnt);
	testList=NULL;
}

menuButton_struct loadLevelMenuPage[]={(menuButton_struct){"Back", (buttonTargetFunction)loadLevelMenuBackButtonFunction}, (menuButton_struct){"OK", (buttonTargetFunction)loadLevelMenuOKButtonFunction}, (menuButton_struct){"Down", (buttonTargetFunction)loadLevelMenuDownButtonFunction}, (menuButton_struct){"Up", (buttonTargetFunction)loadLevelMenuUpButtonFunction}};
u8 loadLevelMenuPageLength=arrayLength(loadLevelMenuPage);
