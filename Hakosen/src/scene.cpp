#include <stdio.h>
#include "graphic.h"
#include "scene.h"
#include "game.h"
#include "title.h"
#include "logo.h"
#include "select.h"
#include "input.h"
#include "Result.h"
#include "Audio.h"

int		iCurrentScene = TITLE_SCENE;

int		iBeforeScene = NONE_SCENE;

bool	flag = false;

enum{
	NONE,
	FADE_OUT,
	FADE_IN,
};

int NextScene = 0;
int SceneChangeFlag = NONE;
int ChangeCount = 0;
CUSTOMVERTEX change[4];
bool InitFlag = false;
// シーン初期化
void SceneInit ()
{
	if ( iBeforeScene != iCurrentScene )
	{
		switch ( iCurrentScene )
		{
		case TITLE_SCENE:

			TitleInit ();

			break;

		case SELECT_SCENE:
		
			SelectInit ();

			break;

		case GAME_SCENE:

			GameInit ();

			break;

		case RESULT_SCENE:

			ResultInit ();

			break;
		}
	}
	iBeforeScene = iCurrentScene;
}

// シーン別処理
void SceneControl ( HWND hWnd )
{
	AllKeyCheck ();
	UpdateJoyStick(hWnd);

	switch ( iCurrentScene )
	{
	case LOGO_SCENE:

		LogoControl ();

		break;

	case TITLE_SCENE:

		TitleControl ();

		break;

	case SELECT_SCENE:

		SelectControl ();

		break;

	case GAME_SCENE:

		GameControl ();

		break;

	case RESULT_SCENE:

		ResultControl ();

		break;
	}

	if( GetButton(0,BUTTON_LB,ON ) &&
		GetButton(0,BUTTON_RB,ON ) &&
		GetButton(0,BUTTON_START,PUSH ) &&
		GetButton(0,BUTTON_SELECT,ON ))
		ChangeScene ( TITLE_SCENE );

#ifdef _DEBUG
	else if( GetButton(0,BUTTON_LB,ON ) &&
		GetButton(0,BUTTON_RB,ON ) &&
		GetButton(0,BUTTON_START,ON ) &&
		GetButton(0,BUTTON_SELECT,PUSH ))
		PostQuitMessage (0);
#endif

	if( SceneChangeFlag ==  FADE_OUT ){
		ColorKey( change , ChangeCount , 0, 0,0);
		ChangeCount+=10;
		if( ChangeCount >= 255 ){
			iCurrentScene = NextScene;
			ReleaseAllTexture ();
			LoadTexture( "image\\blk.png" , BLACK , TEMP_RGB );
			SceneChangeFlag = FADE_IN;

			//	追加日	2/27	亀田
			SoundAllStop();
			ReleaseAllSoundFile();
		}
	}
	if( SceneChangeFlag == FADE_IN ) {
		ColorKey( change , ChangeCount , 0, 0,0);
		ChangeCount-=10;
		if( ChangeCount <= 0 ){
			SceneChangeFlag = NONE;
		}
	}
}

// シーン別描画
void SceneDraw ()
{

	switch ( iCurrentScene )
	{
	case TITLE_SCENE:

		TitleDraw ();

		break;

	case SELECT_SCENE:

		SelectDraw ();

		break;

	case GAME_SCENE:

		GameDraw ();

		break;

	case RESULT_SCENE:

		ResultDraw ();

		break;
	}


	if( SceneChangeFlag != NONE )
		Draw(BLACK,change);
}

void ChangeScene ( int iScene )
{
	SceneChangeFlag = FADE_OUT;
	SetVertex( BLACK , change , 0.f, 0.f );
	ColorKey(change,255,0,0,0);
	for(int i=0;i<4;i++)
		change[i].z = 0.1f;

	NextScene = iScene;
	//ReleaseAllTexture ();
}