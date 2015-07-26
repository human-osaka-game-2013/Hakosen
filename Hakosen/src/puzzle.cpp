#include "puzzle.h"
#include "read_file.h"
#include "input.h"
#include "Audio.h"
#include"select.h"
#include "words.h"
#include "Result.h"

//パズル画面の変数宣言
int BLOCKROOT[B_TYPEMAX][DIRECTION_MAX][4] =
{
	//BLOCK1 縦長棒
	{
		//1,  ↑ 　2. →  3,　↓  4,　←
		{1,0,1,0},
		{0,1,0,1},
		{1,0,1,0},
		{0,1,0,1},
	},
	{
		{0,0,1,1},
		{1,0,0,1},
		{1,1,0,0},
		{0,1,1,0},
	},
	{
		{1,1,1,0},
		{0,1,1,1},
		{1,0,1,1},
		{1,1,0,1},
	},
	{
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1},
	},
	{
		{2,3,3,2},
		{2,2,3,3},
		{3,2,2,3},
		{3,3,2,2},
	},
};
int settablenum[TABLELOOPNUM];
int settabledirection[STAGE_WIDTH][STAGE_HEIGHT][100];

bool	charaupdownflag = true;
float defficultdamage[3]=
{1.5f,1.f,0.8f};
STAGE	stage[STAGE_WIDTH][STAGE_HEIGHT][2];
PLAYER_INFO	player_info[2];
STAGE	addblock[STAGE_WIDTH][STAGE_HEIGHT][2];
STAGE	nextblock[STAGE_WIDTH][STAGE_HEIGHT][2];
int addblocknum[STAGE_WIDTH][2];
bool bRootstop[STAGE_WIDTH][STAGE_HEIGHT][DIRECTION_MAX]={0};	//一度通った所をtrueにしてフラグ管理

bool DropFlag[2] = {false};
float MLen[MAJIC_MAX][2];
bool ThunderFlag[2] = {false};

//最初のカウント
int CountDown = 0 ;
bool CountDounFlag = false;
CUSTOMVERTEX Count[4];

struct ATTACK_EFFECT{
	D3DXVECTOR2 Pos;
	D3DXVECTOR2 Target;
	D3DXVECTOR2 Speed;
	int count;
	bool flag;
};
ATTACK_EFFECT AttackEffect[2];

//	追加日	2/27	亀田
int	iWordsPattern = 0;	//	台詞のパターン変数

