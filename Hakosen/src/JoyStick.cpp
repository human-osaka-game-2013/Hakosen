#include"JoyStick.h"
#include <wbemidl.h>
#include<tchar.h>

#define PAD_MAX 4

struct XINPUT_DEVICE_NODE
{
    DWORD dwVidPid;
    XINPUT_DEVICE_NODE* pNext;
};

struct DI_ENUM_CONTEXT
{
    DIJOYCONFIG* pPreferredJoyCfg;
    bool bPreferredJoyCfgValid;
};

struct JOYSTICK_STATE{
	KEYSTATE Button[BUTTON_MAX];
	KEYSTATE Cross[CROSS_MAX];
	TCHAR* DeviceName;
};
//////////////////////////////////////////////////////

//グローバル変数
int pad_no = 0 ;

bool g_bFilterOutXinputDevices[PAD_MAX] = {false};
XINPUT_DEVICE_NODE*     g_pXInputDeviceList[PAD_MAX]  = {NULL};

//本体の参照受け
LPDIRECTINPUT8 input8;
//ジョイスティック用デバイス
LPDIRECTINPUTDEVICE8    g_pJoystick[PAD_MAX] = {NULL};
//ジョイスティックの入力情報
DIJOYSTATE2 js[PAD_MAX];
DIJOYSTATE2 prevjs[PAD_MAX];
JOYSTICK_STATE JoyStickState[PAD_MAX];
//////////////////////////////////////////////////////

//プロトタイプ宣言
BOOL CALLBACK    EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
void XDeviceCheck();
void CleanupForIsXInputDevice();
HRESULT SetupForIsXInputDevice();

/////////////////////////////////////////////////////

void XDeviceCheck()
{
	TCHAR* strCmdLine;
    int nNumArgs;
    LPWSTR* pstrArgList = CommandLineToArgvW( GetCommandLineW(), &nNumArgs );

    for( int iArg = 1; iArg < nNumArgs; iArg++ )
    {
        strCmdLine = pstrArgList[iArg];

        // Handle flag args
        if( *strCmdLine == _T('/') || *strCmdLine == _T('-') )
        {
            strCmdLine++;

            int nArgLen = ( int )wcslen( _T("noxinput") );
            if( _wcsnicmp( strCmdLine, _T("noxinput"), nArgLen ) == 0 && strCmdLine[nArgLen] == 0 )
            {
                g_bFilterOutXinputDevices[pad_no] = true;
                continue;
            }
        }
    }
    LocalFree( pstrArgList );
	
}

