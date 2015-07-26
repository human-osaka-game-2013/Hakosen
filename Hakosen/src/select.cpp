#include "graphic.h"
#include "select.h"
#include "input.h"
#include "scene.h"
#include "Audio.h"

#define MOJI_SPEED (2.f) 
#define CHARA_MAX ( 2 )

#define CHARA_WIDTH ( 300.f )
#define CHARA_HEIGHT ( 480.f )

#define ARROW_SIZE ( 120.f )

struct PLAYER_SELECT{
	bool SelectFlag;
	int CharNum;
};

CUSTOMVERTEX chara[CHARA_MAX][4];

CUSTOMVERTEX Moji[2][4];

CUSTOMVERTEX Player[2][4];

CUSTOMVERTEX Cursor[2][4];
CUSTOMVERTEX Face[2][4];

PLAYER_SELECT PlayerChar[2] = {false,0};

OBJECT_INFO	objPlMarker[2];

int SelectCount = 0;

//
float Face_pos[2][2]= {
	{580.f,180.f},
	{580.f,420.f}
};
//
float Arrow_pos[2][CHAR_MAX][2] = {
	{
		{ 460.f , 180.f },
		{ 700.f , 180.f }
	},
	{
		{ 460.f , 420.f },
		{ 700.f , 420.f }
	}	
};
//	セレクトの初期化
void SelectInit ()
{
	LoadTexture ( "image\\CharaSele\\background1.png", SELECT_BG_TEXTURE, TEMP_RGB );
	LoadTexture ( "image\\CharaSele\\chara_stand1.png", SELECT_CHARA, TEMP_RGB );
	LoadTexture ( "image\\CharaSele\\arrow.png", SELECT_PLAYER_CURSOR, TEMP_RGB );

	LoadTexture ( "image\\CharaSele\\cs_12P.png", PL_MARKER, TEMP_RGB );

	LoadTexture ( "image\\CharaSele\\cs_OK.png", SELECT_OK, TEMP_RGB );

	LoadTexture ( "image\\CharaSele\\cs_logo.png", SELECT_TITLE, TEMP_RGB );

	LoadTexture ( "image\\CharaSele\\face.png", SELECT_FACE, TEMP_RGB );

	LoadTexture ("image\\puzzle\\Ele_BOX.png", ELE_BOX, 0, 255, 0);
	LoadTexture ("image\\puzzle\\moji.png", ELE_BOX_MOJI, 0, 255, 0);
	LoadTexture ("image\\CharaSele\\CS_AB.png", ELE_BOX_MOJI2, 0, 255, 0);
	SetVertex( SELECT_CHARA , chara[0] , 140.f , 120.f );

	SetVertex( ELE_BOX_MOJI , Moji[0] , WIDTH , 630.f );
	SetVertex( ELE_BOX_MOJI2 , Moji[1] , WIDTH , 655.f );
	Moji[1][0].tv = Moji[1][1].tv = 0.f;
	Moji[1][2].tv = Moji[1][3].tv = 0.5f;

	chara[0][1].x = chara[0][2].x = 140.f + CHARA_WIDTH;
	chara[0][2].y = chara[0][3].y = 120.f + CHARA_HEIGHT;
	chara[0][0].tu = chara[0][3].tu = 0.f;
	chara[0][1].tu = chara[0][2].tu = 0.5f;
	chara[0][0].tv = chara[0][1].tv = 0.f;
	chara[0][2].tv = chara[0][3].tv = 1.f;

	SetVertex( SELECT_CHARA , chara[1]  , 840.f , 120.f );
	chara[1][1].x = chara[1][2].x = 840.f + CHARA_WIDTH ;
	chara[1][2].y = chara[1][3].y = 120.f + CHARA_HEIGHT ;
	chara[1][0].tu = chara[1][3].tu = 0.5f;
	chara[1][1].tu = chara[1][2].tu = 1.f;
	chara[1][0].tv = chara[1][1].tv = 0.f;
	chara[1][2].tv = chara[1][3].tv = 1.f;

	for(int i = 0 ; i < 4 ; i++ ){
		Cursor[0] [i].color = D3DCOLOR_ARGB(255,0,0,255);
		Cursor[1] [i].color = D3DCOLOR_ARGB(255,255,0,0);
	}
	SetVertex( SELECT_PLAYER_CURSOR , Cursor[0] , 0.f , 0.f );
	Cursor[0][0].tu = Cursor[0][3].tu = 0.f;
	Cursor[0][1].tu = Cursor[0][2].tu = 0.5f;
	Cursor[0][0].tv = Cursor[0][1].tv = 0.f;
	Cursor[0][2].tv = Cursor[0][3].tv = 1.f;

	SetVertex( SELECT_PLAYER_CURSOR , Cursor[1] , 0.f , 0.f );
	Cursor[1][0].tu = Cursor[1][3].tu = 0.5f;
	Cursor[1][1].tu = Cursor[1][2].tu = 1.f;
	Cursor[1][0].tv = Cursor[1][1].tv = 0.f;
	Cursor[1][2].tv = Cursor[1][3].tv = 1.f;

	PlayerChar[0].CharNum = 0;
	PlayerChar[0].SelectFlag = false;
	PlayerChar[1].CharNum = 1;
	PlayerChar[1].SelectFlag = false;
	SelectCount = 0;

	objPlMarker[0].fXpos = WIDTH / 2;
	objPlMarker[0].fYpos = Arrow_pos[0][0][1] - ARROW_SIZE / 2;
	objPlMarker[0].fHeight = ARROW_SIZE;
	objPlMarker[0].fWidth = ARROW_SIZE;
	objPlMarker[0].fAngle = 0.f;
	objPlMarker[0].iAlpha = 255;
	objPlMarker[0].iRed	  = 255;
	objPlMarker[0].iGreen = 100;
	objPlMarker[0].iBlue  = 100;

	objPlMarker[1].fXpos = WIDTH / 2;
	objPlMarker[1].fYpos = Arrow_pos[1][0][1] + ARROW_SIZE + ( ARROW_SIZE / 2 ) ;
	objPlMarker[1].fHeight = ARROW_SIZE;
	objPlMarker[1].fWidth = ARROW_SIZE;
	objPlMarker[1].fAngle = 0.f;
	objPlMarker[1].iAlpha = 255;
	objPlMarker[1].iRed	  = 100;
	objPlMarker[1].iGreen = 100;
	objPlMarker[1].iBlue  = 255;

	for( int i = 0 ; i < 2 ; i++ )
		SetVertex( SELECT_FACE , Face[i] , Face_pos[i][0] , Face_pos[i][1] );

	LoadSoundFile ( "sound\\CS_bgm.wav", CS_BGM );
	
	LoadSoundFile ( "sound\\CS_ido.wav", CS_CURSOR_MOVE_SE );
	
	LoadSoundFile ( "sound\\CS_OK.wav", CS_BOY_SE );
	
	LoadSoundFile ( "sound\\CS_OK.wav", CS_GIRL_SE );
	
	LoadSoundFile ( "sound\\CS_OK.wav", CS_SELECT_SE );

	SoundPlay ( CS_BGM, true );
}

