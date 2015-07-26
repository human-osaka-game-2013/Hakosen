//	作成者	亀田

#include "graphic.h"

IDirect3D9*			pDirect3D;
IDirect3DDevice9*	pD3Device;

D3DPRESENT_PARAMETERS	d3dpp;

TEXTUREINFO TextureInfo [TEXTURE_MAX];



//=================================================================================
//
//	グラフィックの初期化
//
//==================================================================================

// Direct3Dの初期化
// 第1引数：ウインドハンドル
bool InitDraw ( HWND hWnd )
{
	D3DDISPLAYMODE d3ddm;
	
	// DirectX オブジェクトの生成
	pDirect3D = Direct3DCreate9 ( D3D_SDK_VERSION );

	// Display Mode の設定
	pDirect3D -> GetAdapterDisplayMode ( D3DADAPTER_DEFAULT, &d3ddm );

	ZeroMemory(	&d3dpp, sizeof ( D3DPRESENT_PARAMETERS ));

	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount  = 1;
	d3dpp.SwapEffect	   = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed		   = TRUE;

	pDirect3D -> CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &pD3Device);

	return true;
}

// テクスチャの読み込み
// 第1引数：("画像の名前.拡張子"), 第2引数：テクスチャの番号, 透過色RGB 
bool LoadTexture ( LPCSTR lpFileName, int TextuerNumber, int iColorRed, int iColorGreen, int iColorBlue )
{
	// テクスチャを既に読み込んでいる場合はここで解放する
	if ( FAILED (D3DXCreateTextureFromFileExA (
					pD3Device,
					lpFileName,
					D3DX_DEFAULT_NONPOW2,
					D3DX_DEFAULT_NONPOW2,
					D3DX_DEFAULT,
					0,
					D3DFMT_UNKNOWN,
					D3DPOOL_MANAGED,
					D3DX_FILTER_NONE,
					D3DX_FILTER_NONE,
					D3DCOLOR_ARGB ( 0, iColorRed, iColorGreen, iColorBlue ),
					NULL, NULL,
					&TextureInfo [TextuerNumber].pTexture))) {
						MessageBoxA(0,"テクスチャが読み込めませんでした(＞＜;)",NULL,MB_OK);
						return false;
	}

	// テクスチャ情報取得
	D3DSURFACE_DESC desc;

	TextureInfo [TextuerNumber].pTexture -> GetLevelDesc ( 0, &desc );
	TextureInfo [TextuerNumber].width = desc.Width;
	TextureInfo [TextuerNumber].height = desc.Height;

	return true;
}
//=========================================================================================
//
// すでにCUSTOMVERTEX型の頂点配列を作っている場合の頂点の操作
//
//========================================================================================

