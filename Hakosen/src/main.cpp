#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <time.h>
#include <crtdbg.h>

#include "main.h"
#include "graphic.h"
#include "scene.h"
#include "input.h"
#include "Audio.h"
#include "read_file.h"

HWND hWnd;

//==================================================================================
//
//	メッセージ処理
//
//=================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if(wp==VK_ESCAPE){
			PostQuitMessage (0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage (0);
		break;

	}
	return DefWindowProc (hWnd, msg, wp, lp);
}

//----------------------------------------------------------------------
//
//	メインルーチン
//
//----------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR IpCmdLine, int nCmdShow )
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(87);

	MSG msg;
	
	WNDCLASS winc;

	// Windows情報の設定
	winc.style			= CS_HREDRAW | CS_VREDRAW; //ウインドのサイズが変わった時再描画する処理
	winc.lpfnWndProc	= WndProc;	// 送られてきたメッセージを元に様々な処理を行う関数
	winc.cbClsExtra		= winc.cbWndExtra = 0;
	winc.hInstance		= hInstance;
	winc.hIcon			= LoadIcon( NULL, IDI_APPLICATION);
	winc.hCursor		= LoadCursor( NULL, IDC_ARROW);
	winc.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= CLASS_NAME;

	// Windowsの登録
	if ( !RegisterClass (&winc))	return 0;

	// Windowsの生成
	int dH = GetSystemMetrics (SM_CYCAPTION) + GetSystemMetrics (SM_CYFRAME) * 2;
	int dW = GetSystemMetrics (SM_CXFRAME) * 2;

	hWnd = CreateWindow(
		CLASS_NAME,
		TITLE,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,		// x
		CW_USEDEFAULT,		// y
		WIDTH + dW,			// Width
		HEIGHT + dH,			// Height
		NULL, NULL, hInstance, NULL
	);

	if ( !hWnd ) return 0;

	// Direct3D周りの初期化処理
	if ( !InitDraw(hWnd) ) return 0;
	
	//	キーの初期化
	InitDinput ( hWnd );

	// サウンドの初期化
	InitXAudio2();

	//	ECBファイル読み込み
	ReadECBFile ();

	srand(( unsigned )time( NULL ));

	DWORD SyncOld = timeGetTime();
	DWORD SyncNow;

	LoadTexture( "image\\blk.png" , BLACK , TEMP_RGB );
	

	timeBeginPeriod(1);

	ZeroMemory (&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		Sleep(1);

		if ( PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}

		else
		{
			SyncNow = timeGetTime();

			if (SyncNow - SyncOld >= 1000 / 60)
			{
				// 描画前処理
				BeginDraw();

				// 初期化
				SceneInit ();

				// 描画
				SceneDraw ();

				// 処理
				SceneControl(hWnd);

				// 描画後処理
				EndDraw();

				SyncOld = SyncNow;

			}
		}
	}

	timeEndPeriod(1);

	ReleaseAllTexture();

	ReleaseXAudio2();
	FreeJoyStick();

	ReleaseRender();


	return (int)msg.wParam;
}
