#include "main.h"
#include "Audio.h"
#include "Lib.h"

//デバイスの準備
IXAudio2*					pXAudio2;
IXAudio2MasteringVoice*	pMasteringVo;
IXAudio2SourceVoice*		pSourceVo[SOUND_MAX]	= {0};
XAUDIO2_BUFFER			buffer[SOUND_MAX]		= {0};

//　Xオーディオの初期化
HRESULT InitXAudio2(void)
{
	HRESULT			hr = NULL;
	unsigned int flag = 0;

	//CO_Mを初期化
	CoInitializeEx(NULL,COINIT_MULTITHREADED);

	pXAudio2 = NULL;
	hr = XAudio2Create(&pXAudio2,flag,XAUDIO2_DEFAULT_PROCESSOR);

	if(SUCCEEDED(hr))
	{
		hr = pXAudio2->CreateMasteringVoice(&pMasteringVo,0,0,0,0,0);
		if(FAILED(hr))
		{
			//エラー時にはメッセージボックスで表示
			ShowCursor(true);
			MessageBox(NULL,TEXT("オーディオの作成に失敗"),TEXT("初期化失敗"),S_OK);
			ShowCursor(false);
		}
	}

	else
	{
		//エラー時にはメッセージボックスで表示
			ShowCursor(true);
			MessageBox(NULL,TEXT("オーディオの再生に失敗しました。"),TEXT("XAudio2がNULL"),S_OK);
			ShowCursor(false);
	}

	return hr;

}



//.wavファイルを開いて解析
bool OpenWaveFile(LPSTR FileName,WAVEFORMATEX &waveFormatEx, BYTE **ppData, DWORD &dataSize)
{
	if(FileName == 0)
	{
		return false;
	}

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	//Waveファイルオープン
	memset( &mmioInfo,0,sizeof(MMIOINFO) );
	hMmio = mmioOpenA(FileName, &mmioInfo, MMIO_READ);

	if(!hMmio)
	{
		return false;		//ファイルオープンに失敗
	}

	// RIFFチャンク検索
	MMRESULT		mmRes;
	MMCKINFO		riffChunk;

	riffChunk.fccType = mmioFOURCC('W','A','V','E');	//WAVEファイルかを確認？？
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);

	if(mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}

	//フォーマットチャンク検索
	MMCKINFO	formatChunk;
	formatChunk.ckid = mmioFOURCC('f','m','t',' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);

	if(mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}

	DWORD fmsize	= formatChunk.cksize;
	DWORD size	= mmioRead(hMmio, (HPSTR)&waveFormatEx, fmsize);

	if(size != fmsize)
	{
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	//データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d','a','t','a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);

	if(mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}

	*ppData = new BYTE[ dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)*ppData, dataChunk.cksize);

	if(size != dataChunk.cksize)
	{
		delete[] *ppData;
		return false;
	}
	dataSize = size;

	//ハンドルクローズ
	mmioClose(hMmio, 0);

	return true;
}


//.wavファイルの読み込み
void LoadSoundFile (LPSTR FileName, int IN_ID)
{
	//////////////////////////////////
	//	サウンドデータ(.wavファイル)の読み込み
	//	ファイル名とIDを指定
	//	(Audio.hに引数を渡す)
	//////////////////////////////////

	//同じIDへの上書きが起こった際、以前のものを解放処理(メモリリーク防止)
	if(buffer[IN_ID].pAudioData != NULL)
	{
		ReleaseSoundFile(IN_ID);
	}

	HRESULT		hr;
	WAVEFORMATEX	wFmt;
	BYTE*		pWaveData	= 0;
	DWORD		waveSize	= 0;

	if(pXAudio2 != NULL)
	{
		if(!OpenWaveFile (FileName, wFmt, &pWaveData, waveSize) )
		{
			//失敗した原因をメッセージボックスで表示
			ShowCursor(true);
			MessageBoxA(NULL,FileName, "読込失敗", S_OK);
			ShowCursor(false);
			return;
		}

		if(FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVo[IN_ID],&wFmt, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL) ) )
		{
			//失敗した原因をメッセージボックスで表示
			ShowCursor(true);
			MessageBox(NULL, TEXT("SourceVoiceのクリエイトに失敗"), TEXT("エラー"), S_OK);
			ShowCursor(false);
			return;
		}

		//バッファーへ値をセット
		buffer[IN_ID].AudioBytes	= waveSize;
		buffer[IN_ID].pAudioData	= pWaveData;
		buffer[IN_ID].Flags		= XAUDIO2_END_OF_STREAM;

	}

	return;
}


