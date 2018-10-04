//	�쐬��	�T�c

#include "graphic.h"

IDirect3D9*			pDirect3D;
IDirect3DDevice9*	pD3Device;

D3DPRESENT_PARAMETERS	d3dpp;

TEXTUREINFO TextureInfo [TEXTURE_MAX];



//=================================================================================
//
//	�O���t�B�b�N�̏�����
//
//==================================================================================

// Direct3D�̏�����
// ��1�����F�E�C���h�n���h��
bool InitDraw ( HWND hWnd )
{
	D3DDISPLAYMODE d3ddm;
	
	// DirectX �I�u�W�F�N�g�̐���
	pDirect3D = Direct3DCreate9 ( D3D_SDK_VERSION );

	// Display Mode �̐ݒ�
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

// �e�N�X�`���̓ǂݍ���
// ��1�����F("�摜�̖��O.�g���q"), ��2�����F�e�N�X�`���̔ԍ�, ���ߐFRGB 
bool LoadTexture ( LPCSTR lpFileName, int TextuerNumber, int iColorRed, int iColorGreen, int iColorBlue )
{
	// �e�N�X�`�������ɓǂݍ���ł���ꍇ�͂����ŉ������
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
						MessageBoxA(0,"�e�N�X�`�����ǂݍ��߂܂���ł���(����;)",NULL,MB_OK);
						return false;
	}

	// �e�N�X�`�����擾
	D3DSURFACE_DESC desc;

	TextureInfo [TextuerNumber].pTexture -> GetLevelDesc ( 0, &desc );
	TextureInfo [TextuerNumber].width = desc.Width;
	TextureInfo [TextuerNumber].height = desc.Height;

	return true;
}
//=========================================================================================
//
// ���ł�CUSTOMVERTEX�^�̒��_�z�������Ă���ꍇ�̒��_�̑���
//
//========================================================================================

//	���_�̏������W�����肷��
// ��1�����F�e�N�X�`���̔ԍ�, ��2�����F�e�N�X�`���̍\����, ��3,4�����F����̒��_����
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
//	�|���S���T�C�Y�ύX
//	����>1�F�e�N�X�`���̔ԍ� 2�F���_�̐擪�|�C���^ 3,4�F�`��̊�ƂȂ鍶��̓_xy, 5,6�F���_�̏c���� 
//	7,8�F����uv���W 9,10�F�A�j���[�V�����摜��1�R�}�̏c����
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

//	��]�p�֐�
//	������1�F���_�z��̐擪�|�C���^ 2,3�F��]�������_X,Y�@4�F��]������p�x, 5,6�F��]�����������_�̏c����
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

//	�A�j���[�V�����p�֐�
//	������1�F�e�N�X�`���̔ԍ� 2,3�F�A�j���[�V���������������_�̏c���� 4,5�F�A�j���[�V�������������摜�̍����UV���W
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

//	���_�F����p�֐�
//	������1�F���_�z��̐擪�|�C���^ 2�`5�FARGB�l
//	���ӁI��ARGB�l��255�ȏ������Ȃ��ŉ������I
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

//	�摜�̐L�k
//	������1�F���_�z��̐擪�|�C���^ 2,3�F�L�k�������_X,Y�@4�F�摜�̌X��, 5,6�F�L�k�����钸�_�̏c����
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
//	�`�揈��
//
//==================================================================================