//ジョイスティックの初期化
HRESULT InitJoyStick( HWND hDlg , LPDIRECTINPUT8& _din8)
{
    HRESULT hr;

	input8 = _din8;

	for( DWORD i = 0 ; i < PAD_MAX ; i++ ){
		g_pJoystick[i] = NULL;
		SecureZeroMemory(&js[i],sizeof(DIJOYSTATE2));
		SecureZeroMemory(&prevjs[i],sizeof(DIJOYSTATE2));
		SecureZeroMemory(&g_pXInputDeviceList[i],sizeof(XINPUT_DEVICE_NODE*));
		SecureZeroMemory(&JoyStickState[i],sizeof(JOYSTICK_STATE));

		for( DWORD j = 0 ; j < BUTTON_MAX ; j++ ){
			JoyStickState[i].Button[j] = OFF;
		}
		for( DWORD j = 0 ; j < CROSS_MAX ; j++ ){
			JoyStickState[i].Cross[j] = OFF;
		}
	}

	DI_ENUM_CONTEXT enumContext[PAD_MAX];
	IDirectInputJoyConfig8* pJoyConfig[PAD_MAX] = {NULL};
	for( pad_no = 0 ; pad_no < PAD_MAX ; pad_no++ ){
	
		XDeviceCheck();

		if( !g_bFilterOutXinputDevices[pad_no] )
			SetupForIsXInputDevice();

		DIJOYCONFIG PreferredJoyCfg = {0};
		
		enumContext[pad_no].pPreferredJoyCfg = &PreferredJoyCfg;
		enumContext[pad_no].bPreferredJoyCfgValid = false;

		
		if( FAILED( hr = input8->QueryInterface( IID_IDirectInputJoyConfig8, ( void** )&pJoyConfig[pad_no] ) ) )
			return hr;

		PreferredJoyCfg.dwSize = sizeof( PreferredJoyCfg );
		if( SUCCEEDED( pJoyConfig[pad_no]->GetConfig( 0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) // This function is expected to fail if no joystick is attached
			enumContext[pad_no].bPreferredJoyCfgValid = true;
		SAFE_RELEASE( pJoyConfig[pad_no] );

		if( g_bFilterOutXinputDevices[pad_no] )
			CleanupForIsXInputDevice();

	
		//デバイスの列挙
		if( FAILED( hr = input8->EnumDevices( DI8DEVCLASS_GAMECTRL,
											 EnumJoysticksCallback,
											 &enumContext[pad_no], DIEDFL_ATTACHEDONLY ) ) )
			continue;
	
		// Make sure we got a joystick
		if( NULL == g_pJoystick[pad_no] )
		{
			continue;
		}

		// データフォーマットのセット
		if( FAILED( hr = g_pJoystick[pad_no]->SetDataFormat( &c_dfDIJoystick2 ) ) )
			continue;

		// 協調レベル
		if( FAILED( hr = g_pJoystick[pad_no]->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE |
														   DISCL_FOREGROUND ) ) )
			continue;

		//の列挙
		if( FAILED( hr = g_pJoystick[pad_no]->EnumObjects( EnumObjectsCallback,
												   ( VOID* )hDlg, DIDFT_ALL ) ) )
			continue;
	}

    return S_OK;
}


//-----------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains 
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it痴 an XInput device
// Unfortunately this information can not be found by just using DirectInput.
// Checking against a VID/PID of 0x028E/0x045E won't find 3rd party or future 
// XInput devices.
//
// This function stores the list of xinput devices in a linked list 
// at g_pXInputDeviceList, and IsXInputDevice() searchs that linked list
//-----------------------------------------------------------------------------
HRESULT SetupForIsXInputDevice()
{
    IWbemServices* pIWbemServices = NULL;
    IEnumWbemClassObject* pEnumDevices = NULL;
    IWbemLocator* pIWbemLocator = NULL;
    IWbemClassObject* pDevices[20] = {0};
    BSTR bstrDeviceID = NULL;
    BSTR bstrClassName = NULL;
    BSTR bstrNamespace = NULL;
    DWORD uReturned = 0;
    bool bCleanupCOM = false;
    UINT iDevice = 0;
    VARIANT var;
    HRESULT hr;

    // CoInit if needed
    hr = CoInitialize( NULL );
    bCleanupCOM = SUCCEEDED( hr );

    // Create WMI
    hr = CoCreateInstance( __uuidof( WbemLocator ),
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           __uuidof( IWbemLocator ),
                           ( LPVOID* )&pIWbemLocator );
    if( FAILED( hr ) || pIWbemLocator == NULL )
        goto LCleanup;

    // Create BSTRs for WMI
    bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" ); if( bstrNamespace == NULL ) goto LCleanup;
    bstrDeviceID = SysAllocString( L"DeviceID" );           if( bstrDeviceID == NULL )  goto LCleanup;
    bstrClassName = SysAllocString( L"Win32_PNPEntity" );    if( bstrClassName == NULL ) goto LCleanup;

    // Connect to WMI 
    hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L,
                                       0L, NULL, NULL, &pIWbemServices );
    if( FAILED( hr ) || pIWbemServices == NULL )
        goto LCleanup;

    // Switch security level to IMPERSONATE
    CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                       RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );

    // Get list of Win32_PNPEntity devices
    hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices );
    if( FAILED( hr ) || pEnumDevices == NULL )
        goto LCleanup;

    // Loop over all devices
    for(; ; )
    {
        // Get 20 at a time
        hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
        if( FAILED( hr ) )
            goto LCleanup;
        if( uReturned == 0 )
            break;

        for( iDevice = 0; iDevice < uReturned; iDevice++ )
        {
            // For each device, get its device ID
            hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
            if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
            {
                // Check if the device ID contains "IG_".  If it does, then it痴 an XInput device
                // Unfortunately this information can not be found by just using DirectInput 
                if( wcsstr( var.bstrVal, L"IG_" ) )
                {
                    // If it does, then get the VID/PID from var.bstrVal
                    DWORD dwPid = 0, dwVid = 0;
                    WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
                    if( strVid && swscanf_s( strVid, L"VID_%4X", &dwVid ) != 1 )
                        dwVid = 0;
                    WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
                    if( strPid && swscanf_s( strPid, L"PID_%4X", &dwPid ) != 1 )
                        dwPid = 0;

                    DWORD dwVidPid = MAKELONG( dwVid, dwPid );

                    // Add the VID/PID to a linked list
                    XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
                    if( pNewNode )
                    {
                        pNewNode->dwVidPid = dwVidPid;
                        pNewNode->pNext = g_pXInputDeviceList[pad_no] ;
                        g_pXInputDeviceList[pad_no]  = pNewNode;
						NULL_DELETE(pNewNode);
                    }
                }
            }
            SAFE_RELEASE( pDevices[iDevice] );
        }
    }