//-------------------------------------------------------
//	ここまでが音楽ファイルの準備
//-------------------------------------------------------



//音声ファイルの再生
void SoundPlay (int ID, bool LOOP)
{

	//--------------------------------------------------------------------
	//	音源の再生
	//	引数
	//	ID	->	再生したいデータのIDを指定（DxAudioX2.hにて列挙している）
	//	LOOP	->	ループするかどうか？を指定する
	//---------------------------------------------------------------------


	//再生中のファイルがあった場合に備えて、念のための停止処理
	SoundStop(ID);

	if(buffer[ID].pAudioData != NULL)
	{
		buffer[ID].LoopBegin	= 0;
		buffer[ID].LoopLength	= 0;
		buffer[ID].PlayBegin	= 0;
		buffer[ID].PlayLength	= 0;

		if(LOOP)
		{
			//ループの指定があった場合ループの設定
			buffer[ID].LoopCount	= XAUDIO2_LOOP_INFINITE;
		}
		
		pSourceVo[ID]->SubmitSourceBuffer(&buffer[ID]);
		pSourceVo[ID]->Start(0);
		
	}
}

//音声ファイルの停止
void SoundStop (int ID)
{
	//--------------------------------------------
	//	音源の停止
	//	引数
	//	ID	->	停止したいデータのIDを指定（DxAudioX2.hにて列挙している）
	//--------------------------------------------

	if(buffer[ID].pAudioData != NULL)
	{
		if(buffer[ID].LoopCount == XAUDIO2_LOOP_INFINITE)
		{
			buffer[ID].LoopCount = 0;
		}

		pSourceVo[ID]->Stop(0);

		pSourceVo[ID]->FlushSourceBuffers();
	}
}


//全ての音源ファイルを停止する
void SoundAllStop (void)
{
	//--------------------------------------------
	//	すべての音源の停止
	//	引数無視で全停止
	//--------------------------------------------

	//サウンドデータの最大数分停止処理を実行
	for(int LOOP = 0; LOOP < SOUND_MAX; LOOP++)
	{
		//再生データを検索
		if(buffer[LOOP].pAudioData != NULL)
		{
			SoundStop(LOOP);
		}
	}
}



//音声ファイルの解放
void ReleaseSoundFile(int ID)
{
	//念ため再生停止
	pSourceVo[ID] -> Stop( 0 );

	NULL_ARRAY_DELETE( buffer[ID].pAudioData );

	pSourceVo[ID] -> DestroyVoice( );
	pSourceVo[ID] = NULL;
	
	return;

}

//全ての音声ファイルの解放
void ReleaseAllSoundFile(void)
{
	//サウンドデータの最大数分停止処理を実行
	for(int LOOP = 0; LOOP < SOUND_MAX; LOOP++)
	{
		//再生データを検索
		if(buffer[LOOP].pAudioData != NULL)
		{
			ReleaseSoundFile(LOOP);
		}
	}
}


//	Xオーディオの解放を行う
HRESULT ReleaseXAudio2(void)
{
	//再生中のファイルがあった場合に備えて、念のための停止処理
	SoundAllStop();
	ReleaseAllSoundFile();


	if(pMasteringVo)
	{
		//読み込んでメモリに保存されている音源ファイルを解放
		pMasteringVo->DestroyVoice();
		pMasteringVo = NULL;
	}

	if(pXAudio2)
	{
		// XAudio自体を解放
		SAFE_RELEASE(pXAudio2);
		pXAudio2 = NULL;
	}



	//	PG亀田の環境ではコメントアウトしないとエラーが出る模様
	//	12/12

	// CO_Mの初期化
	CoUninitialize();

	return S_OK;

}