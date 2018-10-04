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
//	���b�Z�[�W����
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
//	���C�����[�`��
//
//----------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR IpCmdLine, int nCmdShow )
{
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(87);

	MSG msg;
	
	WNDCLASS winc;

	// Windows���̐ݒ�
	winc.style			= CS_HREDRAW | CS_VREDRAW; //�E�C���h�̃T�C�Y���ς�������ĕ`�悷�鏈��
	winc.lpfnWndProc	= WndProc;	// �����Ă������b�Z�[�W�����ɗl�X�ȏ������s���֐�
	winc.cbClsExtra		= winc.cbWndExtra = 0;
	winc.hInstance		= hInstance;
	winc.hIcon			= LoadIcon( NULL, IDI_APPLICATION);
	winc.hCursor		= LoadCursor( NULL, IDC_ARROW);
	winc.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= CLASS_NAME;

	// Windows�̓o�^
	if ( !RegisterClass (&winc))	return 0;

	// Windows�̐���
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

	// Direct3D����̏���������
	if ( !InitDraw(hWnd) ) return 0;
	
	//	�L�[�̏�����
	InitDinput ( hWnd );

	// �T�E���h�̏�����
	InitXAudio2();

	//	ECB�t�@�C���ǂݍ���
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
				// �`��O����
				BeginDraw();

				// ������
				SceneInit ();

				// �`��
				SceneDraw ();

				// ����
				SceneControl(hWnd);

				// �`��㏈��
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
