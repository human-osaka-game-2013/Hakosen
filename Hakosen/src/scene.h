
#include <windows.h>
//	シーンの種類
enum 
{
	NONE_SCENE,

	LOGO_SCENE,

	TITLE_SCENE,

	SELECT_SCENE,

	GAME_SCENE,

	RESULT_SCENE,
	
	SCENE_MAX
};

// シーン初期化
void SceneInit ();

// シーン別処理
void SceneControl ( HWND hWnd );

// シーン別描画
void SceneDraw (void);

//	シーンの切り替え
void ChangeScene ( int iScene );