//	頂点の初期座標を決定する
// 第1引数：テクスチャの番号, 第2引数：テクスチャの構造体, 第3,4引数：左上の頂点ｘｙ
void SetVertex ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fX, float fY )
{
	for ( int ii = 0; ii < 4; ii++, pVertex++ )
	{
		pVertex -> z = 0.5f;
		pVertex -> rhw = 1.0f;
		pVertex -> color = 0xFFFFFFFF;

		switch ( ii )
		{
		case 0:
			pVertex -> x = fX;
			pVertex -> y = fY;
			pVertex -> tu = 0;
			pVertex -> tv = 0;
			break;

		case 1:
			pVertex -> x = fX + TextureInfo[TextuerNumber].width;
			pVertex -> y = fY;
			pVertex -> tu = 1.0f;
			pVertex -> tv = 0;	
			break;

		case 2:
			pVertex -> x = fX + TextureInfo[TextuerNumber].width;
			pVertex -> y = fY + TextureInfo[TextuerNumber].height;
			pVertex -> tu = 1.0f;
			pVertex -> tv = 1.0f;	
			break;

		case 3:
			pVertex -> x = fX;
			pVertex -> y = fY + TextureInfo[TextuerNumber].height;
			pVertex -> tu = 0.0f;
			pVertex -> tv = 1.0f;
			break;
		}
		
	}
}
void SetVertex ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fXpos, float fYpos , float fWidth , float fHeight )
{
	for ( int ii = 0; ii < 4; ii++, pVertex++ )
	{
		pVertex -> z = 0.5f;
		pVertex -> rhw = 1.0f;
		pVertex -> color = 0xFFFFFFFF;

		switch ( ii )
		{
		case 0:
			pVertex -> x = fXpos - fWidth / 2.f;
			pVertex -> y = fYpos - fHeight / 2.f;
			pVertex -> tu = 0;
			pVertex -> tv = 0;
			break;

		case 1:
			pVertex -> x = fXpos + fWidth / 2.f;
			pVertex -> y = fYpos - fHeight / 2.f;
			pVertex -> tu = 1.0f;
			pVertex -> tv = 0;	
			break;

		case 2:
			pVertex -> x = fXpos + fWidth / 2.f;
			pVertex -> y = fYpos + fHeight / 2.f;
			pVertex -> tu = 1.0f;
			pVertex -> tv = 1.0f;	
			break;

		case 3:
			pVertex -> x = fXpos - fWidth / 2.f;
			pVertex -> y = fYpos + fHeight / 2.f;
			pVertex -> tu = 0.0f;
			pVertex -> tv = 1.0f;
			break;
		}
		
	}
}
//	ポリゴンサイズ変更
//	引数>1：テクスチャの番号 2：頂点の先頭ポインタ 3,4：描画の基準となる左上の点xy, 5,6：頂点の縦横幅 
//	7,8：左上uv座標 9,10：アニメーション画像の1コマの縦横幅
void CustomSize ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fX, float fY, float fWidth, float fHeight, 
					float fTu, float fTv, float fAnimWidth, float fAnimHeight )
{
	for ( int ii = 0; ii < 4; ii++, pVertex++ )
	{
		pVertex -> z = 0.5f;
		pVertex -> rhw = 1.0f;
		pVertex -> color = 0xFFFFFFFF;

		switch ( ii )
		{
		case 0:
			pVertex -> x = fX;
			pVertex -> y = fY;
			pVertex -> tu = (fTu + 0.5f) / TextureInfo[TextuerNumber].width;
			pVertex -> tv = (fTv + 0.5f) / TextureInfo[TextuerNumber].height;
			break;

		case 1:
			pVertex -> x = fX + fWidth;
			pVertex -> y = fY;
			pVertex -> tu = (fTu + fAnimWidth + 0.5f) / TextureInfo[TextuerNumber].width;
			pVertex -> tv = (fTv + 0.5f) / TextureInfo[TextuerNumber].height;
			break;

		case 2:
			pVertex -> x = fX + fWidth;
			pVertex -> y = fY + fHeight;
			pVertex -> tu = (fTu + fAnimWidth + 0.5f) / TextureInfo[TextuerNumber].width;
			pVertex -> tv = (fTv + fAnimHeight + 0.5f) / TextureInfo[TextuerNumber].height;	
			break;

		case 3:
			pVertex -> x = fX;
			pVertex -> y = fY + fHeight;
			pVertex -> tu = (fTu + 0.5f) / TextureInfo[TextuerNumber].width;
			pVertex -> tv = (fTv + fAnimHeight + 0.5f) / TextureInfo[TextuerNumber].height;
			break;
		}
		
	}
}