//パズル画面初期化
void PuzzleInit()
{
	charaupdownflag = true;

	LoadSoundFile("sound\\pzl_bgm.wav", BGM_01);
	LoadSoundFile("sound\\pzl_ido.wav", BGM_02);
	LoadSoundFile("sound\\pzl_kaiten.wav",BGM_03);
	LoadSoundFile("sound\\pzl_E_start00.wav", BGM_04);

	LoadSoundFile("sound\\start_a.wav", START_SE_A);
	LoadSoundFile("sound\\start_b.wav", START_SE_B);

	LoadTexture ("image\\puzzle\\block.png", BLOCK_SET, 0, 255, 0);
	LoadTexture ("image\\puzzle\\background1.png", PUZZLE_BACK, 0, 255, 0);
	LoadTexture ("image\\puzzle\\Ele_BOX.png", ELE_BOX, 0, 255, 0);
	LoadTexture ("image\\puzzle\\moji.png", ELE_BOX_MOJI, 0, 255, 0);
	LoadTexture ("image\\puzzle\\cursor.png", CURSOR, 255, 255, 255);
	LoadTexture ("image\\puzzle\\BOY.png", CHARA_BOY, 255, 255, 255);
	LoadTexture ("image\\puzzle\\GIRL.png", CHARA_GIRL, 255, 255, 255);
	LoadTexture ("image\\puzzle\\LIFEMAX.png", LIFE_MAX, 255, 255, 255);
	LoadTexture ("image\\puzzle\\LIFEMIN.png", LIFE_MIN, 255, 255, 255);
	LoadTexture ("image\\puzzle\\thunder.png", THUNDER, 255, 255, 255);
	LoadTexture ("image\\puzzle\\thunder2.png", ATTACK_THUNDER, 255, 255, 255);
	LoadTexture ("image\\puzzle\\maru.png", MARU, 255, 255, 255);
	LoadTexture ("image\\puzzle\\number.png", COUNT_DOWN, 255, 255, 255);
	LoadTexture ("image\\puzzle\\start.png", COUNT_START, 255, 255, 255);
	SetVertex(COUNT_DOWN,Count,0.f,0.f,450.f,600.f);
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < 2 ; j++ )
		{
			addblocknum[i][j] = 0;
		}
	}
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			for( int l = 0 ; l < 100 ; l++ )
			{
				settabledirection[i][j][l] = rand()%4;
			}	
		}
	}
	for( int i = 0 ; i < TABLELOOPNUM ; i++ )
	{
		settablenum[i] = i;
	}
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			
			stage[i][j][0].block.type = stage[i][j][1].block.type = PassStageBlock ( settablenum[0], eTYPE, i, j );
			stage[i][j][0].block.direction = stage[i][j][1].block.direction = settabledirection[i][j][0];
			stage[i][j][0].block.runflag = stage[i][j][1].block.runflag = true;
			stage[i][j][0].block.blockalpha = stage[i][j][1].block.blockalpha = 255;
			stage[i][j][0].block.angle = stage[i][j][1].block.angle = stage[i][j][0].block.direction * 90.f;
			if(PassAddBlock ( 0, eTYPE, i, j ) <= 4)
			{
				addblock[i][j][0].block.type = addblock[i][j][1].block.type = PassAddBlock (settablenum[0], eTYPE, i, j );
				addblock[i][j][0].block.direction = addblock[i][j][1].block.direction = PassAddBlock ( settablenum[0], eDirection, i, j );
				addblock[i][j][0].block.runflag = addblock[i][j][1].block.runflag = true;
			}
			else
			{
				addblock[i][j][0].block.runflag = addblock[i][j][1].block.runflag = false;
			}
			addblock[i][j][0].block.angle = addblock[i][j][1].block.angle = addblock[i][j][0].block.direction * 90.f;
			addblock[i][j][0].block.moveflag = addblock[i][j][1].block.moveflag = false;
			addblock[i][j][0].block.blockalpha = addblock[i][j][1].block.blockalpha = 255;
			//ステージの左端の座標 + ブロックの幅 * 何番目か + ブロックの幅の半分
			stage[i][j][0].x = STAGE_LEFT + BLOCK_WIDTH * i + BLOCK_WIDTH / 2;
			stage[i][j][0].y = STAGE_UP + BLOCK_HEIGHT * j + BLOCK_HEIGHT / 2;
			stage[i][j][1].x = STAGE2_LEFT + BLOCK_WIDTH * i + BLOCK_WIDTH / 2;
			stage[i][j][1].y = STAGE2_UP + BLOCK_HEIGHT * j + BLOCK_HEIGHT / 2;
			stage[i][j][0].block.dropcount = 0;
			stage[i][j][1].block.dropcount = 0;
			stage[i][j][0].block.moveflag = false;
			stage[i][j][1].block.moveflag = false;
			stage[i][j][0].block.sunderalpha = 0;
			stage[i][j][0].block.sunderflag = false;
			stage[i][j][0].block.blockalpha = 255;
			stage[i][j][1].block.sunderalpha = 0;
			stage[i][j][1].block.sunderflag = false;
			stage[i][j][1].block.blockalpha = 255;
		}
	}

	player_info[0].BOXMOZI_X = STAGE_LEFT + STAGE_SIZE_X / 2 + 5.f;
	player_info[1].BOXMOZI_X = STAGE2_LEFT + STAGE_SIZE_X / 2 + 5.f;
	player_info[0].BOXMOZI_Y = player_info[1].BOXMOZI_Y = STAGE_UP + STAGE_SIZE_Y + 15.f;
	player_info[0].fPlayer_X = CHARA_LEFT + CHARA_SIZE_X/2;
	player_info[0].fPlayer_Y = CHARA_UP + CHARA_SIZE_Y/2;
	player_info[1].fPlayer_X = CHARA2_LEFT + CHARA_SIZE_X/2;
	player_info[1].fPlayer_Y = CHARA2_UP + CHARA_SIZE_Y/2;
	player_info[0].fCursor_X = STAGE_LEFT +  BLOCK_WIDTH / 2;
	player_info[0].fCursor_Y = STAGE_UP +  BLOCK_HEIGHT / 2;
	player_info[1].fCursor_X = STAGE2_LEFT +  BLOCK_WIDTH / 2;
	player_info[1].fCursor_Y = STAGE2_UP +  BLOCK_HEIGHT / 2;
	player_info[0].BOXPOS_X = STAGE_LEFT + STAGE_SIZE_X/2;
	player_info[0].BOXPOS_Y = STAGE_UP + STAGE_SIZE_Y/2;
	player_info[1].BOXPOS_X = STAGE2_LEFT + STAGE_SIZE_X/2;
	player_info[1].BOXPOS_Y = STAGE2_UP + STAGE_SIZE_Y/2;
	for( int i = 0 ; i < 2 ; i++ )
	{
		player_info[i].HPreduceflag = false;
		player_info[i].goingHP = HP_MAX;
		player_info[i].difficult = 1;
		player_info[i].chain = 0;
		player_info[i].iCursor[0] = 0;
		player_info[i].iCursor[1] = 0;
		player_info[i].fHp = HP_MAX;
		player_info[i].charatype = GetPlayerChar(i);
		player_info[i].charastate = 0;
		player_info[i].animestep = 0;
		player_info[i].animewidth = 0;
		player_info[i].nextset = 1;
		player_info[i].bAttackflag = false;
		player_info[i].blockchangeflag = false;
		if( player_info[i].charatype == 0 )
		{
			player_info[i].charatype = CHARA_GIRL;
		}
		else if( player_info[i].charatype == 1 )
		{
			player_info[i].charatype = CHARA_BOY;
		}
		else if( player_info[i].charatype == 2 )
		{
			player_info[i].charatype = CHARA_ROBO;
		}

		int pp = i == 0 ? 1 : 0;
		AttackEffect[i].flag = false;
		AttackEffect[i].Pos.x = (CHARA2_LEFT - CHARA_LEFT ) * i + CHARA_LEFT + CHARA_SIZE_X / 2;
		AttackEffect[i].Pos.y = CHARA_UP + CHARA_SIZE_Y / 2;
		AttackEffect[i].Target.x = (CHARA2_LEFT - CHARA_LEFT ) * pp + CHARA_LEFT + CHARA_SIZE_X / 2;
		AttackEffect[i].Target.y = CHARA_UP + CHARA_SIZE_Y / 2;
		AttackEffect[i].count = 0 ;

		ThunderFlag[i] = false;

		DropFlag[i] = false;
	}
	CountDown = 0 ;
	CountDounFlag = false;

	SoundPlay(BGM_01, true);
	for( int i = 0 ; i < 2 ; i++ )
	{
		CheckAttack(i,false,false);
	}
}
//パズル画面処理
void PuzzleControl()
{
	if( !CountDounFlag ){
		CountDown++;
		if(CountDown>=240){
			CountDounFlag = true;
		}
	}

	bool _moveflag[2] ;
	_moveflag[0] = _moveflag[1] = false;

	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			for( int k = 0 ; k < 2 ; k++ )
			{
				if( stage[i][j][k].block.moveflag == true )
				{
					_moveflag[k] = true;
				}
			}
		}
	}
	//---------------------------------------------------------
	//放電中か確認
	for( int i = 0 ; i < 2 ; i++ )
	{
		player_info[i].bAttackflag = false;
	}
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			for( int k = 0 ; k < 2 ; k++ )
			{
				if( stage[i][j][k].block.sunderflag == true )
				{
					player_info[k].bAttackflag = true;
				}
			}
		}
	}
	//----------------------------------------------------------
	for( int i = 0 ; i < 2 ; i++ )
	{
		if( charaupdownflag == true )
		{
			player_info[i].fPlayer_Y -= 0.5f;
		}
		if( charaupdownflag == false )
		{
			player_info[i].fPlayer_Y += 0.5f;
		}
	}
	if(player_info[0].fPlayer_Y > CHARA_UP + CHARA_SIZE_Y/2 + 4.f)
	{
		charaupdownflag = true;
	}
	if(player_info[0].fPlayer_Y < CHARA_UP + CHARA_SIZE_Y/2 - 4.f)
	{
		charaupdownflag = false;
	}
	//---------------------------------------------------------
	//ブロック回転
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			for( int k = 0 ; k < 2 ; k++ )
			{
				if( stage[i][j][k].block.turnflag == true )
				{
					if(stage[i][j][k].block.turndirection == false)
					{
						stage[i][j][k].block.angle -= TURNSPEED;
						if( stage[i][j][k].block.nextturn != 3 )
						{
							if( stage[i][j][k].block.nextturn * 90 > stage[i][j][k].block.angle )
							{
								stage[i][j][k].block.angle = stage[i][j][k].block.nextturn * 90.f;
								stage[i][j][k].block.direction = stage[i][j][k].block.nextturn;
								stage[i][j][k].block.turnflag = false;
								CheckAttack( k,false ,false);
							}
						}
						else
						{
							if( stage[i][j][k].block.angle <= 0.f )
							{
								stage[i][j][k].block.angle = 360.f;
							}
							if( stage[i][j][k].block.nextturn * 90 > stage[i][j][k].block.angle )
							{
								stage[i][j][k].block.angle = stage[i][j][k].block.nextturn * 90.f;
								stage[i][j][k].block.direction = stage[i][j][k].block.nextturn;
								stage[i][j][k].block.turnflag = false;
								CheckAttack( k,false ,false);
							}
						}
					}
					if(stage[i][j][k].block.turndirection == true)
					{
						stage[i][j][k].block.angle += TURNSPEED;
						if( stage[i][j][k].block.nextturn != 0 )
						{
							if( stage[i][j][k].block.nextturn * 90 < stage[i][j][k].block.angle )
							{
								stage[i][j][k].block.angle = stage[i][j][k].block.nextturn * 90.f;
								stage[i][j][k].block.direction = stage[i][j][k].block.nextturn;
								stage[i][j][k].block.turnflag = false;
								CheckAttack( k,false ,false);
							}
						}
						else
						{
							if( stage[i][j][k].block.angle >= 360.f )
							{
								stage[i][j][k].block.angle = 0.f;
								stage[i][j][k].block.angle = stage[i][j][k].block.nextturn * 90.f;
								stage[i][j][k].block.direction = stage[i][j][k].block.nextturn;
								stage[i][j][k].block.turnflag = false;
								CheckAttack( k,false ,false);
							}
						}
					}
				}
			}
		}
	}
	//ブロック回転中か確認
	player_info[0].turnflag = player_info[1].turnflag = false;
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			for( int k = 0 ; k < 2 ; k++ )
			{
				if( stage[i][j][k].block.turnflag == true )
				{
					player_info[k].turnflag = true;
				}
			}
		}
	}
	//----------------------------------------------------------
	//-キー入力処理
	// 1p
	if( CountDounFlag )
	if( _moveflag[0] == false && player_info[0].bAttackflag == false && player_info[0].blockchangeflag == 0 && player_info[0].turnflag == false && player_info[0].cursormoveflag == false)
	{
		if ( GetCrossKey(0,CROSS_RIGHT,PUSH) || StCheck ( KEY_D, PUSH ) )
		{
			if( player_info[0].iCursor[0] < STAGE_WIDTH-1 )
			{
				player_info[0].iGocursor[0] = player_info[0].iCursor[0]+1;
				player_info[0].cursordirection = 1;
				player_info[0].cursormoveflag = true;
				SoundPlay(BGM_02, false);
			}
		}
		if ( GetCrossKey(0,CROSS_LEFT,PUSH) || StCheck ( KEY_A, PUSH ) )
		{
			if( player_info[0].iCursor[0] > 0 )
			{
				player_info[0].iGocursor[0] = player_info[0].iCursor[0]-1;
				player_info[0].cursordirection = 3;
				player_info[0].cursormoveflag = true;
				SoundPlay(BGM_02, false);
			}
		}
		if ( GetCrossKey(0,CROSS_UP,PUSH) || StCheck ( KEY_W, PUSH ) )
		{
			if( player_info[0].iCursor[1] > 0 )
			{
				player_info[0].iGocursor[1] = player_info[0].iCursor[1]-1;
				player_info[0].cursordirection = 0;
				player_info[0].cursormoveflag = true;
				SoundPlay(BGM_02, false);
			}
		}
		if ( GetCrossKey(0,CROSS_DOWN,PUSH) || StCheck ( KEY_S, PUSH ) )
		{
			if( player_info[0].iCursor[1] < STAGE_HEIGHT-1 )
			{
				player_info[0].iGocursor[1] = player_info[0].iCursor[1]+1;
				player_info[0].cursordirection = 2;
				player_info[0].cursormoveflag = true;
				SoundPlay(BGM_02, false);
			}
		}
		if ( GetButton(0,BUTTON_A,PUSH) || StCheck ( KEY_Z, PUSH ) )
		{
			if( stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.direction > 0 )
			{
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.turnflag = true;
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.nextturn = stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.direction - 1;
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.turndirection = false;
			}
			else
			{
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.turnflag = true;
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.nextturn = 3;
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.turndirection = false;
			}
			SoundPlay(BGM_03, false);
		}
		if ( GetButton(0,BUTTON_B,PUSH) || StCheck ( KEY_X, PUSH ) )
		{
			if( stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.direction < 3 )
			{
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.turnflag = true;
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.nextturn = stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.direction + 1;
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.turndirection = true;
			}
			else
			{
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.turnflag = true;
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.nextturn = 0;
				stage[player_info[0].iCursor[0]][player_info[0].iCursor[1]][0].block.turndirection = true;
			}
			SoundPlay(BGM_03, false);
			
		}
		if (  GetButton(0,BUTTON_Y,PUSH) || StCheck ( KEY_C, PUSH ) )
		{
			BlockReset(0);
		}
		if ( GetButton(0,BUTTON_X,PUSH) || StCheck ( KEY_V , PUSH ) )
		{
			CheckAttack( 0,false,true );
		}
	}
	//2p
	if( CountDounFlag )
	if( _moveflag[1] == false && player_info[1].bAttackflag == false && player_info[1].blockchangeflag == 0 && player_info[1].turnflag == false && player_info[1].cursormoveflag == false)
	{
		if (StCheck ( RIGHT, PUSH ) ||  GetCrossKey(1,CROSS_RIGHT,PUSH) )
		{
			if( player_info[1].iCursor[0] < STAGE_WIDTH-1 )
			{	
				player_info[1].iGocursor[0] = player_info[1].iCursor[0]+1;
				player_info[1].cursordirection = 1;
				player_info[1].cursormoveflag = true;
				SoundPlay(BGM_02, false);
			}
		}
		if (  StCheck ( LEFT, PUSH ) || GetCrossKey(1,CROSS_LEFT,PUSH) )
		{
			if( player_info[1].iCursor[0] > 0 )
			{
				player_info[1].iGocursor[0] = player_info[1].iCursor[0]-1;
				player_info[1].cursordirection = 3;
				player_info[1].cursormoveflag = true;
				SoundPlay(BGM_02, false);
			}
		}
		if ( StCheck ( UP, PUSH ) || GetCrossKey(1,CROSS_UP,PUSH) )
		{
			if( player_info[1].iCursor[1] > 0 )
			{
				player_info[1].iGocursor[1] = player_info[1].iCursor[1]-1;
				player_info[1].cursordirection = 0;
				player_info[1].cursormoveflag = true;
				SoundPlay(BGM_02, false);
			}
		}
		if ( StCheck ( DOWN, PUSH ) || GetCrossKey(1,CROSS_DOWN,PUSH) )
		{
			if( player_info[1].iCursor[1] < STAGE_HEIGHT-1 )
			{
				player_info[1].iGocursor[1] = player_info[1].iCursor[1]+1;
				player_info[1].cursordirection = 2;
				player_info[1].cursormoveflag = true;
				SoundPlay(BGM_02, false);
			}
		}
		if ( StCheck ( KEY_K, PUSH ) ||GetButton(1,BUTTON_A,PUSH)  )
		{
			if( stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.direction > 0 )
			{
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.turnflag = true;
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.nextturn = stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.direction - 1;
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.turndirection = false;
			}
			else
			{
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.turnflag = true;
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.nextturn = 3;
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.turndirection = false;
			}
			SoundPlay(BGM_03, false);
		}
		if ( StCheck ( KEY_L, PUSH ) ||  GetButton(1,BUTTON_B,PUSH)  )
		{
			if( stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.direction < 3 )
			{
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.turnflag = true;
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.nextturn = stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.direction + 1;
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.turndirection = true;
			}
			else
			{
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.turnflag = true;
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.nextturn = 0;
				stage[player_info[1].iCursor[0]][player_info[1].iCursor[1]][1].block.turndirection = true;
			}
			SoundPlay(BGM_03, false);
		}
		if (StCheck ( KEY_M, PUSH ) ||  GetButton(1,BUTTON_Y,PUSH)  )
		{
			BlockReset(1);
		}
		if (StCheck ( KEY_O, PUSH )|| GetButton(1,BUTTON_X,PUSH)  )
		{
			CheckAttack( 1,false,true );
		
		}
	}
	ChangeAnime();
	//--------------------------------------------------------------------
	//電流アニメ
	bool _sunderflag[2];
	for( int i = 0 ; i < 2 ; i++ )
	{
		_sunderflag[i] = false;
	}
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_WIDTH ; j++ )
		{
			for( int k = 0 ; k < 2 ; k++ )
			{
				if( stage[i][j][k].block.sunderflag == true )
				{
					_sunderflag[k] = true;
				}
			}
		}
	}
	if( _sunderflag[0] == true )
	{
		for( int i = 0 ; i < STAGE_WIDTH ; i++ )
		{
			for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
			{
				if( stage[i][j][0].block.sunderflag == true )
				{
					stage[i][j][0].block.sundercount--;

					if( stage[i][j][0].block.sundercount <= 0 )
					{
						if( stage[i][j][0].block.sunderalpha > 0 )
						{
							stage[i][j][0].block.blockalpha -= SUNDERDEL;
							stage[i][j][0].block.sunderalpha-= SUNDERDEL;
							if( stage[i][j][0].block.sunderalpha <= 0 )
							{
								stage[i][j][0].block.runflag = false;
								stage[i][j][0].block.sunderflag = false;
							}
						}
					}
				}
			}
		}
		for( int i = 0 ; i < STAGE_WIDTH ; i++ )
		{
			for( int j = 0 ; j < STAGE_WIDTH ; j++ )
			{
				if( stage[i][j][0].block.sunderflag == true )
				{
					_sunderflag[0] = false;
				}
			}
		}
		if(_sunderflag[0] == true )
		{
			player_info[1].HPreduceflag = true;
			player_info[1].goingHP -= player_info[0].givedamage;

			if(_sunderflag[1] == false)
			{
				SoundStop(BGM_04);
			}
			if(player_info[1].animestep == 0)
			{
				player_info[1].goinganime = 2;
				player_info[1].animestep = 1;
			}
			if(BlockDrop(0) == false)
			{
				BlockReset(0);
			}
		}
	}
	if( _sunderflag[1] == true )
	{
		for( int i = 0 ; i < STAGE_WIDTH ; i++ )
		{
			for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
			{
				if( stage[i][j][1].block.sunderflag == true )
				{

					stage[i][j][1].block.sundercount--;
					
					if( stage[i][j][1].block.sundercount <= 0 )
					{
						if( stage[i][j][1].block.sunderalpha > 0 )
						{
							stage[i][j][1].block.blockalpha -= SUNDERDEL;
							stage[i][j][1].block.sunderalpha-= SUNDERDEL;
							if( stage[i][j][1].block.sunderalpha <= 0 )
							{
								stage[i][j][1].block.runflag = false;
								stage[i][j][1].block.sunderflag = false;
							}
						}
					}
				}
			}
		}
		for( int i = 0 ; i < STAGE_WIDTH ; i++ )
		{
			for( int j = 0 ; j < STAGE_WIDTH ; j++ )
			{
				if( stage[i][j][1].block.sunderflag == true )
				{
					_sunderflag[1] = false;
				}
			}
		}
		if(_sunderflag[1] == true )
		{
			bool thundflag = false;
			for( int iii = 0 ; iii < STAGE_WIDTH ; iii++ )
			{
				for( int jjj = 0 ; jjj < STAGE_HEIGHT ; jjj++ )
				{
					if(stage[iii][jjj][0].block.sunderflag == true )
					{
						thundflag = true;
					}
				}
			}
			if(thundflag == false)
			{
				SoundStop(BGM_04);
			}
			player_info[0].HPreduceflag = true;
			player_info[0].goingHP -= player_info[1].givedamage;

			if(player_info[0].animestep == 0)
			{
				player_info[0].goinganime = 2;
				player_info[0].animestep = 1;
			}
			if(BlockDrop(1) == false)
			{
				BlockReset(1);
			}
		}
	}
	//-------------------------------------------------------------------------
	//ブロック落下アニメ
	bool _dropflag[2];
	_dropflag[0] = _dropflag[1] = false;
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			for( int k = 0 ; k < 2 ; k++ )
			{
				if( stage[i][j][k].block.moveflag == true )
				{
					stage[i][j][k].y += DROPSPEED;
					if( stage[i][j][k].block.goalpos.y < stage[i][j][k].y )
					{
						stage[i][j][k].y = stage[i][j][k].block.goalpos.y;
						stage[i][j][k].block.moveflag = false;
						_dropflag[k] = true;
						DropFlag[k] = false;
					}
				}
			}
		}
	}
	for( int ii = 0 ; ii < 2 ; ii++ )
	{
		if( _dropflag[ii] == true )
		{
			for( int i = 0 ; i < STAGE_WIDTH ; i++ )
			{
				for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
				{
					if(stage[i][j][ii].block.moveflag == true)
					{
						_dropflag[ii] = false;
					}
				}
			}
		}
		if( _dropflag[ii] == true )
		{
			if(CheckAttack(ii,true,true) == false)
			{
				BlockReset(ii);
			}
		}
	}
	//-------------------------------------------------------------------------
	//ＨＰ減るアニメ
	for( int i = 0 ; i < 2 ; i++ )
	{
		if( player_info[i].HPreduceflag == true )
		{
			player_info[i].fHp -= HPREDUCESPEED;
			if(player_info[i].fHp < player_info[i].goingHP )
			{
				player_info[i].fHp = player_info[i].goingHP;
				player_info[i].HPreduceflag = false;
			}
		}
	}
	//--------------------------------------------------------------------------------
	bool	Alldelflag[2];
	Alldelflag[0] = Alldelflag[1] = true;
	for( int kk = 0 ; kk < 2 ; kk++ )
	{
		for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
		{
			for( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
			{
			
				if(stage[ii][jj][kk].block.runflag == true  )
				{
					Alldelflag[kk] = false;
					break;
				}
			}
		}
	}
	for( int kk = 0 ; kk < 2 ; kk++ )
	{
		for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
		{
			for( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
			{
			
				if(addblock[ii][jj][kk].block.runflag == true && addblocknum[ii][kk] < jj  )
				{
					Alldelflag[kk] = false;
					break;
				}
			}
		}
	}
	for( int ii = 0 ; ii < 2 ; ii++ )
	{
		if( Alldelflag[ii] == true )
		{
			BlockReset(ii);
		}
	}
	//-----------キャラアニメ
	for( int i = 0 ; i < 2 ; i++ )
	{
		if( player_info[i].animestep == 1 )
		{
			player_info[i].animewidth += 26.f;
			if( player_info[i].animewidth >= CHARA_SIZE_X )
			{
				player_info[i].animewidth = CHARA_SIZE_X;
				player_info[i].animestep = 2;
				player_info[i].charastate = player_info[i].goinganime;

				//	追加日　2/27	亀田
				iWordsPattern = rand()%2;
			}
		}
		else if( player_info[i].animestep == 2 )
		{
			player_info[i].animewidth -= 26.f;
			if( player_info[i].animewidth <= 0.f )
			{
				player_info[i].animewidth = 0.f;
				player_info[i].animestep = 3;
				player_info[i].animetime = 60;
			}
		}
		else if( player_info[i].animestep == 3 )
		{
			player_info[i].animetime--;
			if( player_info[i].animetime <= 0 )
			{
				player_info[i].animestep = 4;
			}
		}
		else if( player_info[i].animestep == 4 )
		{
			player_info[i].animewidth += 26.f;
			if( player_info[i].animewidth >= CHARA_SIZE_X )
			{
				player_info[i].animewidth = CHARA_SIZE_X;
				player_info[i].animestep = 5;
				player_info[i].charastate = 0;
			}
		}
		else if( player_info[i].animestep == 5 )
		{
			player_info[i].animewidth -= 26.f;
			if( player_info[i].animewidth <= 0.f )
			{
				player_info[i].animewidth = 0.f;
				player_info[i].animestep = 0;
			}
		}
	}
	//-------------------------------------------------------------------------
	//カーソルの座標更新
	
	if( player_info[0].cursormoveflag == true )
	{
		if( player_info[0].cursordirection == 0 )
		{
			player_info[0].fCursor_Y -= CURSORSPEED;
			if(player_info[0].fCursor_Y < STAGE_UP + player_info[0].iGocursor[1] * BLOCK_HEIGHT + BLOCK_HEIGHT / 2 )
			{
				player_info[0].fCursor_Y = STAGE_UP + player_info[0].iGocursor[1] * BLOCK_HEIGHT + BLOCK_HEIGHT / 2;
				player_info[0].cursormoveflag = false;
				player_info[0].iCursor[1] = player_info[0].iGocursor[1];
			}
		}
		if( player_info[0].cursordirection == 1 )
		{
			player_info[0].fCursor_X += CURSORSPEED;
			if(player_info[0].fCursor_X > STAGE_LEFT + player_info[0].iGocursor[0] * BLOCK_WIDTH + BLOCK_WIDTH / 2 )
			{
				player_info[0].fCursor_X = STAGE_LEFT + player_info[0].iGocursor[0] * BLOCK_WIDTH + BLOCK_WIDTH / 2;
				player_info[0].cursormoveflag = false;
				player_info[0].iCursor[0] = player_info[0].iGocursor[0];
			}
		}
		if( player_info[0].cursordirection == 2 )
		{
			player_info[0].fCursor_Y += CURSORSPEED;
			if(player_info[0].fCursor_Y > STAGE_UP + player_info[0].iGocursor[1] * BLOCK_HEIGHT + BLOCK_HEIGHT / 2 )
			{
				player_info[0].fCursor_Y = STAGE_UP + player_info[0].iGocursor[1] * BLOCK_HEIGHT + BLOCK_HEIGHT / 2;
				player_info[0].cursormoveflag = false;
				player_info[0].iCursor[1] = player_info[0].iGocursor[1];
			}
		}
		if( player_info[0].cursordirection == 3 )
		{
			player_info[0].fCursor_X -= CURSORSPEED;
			if(player_info[0].fCursor_X < STAGE_LEFT + player_info[0].iGocursor[0] * BLOCK_WIDTH + BLOCK_WIDTH / 2 )
			{
				player_info[0].fCursor_X = STAGE_LEFT + player_info[0].iGocursor[0] * BLOCK_WIDTH + BLOCK_WIDTH / 2;
				player_info[0].cursormoveflag = false;
				player_info[0].iCursor[0] = player_info[0].iGocursor[0];
			}
		}
	}
	if( player_info[1].cursormoveflag == true )
	{
		if( player_info[1].cursordirection == 0 )
		{
			player_info[1].fCursor_Y -= CURSORSPEED;
			if(player_info[1].fCursor_Y < STAGE2_UP + player_info[1].iGocursor[1] * BLOCK_HEIGHT + BLOCK_HEIGHT / 2 )
			{
				player_info[1].fCursor_Y = STAGE2_UP + player_info[1].iGocursor[1] * BLOCK_HEIGHT + BLOCK_HEIGHT / 2;
				player_info[1].cursormoveflag = false;
				player_info[1].iCursor[1] = player_info[1].iGocursor[1];
			}
		}
		if( player_info[1].cursordirection == 1 )
		{
			player_info[1].fCursor_X += CURSORSPEED;
			if(player_info[1].fCursor_X > STAGE2_LEFT + player_info[1].iGocursor[0] * BLOCK_WIDTH + BLOCK_WIDTH / 2 )
			{
				player_info[1].fCursor_X = STAGE2_LEFT + player_info[1].iGocursor[0] * BLOCK_WIDTH + BLOCK_WIDTH / 2;
				player_info[1].cursormoveflag = false;
				player_info[1].iCursor[0] = player_info[1].iGocursor[0];
			}
		}
		if( player_info[1].cursordirection == 2 )
		{
			player_info[1].fCursor_Y += CURSORSPEED;
			if(player_info[1].fCursor_Y > STAGE2_UP + player_info[1].iGocursor[1] * BLOCK_HEIGHT + BLOCK_HEIGHT / 2 )
			{
				player_info[1].fCursor_Y = STAGE2_UP + player_info[1].iGocursor[1] * BLOCK_HEIGHT + BLOCK_HEIGHT / 2;
				player_info[1].cursormoveflag = false;
				player_info[1].iCursor[1] = player_info[1].iGocursor[1];
			}
		}
		if( player_info[1].cursordirection == 3 )
		{
			player_info[1].fCursor_X -= CURSORSPEED;
			if(player_info[1].fCursor_X < STAGE2_LEFT + player_info[1].iGocursor[0] * BLOCK_WIDTH + BLOCK_WIDTH / 2 )
			{
				player_info[1].fCursor_X = STAGE2_LEFT + player_info[1].iGocursor[0] * BLOCK_WIDTH + BLOCK_WIDTH / 2;
				player_info[1].cursormoveflag = false;
				player_info[1].iCursor[0] = player_info[1].iGocursor[0];
			}
		}
	}
	

	//ブロックの向きによって角度を変更　きれいに回すためには用調整
	
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			for( int k = 0 ; k < 2 ; k++ )
			{
				if( player_info[k].turnflag == false)
				{
					switch(stage[i][j][k].block.direction)
					{
						case 0:
							stage[i][j][k].block.angle = 0.f;
							break;
						case 1:
							stage[i][j][k].block.angle = 90.f;
							break;
						case 2:
							stage[i][j][k].block.angle = 180.f;
							break;
						case 3:
							stage[i][j][k].block.angle = 270.f;
							break;
					}
					switch(addblock[i][j][k].block.direction)
					{
						case 0:
							addblock[i][j][k].block.angle = 0.f;
							break;
						case 1:
							addblock[i][j][k].block.angle = 90.f;
							break;
						case 2:
							addblock[i][j][k].block.angle = 180.f;
							break;
						case 3:
							addblock[i][j][k].block.angle = 270.f;
							break;
					}
				}
			}
		}
	}
}
void PuzzleDraw()
{
	// 背景描画
	CenterPointDraw ( PUZZLE_BACK, WIDTH / 2, HEIGHT / 2, 0.f, WIDTH, HEIGHT,
					0.f, 0.f, 1.f , 1.f );
	

	CUSTOMVERTEX _Mver[4];
	int x = STAGE_WIDTH - 1, y = 0;
	
	//輪
	for( int i = 0 ; i < 2 ; i++ ){
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ ){
			if( player_info[i].bAttackflag )
			{
				ThunderFlag[i] = true;
				y = j;
			}
			if( stage[x][j][i].block.sunderflag && !AttackEffect[i].flag )
			{
				AttackEffect[i].flag = true;
				AttackEffect[i].count = 0;
				AttackEffect[i].Speed = (AttackEffect[i].Target - AttackEffect[i].Pos ) / ( 60 + 255 / SUNDERDEL );
			}
		}
		if( ThunderFlag[i] ) {
			CUSTOMVERTEX _Mver[4];
			float cpx = (CHARA2_LEFT - CHARA_LEFT ) * i + CHARA_LEFT + CHARA_SIZE_X / 2 ;
			float cpy = CHARA_UP + CHARA_SIZE_Y / 2;
			float cpw = CHARA_SIZE_X * ( (float)stage[x][y][i].block.sundercount - 60.f ) / 60.f;
			float cph = CHARA_SIZE_X * ( (float)stage[x][y][i].block.sundercount - 60.f ) / 60.f;

			for(int k = 0 ; k < MAJIC_MAX ; k++){ 
				cpx += MLen[k][0];
				cpy += MLen[k][1];
				cpw = CHARA_SIZE_X * ( (float)stage[x][y][i].block.sundercount - 60.f ) / 60.f * MAJIC_SCALE;
				cph = CHARA_SIZE_X * ( (float)stage[x][y][i].block.sundercount - 60.f ) / 60.f * MAJIC_SCALE;
				
				SetVertex( MARU , _Mver , cpx , cpy , cpw,	cph);
				Rotate( _Mver , cpx , cpy , (float)stage[x][y][i].block.sundercount * MAJIC_SPEED, cpw,cph );
				ColorKey( _Mver , 255 - rand()%50 , rand()%255 ,rand()%255,rand()%255 );
				Draw(MARU,_Mver);
			}
		}
		//攻撃
		if( AttackEffect[i].flag ){
			for(int a = 0 ; a < 3 ; a++ ){
				ColorKeyDraw( ATTACK_THUNDER , 
					AttackEffect[i].Pos.x + AttackEffect[i].Speed.x * AttackEffect[i].count ,
					AttackEffect[i].Pos.y + AttackEffect[i].Speed.y * AttackEffect[i].count ,
					(float)(rand()%4) * 90,
					300.f,300.f,
					0.f,0.f,1.f,1.f,
					255 - rand()%50 , 255 ,255,255);
			}

			AttackEffect[i].count++;
			if(AttackEffect[i].count > ( 60 + 255 / SUNDERDEL ) ){
				AttackEffect[i].flag = false;
				AttackEffect[i].count = 0;
				ThunderFlag[i] = false;
			}
		}
	}

	//キャラ描画
	CenterPointDraw ( player_info[0].charatype, player_info[0].fPlayer_X, player_info[0].fPlayer_Y, 0.f, CHARA_SIZE_X-player_info[0].animewidth, CHARA_SIZE_Y,
					0.f + 1.0f / 3.f * player_info[0].charastate, 0.f, 1.0f / 3.f + 1.f / 3.f * player_info[0].charastate, 1.f );
	CenterPointDraw ( player_info[1].charatype, player_info[1].fPlayer_X, player_info[1].fPlayer_Y, 0.f, CHARA_SIZE_X-player_info[1].animewidth, CHARA_SIZE_Y,
					1.0f / 3.f + 1.0f / 3.f * player_info[1].charastate, 0.f, 1.f / 3.f * player_info[1].charastate, 1.f );

	//ライフバー描画
	CustomDraw ( LIFE_MAX,HPGAUGE_LEFT , HPGAUGE_UP, HPGAUGE_LEFT + HPGAUGE_WIDTH / HP_MAX * player_info[0].fHp, HPGAUGE_UP + HPGAUGE_HEIGHT,
				1.f, 0.f, (HP_MAX - player_info[0].fHp)/HP_MAX, 1.f );
	CustomDraw ( LIFE_MAX,HPGAUGE2_LEFT + HPGAUGE_WIDTH / HP_MAX * (HP_MAX - player_info[1].fHp) , HPGAUGE_UP, HPGAUGE2_LEFT + HPGAUGE_WIDTH , HPGAUGE_UP + HPGAUGE_HEIGHT,
				(HP_MAX - player_info[1].fHp)/HP_MAX, 0.f, 1.f, 1.f );

	CenterPointDraw ( LIFE_MIN, HPGAUGE_LEFT + HPGAUGE_WIDTH / 2, HPGAUGE_UP + HPGAUGE_HEIGHT / 2, 180.f, HPGAUGE_WIDTH, HPGAUGE_HEIGHT,
					0.f, 0.f, 1.f, 1.f );
	CenterPointDraw ( LIFE_MIN, HPGAUGE2_LEFT + HPGAUGE_WIDTH / 2, HPGAUGE2_UP + HPGAUGE_HEIGHT / 2, 0.f, HPGAUGE_WIDTH, HPGAUGE_HEIGHT,
					0.f, 0.f, 1.f, 1.f );
	if(player_info[0].fHp >= HP_MAX / 2 )
	{
		CustomCorolDraw ( LIFE_MAX, HPGAUGE_LEFT, HPGAUGE_UP,  HPGAUGE_LEFT + HPGAUGE_WIDTH / HP_MAX * player_info[0].fHp,HPGAUGE_UP + HPGAUGE_HEIGHT,
					1.f, 0.f, (HP_MAX - player_info[0].fHp)/HP_MAX, 1.f , 255, 0, 255, 0 );
	}
	if(player_info[1].fHp >= HP_MAX / 2 )
	{
		CustomCorolDraw ( LIFE_MAX,HPGAUGE2_LEFT + HPGAUGE_WIDTH / HP_MAX * (HP_MAX - player_info[1].fHp) , HPGAUGE_UP, HPGAUGE2_LEFT + HPGAUGE_WIDTH , HPGAUGE_UP + HPGAUGE_HEIGHT,
				(HP_MAX - player_info[1].fHp)/HP_MAX, 0.f, 1.f, 1.f , 255, 0, 255, 0 );
	}

	if(player_info[0].fHp < HP_MAX / 2 && player_info[0].fHp >= HP_MAX / 4 )
	{
		CustomCorolDraw ( LIFE_MAX, HPGAUGE_LEFT, HPGAUGE_UP,  HPGAUGE_LEFT + HPGAUGE_WIDTH / HP_MAX * player_info[0].fHp,HPGAUGE_UP + HPGAUGE_HEIGHT,
					1.f, 0.f, (HP_MAX - player_info[0].fHp)/HP_MAX, 1.f , 255, 255, 255, 0 );
	}
	if(player_info[1].fHp < HP_MAX / 2 && player_info[1].fHp >= HP_MAX / 4 )
	{
		CustomCorolDraw ( LIFE_MAX,HPGAUGE2_LEFT + HPGAUGE_WIDTH / HP_MAX * (HP_MAX - player_info[1].fHp) , HPGAUGE_UP, HPGAUGE2_LEFT + HPGAUGE_WIDTH , HPGAUGE_UP + HPGAUGE_HEIGHT,
				(HP_MAX - player_info[1].fHp)/HP_MAX, 0.f, 1.f, 1.f , 255, 255, 255, 0 );
	}

	if(player_info[0].fHp < HP_MAX / 4 )
	{
		CustomCorolDraw ( LIFE_MAX, HPGAUGE_LEFT, HPGAUGE_UP,  HPGAUGE_LEFT + HPGAUGE_WIDTH / HP_MAX * player_info[0].fHp,HPGAUGE_UP + HPGAUGE_HEIGHT,
					1.f, 0.f, (HP_MAX - player_info[0].fHp)/HP_MAX, 1.f , 255, 255, 0, 0 );
	}
	if(player_info[1].fHp < HP_MAX / 4 )
	{
		CustomCorolDraw ( LIFE_MAX,HPGAUGE2_LEFT + HPGAUGE_WIDTH / HP_MAX * (HP_MAX - player_info[1].fHp) , HPGAUGE_UP, HPGAUGE2_LEFT + HPGAUGE_WIDTH , HPGAUGE_UP + HPGAUGE_HEIGHT,
				(HP_MAX - player_info[1].fHp)/HP_MAX, 0.f, 1.f, 1.f , 255, 255, 0, 0 );
	}

	
	
	//1pブロック描画
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			if( stage[i][j][0].block.runflag == true )
			{
				
				if( stage[i][j][0].block.lightline == false )
				{
					ColorKeyDraw ( BLOCK_SET, stage[i][j][0].x, stage[i][j][0].y, stage[i][j][0].block.angle, BLOCK_WIDTH, BLOCK_HEIGHT,
						stage[i][j][0].block.type * 1.f / B_TYPEMAX, 0.f, stage[i][j][0].block.type * 1.f / B_TYPEMAX + 1.f / B_TYPEMAX, 1.f ,stage[i][j][0].block.blockalpha,155,155,155);
				}
				if( stage[i][j][0].block.lightline == true || stage[i][j][0].block.sunderflag == true )
				{
					ColorKeyDraw ( BLOCK_SET, stage[i][j][0].x, stage[i][j][0].y, stage[i][j][0].block.angle, BLOCK_WIDTH, BLOCK_HEIGHT,
						stage[i][j][0].block.type * 1.f / B_TYPEMAX, 0.f, stage[i][j][0].block.type * 1.f / B_TYPEMAX + 1.f / B_TYPEMAX, 1.f ,stage[i][j][0].block.blockalpha,255,255,255);
				}
				if( stage[i][j][0].block.sunderflag == true )
				{
					ColorKeyDraw ( THUNDER, stage[i][j][0].x, stage[i][j][0].y, (float)(rand()%4) * 90, BLOCK_WIDTH, BLOCK_HEIGHT,
					0.f,0.f,1.f, 1.f ,stage[i][j][0].block.sunderalpha,255,255,255);
				}
			}
		}
	}
	//2pブロック描画
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			if( stage[i][j][1].block.runflag == true )
			{
				
				if( stage[i][j][1].block.lightline == false )
				{
					ColorKeyDraw ( BLOCK_SET, stage[i][j][1].x, stage[i][j][1].y, stage[i][j][1].block.angle, BLOCK_WIDTH, BLOCK_HEIGHT,
						stage[i][j][1].block.type * 1.f / B_TYPEMAX, 0.f, stage[i][j][1].block.type * 1.f / B_TYPEMAX + 1.f / B_TYPEMAX, 1.f ,stage[i][j][1].block.blockalpha,155,155,155);
				}
				if( stage[i][j][1].block.lightline == true||stage[i][j][1].block.sunderflag == true )
				{
					ColorKeyDraw ( BLOCK_SET, stage[i][j][1].x, stage[i][j][1].y, stage[i][j][1].block.angle, BLOCK_WIDTH, BLOCK_HEIGHT,
						stage[i][j][1].block.type * 1.f / B_TYPEMAX, 0.f, stage[i][j][1].block.type * 1.f / B_TYPEMAX + 1.f / B_TYPEMAX, 1.f ,stage[i][j][1].block.blockalpha,255,255,255);
				}
				if( stage[i][j][1].block.sunderflag == true )
				{
					ColorKeyDraw ( THUNDER, stage[i][j][1].x, stage[i][j][1].y, (float)(rand()%4) * 90, BLOCK_WIDTH, BLOCK_HEIGHT,
					0.f,0.f,1.f, 1.f ,stage[i][j][1].block.sunderalpha,255,255,255);
				}
			}
		}
	}
	for( int i = 0 ; i < 2 ; i++ )
	{
		CenterPointDraw ( CURSOR, player_info[i].fCursor_X, player_info[i].fCursor_Y, 0.f, BLOCK_WIDTH, BLOCK_HEIGHT,
				0.f, 0.f, 1.f, 1.f );
	}
	//箱
	CenterPointDraw ( ELE_BOX, player_info[0].BOXPOS_X + 5.f, player_info[0].BOXPOS_Y + 5.f, 0.f,
		STAGE_SIZE_X + 110.f, STAGE_SIZE_Y + 110.f,	0.f, 1.f, 1.f , 0.f );
	CenterPointDraw ( ELE_BOX, player_info[1].BOXPOS_X + 5.f , player_info[1].BOXPOS_Y + 5.f , 0.f,
		STAGE_SIZE_X  + 100.f , STAGE_SIZE_Y + 110.f,
					0.f, 1.f, 1.f , 0.f );
	//箱文字
	CenterPointDraw ( ELE_BOX_MOJI, player_info[0].BOXMOZI_X + 5.f, player_info[0].BOXMOZI_Y , 0.f,
		420.f, 50.f,	0.f, 0.f, 1.f , 1.f );
	CenterPointDraw ( ELE_BOX_MOJI, player_info[1].BOXMOZI_X + 5.f , player_info[1].BOXMOZI_Y , 0.f,
		420.f , 50.f,0.f, 0.f, 1.f , 1.f );

	//箱電気
	if( DropFlag[0] ){
		if(player_info[0].blockchangeflag == 0)
		{
			CenterPointDraw ( THUNDER, STAGE_LEFT-10.f, STAGE_UP + STAGE_SIZE_Y / 2.f + 5.f, 90.f,
			STAGE_SIZE_Y,50.f ,	0.f, 0.f, 1.f , 1.f );
			CenterPointDraw ( THUNDER, STAGE_LEFT + STAGE_SIZE_X + 10.f, STAGE_UP + STAGE_SIZE_Y / 2.f + 5.f, 90.f,
			STAGE_SIZE_Y,50.f,	0.f, 0.f, 1.f , 1.f );
		}
	}
	if( DropFlag[1] ){
		if(player_info[1].blockchangeflag == 0)
		{
			CenterPointDraw ( THUNDER, STAGE2_LEFT-10.f, STAGE_UP + STAGE_SIZE_Y / 2.f + 5.f, 90.f,
			STAGE_SIZE_Y,50.f ,	0.f, 0.f, 1.f , 1.f );
			CenterPointDraw ( THUNDER, STAGE2_LEFT + STAGE_SIZE_X + 10.f, STAGE2_UP + STAGE_SIZE_Y / 2.f + 5.f, 90.f,
			STAGE_SIZE_Y,50.f,	0.f, 0.f, 1.f , 1.f );
		}
	}


	//ネクブロ描画
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		if( addblocknum[i][0] < STAGE_HEIGHT )
		{
			if(addblock[i][addblocknum[i][0]][0].block.runflag == true )
			{
				ColorKeyDraw ( BLOCK_SET, NEXTBLOCK_X + NEXTBLOCK_WIDTH / 2 + NEXTBLOCK_LENGS * i, NEXTBLOCK_Y, addblock[i][addblocknum[i][0]][0].block.angle, NEXTBLOCK_WIDTH, NEXTBLOCK_HEIGHT,
							addblock[i][addblocknum[i][0]][0].block.type * 1.f / B_TYPEMAX, 0.f,addblock[i][addblocknum[i][0]][0].block.type * 1.f / B_TYPEMAX + 1.f / B_TYPEMAX, 1.f ,addblock[i][addblocknum[i][0]][0].block.blockalpha,255,255,255);
			}
		}
	}
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			for( int k = 0 ; k < 2 ; k++ )
			{
				if(stage[i][j][k].block.turnflag == true && stage[i][j][k].block.runflag == true )
				{
					if( stage[i][j][1].block.lightline == true )
					{
						ColorKeyDraw ( BLOCK_SET, stage[i][j][k].x, stage[i][j][k].y, stage[i][j][k].block.angle, BLOCK_WIDTH, BLOCK_HEIGHT,
						stage[i][j][k].block.type * 1.f / B_TYPEMAX, 0.f, stage[i][j][k].block.type * 1.f / B_TYPEMAX + 1.f / B_TYPEMAX, 1.f ,stage[i][j][k].block.blockalpha,255,255,255);
					}
					else
					{
						ColorKeyDraw ( BLOCK_SET, stage[i][j][k].x, stage[i][j][k].y, stage[i][j][k].block.angle, BLOCK_WIDTH, BLOCK_HEIGHT,
						stage[i][j][k].block.type * 1.f / B_TYPEMAX, 0.f, stage[i][j][k].block.type * 1.f / B_TYPEMAX + 1.f / B_TYPEMAX, 1.f ,stage[i][j][k].block.blockalpha,155,155,155);
					}
				}
			}
		}
	}
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		if( addblocknum[i][1] < STAGE_HEIGHT )
		{
			if(addblock[i][addblocknum[i][1]][1].block.runflag == true )
			{
				ColorKeyDraw ( BLOCK_SET, NEXTBLOCK2_X + NEXTBLOCK_WIDTH / 2 + NEXTBLOCK_LENGS * i, NEXTBLOCK_Y, addblock[i][addblocknum[i][1]][1].block.angle, NEXTBLOCK_WIDTH, NEXTBLOCK_HEIGHT,
							addblock[i][addblocknum[i][1]][1].block.type * 1.f / B_TYPEMAX, 0.f,addblock[i][addblocknum[i][1]][1].block.type * 1.f / B_TYPEMAX + 1.f / B_TYPEMAX, 1.f ,addblock[i][addblocknum[i][1]][1].block.blockalpha,255,255,255);
			}
		}
	}

	//輪
	for( int i = 0 ; i < 2 ; i++ ){
		if( ThunderFlag[i] ) {
			float cpx = (CHARA2_LEFT - CHARA_LEFT ) * i + CHARA_LEFT + CHARA_SIZE_X / 2 ;
			float cpy = CHARA_UP + CHARA_SIZE_Y / 2;
			float cpw = CHARA_SIZE_X * ( (float)stage[x][y][i].block.sundercount - 60.f ) / 60.f;
			float cph = CHARA_SIZE_X * ( (float)stage[x][y][i].block.sundercount - 60.f ) / 60.f;

			SetVertex( MARU , _Mver , cpx , cpy , cpw,	cph);
			Rotate( _Mver , cpx , cpy , (float)stage[x][y][i].block.sundercount * MAJIC_SPEED, cpw,cph );
			ColorKey( _Mver , 255 - rand()%50 , rand()%255 ,rand()%255,rand()%255 );
			Draw(MARU,_Mver);
		}
	}

	if( !CountDounFlag ){
		int Scale = 60 - CountDown % 60;
		if( Scale < 10 )
			Scale = 10;
		if(CountDown < 60 ){
			if ( CountDown == 45 )
				SoundPlay ( START_SE_A, false );

			CenterPointDraw ( COUNT_DOWN, WIDTH/2.f, HEIGHT / 2.f , 0.f,
				Scale * 60.f , 100.f + Scale * 60.f ,0.f, 0.5f, 1.f/4.f , 1.f );
		}
		else if(CountDown  >=60 && CountDown < 120 ){
			if ( CountDown == 105 )
				SoundPlay ( START_SE_A, false );
			
			CenterPointDraw ( COUNT_DOWN, WIDTH/2.f, HEIGHT / 2.f , 0.f,
				Scale * 60.f , 100.f + Scale * 60.f ,2.f/4.f, 0.f, 3.f/4.f , 0.5f );
		}
		else if(CountDown >=120 && CountDown < 180 ){
			if ( CountDown == 165 )
				SoundPlay ( START_SE_A, false );

			CenterPointDraw ( COUNT_DOWN, WIDTH/2.f, HEIGHT / 2.f , 0.f,
				Scale * 60.f , 100.f + Scale * 60.f ,1.f/4.f, 0.f, 2.f/4.f , 0.5f );
		}
		else if(CountDown >=180 && CountDown < 240 ){
			//Scale = 60 - Scale;
			if ( CountDown == 220 )
				SoundPlay ( START_SE_B, false );

			CenterPointDraw ( COUNT_START, WIDTH/2.f, HEIGHT / 2.f , 0.f,
				400.f + Scale * 3.f , 300.f + Scale * 3.f ,0.f, 0.f, 1.f , 1.f );
		}
		
	}
}

