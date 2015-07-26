
struct	BLOCK_INFO
{
	int	iType;
	int	iDirection;
};

enum ePass
{
	eTYPE,

	eDirection
};

//	ファイル読み込み関数
//	開けないファイルがあるとfalseを返し、決められたファイルを全て読み込むとtrueを返す
bool ReadECBFile ();

//	ブロックの種類か向きを渡す
//	引数＞1：セット番号 2：種類(eTYPE)か向き(eDirection)を入れる 3：行数(STAGE_WIDTH), 4：列数(STAGE_HEIGHT)
int	PassStageBlock ( int iSetNum, int iInfoType, int iWidth, int iHeight );

//	ブロックの種類か向きを渡す
//	引数＞1：セット番号 2：種類(eTYPE)か向き(eDirection)を入れる 3：行数(STAGE_WIDTH), 4：列数(STAGE_HEIGHT)
int	PassAddBlock ( int iSetNum, int iInfoType, int iWidth, int iHeight );