//	回転用関数
//	引数＞1：頂点配列の先頭ポインタ 2,3：回転させる基準点X,Y　4：回転させる角度, 5,6：回転させたい頂点の縦横幅
void Rotate ( CUSTOMVERTEX* pVertex, float fXpos, float fYpos, float fAngle, float fWidth, float fHeight )
{
	//if ( fAngle > 360 )	{ fAngle = 0; }
	//if ( fAngle < 0 )	{ fAngle = 360; }

	float fTempX = fXpos, fTempY = fYpos;

	float fRad = fAngle * D3DX_PI / 180.0f;

	for ( int ii = 0; ii < 4; ii++, pVertex++ )
	{
		switch ( ii )
		{
		case 0:
			pVertex -> x = fXpos + ( -fWidth/2 ) * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad );
			pVertex -> y = fYpos + ( -fWidth/2 ) * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad );
			break;

		case 1:
			pVertex -> x = fXpos + fWidth/2 * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad );
			pVertex -> y = fYpos + fWidth/2 * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad );
			break;
		
		case 2:
			pVertex -> x = fXpos + fWidth/2 * cosf ( fRad ) - fHeight/2 * sinf ( fRad );
			pVertex -> y = fYpos + fWidth/2 * sinf ( fRad ) + fHeight/2 * cosf ( fRad );
			break;

		case 3:
			pVertex -> x = fXpos + ( -fWidth/2 ) * cosf ( fRad ) - fHeight/2 * sinf ( fRad );
			pVertex -> y = fYpos + ( -fWidth/2 ) * sinf ( fRad ) + fHeight/2 * cosf ( fRad );
			break;
		}
	}
}

//	アニメーション用関数
//	引数＞1：テクスチャの番号 2,3：アニメーションさせたい頂点の縦横幅 4,5：アニメーションさせたい画像の左上のUV座標
void Animation ( int TextuerNumber, CUSTOMVERTEX* pVertex, float fWidth, float fHeight, float fTu, float fTv )
{
	for ( int ii = 0; ii < 4; ii++, pVertex++ )
	{
		switch ( ii )
		{
		case 0:
			pVertex -> tu = (fTu + 0.5f) / TextureInfo[TextuerNumber].width;
			pVertex -> tv = (fTv + 0.5f) / TextureInfo[TextuerNumber].height;
			break;

		case 1:
			pVertex -> tu = (fTu + fWidth + 0.5f) / TextureInfo[TextuerNumber].width;
			pVertex -> tv = (fTv + 0.5f) / TextureInfo[TextuerNumber].height;
			break;

		case 2:
			pVertex -> tu = (fTu + fWidth + 0.5f) / TextureInfo[TextuerNumber].width;
			pVertex -> tv = (fTv + fHeight + 0.5f) / TextureInfo[TextuerNumber].height;
			break;

		case 3:
			pVertex -> tu = (fTu + 0.5f) / TextureInfo[TextuerNumber].width;
			pVertex -> tv = (fTv + fHeight + 0.5f) / TextureInfo[TextuerNumber].height;
			break;
		}
	}
}

//	頂点色操作用関数
//	引数＞1：頂点配列の先頭ポインタ 2～5：ARGB値
//	注意！＞ARGB値に255以上をいれないで下さい！
void ColorKey ( CUSTOMVERTEX* pVertex, int iA, int iR, int iG, int iB )
{
	if ( iA > 255 ) { iA = 0; }
	if ( iA < 0 )	{ iA = 255; }
	if ( iR > 255 ) { iR = 0; }
	if ( iR < 0 )	{ iR = 255; }
	if ( iG > 255 ) { iG = 0; }
	if ( iG < 0 )	{ iG = 255; }
	if ( iB > 255 ) { iB = 0; }
	if ( iB < 0 )	{ iB = 255; }
	
	for ( int ii = 0; ii < 4; ii++ )
	{
		pVertex[ii].color = D3DCOLOR_ARGB( iA, iR, iG, iB );
	}
}