void PuzzleEnd()
{
	SoundAllStop();
	ReleaseTexture ( BLOCK_SET );
	ReleaseTexture ( CURSOR );
	ReleaseTexture ( PUZZLE_BACK );
	ReleaseTexture ( ELE_BOX );
	ReleaseTexture ( CHARA_BOY );
	ReleaseTexture ( CHARA_GIRL );
	ReleaseTexture ( CHARA_ROBO );
	ReleaseTexture ( LIFE_MAX );
	ReleaseTexture ( LIFE_MIN );

	ReleaseSoundFile(BGM_01);
	ReleaseSoundFile(BGM_02);
	ReleaseSoundFile(BGM_03);
	ReleaseSoundFile(BGM_04);
}

bool CheckAttack( int p_num, bool _chain , bool _attack)
{
	//通電の確認
	for( int l = 0 ; l < STAGE_WIDTH ; l++ )
	{
		for( int ll = 0 ; ll < STAGE_HEIGHT ; ll++ )
		{
			stage[l][ll][p_num].block.lightflag = false;
			stage[l][ll][p_num].block.lightline = false;
		}
	}
	   
	//　通ったことがある道を記録
		
	for( int i = 0 ; i < STAGE_HEIGHT ; i++ )
	{
		int	directionprev[40];
		directionprev[0] = D_LEFT;
		bool backflag = false;
		bool lighttemp[STAGE_WIDTH][STAGE_HEIGHT] = {0};
		bool stepblock[STAGE_WIDTH][STAGE_HEIGHT] = {0};	//どのブロックを歩いたか光らせるブロック指定に使う
		bool passsecond[STAGE_WIDTH][STAGE_HEIGHT][DIRECTION_MAX] = {0};	//２度通ろうとして通電してるブロックに接触判定をとったらもう通れなくする
		for ( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
		{
			for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
			{
					
				for ( int kk = 0 ; kk < DIRECTION_MAX ; kk++ )
				{
					bRootstop[ii][jj][kk] = false;
				}
			}
		}
		for( int iii = 0 ; iii < STAGE_WIDTH ; iii++ )
		{
			for( int jjj = 0 ; jjj < STAGE_HEIGHT ; jjj++ )
			{
				stepblock[iii][jjj] = false;
				for( int kkk = 0 ; kkk < DIRECTION_MAX ; kkk++ )
				{
					passsecond[iii][jjj][kkk] = false;
				}
			}
		}
		//int directionprev = D_LEFT;		//どっちの方向から来たかを保存
		int j = 0;
		int stepmemory[40][2];
		bool dualroot[STAGE_WIDTH][STAGE_HEIGHT][2][2];
		for( int _Z = 0 ; _Z < STAGE_WIDTH ; _Z++ )
		{
			for( int _X = 0 ; _X < STAGE_HEIGHT ; _X++ )
			{
				for( int _C = 0 ; _C < 2 ; _C++ )
				{
					for( int _V = 0 ; _V < 2 ; _V++ )
					{
						dualroot[_Z][_X][_C][_V] = false;
					}
				}
			}
		}
		int memorycount = 0;			//さっきどっちから来たかのメモリとそのカウント
			
		if( BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][directionprev[memorycount]] != 0  && stage[j][i][p_num].block.runflag == true )
		{
			// 上にいけるかチェック
			//上のブロックが存在するか
			for(;1;)
			{
				if( stage[j][i][p_num].block.type == 4 )
				{
					if( BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][directionprev[memorycount]] == 2)
					{
						dualroot[j][i][p_num][0] = true;
					}
					else
					{
						dualroot[j][i][p_num][1] = true;
					}
				}
				//一手前に来た道は上ではないか　		
				//今いるブロックは↑にいけるのか	
				//上のブロックの下はあいているのか	
				//再度通ろうとしていないか
				//if(BLOCKROOT[stage[0][i].block.type][stage[0][i].block.direction][LEFT] )
				if( i - 1 >= 0 && directionprev[memorycount] != D_TOP && ( BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_TOP] == 1 || BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_TOP] == BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][directionprev[memorycount]] ) &&
					BLOCKROOT[stage[j][i-1][p_num].block.type][stage[j][i-1][p_num].block.direction][D_BOT] != 0
					&& backflag == false && passsecond[j][i][D_TOP] == false && stage[j][i-1][p_num].block.runflag == true )
				{
					//もし一度通っていて電気が通っているところなら
					if( bRootstop[j][i][D_TOP] == true )
					{
						stepblock[j][i] = true;
						passsecond[j][i][D_TOP] = true;
						if( lighttemp[j][i-1] == true )
						{
							if(stage[j][i-1][p_num].block.type != 4)
							{
								//通電成功
								for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
								{
									for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
									{
										if(stepblock[ii][jj] == true)
										{
											lighttemp[ii][jj] = true;
										}
									}
								}
							}
							else
							{
								if( BLOCKROOT[stage[j][i-1][p_num].block.type][stage[j][i-1][p_num].block.direction][D_BOT] == 
									BLOCKROOT[stage[j][i-1][p_num].block.type][stage[j][i-1][p_num].block.direction][D_LEFT] )
								{
									if( j - 1 > 0 )
									{
										if( BLOCKROOT[stage[j-1][i-1][p_num].block.type][stage[j-1][i-1][p_num].block.direction][D_RIGHT] < 0 
											&& lighttemp[j-1][i-1] == true)
										{
											//通電成功
											for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
											{
												for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
												{
													if(stepblock[ii][jj] == true)
													{
														lighttemp[ii][jj] = true;
													}
												}
											}
										}
									}
								}
								if( BLOCKROOT[stage[j][i-1][p_num].block.type][stage[j][i-1][p_num].block.direction][D_BOT] == 
									BLOCKROOT[stage[j][i-1][p_num].block.type][stage[j][i-1][p_num].block.direction][D_RIGHT] )
								{
									if( j+1 >= STAGE_WIDTH)
									{
										//通電成功
										for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
										{
											for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
											{
												if(stepblock[ii][jj] == true)
												{
													lighttemp[ii][jj] = true;
												}
											}
										}
									}
										
									if( BLOCKROOT[stage[j+1][i-1][p_num].block.type][stage[j+1][i-1][p_num].block.direction][D_LEFT] < 0)
									{
										//通電成功
										for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
										{
											for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
											{
												if(stepblock[ii][jj] == true)
												{
													lighttemp[ii][jj] = true;
												}
											}
										}
									}
											
									else if(lighttemp[j+1][i-1] == true )
									{
										//通電成功
										for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
										{
											for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
											{
												if(stepblock[ii][jj] == true)
												{
													lighttemp[ii][jj] = true;
												}
											}
										}
									}
										
								}
							}
						}
						//memorycount--;
						if(memorycount >= 0)
						{
							stepblock[j][i] = false;
							//j = stepmemory[memorycount][0];
							//i = stepmemory[memorycount][1];
						}
						if(memorycount < 0)
						{
							break;
						}
					}
					else
					{
						//下から来たということを保存　一度通ったということを保存
						bRootstop[j][i][D_TOP] = true;
						bRootstop[j][i-1][D_BOT] = true;
						stepblock[j][i] = true;
						stepmemory[memorycount][0] = j;
						stepmemory[memorycount][1] = i;
						memorycount++;
						directionprev[memorycount] = D_BOT;
						
						i--;
					}
				}
				// 右に行こうとして　それがゴールなら
				else if( ( BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_RIGHT] == 1 || BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_RIGHT] == BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][directionprev[memorycount]] ) && j==STAGE_WIDTH-1
						&& bRootstop[j][i][D_RIGHT] == false && passsecond[j][i][D_RIGHT] == false)
				{
					//通電成功
					stepblock[j][i] = true;
					bRootstop[j][i][D_RIGHT] = true;
					passsecond[j][i][D_RIGHT] = true;
					for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
					{
						for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
						{
							if(stepblock[ii][jj] == true)
							{
								lighttemp[ii][jj] = true;
							}
						}
					}
				}
				else if( j + 1 < STAGE_WIDTH && directionprev[memorycount] != D_RIGHT && ( BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_RIGHT] == 1 || BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_RIGHT] == BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][directionprev[memorycount]] ) &&
					BLOCKROOT[stage[j+1][i][p_num].block.type][stage[j+1][i][p_num].block.direction][D_LEFT] != 0
					&&  backflag == false && passsecond[j][i][D_RIGHT] == false && stage[j+1][i][p_num].block.runflag == true)
				{
					if( bRootstop[j][i][D_RIGHT] == true )
					{
						stepblock[j][i] = true;
						passsecond[j][i][D_RIGHT] = true;
						if( lighttemp[j+1][i] == true )
						{
							if( stage[j+1][i][p_num].block.type != 4 )
							{
								//通電成功
								for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
								{
									for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
									{
										if(stepblock[ii][jj] == true)
										{
											lighttemp[ii][jj] = true;
										}
									}
								}
							}
							else
							{
								if( BLOCKROOT[stage[j+1][i][p_num].block.type][stage[j+1][i][p_num].block.direction][D_LEFT] == 
									BLOCKROOT[stage[j+1][i][p_num].block.type][stage[j+1][i][p_num].block.direction][D_TOP] )
								{
									if( i - 1 > 0 )
									{
										if( BLOCKROOT[stage[j+1][i-1][p_num].block.type][stage[j+1][i-1][p_num].block.direction][D_BOT] < 0 
											&& lighttemp[j+1][i-1] == true)
										{
											//通電成功
											for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
											{
												for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
												{
													if(stepblock[ii][jj] == true)
													{
														lighttemp[ii][jj] = true;
													}
												}
											}
										}
									}
								}
								if( BLOCKROOT[stage[j+1][i][p_num].block.type][stage[j+1][i][p_num].block.direction][D_LEFT] == 
									BLOCKROOT[stage[j+1][i][p_num].block.type][stage[j+1][i][p_num].block.direction][D_BOT] )
								{
									if( i+1 < STAGE_HEIGHT )
									{
										if( BLOCKROOT[stage[j+1][i+1][p_num].block.type][stage[j+1][i+1][p_num].block.direction][D_TOP] < 0 
											&& lighttemp[j+1][i+1] == true)
										{
											//通電成功
											for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
											{
												for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
												{
													if(stepblock[ii][jj] == true)
													{
														lighttemp[ii][jj] = true;
													}
												}
											}
										}
									}
								}
							}
						}
						//memorycount--;
						if(memorycount >= 0)
						{
							stepblock[j][i] = false;
							//j = stepmemory[memorycount][0];
							//i = stepmemory[memorycount][1];
						}
						if(memorycount < 0)
						{
							break;
						}
					}
					else
					{
								
						bRootstop[j][i][D_RIGHT] = true;
						bRootstop[j+1][i][D_LEFT] = true;
						stepblock[j][i] = true;
						stepmemory[memorycount][0] = j;
						stepmemory[memorycount][1] = i;
						memorycount++;
						directionprev[memorycount] = D_LEFT;
					
						j++;
					}
				}
				else if( i + 1 < STAGE_HEIGHT && directionprev[memorycount] != D_BOT && ( BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_BOT] == 1 ||  BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_BOT] == BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][directionprev[memorycount]] ) &&
					BLOCKROOT[stage[j][i+1][p_num].block.type][stage[j][i+1][p_num].block.direction][D_TOP] != 0
					&& backflag == false && passsecond[j][i][D_BOT] == false && stage[j][i+1][p_num].block.runflag == true)
				{
					if( bRootstop[j][i][D_BOT] == true )
					{
						stepblock[j][i] = true;
						passsecond[j][i][D_BOT] = true;
						if( lighttemp[j][i+1] == true )
						{
							if( stage[j][i+1][p_num].block.type != 4 )
							{
								//通電成功
								for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
								{
									for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
									{
										if(stepblock[ii][jj] == true)
										{
											lighttemp[ii][jj] = true;
										}
									}
								}
							}
							else
							{
								if( i + 1 < STAGE_HEIGHT )
								{
									if( BLOCKROOT[stage[j][i+1][p_num].block.type][stage[j][i+1][p_num].block.direction][D_TOP] == 
										BLOCKROOT[stage[j][i+1][p_num].block.type][stage[j][i+1][p_num].block.direction][D_LEFT] )
									{
										if( BLOCKROOT[stage[j-1][i+1][p_num].block.type][stage[j-1][i+1][p_num].block.direction][D_RIGHT] < 0 
											&& lighttemp[j-1][i+1] == true)
										{
											//通電成功
											for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
											{
												for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
												{
													if(stepblock[ii][jj] == true)
													{
														lighttemp[ii][jj] = true;
													}
												}
											}
										}
									}
									if( BLOCKROOT[stage[j][i+1][p_num].block.type][stage[j][i+1][p_num].block.direction][D_TOP] == 
										BLOCKROOT[stage[j][i+1][p_num].block.type][stage[j][i+1][p_num].block.direction][D_RIGHT] )
									{
										if( BLOCKROOT[stage[j+1][i+1][p_num].block.type][stage[j+1][i+1][p_num].block.direction][D_LEFT] < 0 
											&& lighttemp[j+1][i+1] == true)
										{
											//通電成功
											for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
											{
												for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
												{
													if(stepblock[ii][jj] == true)
													{
														lighttemp[ii][jj] = true;
													}
												}
											}
										}
									}
								}
							}
						}
						//memorycount--;
						if(memorycount >= 0)
						{
							stepblock[j][i] = false;
							//j = stepmemory[memorycount][0];
							//i = stepmemory[memorycount][1];
						}
						if(memorycount < 0)
						{
							break;
						}
					}
					else
					{
								
						bRootstop[j][i][D_BOT] = true;
						bRootstop[j][i+1][D_TOP] = true;
						stepblock[j][i] = true;
						stepmemory[memorycount][0] = j;
						stepmemory[memorycount][1] = i;
						memorycount++;
						directionprev[memorycount] = D_TOP;
						
						i++;	
					}
				}
				else if( j - 1 >= 0 && directionprev[memorycount] != D_LEFT && ( BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_LEFT] == 1 || BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][D_LEFT] == BLOCKROOT[stage[j][i][p_num].block.type][stage[j][i][p_num].block.direction][directionprev[memorycount]]  ) &&
					BLOCKROOT[stage[j-1][i][p_num].block.type][stage[j-1][i][p_num].block.direction][D_RIGHT] != 0
					&& backflag == false && passsecond[j][i][LEFT] == false && stage[j-1][i][p_num].block.runflag == true)
				{
					if( bRootstop[j][i][D_LEFT] == true)
					{
						stepblock[j][i] = true;
						passsecond[j][i][LEFT] = true;
						if( lighttemp[j-1][i] == true )
						{
							if( stage[j-1][i][p_num].block.type != 4 )
							{
								//通電成功
								for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
								{
									for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
									{
										if(stepblock[ii][jj] == true)
										{
											lighttemp[ii][jj] = true;
										}
									}
								}
							}
							else
							{
								if( BLOCKROOT[stage[j-1][i][p_num].block.type][stage[j-1][i][p_num].block.direction][D_RIGHT] == 
									BLOCKROOT[stage[j-1][i][p_num].block.type][stage[j-1][i][p_num].block.direction][D_TOP] )
								{
									if( i - 1 > 0 )
									{
										if( BLOCKROOT[stage[j-1][i-1][p_num].block.type][stage[j-1][i-1][p_num].block.direction][D_BOT] < 0 
											&& lighttemp[j-1][i-1] == true)
										{
											//通電成功
											for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
											{
												for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
												{
													if(stepblock[ii][jj] == true)
													{
														lighttemp[ii][jj] = true;
													}
												}
											}
										}
									}
								}
								if( BLOCKROOT[stage[j-1][i][p_num].block.type][stage[j-1][i][p_num].block.direction][D_RIGHT] == 
									BLOCKROOT[stage[j-1][i][p_num].block.type][stage[j-1][i][p_num].block.direction][D_BOT] )
								{
									if( i + 1 < STAGE_HEIGHT )
									{
										if( BLOCKROOT[stage[j-1][i+1][p_num].block.type][stage[j-1][i+1][p_num].block.direction][D_TOP] < 0 
											&& lighttemp[j-1][i+1] == true)
										{
											//通電成功
											for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
											{
												for ( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
												{
													if(stepblock[ii][jj] == true)
													{
														lighttemp[ii][jj] = true;
													}
												}
											}
										}
									}
								}
							}
						}
						//memorycount--;
						if(memorycount >= 0)
						{
							
							stepblock[j][i] = false;
							//j = stepmemory[memorycount][0];
							//i = stepmemory[memorycount][1];
						}
						if(memorycount < 0)
						{
							break;
						}
					}
					else
					{
								
						bRootstop[j][i][D_LEFT] = true;
						bRootstop[j-1][i][D_RIGHT] = true;
						stepblock[j][i] = true;
						stepmemory[memorycount][0] = j;
						stepmemory[memorycount][1] = i;
						memorycount++;
						directionprev[memorycount] = D_RIGHT;
						j--;
					}
				}
				else
				{
					memorycount--;
					if(memorycount >= 0)
					{
						passsecond[j][i][directionprev[memorycount]] = true;
						stepblock[j][i] = false;
						if( dualroot[j][i][p_num][0] == true && dualroot[j][i][p_num][1] == true && stage[j][i][p_num].block.type == 4)
						{
							stepblock[j][i] = true;
						}
						if(BLOCKROOT[stage[stepmemory[memorycount][0]][stepmemory[memorycount][1]][p_num].block.type]
							[stage[stepmemory[memorycount][0]][stepmemory[memorycount][1]][p_num].block.direction]
							[directionprev[memorycount]]== 2 )
						{
							dualroot[j][i][p_num][0] = false;
						}
						else
						{
							dualroot[j][i][p_num][1] = false;
						}
						j = stepmemory[memorycount][0];
						i = stepmemory[memorycount][1];
					}
					if(memorycount < 0)
					{
						break;
					}
				}
			}
		}
		for( int ix = 0 ; ix < STAGE_WIDTH ; ix++ )
		{
			for( int jx = 0 ; jx < STAGE_HEIGHT ; jx++ )
			{
				if(lighttemp[ix][jx] == true)
				{
					stage[ix][jx][p_num].block.lightflag = true;
				}
			}
		}
	}
	if( p_num == 0 &&(GetButton(0,BUTTON_X,PUSH) || StCheck ( KEY_V, PUSH ))
		|| p_num == 1 && (GetButton(1,BUTTON_X,PUSH) || StCheck ( KEY_O, PUSH ))||_chain)
	{
		player_info[p_num].chain++;
		int damagecount = 0;
		for( int i = 0 ; i < STAGE_HEIGHT ; i++ )
		{
			if( stage[0][i][p_num].block.lightflag == true )
			{
				if( BLOCKROOT[stage[0][i][p_num].block.type][stage[0][i][p_num].block.direction][D_LEFT] == 1 )
				{
					damagecount++;
				}
				else if(BLOCKROOT[stage[0][i][p_num].block.type][stage[0][i][p_num].block.direction][D_LEFT] >= 1 && 
						BLOCKROOT[stage[0][i][p_num].block.type][stage[0][i][p_num].block.direction][D_LEFT] == BLOCKROOT[stage[0][i][p_num].block.type][stage[0][i][p_num].block.direction][D_TOP])
				{		
					if( stage[0][i-1][p_num].block.lightflag == true )
					{
						damagecount++;
					}
				}
				else if(BLOCKROOT[stage[0][i][p_num].block.type][stage[0][i][p_num].block.direction][D_LEFT] >= 1 && 
						BLOCKROOT[stage[0][i][p_num].block.type][stage[0][i][p_num].block.direction][D_LEFT] == BLOCKROOT[stage[0][i][p_num].block.type][stage[0][i][p_num].block.direction][D_BOT])
				{		
					if( stage[0][i+1][p_num].block.lightflag == true )
					{
						damagecount++;
					}
				}
			}
				
		}
		for( int i = 0 ; i < STAGE_HEIGHT ; i++ )
		{
			if( stage[STAGE_WIDTH-1][i][p_num].block.lightflag == true )
			{
				if( BLOCKROOT[stage[STAGE_WIDTH-1][i][p_num].block.type][stage[STAGE_WIDTH-1][i][p_num].block.direction][D_RIGHT] == 1 )
				{
					damagecount++;
				}
				else if(BLOCKROOT[stage[STAGE_WIDTH-1][i][p_num].block.type][stage[STAGE_WIDTH-1][i][p_num].block.direction][D_RIGHT] >= 1 && 
						BLOCKROOT[stage[STAGE_WIDTH-1][i][p_num].block.type][stage[STAGE_WIDTH-1][i][p_num].block.direction][D_RIGHT] == BLOCKROOT[stage[STAGE_WIDTH-1][i][p_num].block.type][stage[STAGE_WIDTH-1][i][p_num].block.direction][D_TOP])
				{		
					if( stage[STAGE_WIDTH-1][i-1][p_num].block.lightflag == true )
					{
						damagecount++;
					}
				}
				else if(BLOCKROOT[stage[STAGE_WIDTH-1][i][p_num].block.type][stage[STAGE_WIDTH-1][i][p_num].block.direction][D_RIGHT] >= 1 && 
						BLOCKROOT[stage[STAGE_WIDTH-1][i][p_num].block.type][stage[STAGE_WIDTH-1][i][p_num].block.direction][D_RIGHT] == BLOCKROOT[stage[STAGE_WIDTH-1][i][p_num].block.type][stage[STAGE_WIDTH-1][i][p_num].block.direction][D_BOT])
				{		
					if( stage[STAGE_WIDTH-1][i+1][p_num].block.lightflag == true )
					{
						damagecount++;
					}
				}
			}
		}
		for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
		{
			for( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
			{
				if( stage[ii][jj][p_num].block.lightflag == true )
				{
					if(damagecount > 0 )
					{
						SoundPlay(BGM_04,true);
					}
					stage[ii][jj][p_num].block.sunderalpha = 255;
					stage[ii][jj][p_num].block.sunderflag = true;
					stage[ii][jj][p_num].block.sunderstep = false;
					stage[ii][jj][p_num].block.sundercount = SUNDERCOUNT;
				}
			}
		}
		if(damagecount > 0 )
		{
			float alldelflag = ALLDELDAMAGE;
			for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
			{
				for( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
				{
					if(stage[ii][jj][p_num].block.runflag == true  )
					{
						if(stage[ii][jj][p_num].block.sunderflag == false  )
						{
							alldelflag = 0;
						}
					}
				}
			}
			for( int ii = 0 ; ii < STAGE_WIDTH ; ii++ )
			{
				for( int jj = 0 ; jj < STAGE_HEIGHT ; jj++ )
				{
					if(addblock[ii][jj][p_num].block.runflag == true && addblocknum[ii][p_num] < jj  )
					{
						alldelflag = 0;
					}
				}
			}
			player_info[p_num].givedamage = (float)((N_DAMAGE+player_info[p_num].chain*C_DAMAGE + damagecount*P_DAMAGE)*player_info[p_num].difficult + alldelflag);
			player_info[p_num].goinganime = 1;
			player_info[p_num].animestep = 1;

			//-----------------------------------------
			// p_num にどっちのプレイヤーが攻撃するかが入っていて
			//攻撃成功が確定
			//-----------------------------------------
			return true;
		}
	}
	if( _attack == false )
	{
		if( _attack == false )
		{
			for( int i = 0 ; i < STAGE_WIDTH ; i++ )
			{
				for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
				{
					if( stage[i][j][p_num].block.lightflag == true)
					{
						stage[i][j][p_num].block.lightline = true;
						stage[i][j][p_num].block.lightflag = false;
					}
				}
			}
		}
	}
	if(_attack){
		for(int i = 0 ; i < MAJIC_MAX ; i++ ){
			MLen[i][0] = MAJIC_LENGTH * ((float)((rand()%1000)-500) / 500.f );
			MLen[i][1] = MAJIC_LENGTH * ((float)((rand()%1000)-500) / 500.f );
		}
	}
	return false;
	//------------------------------------通電確認ここまで
}
bool BlockDrop( int p_num )
{
	bool blockdropingflag = false;
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			stage[i][j][p_num].block.dropflag = false;
			if( j < STAGE_HEIGHT-1 )
			{
				for( int k = j ; k < STAGE_HEIGHT - 1 ; k++ )
				{
					if( stage[i][k+1][p_num].block.runflag == false )
					{
						stage[i][j][p_num].block.dropcount++;
					}
				}
			}
		}
	}
	STAGE blockdroptemp[STAGE_WIDTH][STAGE_HEIGHT];
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = STAGE_HEIGHT-1 ; j >= 0 ; j-- )
		{
			blockdroptemp[i][j] = stage[i][j][p_num];
			if( stage[i][j][p_num].block.dropcount > 0 && stage[i][j][p_num].block.runflag == true )
			{
				stage[i][j][p_num].block.runflag = false;
				stage[i][j][p_num].block.dropflag = true;
			}
		}
	}
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			if(stage[i][j][p_num].block.dropflag == true)
			{
				blockdropingflag = true;
				stage[i][j+stage[i][j][p_num].block.dropcount][p_num].block.direction = blockdroptemp[i][j].block.direction;
				stage[i][j+stage[i][j][p_num].block.dropcount][p_num].block.type = blockdroptemp[i][j].block.type;
				stage[i][j+stage[i][j][p_num].block.dropcount][p_num].block.runflag= true;
				stage[i][j+stage[i][j][p_num].block.dropcount][p_num].block.lightflag = false;
				stage[i][j+stage[i][j][p_num].block.dropcount][p_num].block.goalpos = D3DXVECTOR2(stage[i][j+stage[i][j][p_num].block.dropcount][p_num].x,stage[i][j+stage[i][j][p_num].block.dropcount][p_num].y);
				stage[i][j+stage[i][j][p_num].block.dropcount][p_num].y = STAGE_UP + j * BLOCK_HEIGHT + BLOCK_HEIGHT / 2;//stage[i][j][p_num].y;
				stage[i][j+stage[i][j][p_num].block.dropcount][p_num].block.moveflag = true;
			}
		}
	}
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			stage[i][j][p_num].block.dropcount = 0;
			stage[i][j][p_num].block.dropflag = false;
		}
	}
	int addblockanime[STAGE_WIDTH];		//追加ブロックのラインごとの段数
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		addblockanime[i] = 0;
	}
	for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = STAGE_HEIGHT-1 ; j >= 0 ; j-- )
		{
			if(stage[i][j][p_num].block.runflag == false)
			{
				if(addblocknum[i][p_num] < STAGE_HEIGHT )
				{
					blockdropingflag = true;
					addblockanime[i]++;
					stage[i][j][p_num].block = addblock[i][addblocknum[i][p_num]][p_num].block;
					stage[i][j][p_num].block.goalpos = D3DXVECTOR2(stage[i][j][p_num].x,stage[i][j][p_num].y);
					stage[i][j][p_num].y = STAGE_UP - BLOCK_HEIGHT * addblockanime[i] + BLOCK_HEIGHT / 2;
					stage[i][j][p_num].block.moveflag = true;
					stage[i][j][p_num].block.runflag = addblock[i][addblocknum[i][p_num]][p_num].block.runflag;
					addblocknum[i][p_num]++;
					DropFlag[p_num] = true;
				}
			}
		}
	}
	if(blockdropingflag == true)
	{
		return true;
	}
	return false;
}
void AdBlock( int p_num )
{
	
}
void BlockReset( int p_num )
{
	player_info[p_num].blockchangeflag = true;
	DropFlag[p_num] = true;
	/*for( int i = 0 ; i < STAGE_WIDTH ; i++ )
	{
		for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
		{
			stage[i][j][p_num].block.type = rand()%5;
			stage[i][j][p_num].block.direction = rand()%4;
			stage[i][j][p_num].block.lightflag = false;
			stage[i][j][p_num].block.runflag = true;
			stage[i][j][p_num].block.blockalpha = 255;

			addblock[i][j][p_num].block.type = rand()%5;
			addblock[i][j][p_num].block.direction = rand()%4;
			addblock[i][j][p_num].block.lightflag = false;
			addblock[i][j][p_num].block.runflag = true;
			addblocknum[i][p_num] = 0;
			addblock[i][j][p_num].block.blockalpha = 255;
		}
	}*/
	player_info[p_num].chain = 0;
}
void ChangeAnime()
{
	for( int _player = 0; _player < 2 ; _player++ )
	{
		if( player_info[_player].blockchangeflag == 1 )
		{
			if(_player == 0 )
			{
				player_info[0].BOXPOS_X -= CHANGESPEED;
				player_info[0].BOXMOZI_X -= CHANGESPEED;
			}
			if(_player == 1 )
			{
				player_info[1].BOXPOS_X += CHANGESPEED;
				player_info[1].BOXMOZI_X += CHANGESPEED;
			}
			for( int i = 0 ; i < STAGE_WIDTH ; i++ )
			{
				for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
				{
					if( _player == 0 )
					{
						stage[i][j][_player].x -= CHANGESPEED;
					}
					if( _player == 1 )
					{
						stage[i][j][_player].x += CHANGESPEED;
					}
				}
			}
			if( (stage[STAGE_WIDTH-1][0][_player].x < 0 - BLOCK_WIDTH/2 && _player == 0)
				|| ( stage[0][0][_player].x > WIDTH + BLOCK_WIDTH/2 && _player == 1))
			{
				
				player_info[_player].blockchangeflag = 2;
				if( _player == 0 )
				{
					player_info[0].BOXPOS_X = STAGE_LEFT + STAGE_SIZE_X/2;
					player_info[0].BOXPOS_Y = - STAGE_SIZE_Y/2;
					player_info[0].BOXMOZI_X = STAGE_LEFT + STAGE_SIZE_X / 2 + 5.f;
					player_info[0].BOXMOZI_Y = -  STAGE_SIZE_Y + STAGE_SIZE_Y + 15.f;
				}
				if( _player == 1 )
				{
					player_info[1].BOXPOS_X = STAGE2_LEFT + STAGE_SIZE_X/2;
					player_info[1].BOXPOS_Y = - STAGE_SIZE_Y/2;
					player_info[1].BOXMOZI_X = STAGE2_LEFT + STAGE_SIZE_X / 2 + 5.f;
					player_info[1].BOXMOZI_Y = - STAGE_SIZE_Y + STAGE_SIZE_Y + 15.f;
				}
				for( int i = 0 ; i < STAGE_WIDTH ; i++ )
				{
					for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
					{
						if( _player == 0 )
						{
							stage[i][j][_player].x = STAGE_LEFT + BLOCK_WIDTH * i + BLOCK_WIDTH / 2;
						}
						else
						{
							stage[i][j][_player].x = STAGE2_LEFT + BLOCK_WIDTH * i + BLOCK_WIDTH / 2;
						}
						stage[i][j][_player].y = 0- + BLOCK_HEIGHT * (STAGE_HEIGHT-1 - j) - BLOCK_HEIGHT / 2;
						//------------------------------------------
						// ここでブロックのセットを次のやつに変更
						//------------------------------------------
						if(PassStageBlock ( settablenum[player_info[_player].nextset], eTYPE, i, j ) <= 4)
						{
							stage[i][j][_player].block.type = PassStageBlock ( settablenum[player_info[_player].nextset], eTYPE, i, j );
							stage[i][j][_player].block.direction = settabledirection[i][j][player_info[_player].nextset];
							stage[i][j][_player].block.runflag = true;
							stage[i][j][_player].block.angle = stage[i][j][_player].block.direction * 90.f;
						}
						else
						{
							stage[i][j][_player].block.runflag = false;
						}
						stage[i][j][_player].block.lightflag = false;
						stage[i][j][_player].block.blockalpha = 255;

						if(PassAddBlock ( settablenum[player_info[_player].nextset], eTYPE, i, j ) <= 4)
						{
							addblock[i][j][_player].block.type = PassAddBlock ( settablenum[player_info[_player].nextset], eTYPE, i, j );
							addblock[i][j][_player].block.direction = PassAddBlock ( settablenum[player_info[_player].nextset], eDirection, i, j );
							addblock[i][j][_player].block.runflag = true;
							addblock[i][j][_player].block.angle = addblock[i][j][_player].block.direction * 90.f;
						}
						else
						{
							addblock[i][j][_player].block.runflag = false;
						}
						addblock[i][j][_player].block.lightflag = false;
						//addblock[i][j][_player].block.runflag = true;
						addblocknum[i][_player] = 0;
						addblock[i][j][_player].block.blockalpha = 255;
						
					}
				}
				player_info[_player].nextset++;
				if(player_info[_player].nextset >= TABLELOOPNUM )
				{
					player_info[_player].nextset = 0;
				}
			}
		}
		else if( player_info[_player].blockchangeflag == 2 )
		{
		
			if( _player == 0 )
			{
				player_info[0].BOXPOS_Y += CHANGESPEED;
				player_info[0].BOXMOZI_Y += CHANGESPEED;
			}
			if( _player == 1 )
			{
				player_info[1].BOXPOS_Y += CHANGESPEED;
				player_info[1].BOXMOZI_Y += CHANGESPEED;
			}
			
			for( int i = 0 ; i < STAGE_WIDTH ; i++ )
			{
				for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
				{
					stage[i][j][_player].y += CHANGESPEED;
				}
			}
			if( stage[0][0][_player].y > STAGE_UP + BLOCK_HEIGHT / 2 )
			{
				if( _player == 0 )
				{
					player_info[0].BOXPOS_Y = STAGE_UP + STAGE_SIZE_Y/2;
					player_info[0].BOXMOZI_Y = STAGE_UP + STAGE_SIZE_Y + 15.f;
				}
				if( _player == 1 )
				{
					player_info[1].BOXPOS_Y = STAGE_UP + STAGE_SIZE_Y/2;
					player_info[1].BOXMOZI_Y = STAGE_UP + STAGE_SIZE_Y + 15.f;
				}
				for( int i = 0 ; i < STAGE_WIDTH ; i++ )
				{
					for( int j = 0 ; j < STAGE_HEIGHT ; j++ )
					{
						if( _player == 0 )
						{
							stage[i][j][0].x = STAGE_LEFT + BLOCK_WIDTH * i + BLOCK_WIDTH / 2;
							stage[i][j][0].y = STAGE_UP + BLOCK_HEIGHT * j + BLOCK_HEIGHT / 2;
						}
						if( _player == 1 )
						{
							stage[i][j][1].x = STAGE2_LEFT + BLOCK_WIDTH * i + BLOCK_WIDTH / 2;
							stage[i][j][1].y = STAGE2_UP + BLOCK_HEIGHT * j + BLOCK_HEIGHT / 2;
						}
					}
				}
				DropFlag[_player] = false;
				player_info[_player].blockchangeflag = 0;
				CheckAttack( _player, false, false );
			}
		}
	}
}
bool GetPlayerDeath( int _pn )
{
	if( player_info[_pn].fHp <= 0 )
		return true;
	return false;
}

