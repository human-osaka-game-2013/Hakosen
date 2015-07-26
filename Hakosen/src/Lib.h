#pragma once

#define DIRECTINPUT_VERSION 0x0800

//プリプロ関連ヘッダーファイル
#include <Windows.h>	//window基礎ファイル
#include <dinput.h>		//キー入力
#include <dinputd.h>	//キー入力
#include <XAudio2.h>	//mp3,wma再生
#include <d3d9.h>
#include <d3dx9xof.h>

//ライブラリ読込
#pragma comment	(lib, "d3dx9d.lib")
#pragma comment	(lib, "dinput8.lib")	
#pragma comment	(lib, "dsound.lib")	//wav再生
#pragma comment	(lib, "dxguid.lib")
#pragma comment	(lib, "winmm.lib")

#define SAFE_RELEASE(p) { if(p) { (p) -> Release(); (p) = NULL; } }
#define NULL_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#define NULL_ARRAY_DELETE(p) { if(p) { delete[] (p); (p) = NULL; } }

#define M_PI 3.141592f

//ラジアン求める・角度求める
#define AngleToRad(p) (p) / 180.f * M_PI
#define RadToAngle(p) (p) / M_PI * 180.f