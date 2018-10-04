//	�쐬��	�T�c

#ifndef INCLUDE_DINPUT
#define INCLUDE_DINPUT

#include"JoyStick.h"

// �g�p����L�[�̎��
// KeyCheck�Ƌ��Ɏg���L�[��������΃R�R�Ɏ�ނ�ǉ����Ă���
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


HRESULT InitDinput(HWND);	//Direct�C���v�b�g�̏�����

HRESULT	InitKey ( HWND hWnd );	//	�L�[�̏�����


//	�L�[��ԃ`�F�b�N
//	��1����( �L�[�̒萔 )�A��2����( �L�[�̎�� )
void KeyCheck ( int DIK, int st );

//	�S�ẴL�[�̏�ԃ`�F�b�N
void AllKeyCheck ();

//	�L�[�ʏ�Ԕ���
//	��1����( �L�[�̎�� )�A��2����( �L�[�̏�� )
bool StCheck(int st, int Keyst);

#endif