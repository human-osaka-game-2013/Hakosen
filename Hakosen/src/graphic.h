//	作成者	亀田

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

// テクスチャの番号とテクスチャ全体の縦横の大きさ
struct TEXTUREINFO
{
	// テクスチャの格納ポインタ
	LPDIRECT3DTEXTURE9	pTexture;

	int		width, height;
};

// 画像の頂点座標などの構造体
struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;

	DWORD	color;

	FLOAT	tu, tv;
};

//	オブジェクトの情報
struct OBJECT_INFO
{
	float	fXpos, fYpos;

	float	fWidth, fHeight;

	float	fAngle;

	int		iAlpha, iRed, iGreen, iBlue; 
};

// 使用するテクスチャ
// 使いたいテクスチャがあれば列挙子内に追加していく
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
	PL_MARKER,	//	プレイヤーのマーカー
	SELECT_OK,
	SELECT_FACE,
	
	BLOCK_SET,

	CURSOR,			//枠画像

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

	GIRLS_WORDS,	//	女性キャラの台詞
	BOYS_WORDS,		//	男性キャラの台詞
	WORDS_BALLOON1,	//	台詞吹き出し
	WORDS_BALLOON2,	//	台詞吹き出し

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
// Direct3Dの初期化
//-----------------------------------------------------------------------
// 第1引数：ウインドハンドル
bool InitDraw ( HWND hWnd );

//-----------------------------------------------------------------------
// テクスチャの読み込み
//-----------------------------------------------------------------------
// 第1引数：("画像の名前.拡張子"), 第2引数：テクスチャの番号, 透過色RGB 
bool LoadTexture ( LPCSTR lpFileName, int TextuerNumber, int iColorRed, int iColorGreen, int iColorBlue );

//----------------------------------------------------------------------
// 頂点の操作
//---------------------------------------------------------------------
//	頂点の初期座標を決定する
// 第1引数：テクスチャの番号, 第2引数：テクスチャの構造体, 第3,4引数：左上の頂点ｘｙ
void SetVertex ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fX, float fY );
void SetVertex ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fXpos, float fYpos , float fWidth , float fHeight );
//	ポリゴンサイズ変更
//	引数>1：テクスチャの番号 2：頂点の先頭ポインタ 3,4：描画の基準となる左上の点xy, 5,6：頂点の縦横幅 
//	7,8：左上uv座標 9,10：アニメーション画像の1コマの縦横幅
void CustomSize ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fX, float fY, float fWidth, float fHeight, 
					float fTu, float fTv, float fAnimWidth, float fAnimHeight );

//	回転用関数
//	引数＞1：頂点配列の先頭ポインタ 2,3：回転させる基準点X,Y　4：回転させる角度, 5,6：回転させたい頂点の縦横幅
void Rotate ( CUSTOMVERTEX* pVertex, float fXpos, float fYpos, float fAngle, float fWidth, float fHeight );

//	アニメーション用関数
//	引数＞1：テクスチャの番号 2,3：アニメーションさせたい頂点の縦横幅 4,5：アニメーションさせたい画像の左上のUV座標
void Animation ( int , CUSTOMVERTEX* , float , float , float , float  );

//	頂点色操作用関数
//	引数＞1：頂点配列の先頭ポインタ 2～5：ARGB値
//	注意！＞ARGB値に255以上をいれないで下さい！
void ColorKey ( CUSTOMVERTEX* pVertex, int iA, int iR, int iG, int iB );


//	画像の伸縮
//	引数＞1：頂点配列の先頭ポインタ 2,3：伸縮させる基準点X,Y　4：画像の傾き, 5,6：伸縮させる頂点の縦横幅
void Elasticity ( CUSTOMVERTEX* pVertex, float fXpos, float fYpos, float& fAngle, float fWidth, float fHeight, int iBasePattern );

//----------------------------------------------------------------------
//	描画系
//------------------------------------------------------------------------
// 描画前処理
void BeginDraw (void);

// 描画処理
// 引数＞1：テクスチャの番号 2,3：：左上の頂点ｘｙ
void Draw ( int TextuerNumber, float fX, float fY );
//	頂点情報を引数として渡して描画
// 引数＞1：テクスチャの番号
void Draw ( int TextuerNumber, CUSTOMVERTEX* );

//	大きさを変えて描画
//	引数>1：テクスチャの番号 2,3：描画の基準となる左上の点xy, 4,5：右下の点xy 
//	6,7：左上uv座標 8,9：右下のuv座標
void CustomDraw ( int TextuerNumber, float fX, float fY, float X2,float Y2,
				float fTu, float fTv, float fTu2, float fTv2 );

//	大きさと色調を変えて(左上基準で)描画
//	引数>1：テクスチャの番号 2,3：描画の基準となる左上の点xy, 4,5：右下の点xy 
//	6,7：左上uv座標 8,9：アニメーション画像の1コマの縦横幅
//	引数＞11～14：：ARGB値
void CustomCorolDraw ( int TextuerNumber, float fX, float fY, float X2,float Y2,
				float fTu, float fTv, float fTu2, float fTv2, int iA, int iR, int iG, int iB );

//	中心点から描画 (回転)
//	引数＞1：テクスチャの番号 2,3：基準となる中心点 4：傾けたい角度(傾けないなら0でOK) 5,6：画像の縦横幅
//	引数＞7,8：画像の左上のUV座標 9,10:画像の右下のUV座標
void CenterPointDraw ( int TextuerNumber, float fXpos, float fYpos, float fAngle, float fWidth, float fHeight,
						float fTu1, float fTv1, float fTu2, float fTv2 );


//	中心点から描画 (色調変化)
//	引数＞1：テクスチャの番号 2,3：基準となる中心点 4：傾けたい角度(傾けないなら0でOK) 5,6：伸縮させる頂点の縦横幅
//	引数＞7,8：画像の左上のUV座標 9,10:画像の右下のUV座標 11～14：ARGB値
void ColorKeyDraw ( int TextuerNumber, float fXpos, float fYpos, float fAngle, float fWidth, float fHeight,
						float fTu1, float fTv1, float fTu2, float fTv2, int iA, int iR, int iG, int iB );

//	中心点から描画 (伸縮)
//	引数＞1：テクスチャの番号 2,3：伸縮させる基準点X,Y 4：角度 5,6：画像の縦横幅
//	引数＞7,8：画像の左上のUV座標 9,10:画像の右下のUV座標 11～14：ARGB値
void ElasticityDraw ( int TextuerNumber, float fXpos, float fYpos, float& fAngle, float fWidth, float fHeight,
						float fTu1, float fTv1, float fTu2, float fTv2, int& iA, int& iR, int& iG, int& iB );

// 描画後処理
void EndDraw (void);


//----------------------------------------------------------------
// 解放
//-----------------------------------------------------------------
// 第1引数：テクスチャの番号
void ReleaseTexture ( int TextuerNumber );

// 使用している全てのテクスチャの解放
void ReleaseAllTexture (void);

// Direct3Dオブジェクトの解放
void ReleaseRender (void);


void ScreenModeChange(HWND&);
BOOL GetWindowMode();
