#include "graphic.h"
#include "title.h"
#include "title_object.h"
#include "input.h"
#include "scene.h"
#include "Audio.h"

#define EmblemRoll 1.0f

//サブタイトル
CUSTOMVERTEX SubTit[4];
//ボタン用
CUSTOMVERTEX tit_button[4];
//スタートorエンド
bool StartFlag = true;
//回転中
bool RollFlag = 0;
float Roll = 0.f;
int RollCount = 0;

float EmblemCount = 0.f;
bool EmblemFlag = false;

//	タイトルの初期化
void TitleInit ()
{

	LoadTexture ( "image\\title\\background1.png", TITLE_BG_TEXTURE, TEMP_RGB );

	LoadTexture ( "image\\title\\tit_titlogo.png", TITLE_TITLE, TEMP_RGB );

	LoadTexture ( "image\\title\\tit_sblogo.png", TITLE_SUB_TITLE, TEMP_RGB );

	LoadTexture ( "image\\title\\tit_button.png", TITLE_BUTTON, TEMP_RGB );

	LoadTexture ( "image\\title\\emblem.png", TITLE_EMBLEM, TEMP_RGB );

	SetVertex( TITLE_SUB_TITLE , SubTit , 0.f , 50.f );
	Rotate( SubTit , 250.f,100.f,340.f,640.f,80.f);

	SetVertex( TITLE_BUTTON , tit_button , 320.f , 600.f );
	tit_button[0].tu = tit_button[3].tu = 0.f;
	tit_button[1].tu = tit_button[2].tu = 1.f;
	tit_button[0].tv = tit_button[1].tv = 0.f;
	tit_button[3].tv = tit_button[2].tv = 0.5f;

	tit_button[2].y = tit_button[3].y = 680.f;


	StartFlag = true;
	RollFlag = 0;
	Roll = 0.f;
	RollCount = 0;
	EmblemCount = 0;
	EmblemFlag = false;
	LoadSoundFile ( "sound\\tit_bgm.wav", TITLE_BGM );
	
	LoadSoundFile ( "sound\\tit_ido.wav", T_CURSOR_MOVE_SE );
	
	LoadSoundFile ( "sound\\tit_kettei.wav", T_CURSOR_SELECT_SE );

	SoundPlay ( TITLE_BGM, true );
}

//	タイトルの処理
void TitleControl ()
{

	if( !RollFlag && ( GetCrossKey( 0 , CROSS_UP , PUSH ) || StCheck(UP,PUSH) ) )
	{
		SoundPlay ( T_CURSOR_MOVE_SE, false );
		
		StartFlag = StartFlag ? false : true;
		RollFlag = true;
		Roll = -0.05f;
	}
	else if( !RollFlag && ( GetCrossKey( 0 , CROSS_DOWN , PUSH ) || StCheck(DOWN,PUSH) ) )
	{
		SoundPlay ( T_CURSOR_MOVE_SE, false );

		StartFlag = StartFlag ? false : true;
		RollFlag = true;
		Roll = 0.05f;
	}

	if( RollFlag ){
		RollCount++;
		if( RollCount >= 11 ){
			RollFlag = false;
			RollCount = 0;
			Roll = 0.f;
		}
	}

	if(!EmblemFlag){
		EmblemCount+=EmblemRoll;
		if( EmblemCount >= 100.f )
			EmblemFlag = true;
	}
	else{
		EmblemCount-=EmblemRoll;
		if( EmblemCount <= 0.f )
			EmblemFlag = false;
	}

	if ( !RollFlag && ( GetButton( 0 , BUTTON_A , PUSH ) || StCheck(ENTER,PUSH) ) )
	{
		SoundPlay ( T_CURSOR_SELECT_SE, false );

		if( StartFlag )
			ChangeScene ( SELECT_SCENE );
		else 
			PostQuitMessage (0);
	}


}

//	タイトルの描画
void TitleDraw ()
{
	Draw ( TITLE_BG_TEXTURE , 0.f , 0.f );

	ColorKeyDraw( BLACK , (float)WIDTH / 2.f , (float)HEIGHT / 2.f , 0.f , (float)WIDTH , (float)HEIGHT ,
		0.f , 0.f , 1.f ,1.f , 120 , 0 , 0 , 0 );
	
	//Draw ( TITLE_TITLE , 40.f , 160.f );

	CenterPointDraw(TITLE_EMBLEM,
		WIDTH/2.f,
		HEIGHT/2.f,
		0.f /* + ( EmblemCount - 50.f ) / 5.f*/ ,
		fabs( 600.f - EmblemCount * 12.f ) , 600.f ,
		EmblemCount <= 50 ? 0.f : 1.f  , 0.f ,
		EmblemCount <= 50 ? 1.f : 0.f , 1.f );

	CenterPointDraw(TITLE_TITLE,640.f,400.f,0.f,1200.f,600.f,0.f,0.f,1.f,1.f);

	Draw ( TITLE_SUB_TITLE , SubTit );

	tit_button[0].tu = tit_button[3].tu = 0.f;
	tit_button[1].tu = tit_button[2].tu = 1.f;
	tit_button[0].tv += Roll; 
	tit_button[3].tv += Roll;
	tit_button[1].tv += Roll;
	tit_button[2].tv += Roll;

	Draw ( TITLE_BUTTON , tit_button );
}
