#ifndef INCLUDE_JOYSTICK
#define INCLUDE_JOYSTICK

#include"Lib.h"

HRESULT InitJoyStick( HWND , LPDIRECTINPUT8& );

VOID FreeJoyStick();
HRESULT UpdateJoyStick( HWND );

// �L�[�̏�Ԃ̎��
enum KEYSTATE
{
	PUSH,
	RELEASE,
	ON,
	OFF,
	KEYSTATE_MAX
};

//�\���L�[
enum CROSS_KEY{
	CROSS_UP,
	CROSS_RIGHT,
	CROSS_DOWN,
	CROSS_LEFT,
	CROSS_MAX,
};
//�{�^��(XBox360)
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

//�{�^����Ԏ擾
BOOL GetButton( int , BUTTON , KEYSTATE );
//�\���L�[�̓��͏��
BOOL GetCrossKey( int , CROSS_KEY , KEYSTATE );

#endif