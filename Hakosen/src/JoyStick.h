#ifndef INCLUDE_JOYSTICK
#define INCLUDE_JOYSTICK

#include"Lib.h"

HRESULT InitJoyStick( HWND , LPDIRECTINPUT8& );

VOID FreeJoyStick();
HRESULT UpdateJoyStick( HWND );

// キーの状態の種類
enum KEYSTATE
{
	PUSH,
	RELEASE,
	ON,
	OFF,
	KEYSTATE_MAX
};

//十字キー
enum CROSS_KEY{
	CROSS_UP,
	CROSS_RIGHT,
	CROSS_DOWN,
	CROSS_LEFT,
	CROSS_MAX,
};
//ボタン(XBox360)
enum BUTTON{
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_LB,
	BUTTON_RB,
	BUTTON_SELECT,
	BUTTON_START,
	BUTTON_LEFT_STICK,
	BUTTON_RIGHT_STICK,
	BUTTON_MAX,
};

//ボタン状態取得
BOOL GetButton( int , BUTTON , KEYSTATE );
//十字キーの入力状態
BOOL GetCrossKey( int , CROSS_KEY , KEYSTATE );

#endif