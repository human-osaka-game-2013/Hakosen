#include"Result.h"
#include "puzzle.h"
#include "input.h"
#include "scene.h"
#include"select.h"
#include"game.h"
#include "words.h"
#include "Audio.h"

int Winner = 0;

enum{
	R_RETRY,
	R_CHARASELECT,
	R_TITLE,
	R_MAX,
};
int Retry = R_RETRY;
bool R_RollFlag = false;
int R_RollCount = 0;
float R_Roll = 0.f;
CUSTOMVERTEX Chara[2][4];
CUSTOMVERTEX Result[2][4];
CUSTOMVERTEX RSelect[4];

//	’Ç‰Á“ú	2/27	‹T“c
int iR_Count;
int	iR_Size;
float fR_Angle;

OBJECT_INFO objChara[2];
OBJECT_INFO objResult[2];

void ResultInit()
{
	LoadTexture ("image\\puzzle\\background1.png", PUZZLE_BACK, 0, 255, 0);
	LoadTexture ("image\\puzzle\\BOY.png", CHARA_BOY, 255, 255, 255);
	LoadTexture ("image\\puzzle\\GIRL.png", CHARA_GIRL, 255, 255, 255);
	LoadTexture ( "image\\Result\\WIN.png", RESULT_WIN, TEMP_RGB );
	LoadTexture ( "image\\Result\\LOSE.png", RESULT_LOSE, TEMP_RGB );
	LoadTexture ( "image\\Result\\retry_select.png", RESULT_SELECT , TEMP_RGB );
	for( int i = 0 ; i < 2 ; i++ ){
		SetVertex(GetPlayerType(i),Chara[i],
			(RESULT_CHARA2_LEFT - RESULT_CHARA1_LEFT ) * i + RESULT_CHARA1_LEFT + RESULT_CHARA_SIZE / 2.f,
			RESULT_CHARA_TOP + RESULT_CHARA_SIZE / 2.f,
			RESULT_CHARA_SIZE,
			RESULT_CHARA_SIZE);

		SetVertex( RESULT_WIN ,Result[i] ,
			(RESULT_LEFT2 - RESULT_LEFT ) * i + RESULT_LEFT + RESULT_SIZE_X / 2.f,
			RESULT_TOP + RESULT_SIZE_Y / 2.f,
			RESULT_SIZE_X,
			RESULT_SIZE_Y);

		objChara[i].fXpos = Chara[i][0].x + RESULT_CHARA_SIZE/2;
		objChara[i].fYpos = Chara[i][0].y + RESULT_CHARA_SIZE/2;
		objChara[i].fHeight = RESULT_CHARA_SIZE;
		objChara[i].fWidth = RESULT_CHARA_SIZE;

		objResult[i].fXpos = Result[i][0].x + RESULT_SIZE_X/2;
		objResult[i].fYpos = Result[i][0].y + RESULT_SIZE_Y/2;
		objResult[i].fHeight = RESULT_SIZE_Y;
		objResult[i].fWidth = RESULT_SIZE_X;
	}
	SetVertex( RESULT_WIN ,RSelect , 340.f , 570.f );

	Retry = R_RETRY;
	R_RollFlag = false;
	R_RollCount = 0;
	R_Roll = 0.f;

	int iR_Count = 0;
	int	iR_Size = 10;
	float fR_Angle = 0.f;

	LoadTexture ( "image\\puzzle\\girl_message.png", GIRLS_WORDS, TEMP_RGB2);
	
	LoadTexture ( "image\\puzzle\\boy_message.png", BOYS_WORDS, TEMP_RGB2);

	LoadTexture ( "image\\puzzle\\hukidasi_1.png", WORDS_BALLOON1, TEMP_RGB2);

	LoadTexture ( "image\\puzzle\\hukidasi_2.png", WORDS_BALLOON2, TEMP_RGB2);

	LoadSoundFile ( "sound\\pzl_bgm2.wav", RESULT_BGM );

	LoadSoundFile ( "sound\\tit_ido.wav", R_CURSOR_MOVE_SE );

	LoadSoundFile ( "sound\\tit_kettei.wav", R_CURSOR_SELECT_SE );

	SoundPlay ( RESULT_BGM, false );
}