//	セレクトの処理
void SelectControl ()
{
	for( int i = 0 ; i < 2 ; i++ ){
		if( !PlayerChar[i].SelectFlag && ( 
			( GetCrossKey( i , CROSS_LEFT , PUSH ) || GetCrossKey( i , CROSS_RIGHT , PUSH ) ) ||
			( ( i==1 ? StCheck ( LEFT, PUSH ) : StCheck (KEY_A, PUSH ) ) || ( i==1?StCheck ( RIGHT, PUSH ):StCheck(KEY_D, PUSH ) ) ) 
			) ){
			PlayerChar[i].CharNum = PlayerChar[i].CharNum==0 ? 1 : 0 ;

			SoundPlay ( CS_CURSOR_MOVE_SE, false );
		}
		else if( GetButton( i , BUTTON_A , PUSH ) || ( i==1?StCheck ( KEY_K, PUSH ):StCheck (KEY_Z, PUSH ) ) )
		{
			PlayerChar[i]	.SelectFlag = true;

			if ( PlayerChar[i].CharNum == 0  ){
				SoundPlay ( CS_GIRL_SE, false );
			}
			else if ( PlayerChar[i].CharNum == 1  ){
				SoundPlay ( CS_BOY_SE, false );
			}
		}
		else if( PlayerChar[i].SelectFlag && ( GetButton( i , BUTTON_B , PUSH )  || ( i==1?StCheck ( KEY_L, PUSH ):StCheck (KEY_X, PUSH ))))
			PlayerChar[i].SelectFlag = false;
	}

	for(int i = 0; i < 2 ; i++ )
	{
		Moji[i][0].x = Moji[i][3].x -= MOJI_SPEED;
		Moji[i][1].x = Moji[i][2].x -= MOJI_SPEED;
		if( Moji[i][1].x <= 0 ){
			SetVertex( ELE_BOX_MOJI + i , Moji[i] , WIDTH , 630.f + 40.f*i );
		}
	}
	Moji[1][2].y = Moji[1][3].y = Moji[1][0].y + 40.f ;
	Moji[1][0].tv = Moji[1][1].tv = 0.f;
	Moji[1][2].tv = Moji[1][3].tv = 0.5f;
	if( PlayerChar[0]	.SelectFlag && PlayerChar[1].SelectFlag ){
		Moji[1][0].tv = Moji[1][1].tv = 0.5f;
		Moji[1][2].tv = Moji[1][3].tv = 1.0f;

		if( GetButton( 0 , BUTTON_START , PUSH ) || StCheck(ENTER,PUSH)){
			ChangeScene ( GAME_SCENE );

			SoundPlay ( CS_SELECT_SE, false );
		}
	}
	//if( StCheck(ENTER,PUSH) ){
	//	ChangeScene ( GAME_SCENE );

	//	SoundPlay ( CS_SELECT_SE, false );
	//}

	for(int i = 0; i < 2 ; i++ )
	{
		Moji[i][0].x = Moji[i][3].x -= MOJI_SPEED;
		Moji[i][1].x = Moji[i][2].x -= MOJI_SPEED;
		if( Moji[i][1].x <= 0 ){
			SetVertex( ELE_BOX_MOJI+i , Moji[i] , WIDTH , 630.f + 25.f*i );
		}
	}
}

