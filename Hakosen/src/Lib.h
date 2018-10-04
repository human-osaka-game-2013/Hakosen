#pragma once

#define DIRECTINPUT_VERSION 0x0800

//�v���v���֘A�w�b�_�[�t�@�C��
#include <Windows.h>	//window��b�t�@�C��
#include <dinput.h>		//�L�[����
#include <dinputd.h>	//�L�[����
#include <XAudio2.h>	//mp3,wma�Đ�
#include <d3d9.h>
#include <d3dx9xof.h>

//���C�u�����Ǎ�
#pragma comment	(lib, "d3dx9d.lib")
#pragma comment	(lib, "dinput8.lib")	
#pragma comment	(lib, "dsound.lib")	//wav�Đ�
#pragma comment	(lib, "dxguid.lib")
#pragma comment	(lib, "winmm.lib")

#define SAFE_RELEASE(p) { if(p) { (p) -> Release(); (p) = NULL; } }
#define NULL_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#define NULL_ARRAY_DELETE(p) { if(p) { delete[] (p); (p) = NULL; } }

#define M_PI 3.141592f

//���W�A�����߂�E�p�x���߂�
#define AngleToRad(p) (p) / 180.f * M_PI
#define RadToAngle(p) (p) / M_PI * 180.f