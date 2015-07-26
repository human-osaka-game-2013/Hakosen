//	作成者	亀田

#include <stdio.h>

#include "Lib.h"
#include "input.h"


LPDIRECTINPUT8			pDinput			= NULL; //インプット -> 入力処理
LPDIRECTINPUTDEVICE8	pKeyDevice		= NULL; //キーデバイス -> 外部キー入力
LPDIRECTINPUTDEVICE8	pMouseDevice	= NULL; //マウスデバイス -> 外部マウス入力

// キーの状態を種類別に格納する変数
int Key[KEYMAX];

// キーの種類別に押されているかのフラグ
bool PreKey[KEYMAX] = {false};

//==================================================================
//
//	ダイレクトインプットの初期化
//
//=================================================================
//ダイレクトインプットの初期化関数
HRESULT InitDinput(HWND hWnd)
{
	HRESULT hr;
	
	//｢DirectInput｣オブジェクトの作成
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
//	キーの設定
//	
//-----------------------------------------------------------------
HRESULT	InitKey ( HWND hWnd )
{
	HRESULT hr;

	//｢DirectInputデバイス｣オブジェクトの作成(キーボード)
	if(FAILED (hr = pDinput->CreateDevice (
	GUID_SysKeyboard,
	&pKeyDevice,
	NULL

	) ) )
	{
		return hr;
	}
	//デバイスをキーボードに設定
	if(FAILED (hr = pKeyDevice->SetDataFormat (&c_dfDIKeyboard) ) )
	{
		return hr;
	}
	//協調レベルの設定		*当該アプリケーション(ゲーム)と、windowsOS自体のキーボードデバイスの占有率設定
	if(FAILED (hr = pKeyDevice->SetCooperativeLevel(
		hWnd,
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND		// * 非排他でバック・グラウンド
		) ) )
	{
		return hr;
	}
	//デバイスを｢取得｣する
	pKeyDevice->Acquire();							// * 作成したデバイスへのアクセス権取得
	return S_OK;
}

//==================================================================
//	キー状態チェック
// 第1引数( キーの定数 )、第2引数( キーの種類 )
//=================================================================
//	第1引数( キーの定数 )、第2引数( キーの種類 )
void KeyCheck ( int DIK, int st )
{
	HRESULT hr = pKeyDevice->Acquire();		//デバイスへのアクセス権の再取得
	
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

//	キー別状態判定
//	第1引数( キーの種類 )、第2引数( キーの状態 )
bool StCheck(int st, int Keyst) 
{
	return ( Key[st] == Keyst );
}

//	全てのキーの状態チェック
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