//	画像の伸縮
//	引数＞1：頂点配列の先頭ポインタ 2,3：伸縮させる基準点X,Y　4：画像の傾き, 5,6：伸縮させる頂点の縦横幅
void Elasticity ( CUSTOMVERTEX* pVertex, float fXpos, float fYpos, float& fAngle, float fWidth, float fHeight, int iBasePattern )
{
	if ( fAngle > 360 )	{ fAngle = 0; }
	if ( fAngle < 0 )	{ fAngle = 360; }

	float fTempX = fXpos, fTempY = fYpos;

	float fRad = fAngle * D3DX_PI / 180.0f;

	for ( int ii = 0; ii < 4; ii++, pVertex++ )
	{
		switch ( ii )
		{
		case 0:
			if( iBasePattern != E_B_TOP && iBasePattern != E_B_LEFT )
			{
				pVertex -> x = fXpos + ( -fWidth/2 ) * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad );
				pVertex -> y = fYpos + ( -fWidth/2 ) * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad );
				break;
			}
		case 1:
			if( iBasePattern != E_B_TOP && iBasePattern != E_B_RIGHT )
			{
				pVertex -> x = fXpos + fWidth/2 * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad );
				pVertex -> y = fYpos + fWidth/2 * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad );
				break;
			}
		case 2:
			if( iBasePattern != E_B_RIGHT && iBasePattern != E_B_BOTTOM )
			{
				pVertex -> x = fXpos + fWidth/2 * cosf ( fRad ) - fHeight/2 * sinf ( fRad );
				pVertex -> y = fYpos + fWidth/2 * sinf ( fRad ) + fHeight/2 * cosf ( fRad );
				break;
			}
		case 3:
			if( iBasePattern != E_B_BOTTOM && iBasePattern != E_B_LEFT )
			{
				pVertex -> x = fXpos + ( -fWidth/2 ) * cosf ( fRad ) - fHeight/2 * sinf ( fRad );
				pVertex -> y = fYpos + ( -fWidth/2 ) * sinf ( fRad ) + fHeight/2 * cosf ( fRad );
				break;
			}
		}
	}
}


//==================================================================================
//
//	描画処理
//
//==================================================================================

// 描画前処理
void BeginDraw (void)
{
	if ( !pD3Device )
		return;

	// ステージステートの設定
	pD3Device -> SetRenderState ( D3DRS_ALPHABLENDENABLE,	true );
	
	pD3Device -> SetTextureStageState ( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE	);
	pD3Device -> SetTextureStageState ( 0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE	);

	pD3Device -> SetTextureStageState ( 0, D3DTSS_COLOROP,		D3DTOP_MODULATE	);

	pD3Device -> SetTextureStageState ( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE	);
	pD3Device -> SetTextureStageState ( 0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE	);

	pD3Device -> SetTextureStageState ( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE );

	// 描画方法の設定
	

	pD3Device ->  SetRenderState ( D3DRS_SRCBLEND,				D3DBLEND_SRCALPHA	);
	pD3Device ->  SetRenderState ( D3DRS_DESTBLEND,				D3DBLEND_INVSRCALPHA);
	pD3Device ->  SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1		);
	
	
	pD3Device -> SetRenderState ( D3DRS_ZENABLE,			false );
	
	// 画面の消去

	HRESULT hr = pD3Device->Clear(0, NULL,
				D3DCLEAR_TARGET,
				D3DCOLOR_XRGB ( 0 , 0 , 0 ),
				1.f, 0 );

	if( FAILED(hr)) 
		return;

	// 描画の開始
	pD3Device -> BeginScene();

	// 画面の描画
	pD3Device -> SetFVF ( D3DFVF_CUSTOMVERTEX );

}

void Draw ( int TextuerNumber, CUSTOMVERTEX* _ver )
{
	pD3Device -> SetTexture ( 0, TextureInfo[TextuerNumber].pTexture );
	pD3Device -> DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		_ver,
		sizeof ( CUSTOMVERTEX ) );
}

// 描画処理
// 第1引数：頂点配列の先頭ポインタ, 第2引数：テクスチャの番号
void Draw ( int TextuerNumber, float fX, float fY )
{
	CUSTOMVERTEX customVertex[4] ={0};

	SetVertex( TextuerNumber, customVertex, fX, fY );

	pD3Device -> SetTexture ( 0, TextureInfo[TextuerNumber].pTexture );
	pD3Device -> DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		customVertex,
		sizeof ( CUSTOMVERTEX ) );
}

