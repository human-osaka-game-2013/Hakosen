#include <Windows.h>


enum SOUND_ID
{
	//　BGMの曲数を用意
	TITLE_BGM,
	
	CS_BGM,
		
	RESULT_BGM,

	BGM_01,
	BGM_02,
	BGM_03,
	BGM_04,
	BGM_05,

	//　SEの曲数を用意
	T_CURSOR_MOVE_SE,
	T_CURSOR_SELECT_SE,
	
	CS_CURSOR_MOVE_SE,
	CS_BOY_SE,
	CS_GIRL_SE,
	CS_SELECT_SE,

	R_CURSOR_MOVE_SE,
	R_CURSOR_SELECT_SE,

	START_SE_A,
	START_SE_B,

	//配列０補正
	SOUND_MAX

};


//////////////////////////////////////////
//
//	※使用法
//	
//	//アプリ開始時にWin_Mainで呼出
//		InitXAudio2
//
//		//主にシーン初期化時で呼出
//			LoadSoundFile
//				->	OpenWaveFile	(内部で呼出・・・必要なファイル名をここでロード)
//
//			//サウンドの使用したい場面で呼出
//			SoundPlay, SoundStop, SoundAllStop
//
//		//主にシーン移行時で呼出
//			ReleaseSoundFile, ReleaseAllSoundFile
//
//
//
//		//
//
//
//	//アプリ終了時にリリース処理で呼出
//			ReleaseXAudio2
//
//////////////////////////////////////////



//XAudioの初期化
HRESULT InitXAudio2(void);

//読込んだ.wavファイルを開いて解析
bool OpenWaveFile(LPSTR FileName, WAVEFORMATEX &waveFormatEx, BYTE **ppData, DWORD &dataSize);

//音声ファイルの読み込み	->	解析できたら使用準備完了
void LoadSoundFile(LPSTR FileName, int IN_ID);

//サウンド再生
void SoundPlay(int ID, bool LOOP);

//サウンド停止
void SoundStop(int ID);

//サウンド全停止
void SoundAllStop(void);

//ファイル->解放
void ReleaseSoundFile(int ID);

//ファイル->全解放
void ReleaseAllSoundFile(void);

//XAudioの解放
HRESULT ReleaseXAudio2(void);











