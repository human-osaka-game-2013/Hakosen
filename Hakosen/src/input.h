//	作成者	亀田

#ifndef INCLUDE_DINPUT
#define INCLUDE_DINPUT

#include"JoyStick.h"

// 使用するキーの種類
// KeyCheckと共に使うキーが増えればココに種類を追加していく
enum KEYKIND
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ENTER,
	KEY_X,
	KEY_Z,
	KEY_C,
	KEY_V,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_W,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_O,

	KEYMAX
};


HRESULT InitDinput(HWND);	//Directインプットの初期化

HRESULT	InitKey ( HWND hWnd );	//	キーの初期化


//	キー状態チェック
//	第1引数( キーの定数 )、第2引数( キーの種類 )
void KeyCheck ( int DIK, int st );

//	全てのキーの状態チェック
void AllKeyCheck ();

//	キー別状態判定
//	第1引数( キーの種類 )、第2引数( キーの状態 )
bool StCheck(int st, int Keyst);

#endif