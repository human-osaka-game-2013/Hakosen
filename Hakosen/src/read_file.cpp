#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read_file.h"

#define TABLE_NUM		21
#define STAGE_WIDTH		5
#define STAGE_HEIGHT	4

#define TEMP_FILE_ADD	"table\\t_b_"
#define EXTENSION		".ECB"

BLOCK_INFO	iBlockTable[TABLE_NUM][STAGE_WIDTH][STAGE_HEIGHT] = {0};	//	初期のフィールド内のブロックテーブル
BLOCK_INFO	iAddBlockTable[TABLE_NUM][STAGE_WIDTH][STAGE_HEIGHT] = {0};	//	控えてるブロックテーブル
bool	bReadFlag = false;

void SetTable ( int iTableNum, int i_W, int i_H, int i_L_C, char* p )
{
	int iTemp = atoi(p);

	if ( i_L_C < 5 )
	{
		iAddBlockTable[iTableNum][i_W][i_H].iType		= iTemp / 10;
		iAddBlockTable[iTableNum][i_W][i_H].iDirection	= iTemp % 10;
	}
	else
	{
		iBlockTable[iTableNum][i_W][i_H].iType		= iTemp / 10;
		iBlockTable[iTableNum][i_W][i_H].iDirection	= iTemp % 10;
	}
}

#ifndef	STR
void ReadTable ( FILE* fp, int iTableNum )
{
	char	buf[256];
	char* p, *q;
	
	int iWidth = 0, iHeight = 3;
	int	iLoopCount = 1;

	while ( fgets ( buf, 256, fp) != NULL )
	{
		p = buf;
		
		do
		{
			q = strstr( p, "," );
			
			if ( q != NULL ) *q = '\0';

			SetTable ( iTableNum, iWidth, iHeight, iLoopCount, p );

			p = q + 1;

			iWidth++;

		}while ( q != NULL );

		//printf ( "\n" );
		if ( iLoopCount < 4 )		{ iHeight--;	}
		else if ( iLoopCount == 4 )	{ iHeight = 0;	}
		else if ( iLoopCount > 4 )	{ iHeight++;	}

		iWidth = 0;
		iLoopCount++;
	}
	iHeight = 0;
}

#else

void SetTable ( FILE* fp, int iTableNum )
{
	int result = 0, iWidth = 0;
	while ( ( result = fscanf( fp, "%d, %d, %d, %d, %d,"
							,&iBlockTable[iTableNum][0][iHeight] 
							,&iBlockTable[iTableNum][1][iHeight]
							,&iBlockTable[iTableNum][2][iHeight]
							,&iBlockTable[iTableNum][3][iHeight]
							,&iBlockTable[iTableNum][4][iHeight] ) ) != EOF )
	{
		printf("%d %d %d %d %d\n"
				,iBlockTable[iTableNum][0][iHeight]
				,iBlockTable[iTableNum][1][iHeight]
				,iBlockTable[iTableNum][2][iHeight]
				,iBlockTable[iTableNum][3][iHeight]
				,iBlockTable[iTableNum][4][iHeight]	);

		iHeight++;
	}
}

#endif

bool ReadECBFile ()
{
	FILE*	fp;

	char	cFileName[32];
	char	cTemp_F_Name[] = TEMP_FILE_ADD;
	char	cExtension[] = EXTENSION;

	int		ii;

	for ( ii = 0; ii < TABLE_NUM; ii++ )
	{
		sprintf( cFileName, "%s%d%s", cTemp_F_Name, ii, cExtension );

		if ( ( fp = fopen( cFileName, "r" ) ) != NULL )
		{
			ReadTable (fp, ii);

			fclose (fp);
			bReadFlag = true;
		}
		else
		{
			//printf( "%sファイルが開けません", cFileName );
			return false;
		}
	}
	return true;
}

//	ステージブロックの種類か向きを渡す
//	引数＞1：セット番号 2：ブロックの種類(eTYPE),ブロックの向き(eDirection) 3：行数(STAGE_WIDTH), 4：列数(STAGE_HEIGHT)
int	PassStageBlock ( int iSetNum, int iInfoType, int iWidth, int iHeight )
{
	int	iTemp = 0;

//#ifdef	_DEBUG
//	iSetNum = 6;
//#endif

	switch ( iInfoType )
	{
	case eTYPE:
		iTemp = iBlockTable[iSetNum][iWidth][iHeight].iType;

		break;

	case eDirection:
		iTemp = iBlockTable[iSetNum][iWidth][iHeight].iDirection;

		break;
	}

	return iTemp;
}

//	控えブロックの種類か向きを渡す
//	引数＞1：セット番号 2：種類(eTYPE)か向き(eDirection)を入れる 3：行数(STAGE_WIDTH), 4：列数(STAGE_HEIGHT)
int	PassAddBlock ( int iSetNum, int iInfoType, int iWidth, int iHeight )
{
	int	iTemp = 0;

//#ifdef	_DEBUG
//	iSetNum = 6;
//#endif

	switch ( iInfoType )
	{
	case eTYPE:
		iTemp = iAddBlockTable[iSetNum][iWidth][iHeight].iType;

		break;

	case eDirection:
		iTemp = iAddBlockTable[iSetNum][iWidth][iHeight].iDirection;

		break;
	}

	return iTemp;
}