//	大きさを変えて描画
//	引数>1：テクスチャの番号 2,3：描画の基準となる左上の点xy, 4,5：右下の点xy 
//	6,7：左上uv座標 8,9：右下のuv座標
void CustomDraw ( int TextuerNumber, float fX, float fY, float X2,float Y2,
				float fTu, float fTv, float fTu2, float fTv2 )
{
	
	CUSTOMVERTEX customVertex[4] ={
	 {fX,fY,0.5f,1.0f, 0xffffffff, fTu, fTv },
	 {X2,fY,0.5f,1.0f, 0xffffffff, fTu2, fTv },
	 {X2,Y2,0.5f,1.0f, 0xffffffff, fTu2, fTv2 },
	 {fX,Y2,0.5f,1.0f, 0xffffffff, fTu, fTv2 }
	};

	
	pD3Device -> SetTexture ( 0, TextureInfo[TextuerNumber].pTexture );
	pD3Device -> DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		customVertex,
		sizeof ( CUSTOMVERTEX ) );
}

//	大きさと色調を変えて(左上基準で)描画
//	引数>1：テクスチャの番号 2,3：描画の基準となる左上の点xy, 4,5：右下の点xy 
//	6,7：左上uv座標 8,9：アニメーション画像の1コマの縦横幅
//	引数＞11～14：：ARGB値
void CustomCorolDraw ( int TextuerNumber, float fX, float fY, float X2,float Y2,
				float fTu, float fTv, float fTu2, float fTv2, int iA, int iR, int iG, int iB )
{
		CUSTOMVERTEX customVertex[4] ={
	 {fX,fY,0.5f,1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu, fTv },
	 {X2,fY,0.5f,1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu2, fTv },
	 {X2,Y2,0.5f,1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu2, fTv2 },
	 {fX,Y2,0.5f,1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu, fTv2 }
	};

	
	pD3Device -> SetTexture ( 0, TextureInfo[TextuerNumber].pTexture );
	pD3Device -> DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		customVertex,
		sizeof ( CUSTOMVERTEX ) );
}

//	中心点から描画 (回転)
//	引数＞1：テクスチャの番号 2,3：基準となる中心点 4：傾けたい角度(傾けないなら0でOK) 5,6：画像の縦横幅
//	引数＞7,8：画像の左上のUV座標 9,10:画像の右下のUV座標
void CenterPointDraw ( int TextuerNumber, float fXpos, float fYpos, float fAngle, float fWidth, float fHeight,
						float fTu1, float fTv1, float fTu2, float fTv2 )
{
	//if ( fAngle > 360 )	{ fAngle = 0; }
	//if ( fAngle < 0 )	{ fAngle = 360; }

	float fRad = fAngle * D3DX_PI / 180.0f;

	CUSTOMVERTEX customVertex[4] = {
		{ fXpos + ( -fWidth/2 ) * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad ),
		  fYpos + ( -fWidth/2 ) * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad ),
		  0.5f, 1.0f, 0xffffffff, fTu1, fTv1 },
		
		{ fXpos + fWidth/2 * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad ),
		  fYpos + fWidth/2 * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad ),
		  0.5f, 1.0f, 0xffffffff, fTu2, fTv1 },
		
		{ fXpos + fWidth/2 * cosf ( fRad ) - fHeight/2 * sinf ( fRad ),
		  fYpos + fWidth/2 * sinf ( fRad ) + fHeight/2 * cosf ( fRad ),
		  0.5f, 1.0f, 0xffffffff, fTu2, fTv2 },
		
		{ fXpos + ( -fWidth/2 ) * cosf ( fRad ) - fHeight/2 * sinf ( fRad ),
		  fYpos + ( -fWidth/2 ) * sinf ( fRad ) + fHeight/2 * cosf ( fRad ),
		  0.5f, 1.0f, 0xffffffff, fTu1, fTv2 },
	};

	pD3Device -> SetTexture ( 0, TextureInfo[TextuerNumber].pTexture );
	pD3Device -> DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		customVertex,
		sizeof ( CUSTOMVERTEX ) );	
}