LCleanup:
    if( bstrNamespace )
        SysFreeString( bstrNamespace );
    if( bstrDeviceID )
        SysFreeString( bstrDeviceID );
    if( bstrClassName )
        SysFreeString( bstrClassName );
    for( iDevice = 0; iDevice < 20; iDevice++ )
    SAFE_RELEASE( pDevices[iDevice] );
    SAFE_RELEASE( pEnumDevices );
    SAFE_RELEASE( pIWbemLocator );
    SAFE_RELEASE( pIWbemServices );

    return hr;
}


//-----------------------------------------------------------------------------
// Returns true if the DirectInput device is also an XInput device.
// Call SetupForIsXInputDevice() before, and CleanupForIsXInputDevice() after
//-----------------------------------------------------------------------------
bool IsXInputDevice( const GUID* pGuidProductFromDirectInput )
{
    // Check each xinput device to see if this device's vid/pid matches
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList[pad_no] ;
    while( pNode )
    {
        if( pNode->dwVidPid == pGuidProductFromDirectInput->Data1 )
            return true;
        pNode = pNode->pNext;
    }

    return false;
}


//-----------------------------------------------------------------------------
// Cleanup needed for IsXInputDevice()
//-----------------------------------------------------------------------------
void CleanupForIsXInputDevice()
{
    // Cleanup linked list
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList[pad_no] ;
    while( pNode )
    {
        XINPUT_DEVICE_NODE* pDelete = pNode;
        pNode = pNode->pNext;
        NULL_DELETE( pDelete );
    }
}



//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
    DI_ENUM_CONTEXT* pEnumContext = ( DI_ENUM_CONTEXT* )pContext;
    HRESULT hr;

	// 既に取得済みのデバイスだった場合はもう一回実行する
	for( int i = 0; i < pad_no; i++ )
	{
		if( g_pJoystick[i] && pEnumContext->pPreferredJoyCfg->guidInstance == pdidInstance->guidInstance)
			return DIENUM_CONTINUE;
	}

    if( g_bFilterOutXinputDevices[pad_no] && IsXInputDevice( &pdidInstance->guidProduct ) )
        return DIENUM_CONTINUE;

    // Skip anything other than the perferred joystick device as defined by the control panel.  
    // Instead you could store all the enumerated joysticks and let the user pick.
   /* if( pEnumContext->bPreferredJoyCfgValid &&
        !IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
        return DIENUM_CONTINUE;*/

    // Obtain an interface to the enumerated joystick.
    hr = input8->CreateDevice( pdidInstance->guidInstance, &g_pJoystick[pad_no], NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED( hr ) )
        return DIENUM_CONTINUE;


	//JoyStickState[pad_no].DeviceName = new TCHAR[wcslen(pdidInstance->tszInstanceName)];
	//_tcscpy(JoyStickState[pad_no].DeviceName,(TCHAR*)pdidInstance->tszInstanceName ); 
	//JoyStickState[pad_no].DeviceName = (TCHAR*)pdidInstance->tszInstanceName;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
}




