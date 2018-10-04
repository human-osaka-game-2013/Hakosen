//	�쐬��	�T�c

#include <stdio.h>

#include "Lib.h"
#include "input.h"


LPDIRECTINPUT8			pDinput			= NULL; //�C���v�b�g -> ���͏���
LPDIRECTINPUTDEVICE8	pKeyDevice		= NULL; //�L�[�f�o�C�X -> �O���L�[����
LPDIRECTINPUTDEVICE8	pMouseDevice	= NULL; //�}�E�X�f�o�C�X -> �O���}�E�X����

// �L�[�̏�Ԃ���ޕʂɊi�[����ϐ�
int Key[KEYMAX];

// �L�[�̎�ޕʂɉ�����Ă��邩�̃t���O
bool PreKey[KEYMAX] = {false};

//==================================================================
//
//	�_�C���N�g�C���v�b�g�̏�����
//
//=================================================================
//�_�C���N�g�C���v�b�g�̏������֐�
HRESULT InitDinput(HWND hWnd)
{
	HRESULT hr;
	
	//�DirectInput��I�u�W�F�N�g�̍쐬
	if(FAILED (hr = DirectInput8Create (
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(VOID**)&pDinput,
		NULL
		) ) )
	{
		return hr;
	}

	InitJoyStick(hWnd,pDinput);

	//if(FAILED ( hr = pDinput->EnumDevices(
	//		DI8DEVCLASS_GAMECTRL,
	//		lpCallback,
	//		pvRef,
	//		DIEDFL_ATTACHEDONLY
	//	)))
	//{
	//	return hr;
	//}
	if (FAILED ( InitKey ( hWnd )))
	{
		return hr;
	}	
	return S_OK;
}

//------------------------------------------------------------------
//
//	�L�[�̐ݒ�
//	
//-----------------------------------------------------------------
HRESULT	InitKey ( HWND hWnd )
{
	HRESULT hr;

	//�DirectInput�f�o�C�X��I�u�W�F�N�g�̍쐬(�L�[�{�[�h)
	if(FAILED (hr = pDinput->CreateDevice (
	GUID_SysKeyboard,
	&pKeyDevice,
	NULL

	) ) )
	{
		return hr;
	}
	//�f�o�C�X���L�[�{�[�h�ɐݒ�
	if(FAILED (hr = pKeyDevice->SetDataFormat (&c_dfDIKeyboard) ) )
	{
		return hr;
	}
	//�������x���̐ݒ�		*���Y�A�v���P�[�V����(�Q�[��)�ƁAwindowsOS���̂̃L�[�{�[�h�f�o�C�X�̐�L���ݒ�
	if(FAILED (hr = pKeyDevice->SetCooperativeLevel(
		hWnd,
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND		// * ��r���Ńo�b�N�E�O���E���h
		) ) )
	{
		return hr;
	}
	//�f�o�C�X��擾�����
	pKeyDevice->Acquire();							// * �쐬�����f�o�C�X�ւ̃A�N�Z�X���擾
	return S_OK;
}

//==================================================================
//	�L�[��ԃ`�F�b�N
// ��1����( �L�[�̒萔 )�A��2����( �L�[�̎�� )
//=================================================================
//	��1����( �L�[�̒萔 )�A��2����( �L�[�̎�� )
void KeyCheck ( int DIK, int st )
{
	HRESULT hr = pKeyDevice->Acquire();		//�f�o�C�X�ւ̃A�N�Z�X���̍Ď擾
	
	if( (hr==DI_OK) || (hr==S_FALSE) )	
	{
		BYTE diks[256];
		pKeyDevice->GetDeviceState(sizeof (diks), &diks);

		if ( diks[DIK] & 0x80 )
		{
			if ( !PreKey[st] )
			{
				Key[st] = PUSH;
			}
			else
			{
				Key[st] = ON;
			}
			PreKey[st] = true;
		}

		else
		{
			if ( !PreKey[st] )
			{
				Key[st] = OFF;
			}
			else
			{
				Key[st] = RELEASE;
			}
			PreKey[st] = false;
		}
	}
}

//	�L�[�ʏ�Ԕ���
//	��1����( �L�[�̎�� )�A��2����( �L�[�̏�� )
bool StCheck(int st, int Keyst) 
{
	return ( Key[st] == Keyst );
}

//	�S�ẴL�[�̏�ԃ`�F�b�N
void AllKeyCheck ()
{
	KeyCheck ( DIK_RETURN, ENTER );
	KeyCheck ( DIK_LEFT, LEFT );
	KeyCheck ( DIK_RIGHT, RIGHT );
	KeyCheck ( DIK_UP, UP );
	KeyCheck ( DIK_DOWN, DOWN );
	KeyCheck ( DIK_X, KEY_X );
	KeyCheck ( DIK_Z, KEY_Z );
	KeyCheck ( DIK_C, KEY_C );
	KeyCheck ( DIK_V, KEY_V );
	KeyCheck ( DIK_A, KEY_A );
	KeyCheck ( DIK_S, KEY_S );
	KeyCheck ( DIK_D, KEY_D );
	KeyCheck ( DIK_W, KEY_W );
	KeyCheck ( DIK_K, KEY_K );
	KeyCheck ( DIK_L, KEY_L );
	KeyCheck ( DIK_M, KEY_M );
	KeyCheck ( DIK_O, KEY_O );
	
}

