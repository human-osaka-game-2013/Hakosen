//	�쐬��	�T�c

#pragma once

#include <d3dx9.h>

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define SAFE_RELEASE(p) { if(p) { (p) -> Release(); (p) = NULL; } }
#define WIDTH	1280
#define HEIGHT	720

#define TEMP_RGB	255, 0, 0

// �e�N�X�`���̔ԍ��ƃe�N�X�`���S�̂̏c���̑傫��
struct TEXTUREINFO
{
	// �e�N�X�`���̊i�[�|�C���^
	LPDIRECT3DTEXTURE9	pTexture;

	int		width, height;
};

// �摜�̒��_���W�Ȃǂ̍\����
struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;

	DWORD	color;

	FLOAT	tu, tv;
};

//	�I�u�W�F�N�g�̏��
struct OBJECT_INFO
{
	float	fXpos, fYpos;

	float	fWidth, fHeight;

	float	fAngle;

	int		iAlpha, iRed, iGreen, iBlue; 
};

// �g�p����e�N�X�`��
// �g�������e�N�X�`��������Η񋓎q���ɒǉ����Ă���
enum
{
	LOGO_TEXTURE,

	TITLE_BG_TEXTURE,
	TITLE_TITLE,
	TITLE_SUB_TITLE,
	TITLE_BUTTON,
	TITLE_EMBLEM,

	GAME_TEXTURE,

	SELECT_BG_TEXTURE,
	SELECT_CHARA,
	SELECT_TITLE,
	SELECT_PLAYER_CURSOR,
	PL_MARKER,	//	�v���C���[�̃}�[�J�[
	SELECT_OK,
	SELECT_FACE,
	
	BLOCK_SET,

	CURSOR,			//�g�摜

	BLACK,
	
	PUZZLE_BACK,
	COUNT_DOWN,
	COUNT_START,
	ELE_BOX,
	ELE_BOX_MOJI,
	ELE_BOX_MOJI2,

	CHARA_BOY,
	CHARA_GIRL,
	CHARA_ROBO,

	LIFE_MAX,
	LIFE_MIN,

	THUNDER,
	ATTACK_THUNDER,
	MARU,

	GIRLS_WORDS,	//	�����L�����̑䎌
	BOYS_WORDS,		//	�j���L�����̑䎌
	WORDS_BALLOON1,	//	�䎌�����o��
	WORDS_BALLOON2,	//	�䎌�����o��

	RESULT_WIN,
	RESULT_LOSE,
	RESULT_SELECT,

	TEXTURE_MAX
};

enum ELASTICITY_BASE_POINT
{
	E_B_TOP,

	E_B_BOTTOM,

	E_B_RIGHT,

	E_B_LEFT
};

//-----------------------------------------------------------------------
// Direct3D�̏�����
//-----------------------------------------------------------------------
// ��1�����F�E�C���h�n���h��
bool InitDraw ( HWND hWnd );

//-----------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ���
//-----------------------------------------------------------------------
// ��1�����F("�摜�̖��O.�g���q"), ��2�����F�e�N�X�`���̔ԍ�, ���ߐFRGB 
bool LoadTexture ( LPCSTR lpFileName, int TextuerNumber, int iColorRed, int iColorGreen, int iColorBlue );

//----------------------------------------------------------------------
// ���_�̑���
//---------------------------------------------------------------------
//	���_�̏������W�����肷��
// ��1�����F�e�N�X�`���̔ԍ�, ��2�����F�e�N�X�`���̍\����, ��3,4�����F����̒��_����
void SetVertex ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fX, float fY );
void SetVertex ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fXpos, float fYpos , float fWidth , float fHeight );
//	�|���S���T�C�Y�ύX
//	����>1�F�e�N�X�`���̔ԍ� 2�F���_�̐擪�|�C���^ 3,4�F�`��̊�ƂȂ鍶��̓_xy, 5,6�F���_�̏c���� 
//	7,8�F����uv���W 9,10�F�A�j���[�V�����摜��1�R�}�̏c����
void CustomSize ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fX, float fY, float fWidth, float fHeight, 
					float fTu, float fTv, float fAnimWidth, float fAnimHeight );

//	��]�p�֐�
//	������1�F���_�z��̐擪�|�C���^ 2,3�F��]�������_X,Y�@4�F��]������p�x, 5,6�F��]�����������_�̏c����
void Rotate ( CUSTOMVERTEX* pVertex, float fXpos, float fYpos, float fAngle, float fWidth, float fHeight );