//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext )
{
    HWND hDlg = ( HWND )pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof( DIPROPRANGE );
        diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin = -1000;
        diprg.lMax = +1000;

        // Set the range for the axis
        if( FAILED( g_pJoystick[pad_no]->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
            return DIENUM_STOP;

    }

    return DIENUM_CONTINUE;
}

void KeyStateCheck( int i )
{
	for( DWORD j = 0 ; j < BUTTON_MAX ; j++ ){ 
		if( js[i].rgbButtons[j] ){
			if( prevjs[i].rgbButtons[j])
				JoyStickState[i].Button[j] = ON;
			else 
				JoyStickState[i].Button[j] = PUSH;
		}
		else{
			if( prevjs[i].rgbButtons[j])
				JoyStickState[i].Button[j] = RELEASE;
			else 
				JoyStickState[i].Button[j] = OFF;
		}
	}
	
	if( js[i].lX == -1000 ){
		if( prevjs[i].lX == -1000 )
			JoyStickState[i].Cross[CROSS_LEFT] = ON;
		else 
			JoyStickState[i].Cross[CROSS_LEFT] = PUSH;
	}
	else{
		if( prevjs[i].lX == -1000)
			JoyStickState[i].Cross[CROSS_LEFT] = RELEASE;
		else 
			JoyStickState[i].Cross[CROSS_LEFT] = OFF;
	}

	if( js[i].lX == 1000 ){
		if( prevjs[i].lX == 1000 )
			JoyStickState[i].Cross[CROSS_RIGHT] = ON;
		else 
			JoyStickState[i].Cross[CROSS_RIGHT] = PUSH;
	}
	else{
		if( prevjs[i].lX == 1000)
			JoyStickState[i].Cross[CROSS_RIGHT] = RELEASE;
		else 
			JoyStickState[i].Cross[CROSS_RIGHT] = OFF;
	}

	if( js[i].lY == -1000 ){
		if( prevjs[i].lY== -1000 )
			JoyStickState[i].Cross[CROSS_UP] = ON;
		else 
			JoyStickState[i].Cross[CROSS_UP] = PUSH;
	}
	else{
		if( prevjs[i].lY == -1000)
			JoyStickState[i].Cross[CROSS_UP] = RELEASE;
		else 
			JoyStickState[i].Cross[CROSS_UP] = OFF;
	}

	if( js[i].lY == 1000 ){
		if( prevjs[i].lY == 1000 )
			JoyStickState[i].Cross[CROSS_DOWN] = ON;
		else 
			JoyStickState[i].Cross[CROSS_DOWN] = PUSH;
	}
	else{
		if( prevjs[i].lY == 1000 )
			JoyStickState[i].Cross[CROSS_DOWN] = RELEASE;
		else 
			JoyStickState[i].Cross[CROSS_DOWN] = OFF;
	}
}

void XBoxKeyStateCheck( int i )
{
	for( DWORD j = 0 ; j < BUTTON_MAX ; j++ ){ 
		if( js[i].rgbButtons[j] ){
			if( prevjs[i].rgbButtons[j])
				JoyStickState[i].Button[j] = ON;
			else 
				JoyStickState[i].Button[j] = PUSH;
		}
		else{
			if( prevjs[i].rgbButtons[j])
				JoyStickState[i].Button[j] = RELEASE;
			else 
				JoyStickState[i].Button[j] = OFF;
		}
	}
	for( DWORD j = 0 ; j < CROSS_MAX ; j++ ){ 
		
		if( js[i].rgdwPOV[0] == j * (DWORD)9000 ){
			if( prevjs[i].rgdwPOV[0] == js[i].rgdwPOV[0] )
				JoyStickState[i].Cross[j] = ON;
			else 
				JoyStickState[i].Cross[j] = PUSH;
		}
		else{
			if( prevjs[i].rgdwPOV == js[i].rgdwPOV )
				JoyStickState[i].Cross[j] = RELEASE;
			else 
				JoyStickState[i].Cross[j] = OFF;
		}
	}
}
//ジョイスティック更新
HRESULT UpdateJoyStick( HWND hDlg )
{
    HRESULT hr;

	for( int i = 0 ; i < PAD_MAX ; i++ ){
		//デバイスなければ、抜ける
		if( NULL == g_pJoystick[i] )
			continue;

		memcpy( &prevjs[i] , &js[i] , sizeof(DIJOYSTATE2));

		// Poll the device to read the current state
		hr = g_pJoystick[i]->Poll();
		if( FAILED( hr )  )
		{
			//Poll失敗してると、とりあえず、デバイスを取得する
			hr = g_pJoystick[i]->Acquire();
			if( hr == DIERR_INPUTLOST )
				continue;

        
			continue;
		}

		//デバイスの状態を得る
		if( FAILED( hr = g_pJoystick[i]->GetDeviceState( sizeof( DIJOYSTATE2 ), &js[i] ) ) )
			continue;

		//if ( !_tcscmp(JoyStickState[i].DeviceName , _T("XBOX 360 For Windows (Controller)") ) ){ 
		//	XBoxKeyStateCheck(i);
		//}
		//if ( !_tcscmp(JoyStickState[i].DeviceName , _T("MotioninJoy Virtual Game Controller") ) ){ 
		//	KeyStateCheck(i);
		//}
		XBoxKeyStateCheck(i);
	}

    return S_OK;
}



//ジョイスティックの解放
VOID FreeJoyStick()
{
	for( int i = 0 ; i < PAD_MAX ; i++ ){
		//SecureZeroMemory(&JoyStickState[i] , sizeof(JOYSTICK_STATE));
		//停止
		if( g_pJoystick[i] ){
			g_pJoystick[i]->Unacquire();

			
		}
		//解放
		SAFE_RELEASE( g_pJoystick[i] );
	}
}

//ボタンの取得
BOOL GetButton( int pn , BUTTON _b , KEYSTATE _st)
{
	return JoyStickState[pn].Button[_b] == _st ? TRUE : FALSE ;
}
//十字キーの取得
BOOL GetCrossKey( int pn , CROSS_KEY _ck , KEYSTATE _st )
{
	return JoyStickState[pn].Cross[_ck] == _st ? TRUE : FALSE ;
}