//	セレクトの描画
void SelectDraw ()
{
	Draw (SELECT_BG_TEXTURE , 0.f , 0.f );

	ColorKeyDraw( BLACK , (float)WIDTH / 2.f , (float)HEIGHT / 2.f , 0.f , (float)WIDTH , (float)HEIGHT ,
		0.f , 0.f , 1.f ,1.f , 120 , 0 , 0 , 0 );

	Draw (SELECT_TITLE , 340.f , 20.f );	

	for( int i = 0 ; i < CHARA_MAX ; i++ ){
		Draw(SELECT_CHARA,chara[i]);
	}

	for( int i = 0 ; i < 2 ; i++ ){
		//矢印
		if( i == 0 ){
			Cursor[0][0].tu = Cursor[0][3].tu = 0.0f;
			Cursor[0][1].tu = Cursor[0][2].tu = 1.f;
			ColorKey ( Cursor[i], 255, 255, 100, 100 );
		}
		else if( i == 1 ){
			Cursor[1][0].tu = Cursor[1][3].tu = 0.0f;
			Cursor[1][1].tu = Cursor[1][2].tu = 1.f;
			ColorKey ( Cursor[i], 255, 100, 100, 255 );
		}

		Cursor[i][0].x = Cursor[i][3].x = Arrow_pos[i][PlayerChar[i].CharNum][0] ;
		Cursor[i][0].y = Cursor[i][1].y = Arrow_pos[i][PlayerChar[i].CharNum][1];
		Cursor[i][1].x = Cursor[i][2].x = Cursor[i][0].x + ARROW_SIZE;
		Cursor[i][3].y = Cursor[i][2].y = Cursor[i][0].y + ARROW_SIZE;

		if( !PlayerChar[i].SelectFlag ){
			if( PlayerChar[i].CharNum == 0 ){
				float tmp = 0.f;
				tmp = Cursor[i][0].tu;
				Cursor[i][0].tu = Cursor[i][3].tu = Cursor[i][1].tu;
				Cursor[i][1].tu = Cursor[i][2].tu = tmp;
			}	
			Draw( SELECT_PLAYER_CURSOR , Cursor[i] );
		}
		else
			Draw( SELECT_OK , Cursor[i] );

		//顔
	
		Face[i][1].x = Face[i][2].x = Face_pos[i][0] + ARROW_SIZE ;
		Face[i][2].y = Face[i][3].y = Face_pos[i][1] + ARROW_SIZE ;

		Face[i][0].tu = Face[i][3].tu = 0.5f * PlayerChar[i].CharNum;
		Face[i][1].tu = Face[i][2].tu = Face[i][0].tu + 0.5f;
		Draw( BLACK , Face[i] );
		Draw( SELECT_FACE ,Face[i]);
		
		CenterPointDraw(ELE_BOX,Face_pos[i][0] + ARROW_SIZE/2.f , Face_pos[i][1] + ARROW_SIZE/2.f,
			0.f, ARROW_SIZE + 20.f , ARROW_SIZE + 20.f , 0.f , 0.f , 1.f , 1.f );

		Draw( SELECT_FACE , Face[i] );
		
		Draw(ELE_BOX_MOJI+i , Moji[i]);

		ColorKeyDraw ( PL_MARKER, objPlMarker[0].fXpos, objPlMarker[0].fYpos, objPlMarker[0].fAngle, 
			objPlMarker[0].fWidth, objPlMarker[0].fHeight, 0.f, 0.f, 0.5f, 1.0f, 
			objPlMarker[0].iAlpha, objPlMarker[0].iRed, objPlMarker[0].iGreen, objPlMarker[0].iBlue);
		ColorKeyDraw ( PL_MARKER, objPlMarker[1].fXpos, objPlMarker[1].fYpos, objPlMarker[1].fAngle, 
			objPlMarker[1].fWidth, objPlMarker[1].fHeight, 0.5f, 0.f, 1.0f, 1.0f, 
			objPlMarker[1].iAlpha, objPlMarker[1].iRed, objPlMarker[1].iGreen, objPlMarker[1].iBlue);
	}

}

int GetPlayerChar( int _pn )
{
	return PlayerChar[_pn].CharNum;
}