int GetPlayerType( int _pn )
{
	return player_info[_pn].charatype;
}

//ここから下
//製作者	亀田
//---------------------------------------------------------------------------------
//	台詞
	
OBJECT_INFO objWords[2] = {0};		//	台詞
OBJECT_INFO	objBalloons[2] = {0};	//	吹き出し

enum eWORDS
{
	ATT_WORDS_1,

	ATT_WORDS_2,

	DAMAGE_WORDS_1,

	DAMAGE_WORDS_2,

	WIN_WORDS,

	WORDS_MAX
};

enum	eBalloon
{
	BALLOON1,

	BALLOON2,

	BALLOON_MAX
};

float fWordsUVTable[WORDS_MAX][2][2] = {
	{ { 0.0f, 0.0f}, { 0.33f, 0.5f}},
	{ { 0.0f, 0.5f}, { 0.33f, 1.0f}},
	{ { 0.33f, 0.0f}, { 0.66f, 0.5f}},
	{ { 0.33f, 0.5f}, { 0.66f, 1.0f}},
	{ { 0.66f, 0.0f}, { 1.0f, 0.5f}}/*,
	{ { 0.66f, 0.5f}, { 1.0f, 1.0f}}*/
};

float fW_BalloonUVTable[2] = {	0.0f, 1.0f };