// �`��O����
void BeginDraw (void)
{
	if ( !pD3Device )
		return;

	// �X�e�[�W�X�e�[�g�̐ݒ�
	pD3Device -> SetRenderState ( D3DRS_ALPHABLENDENABLE,	true );
	
	pD3Device -> SetTextureStageState ( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE	);
	pD3Device -> SetTextureStageState ( 0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE	);

	pD3Device -> SetTextureStageState ( 0, D3DTSS_COLOROP,		D3DTOP_MODULATE	);

	pD3Device -> SetTextureStageState ( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE	);
	pD3Device -> SetTextureStageState ( 0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE	);

	pD3Device -> SetTextureStageState ( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE );

	// �`����@�̐ݒ�
	

	pD3Device ->  SetRenderState ( D3DRS_SRCBLEND,				D3DBLEND_SRCALPHA	);
	pD3Device ->  SetRenderState ( D3DRS_DESTBLEND,				D3DBLEND_INVSRCALPHA);
	pD3Device ->  SetRenderState ( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1		);
	
	
	pD3Device -> SetRenderState ( D3DRS_ZENABLE,			false );
	
	// ��ʂ̏���

	HRESULT hr = pD3Device->Clear(0, NULL,
				D3DCLEAR_TARGET,
				D3DCOLOR_XRGB ( 0 , 0 , 0 ),
				1.f, 0 );

	if( FAILED(hr)) 
		return;

	// �`��̊J�n
	pD3Device -> BeginScene();

	// ��ʂ̕`��
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

// �`�揈��
// ��1�����F���_�z��̐擪�|�C���^, ��2�����F�e�N�X�`���̔ԍ�
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

//	�傫����ς��ĕ`��
//	����>1�F�e�N�X�`���̔ԍ� 2,3�F�`��̊�ƂȂ鍶��̓_xy, 4,5�F�E���̓_xy 
//	6,7�F����uv���W 8,9�F�E����uv���W
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

//	�傫���ƐF����ς���(������)�`��
//	����>1�F�e�N�X�`���̔ԍ� 2,3�F�`��̊�ƂȂ鍶��̓_xy, 4,5�F�E���̓_xy 
//	6,7�F����uv���W 8,9�F�A�j���[�V�����摜��1�R�}�̏c����
//	������11�`14�F�FARGB�l
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

//	���S�_����`�� (��])
//	������1�F�e�N�X�`���̔ԍ� 2,3�F��ƂȂ钆�S�_ 4�F�X�������p�x(�X���Ȃ��Ȃ�0��OK) 5,6�F�摜�̏c����
//	������7,8�F�摜�̍����UV���W 9,10:�摜�̉E����UV���W
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

//	���S�_����`�� (�F���ω�)
//	������1�F�e�N�X�`���̔ԍ� 2,3�F��ƂȂ钆�S�_ 4�F�X�������p�x(�X���Ȃ��Ȃ�0��OK) 5,6�F�L�k�����钸�_�̏c����
//	������7,8�F�摜�̍����UV���W 9,10:�摜�̉E����UV���W 11�`14�FARGB�l
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

//	���S�_����`�� (�L�k)
//	������1�F�e�N�X�`���̔ԍ� 2,3�F�L�k�������_X,Y 4�F�p�x 5,6�F�摜�̏c����
//	������7,8�F�摜�̍����UV���W 9,10:�摜�̉E����UV���W 11�`14�FARGB�l
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

// �`��㏈��
void EndDraw (void)
{
	// �`��̏I��
	pD3Device -> EndScene();

	// �\��
	pD3Device -> Present ( NULL, NULL, NULL, NULL );
}

// �e�N�X�`���̉��
// ��1�����F�e�N�X�`���̔ԍ�
void ReleaseTexture ( int TextuerNumber )
{
	SAFE_RELEASE ( TextureInfo[TextuerNumber].pTexture );
}

// �g�p���Ă���S�Ẵe�N�X�`���̉��
void ReleaseAllTexture (void)
{
	for ( int i = 0; i < TEXTURE_MAX; i++ )
	{
		ReleaseTexture (i);
	}
}

// Direct3D�I�u�W�F�N�g�̉��
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
   //�E�B���h�E�X�^�C���̐؂�ւ��v�������������Ƃ�
   //****************************************************************
    //���݃E�B���h�E���[�h�̂Ƃ�
	if( d3dpp.Windowed == TRUE )
    {
        //�E�B���h�E���[�h�̂Ƃ����݂̃E�B���h�E�̕\���ʒu��ۑ�����B
        //WindowPos�́A�E�B���h�E���[�h����t���X�N���[���֐؂�ւ��A�ĂуE�B���h�E���[�h�ɐ؂�ւ����Ƃ��A�����Ŏ擾�������W�ŃE�B���h�E��\������B
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


    //�f�o�C�X�̕������s��
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
        //�E�B���h�E���[�h�֕ύX����B
        ::SetWindowLong( hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE );
        //�E�B���h�E���[�h�֕ύX�����Ƃ��A�E�B���h�E�̈ʒu��ύX����B
        ::SetWindowPos( hWnd, 
                        HWND_NOTOPMOST,                           //�t���X�N���[���͍őO�ʕ\������Ă���̂ŁA��������B
                        0,
                        0,
                        WIDTH,
                        HEIGHT,
                        SWP_SHOWWINDOW );		
    }
    else
    {
		//�𑜓x��1280*720�ɂ�����
		//�O���{�̐��\�v�`�F�b�N
		//�Ή����Ă�����̂����𑜓x�̕ύX�͂ł��Ȃ�
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