//	�A�j���[�V�����p�֐�
//	������1�F�e�N�X�`���̔ԍ� 2,3�F�A�j���[�V���������������_�̏c���� 4,5�F�A�j���[�V�������������摜�̍����UV���W
void Animation ( int , CUSTOMVERTEX* , float , float , float , float  );

//	���_�F����p�֐�
//	������1�F���_�z��̐擪�|�C���^ 2�`5�FARGB�l
//	���ӁI��ARGB�l��255�ȏ������Ȃ��ŉ������I
void ColorKey ( CUSTOMVERTEX* pVertex, int iA, int iR, int iG, int iB );


//	�摜�̐L�k
//	������1�F���_�z��̐擪�|�C���^ 2,3�F�L�k�������_X,Y�@4�F�摜�̌X��, 5,6�F�L�k�����钸�_�̏c����
void Elasticity ( CUSTOMVERTEX* pVertex, float fXpos, float fYpos, float& fAngle, float fWidth, float fHeight, int iBasePattern );

//----------------------------------------------------------------------
//	�`��n
//------------------------------------------------------------------------
// �`��O����
void BeginDraw (void);

// �`�揈��
// ������1�F�e�N�X�`���̔ԍ� 2,3�F�F����̒��_����
void Draw ( int TextuerNumber, float fX, float fY );
//	���_���������Ƃ��ēn���ĕ`��
// ������1�F�e�N�X�`���̔ԍ�
void Draw ( int TextuerNumber, CUSTOMVERTEX* );

//	�傫����ς��ĕ`��
//	����>1�F�e�N�X�`���̔ԍ� 2,3�F�`��̊�ƂȂ鍶��̓_xy, 4,5�F�E���̓_xy 
//	6,7�F����uv���W 8,9�F�E����uv���W
void CustomDraw ( int TextuerNumber, float fX, float fY, float X2,float Y2,
				float fTu, float fTv, float fTu2, float fTv2 );

//	�傫���ƐF����ς���(������)�`��
//	����>1�F�e�N�X�`���̔ԍ� 2,3�F�`��̊�ƂȂ鍶��̓_xy, 4,5�F�E���̓_xy 
//	6,7�F����uv���W 8,9�F�A�j���[�V�����摜��1�R�}�̏c����
//	������11�`14�F�FARGB�l
void CustomCorolDraw ( int TextuerNumber, float fX, float fY, float X2,float Y2,
				float fTu, float fTv, float fTu2, float fTv2, int iA, int iR, int iG, int iB );

//	���S�_����`�� (��])
//	������1�F�e�N�X�`���̔ԍ� 2,3�F��ƂȂ钆�S�_ 4�F�X�������p�x(�X���Ȃ��Ȃ�0��OK) 5,6�F�摜�̏c����
//	������7,8�F�摜�̍����UV���W 9,10:�摜�̉E����UV���W
void CenterPointDraw ( int TextuerNumber, float fXpos, float fYpos, float fAngle, float fWidth, float fHeight,
						float fTu1, float fTv1, float fTu2, float fTv2 );


//	���S�_����`�� (�F���ω�)
//	������1�F�e�N�X�`���̔ԍ� 2,3�F��ƂȂ钆�S�_ 4�F�X�������p�x(�X���Ȃ��Ȃ�0��OK) 5,6�F�L�k�����钸�_�̏c����
//	������7,8�F�摜�̍����UV���W 9,10:�摜�̉E����UV���W 11�`14�FARGB�l
void ColorKeyDraw ( int TextuerNumber, float fXpos, float fYpos, float fAngle, float fWidth, float fHeight,
						float fTu1, float fTv1, float fTu2, float fTv2, int iA, int iR, int iG, int iB );

//	���S�_����`�� (�L�k)
//	������1�F�e�N�X�`���̔ԍ� 2,3�F�L�k�������_X,Y 4�F�p�x 5,6�F�摜�̏c����
//	������7,8�F�摜�̍����UV���W 9,10:�摜�̉E����UV���W 11�`14�FARGB�l
void ElasticityDraw ( int TextuerNumber, float fXpos, float fYpos, float& fAngle, float fWidth, float fHeight,
						float fTu1, float fTv1, float fTu2, float fTv2, int& iA, int& iR, int& iG, int& iB );

// �`��㏈��
void EndDraw (void);


//----------------------------------------------------------------
// ���
//-----------------------------------------------------------------
// ��1�����F�e�N�X�`���̔ԍ�
void ReleaseTexture ( int TextuerNumber );

// �g�p���Ă���S�Ẵe�N�X�`���̉��
void ReleaseAllTexture (void);

// Direct3D�I�u�W�F�N�g�̉��
void ReleaseRender (void);


void ScreenModeChange(HWND&);
BOOL GetWindowMode();