//	台詞
#define W_WIDTH		300.f	//	幅
#define W_HEIGHT	200.f	//	高さ
#define	W_P1_X_POS	260.f	//	プレイヤー1のX
#define	W_P1_Y_POS	500.f	//	プレイヤー1のY
#define	W_P2_X_POS	664.f	//	プレイヤー2のX
#define	W_P2_Y_POS	500.f	//	プレイヤー2のY

#define RESULT_W_X_POS	450.f	
#define RESULT_W_Y_POS	RESULT_CHARA_TOP + 20

//	吹き出し
#define B_WIDTH		340.f	//	幅
#define B_HEIGHT	240.f	//	高さ
#define	B_P1_X_POS	240.f	//	プレイヤー1吹き出しのX
#define	B_P1_Y_POS	480.f	//	プレイヤー1吹き出しのY
#define	B_P2_X_POS	644.f	//	プレイヤー2吹き出しのX
#define	B_P2_Y_POS	480.f	//	プレイヤー2吹き出しのY

#define RESULT_B_X_POS	RESULT_W_X_POS - 20.f
#define RESULT_B_Y_POS	RESULT_CHARA_TOP

void WordInit ()
{
	LoadTexture ( "image\\puzzle\\girl_message.png", GIRLS_WORDS, TEMP_RGB2);
	
	LoadTexture ( "image\\puzzle\\boy_message.png", BOYS_WORDS, TEMP_RGB2);
	
	LoadTexture ( "image\\puzzle\\hukidasi_1.png", WORDS_BALLOON1, TEMP_RGB2);
	
	LoadTexture ( "image\\puzzle\\hukidasi_2.png", WORDS_BALLOON2, TEMP_RGB2);

	for ( int ii = 0; ii < 2; ii++ )
	{
		objWords[ii].fHeight	= W_HEIGHT;
		objWords[ii].fWidth		= W_WIDTH;
		objWords[ii].iAlpha		= 255;
		objWords[ii].iRed		= 255;
		objWords[ii].iGreen		= 255;
		objWords[ii].iBlue		= 255;

		objBalloons[ii].fHeight		= B_HEIGHT;
		objBalloons[ii].fWidth		= B_WIDTH;
		objBalloons[ii].iAlpha		= 150;
		objBalloons[ii].iRed		= 255;
		objBalloons[ii].iGreen		= 255;
		objBalloons[ii].iBlue		= 255;

		switch ( ii )
		{
		case 0:
			objWords[ii].fXpos		= W_P1_X_POS;
			objWords[ii].fYpos		= W_P1_Y_POS;
			
			objBalloons[ii].fXpos	= B_P1_X_POS;
			objBalloons[ii].fYpos	= B_P1_Y_POS;

			break;

		case 1:
			objWords[ii].fXpos		= W_P2_X_POS;
			objWords[ii].fYpos		= W_P2_Y_POS;

			objBalloons[ii].fXpos	= B_P2_X_POS;
			objBalloons[ii].fYpos	= B_P2_Y_POS;

			break;
		}
	}
}