//	中心点から描画 (色調変化)
//	引数＞1：テクスチャの番号 2,3：基準となる中心点 4：傾けたい角度(傾けないなら0でOK) 5,6：伸縮させる頂点の縦横幅
//	引数＞7,8：画像の左上のUV座標 9,10:画像の右下のUV座標 11～14：ARGB値
void ColorKeyDraw ( int TextuerNumber, float fXpos, float fYpos, float fAngle, float fWidth, float fHeight,
						float fTu1, float fTv1, float fTu2, float fTv2, int iA, int iR, int iG, int iB )
{
	if ( fAngle > 360 )	{ fAngle = 0; }
	if ( fAngle < 0 )	{ fAngle = 360; }

	if ( iA > 255 ) { iA = 0; }
	if ( iA < 0 )	{ iA = 255; }
	if ( iR > 255 ) { iR = 0; }
	if ( iR < 0 )	{ iR = 255; }
	if ( iG > 255 ) { iG = 0; }
	if ( iG < 0 )	{ iG = 255; }
	if ( iB > 255 ) { iB = 0; }
	if ( iB < 0 )	{ iB = 255; }

	float fRad = fAngle * D3DX_PI / 180.0f;

	CUSTOMVERTEX customVertex[4] = {
		{ fXpos + ( -fWidth/2 ) * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad ),
		  fYpos + ( -fWidth/2 ) * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad ),
		  0.5f, 1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu1, fTv1 },
		
		{ fXpos + fWidth/2 * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad ),
		  fYpos + fWidth/2 * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad ),
		  0.5f, 1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu2, fTv1 },
		
		{ fXpos + fWidth/2 * cosf ( fRad ) - fHeight/2 * sinf ( fRad ),
		  fYpos + fWidth/2 * sinf ( fRad ) + fHeight/2 * cosf ( fRad ),
		  0.5f, 1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu2, fTv2 },
		
		{ fXpos + ( -fWidth/2 ) * cosf ( fRad ) - fHeight/2 * sinf ( fRad ),
		  fYpos + ( -fWidth/2 ) * sinf ( fRad ) + fHeight/2 * cosf ( fRad ),
		  0.5f, 1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu1, fTv2 },
	};

	pD3Device -> SetTexture ( 0, TextureInfo[TextuerNumber].pTexture );
	pD3Device -> DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		customVertex,
		sizeof ( CUSTOMVERTEX ) );	
}

//	中心点から描画 (伸縮)
//	引数＞1：テクスチャの番号 2,3：伸縮させる基準点X,Y 4：角度 5,6：画像の縦横幅
//	引数＞7,8：画像の左上のUV座標 9,10:画像の右下のUV座標 11～14：ARGB値
void ElasticityDraw ( int TextuerNumber, float fXpos, float fYpos, float& fAngle, float fWidth, float fHeight,
						float fTu1, float fTv1, float fTu2, float fTv2, int& iA, int& iR, int& iG, int& iB )
{
	if ( fAngle > 360 )	{ fAngle = 0; }
	if ( fAngle < 0 )	{ fAngle = 360; }

	if ( iA > 255 ) { iA = 0; }
	if ( iA < 0 )	{ iA = 255; }
	if ( iR > 255 ) { iR = 0; }
	if ( iR < 0 )	{ iR = 255; }
	if ( iG > 255 ) { iG = 0; }
	if ( iG < 0 )	{ iG = 255; }
	if ( iB > 255 ) { iB = 0; }
	if ( iB < 0 )	{ iB = 255; }

	float fRad = fAngle * D3DX_PI / 180.0f;

	CUSTOMVERTEX customVertex[4] = {
		{ fXpos + ( -fWidth/2 ) * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad ),
		  fYpos + ( -fWidth/2 ) * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad ),
		  0.5f, 1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu1, fTv1 },
		
		{ fXpos + fWidth/2 * cosf ( fRad ) - ( -fHeight/2 ) * sinf ( fRad ),
		  fYpos + fWidth/2 * sinf ( fRad ) + ( -fHeight/2 ) * cosf ( fRad ),
		  0.5f, 1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu2, fTv1 },
		
		{ fXpos + fWidth/2 * cosf ( fRad ) - fHeight/2 * sinf ( fRad ),
		  fYpos + fWidth/2 * sinf ( fRad ) + fHeight/2 * cosf ( fRad ),
		  0.5f, 1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu2, fTv2 },
		
		{ fXpos + ( -fWidth/2 ) * cosf ( fRad ) - fHeight/2 * sinf ( fRad ),
		  fYpos + ( -fWidth/2 ) * sinf ( fRad ) + fHeight/2 * cosf ( fRad ),
		  0.5f, 1.0f, D3DCOLOR_ARGB( iA, iR, iG, iB ), fTu1, fTv2 },
	};

	pD3Device -> SetTexture ( 0, TextureInfo[TextuerNumber].pTexture );
	pD3Device -> DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		customVertex,
		sizeof ( CUSTOMVERTEX ) );	
}

