
#include <windows.h>
//	�V�[���̎��
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

// �V�[��������
void SceneInit ();

// �V�[���ʏ���
void SceneControl ( HWND hWnd );

// �V�[���ʕ`��
void SceneDraw (void);

//	�V�[���̐؂�ւ�
void ChangeScene ( int iScene );