void WordDraw ( bool bResultFlag )
{
	for ( int ii = 0; ii < 2; ii++ )
	{	
		if ( bResultFlag == false )
		{
			switch ( player_info[ii].charastate )
			{
			case 0:
				break;

			case 1:	//	攻撃
				//	吹き出し描画
				if ( ii == 0 )
				CustomCorolDraw( WORDS_BALLOON2, objBalloons[ii].fXpos, objBalloons[ii].fYpos,
					objBalloons[ii].fXpos + objBalloons[ii].fWidth, objBalloons[ii].fYpos + objBalloons[ii].fHeight,
					fW_BalloonUVTable[0], fW_BalloonUVTable[0],	fW_BalloonUVTable[1], fW_BalloonUVTable[1],
					objBalloons[ii].iAlpha, objBalloons[ii].iRed, objBalloons[ii].iGreen, objBalloons[ii].iBlue );
				
				if ( ii == 1 )
				CustomCorolDraw( WORDS_BALLOON2, objBalloons[ii].fXpos, objBalloons[ii].fYpos,
					objBalloons[ii].fXpos + objBalloons[ii].fWidth, objBalloons[ii].fYpos + objBalloons[ii].fHeight,
					fW_BalloonUVTable[1], fW_BalloonUVTable[0],fW_BalloonUVTable[0], fW_BalloonUVTable[1],
					objBalloons[ii].iAlpha, objBalloons[ii].iRed, objBalloons[ii].iGreen, objBalloons[ii].iBlue );

				switch ( player_info[ii].charatype )
				{
				case CHARA_GIRL:	//	女性

					//	台詞
					CustomDraw( GIRLS_WORDS, objWords[ii].fXpos, objWords[ii].fYpos,
						objWords[ii].fXpos + objWords[ii].fWidth, objWords[ii].fYpos + objWords[ii].fHeight,
						fWordsUVTable[ATT_WORDS_1 + iWordsPattern][0][0], fWordsUVTable[ATT_WORDS_1 + iWordsPattern][0][1],
						fWordsUVTable[ATT_WORDS_1 + iWordsPattern][1][0], fWordsUVTable[ATT_WORDS_1 + iWordsPattern][1][1] );
									
					break;

				case CHARA_BOY:	//	男性

					//	台詞
					CustomDraw( BOYS_WORDS, objWords[ii].fXpos, objWords[ii].fYpos,
						objWords[ii].fXpos + objWords[ii].fWidth, objWords[ii].fYpos + objWords[ii].fHeight,
						fWordsUVTable[ATT_WORDS_1 + iWordsPattern][0][0], fWordsUVTable[ATT_WORDS_1 + iWordsPattern][0][1],
						fWordsUVTable[ATT_WORDS_1 + iWordsPattern][1][0], fWordsUVTable[ATT_WORDS_1 + iWordsPattern][1][1] );

					break;

				case CHARA_ROBO:	//	ロボット

					break;
				}
				break;

			case 2:	//	ダメージ状態
				//	吹き出し描画
				if ( ii == 0 )
				CustomCorolDraw( WORDS_BALLOON1, objBalloons[ii].fXpos, objBalloons[ii].fYpos,
					objBalloons[ii].fXpos + objBalloons[ii].fWidth, objBalloons[ii].fYpos + objBalloons[ii].fHeight,
					fW_BalloonUVTable[0], fW_BalloonUVTable[0],	fW_BalloonUVTable[1], fW_BalloonUVTable[1],
					objBalloons[ii].iAlpha, objBalloons[ii].iRed, objBalloons[ii].iGreen, objBalloons[ii].iBlue );
				
				if ( ii == 1 )
				CustomCorolDraw( WORDS_BALLOON1, objBalloons[ii].fXpos, objBalloons[ii].fYpos,
					objBalloons[ii].fXpos + objBalloons[ii].fWidth, objBalloons[ii].fYpos + objBalloons[ii].fHeight,
					fW_BalloonUVTable[1], fW_BalloonUVTable[0],fW_BalloonUVTable[0], fW_BalloonUVTable[1],
					objBalloons[ii].iAlpha, objBalloons[ii].iRed, objBalloons[ii].iGreen, objBalloons[ii].iBlue );

				switch ( player_info[ii].charatype )
				{
				case CHARA_GIRL:	//	女性

					//	台詞
					CustomDraw( GIRLS_WORDS, objWords[ii].fXpos, objWords[ii].fYpos,
						objWords[ii].fXpos + objWords[ii].fWidth, objWords[ii].fYpos + objWords[ii].fHeight,
						fWordsUVTable[DAMAGE_WORDS_1 + iWordsPattern][0][0], fWordsUVTable[DAMAGE_WORDS_1 + iWordsPattern][0][1],
						fWordsUVTable[DAMAGE_WORDS_1 + iWordsPattern][1][0], fWordsUVTable[DAMAGE_WORDS_1 + iWordsPattern][1][1] );				
					
					break;

				case CHARA_BOY:	//	男性

					//	台詞
					CustomDraw( BOYS_WORDS, objWords[ii].fXpos, objWords[ii].fYpos,
						objWords[ii].fXpos + objWords[ii].fWidth, objWords[ii].fYpos + objWords[ii].fHeight,
						fWordsUVTable[DAMAGE_WORDS_1 + iWordsPattern][0][0], fWordsUVTable[DAMAGE_WORDS_1 + iWordsPattern][0][1],
						fWordsUVTable[DAMAGE_WORDS_1 + iWordsPattern][1][0], fWordsUVTable[DAMAGE_WORDS_1 + iWordsPattern][1][1] );
					
					break;

				case CHARA_ROBO:	//	ロボット

					break;
				}
				break;
			}
		}

		if ( GetPlayerDeath( ii ) && bResultFlag == true )
		{
			int temp = (ii + 1) % 2;

			objWords[temp].fXpos	= RESULT_W_X_POS;
			objWords[temp].fYpos	= RESULT_W_Y_POS;
			
			objBalloons[temp].fXpos	= RESULT_B_X_POS;
			objBalloons[temp].fYpos	= RESULT_B_Y_POS;

			switch ( player_info[temp].charatype )
			{
			case CHARA_GIRL:	//	女性
				//	吹き出し描画
				if ( temp == 0 )
				CustomCorolDraw( WORDS_BALLOON1, objBalloons[temp].fXpos, objBalloons[temp].fYpos,
					objBalloons[temp].fXpos + objBalloons[temp].fWidth, objBalloons[temp].fYpos + objBalloons[temp].fHeight,
					fW_BalloonUVTable[0], fW_BalloonUVTable[0],	fW_BalloonUVTable[1], fW_BalloonUVTable[1],
					objBalloons[temp].iAlpha, objBalloons[temp].iRed, objBalloons[temp].iGreen, objBalloons[temp].iBlue );

				if ( temp == 1 )
				CustomCorolDraw( WORDS_BALLOON1, objBalloons[temp].fXpos, objBalloons[temp].fYpos,
					objBalloons[temp].fXpos + objBalloons[temp].fWidth, objBalloons[temp].fYpos + objBalloons[temp].fHeight,
					fW_BalloonUVTable[1], fW_BalloonUVTable[0],fW_BalloonUVTable[0], fW_BalloonUVTable[1],
					objBalloons[temp].iAlpha, objBalloons[temp].iRed, objBalloons[temp].iGreen, objBalloons[temp].iBlue );

				//	台詞
				CustomDraw( GIRLS_WORDS, objWords[temp].fXpos, objWords[temp].fYpos,
					objWords[temp].fXpos + objWords[temp].fWidth, objWords[temp].fYpos + objWords[temp].fHeight,
					fWordsUVTable[WIN_WORDS][0][0], fWordsUVTable[WIN_WORDS][0][1],
					fWordsUVTable[WIN_WORDS][1][0], fWordsUVTable[WIN_WORDS][1][1] );				
				
				break;

			case CHARA_BOY:	//	男性
				//	吹き出し描画
				if ( temp == 0 )
				CustomCorolDraw( WORDS_BALLOON1, objBalloons[temp].fXpos, objBalloons[temp].fYpos,
					objBalloons[temp].fXpos + objBalloons[temp].fWidth, objBalloons[temp].fYpos + objBalloons[temp].fHeight,
					fW_BalloonUVTable[0], fW_BalloonUVTable[0],	fW_BalloonUVTable[1], fW_BalloonUVTable[1],
					objBalloons[temp].iAlpha, objBalloons[temp].iRed, objBalloons[temp].iGreen, objBalloons[temp].iBlue );

				if ( temp == 1 )
				CustomCorolDraw( WORDS_BALLOON1, objBalloons[temp].fXpos, objBalloons[temp].fYpos,
					objBalloons[temp].fXpos + objBalloons[temp].fWidth, objBalloons[temp].fYpos + objBalloons[temp].fHeight,
					fW_BalloonUVTable[1], fW_BalloonUVTable[0],fW_BalloonUVTable[0], fW_BalloonUVTable[1],
					objBalloons[temp].iAlpha, objBalloons[temp].iRed, objBalloons[temp].iGreen, objBalloons[temp].iBlue );

				//	台詞
				CustomDraw( BOYS_WORDS, objWords[temp].fXpos, objWords[temp].fYpos,
					objWords[temp].fXpos + objWords[temp].fWidth, objWords[temp].fYpos + objWords[temp].fHeight,
					fWordsUVTable[WIN_WORDS][0][0], fWordsUVTable[WIN_WORDS][0][1],
					fWordsUVTable[WIN_WORDS][1][0], fWordsUVTable[WIN_WORDS][1][1] );
				
				break;

			case CHARA_ROBO:	//	ロボット

				break;
			}
		}
	}
}