void ResultControl()
{
	for(int i = 0 ; i < 2 ; i++ ){
		if( GetPlayerDeath(i) )
			Winner = i == 0 ? 1 : 0;
	}
	for( int i = 0 ; i < 2 ; i++ ){
		if(Winner==i){
			Chara[i][0].tu = Chara[i][3].tu = 1.f / 3.f;
			Chara[i][1].tu = Chara[i][2].tu = 2.f / 3.f;
		}
		else{
			Chara[i][0].tu = Chara[i][3].tu = 2.f / 3.f;
			Chara[i][1].tu = Chara[i][2].tu = 3.f / 3.f;
		}
	}

	if( !R_RollFlag && ( GetCrossKey( 0 , CROSS_UP , PUSH ) || StCheck(UP,PUSH) ) )
	{
		Retry--;
		if(Retry < R_RETRY)
			Retry = R_TITLE;
		R_RollFlag = true;
		R_Roll = -1.f / 3.f / 10.f;

		SoundPlay ( R_CURSOR_MOVE_SE, false );
	}
	else if( !R_RollFlag && ( GetCrossKey( 0 , CROSS_DOWN , PUSH ) || StCheck(DOWN,PUSH) ) )
	{
		Retry++;
		if(Retry >= R_MAX )
			Retry = R_RETRY;
		R_RollFlag = true;
		R_Roll = 1.f / 3.f / 10.f;

		SoundPlay ( R_CURSOR_MOVE_SE, false );
	}

	if( R_RollFlag ){
		R_RollCount++;
		if( R_RollCount >= 11 ){
			R_RollFlag = false;
			R_RollCount = 0;
			R_Roll = 0.f;
		}
	}

	if ( !R_RollFlag && ( GetButton( 0 , BUTTON_A , PUSH ) || StCheck(ENTER,PUSH) ) )
	{
		if( Retry == R_CHARASELECT )
			ChangeScene ( SELECT_SCENE );
		else if( Retry == R_TITLE )
			ChangeScene ( TITLE_SCENE );
		else if( Retry == R_RETRY){
			ChangeScene ( GAME_SCENE );
		}

		SoundPlay ( R_CURSOR_SELECT_SE, false );
	}

	if ( iR_Count < 30 )
	{
		iR_Size += 2;
	}
	else if ( iR_Count < 60 )
	{
		iR_Size -= 2;
	}
	else
	{
		iR_Count = 0;
		iR_Size	= 0;
	}

	iR_Count++;
	

}

void ResultDraw()
{
	Draw(PUZZLE_BACK,0.f,0.f);

	ColorKeyDraw( BLACK , (float)WIDTH / 2.f , (float)HEIGHT / 2.f , 0.f , (float)WIDTH , (float)HEIGHT ,
		0.f , 0.f , 1.f ,1.f , 120 , 0 , 0 , 0 );

	for( int i = 0 ; i < 2 ; i++ ){
		if( GetPlayerDeath(i) )
			Winner = i == 0 ? 1 : 0;

		if(Winner==i)
		{
			objResult[i].fHeight = RESULT_SIZE_Y * 1.1f;
			objResult[i].fWidth = RESULT_SIZE_X * 1.1f;

			objChara[i].fHeight = RESULT_CHARA_SIZE * 1.1f;
			objChara[i].fWidth = RESULT_CHARA_SIZE * 1.1f;

			Elasticity ( Chara[i], objChara[i].fXpos, objChara[i].fYpos,
				fR_Angle, objChara[i].fWidth, objChara[i].fHeight, -1 );

			Elasticity ( Result[i], objResult[i].fXpos, objResult[i].fYpos,
				fR_Angle, objResult[i].fWidth + iR_Size, objResult[i].fHeight + iR_Size, -1 );

			Draw(GetPlayerType(i),Chara[i]);

			Draw( RESULT_WIN ,Result[i]);
		}
		else
		{
			objResult[i].fHeight = RESULT_SIZE_Y * 0.9f;
			objResult[i].fWidth = RESULT_SIZE_X * 0.9f;

			objChara[i].fHeight = RESULT_CHARA_SIZE * 0.9f;
			objChara[i].fWidth = RESULT_CHARA_SIZE * 0.9f;

			Elasticity ( Chara[i], objChara[i].fXpos, objChara[i].fYpos,
				fR_Angle, objChara[i].fWidth, objChara[i].fHeight, -1 );

			Elasticity ( Result[i], objResult[i].fXpos, objResult[i].fYpos,
				fR_Angle, objResult[i].fWidth - iR_Size, objResult[i].fHeight - iR_Size, -1 );

			Draw(GetPlayerType(i),Chara[i]);

			Draw( RESULT_LOSE ,Result[i]);
		}
	}

	WordDraw ( true );

	RSelect[0].tu = RSelect[3].tu = 0.f;
	RSelect[1].tu = RSelect[2].tu = 1.f;
	RSelect[0].tv += R_Roll; 
	RSelect[3].tv += R_Roll;
	RSelect[1].tv += R_Roll;
	RSelect[2].tv += R_Roll;
	Draw( RESULT_SELECT, RSelect );
}