// 描画後処理
void EndDraw (void)
{
	// 描画の終了
	pD3Device -> EndScene();

	// 表示
	pD3Device -> Present ( NULL, NULL, NULL, NULL );
}

// テクスチャの解放
// 第1引数：テクスチャの番号
void ReleaseTexture ( int TextuerNumber )
{
	SAFE_RELEASE ( TextureInfo[TextuerNumber].pTexture );
}

// 使用している全てのテクスチャの解放
void ReleaseAllTexture (void)
{
	for ( int i = 0; i < TEXTURE_MAX; i++ )
	{
		ReleaseTexture (i);
	}
}

// Direct3Dオブジェクトの解放
void ReleaseRender (void)
{
	SAFE_RELEASE(pD3Device);
	SAFE_RELEASE(pDirect3D);
}

RECT DevDesc;
RECT WindowPos;

void ScreenModeChange(HWND& hWnd)
{
	HRESULT hr;
   
   //****************************************************************
   //ウィンドウスタイルの切り替え要求が発生したとき
   //****************************************************************
    //現在ウィンドウモードのとき
	if( d3dpp.Windowed == TRUE )
    {
        //ウィンドウモードのとき現在のウィンドウの表示位置を保存する。
        //WindowPosは、ウィンドウモードからフルスクリーンへ切り替え、再びウィンドウモードに切り替えたとき、ここで取得した座標でウィンドウを表示する。
        ::GetWindowRect(hWnd, &WindowPos);
        d3dpp.Windowed = FALSE;
		HWND hDisplayWnd;
		RECT DisplayRect;

		hDisplayWnd = GetDesktopWindow();
		GetWindowRect( hDisplayWnd, (LPRECT)&DisplayRect );

		DevDesc.right = DisplayRect.right;
		DevDesc.bottom = DisplayRect.bottom;
		
    }
    else
       d3dpp.Windowed = TRUE;


    //デバイスの復元を行う
    pD3Device->Reset( &d3dpp );

	

	DEVMODE devmode;
	ZeroMemory( &devmode, sizeof(devmode) );
	devmode.dmSize = sizeof(devmode);
    if( d3dpp.Windowed == TRUE )	
    {
		devmode.dmPelsWidth = DevDesc.right;
		devmode.dmPelsHeight = DevDesc.bottom;
		devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
		hr = ChangeDisplaySettings( &devmode, CDS_FULLSCREEN );
        //ウィンドウモードへ変更する。
        ::SetWindowLong( hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE );
        //ウィンドウモードへ変更したとき、ウィンドウの位置を変更する。
        ::SetWindowPos( hWnd, 
                        HWND_NOTOPMOST,                           //フルスクリーンは最前面表示されているので、解除する。
                        0,
                        0,
                        WIDTH,
                        HEIGHT,
                        SWP_SHOWWINDOW );		
    }
    else
    {
		//解像度を1280*720にかえる
		//グラボの性能要チェック
		//対応しているものしか解像度の変更はできない
		devmode.dmPelsWidth = 1280;
		devmode.dmPelsHeight = 720;
		devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
		hr = ChangeDisplaySettings( &devmode, CDS_FULLSCREEN );
        ::SetWindowLong( hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE );
        ::SetWindowPos( hWnd, 
                        HWND_TOPMOST,
                        0,
                        0,
                        WIDTH,
                        HEIGHT,
                        SWP_SHOWWINDOW );   
		
    }
	
}

BOOL GetWindowMode()
{
	return d3dpp.Windowed;
}