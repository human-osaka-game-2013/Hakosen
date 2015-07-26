#include "graphic.h"
#define STAGE_WIDTH		5			//ステージのブロック数（横）
#define STAGE_HEIGHT	4			//ステージのブロック数（縦）			

#define BLOCK_WIDTH	100.f		//ブロックの横幅
#define BLOCK_HEIGHT	100.f		//ブロックの縦幅

#define STAGE_LEFT	40.f		//ステージの左端の座標
#define STAGE_UP		60.f		//ステージの上端の座標
#define STAGE2_LEFT	740.f		//2pがわ
#define STAGE2_UP		60.f
#define STAGE_SIZE_X	BLOCK_WIDTH * STAGE_WIDTH	//ステージのブロックが並んだときの横の長さ
#define STAGE_SIZE_Y	BLOCK_HEIGHT * STAGE_HEIGHT	//縦の長さ

#define CURSOR_WIDTH	100.f		//カーソルの横幅
#define CURSOR_HEIGHT	100.f		//カーソルの縦幅

#define CHARA_SIZE_X	200.f		//キャラ画像の長さ(横）
#define CHARA_SIZE_Y	200.f		//キャラ画像の長さ(縦）
#define CHARA_LEFT	40.f		//キャラ画像の左端座標
#define CHARA_UP		510.f		//キャラ画像の上端座標
#define CHARA2_LEFT	WIDTH - CHARA_LEFT - CHARA_SIZE_X		//2p
#define CHARA2_UP		510.f


#define HPGAUGE_LEFT	316.f		//ＨＰゲージの左端
#define HPGAUGE_UP		610.f		//ＨＰゲージの上端
#define HPGAUGE2_LEFT	644.f		//2p
#define HPGAUGE2_UP		610.f		//2p
#define HPGAUGE_WIDTH	320.f		//ＨＰゲージの横長さ
#define	HPGAUGE_HEIGHT	80.f		//ＨＰゲージの縦長さ
#define HP_MAX			100.f
#define ALLDELDAMAGE	10.f		//全けしのときのダメージ

#define SUNDERCOUNT		60			//電流の明度が下がり始めるまでの時間
#define SUNDERDEL		10			//電流の明度が下がるスピード

#define BOX_WIDTH		580.f
#define BOX_HEIGHT		520.f

#define BOX_LEFT		STAGE_LEFT - 70.f
#define BOX_UP			STAGE_UP - 70.f

#define DROPSPEED		10.f			//ブロックを消したときの落下アニメーション速度
#define CHANGESPEED		15.f			//基盤交換スピード
#define TURNSPEED		20.f			//ブロック回転速度
#define CURSORSPEED		20.f			//カーソル移動速度
//[基本値+連鎖数*C_DAMAGE+P_DAMAGE]*難易度
#ifdef _DEBUG
#define N_DAMAGE		5.f			//ダメージの基本値　
#else
#define N_DAMAGE		5			//ダメージの基本値　
#endif
#define C_DAMAGE		2.f			//チェインダメージ
#define P_DAMAGE		1.f			//入出力依存のダメージ

#define HPREDUCESPEED	0.5f

#define NEXTBLOCK_WIDTH		50.f
#define NEXTBLOCK_HEIGHT	50.f
#define NEXTBLOCK_LENGS		100.f
#define	NEXTBLOCK_X			60.f
#define NEXTBLOCK2_X		760.f
#define NEXTBLOCK_Y			25.f
//魔方陣エフェクト
#define MAJIC_SPEED 10.f
#define MAJIC_MAX 5
#define MAJIC_LENGTH 50.f
#define MAJIC_SCALE 0.4f

//攻撃エフェクト
#define VOLT_X (float)(WIDTH/2)
#define VOLT_Y (float)(HEIGHT/2)

#define TABLELOOPNUM 21

enum BLOCKTYPE
{
	B_TYPE1,
	B_TYPE2,
	B_TYPE3,
	B_TYPE4,
	B_TYPE5,
	B_TYPEMAX,
};
enum DIRECTION
{
	D_TOP,
	D_RIGHT,
	D_BOT,
	D_LEFT,

	DIRECTION_MAX,
};
//-------------------------------
//プロトタイプ宣言
void PuzzleInit();		//パズル画面初期化
void PuzzleControl();	//パズル画面処理
void PuzzleDraw();		//パズル画面処理
bool CheckAttack(int,bool,bool);	//通電チェック
bool BlockDrop( int  );	//ブロック消滅時のブロック落下
void AddBlock( int );
void BlockReset( int );	//ブロック交換
void ChangeAnime();

bool GetPlayerDeath(int);
int GetPlayerType( int _pn );

//プレイヤー構造体
struct PLAYER_INFO
{
	D3DXVECTOR2		Picturepos;		//キャラ画像用座標
	int				iCursor[2];		//カーソルが何列何行にいるか
	int				iGocursor[2];	//次のカーソルの場所
	int				cursordirection;//カーソルがどっちに移動するのか
	bool			cursormoveflag;	//カーソル移動中かどうかのフラグ
	float			fHp;			//プレイヤーのＨＰ
	bool			bAttackflag;	//放電中かどうかのフラグ
	float			fCursor_X;		//カーソルのＸ座標(描画用）
	float			fCursor_Y;		//カーソルのＹ座標(描画用）
	float			fPlayer_X;		//プレイヤー画像の座標
	float			fPlayer_Y;		
	int				charatype;		//男女ロボットどれか
	int				charastate;		//立ち　攻撃　ダメージ状態どれか
	int				chain;			//連鎖数
	float			givedamage;		//与えるダメージ保存
	int				difficult;		//難易度　甘口　中辛　激辛　与ダメ倍率がかわる
	int				animetime;
	float			animewidth;
	int				goinganime;		//どの画像になろうとしているのかを保存
	int				animestep;		//アニメーション管理用
	int				blockchangeflag;//ブロック入れ替えアニメーション用
	int				nextset;
	bool			turnflag;
	float			goingHP;		//ＨＰどこまでへるか
	bool			HPreduceflag;
	float			BOXPOS_X;
	float			BOXPOS_Y;
	float			BOXMOZI_X;
	float			BOXMOZI_Y;
};
//ブロック構造体
struct BLOCK
{
	bool	runflag;
	int		type;								//ブロックの型
	int		direction;							//ブロックの向き
	int		root[4];							//ブロックの進行可能方向
	float	angle;								//ブロックの角度
	int		pos[STAGE_WIDTH][STAGE_HEIGHT];		//ブロックのステージ上の場所
	bool	lightflag;							//そのブロックが電流の流れるブロックかどうか
	int		dropcount;							//ブロックが消えたとき何段落ちるのかのカウント
	bool	dropflag;							//どのブロックが落ちるのかのフラグ
	D3DXVECTOR2 goalpos;						//どこまで落ちるのか
	bool	moveflag;							//落ちてるかどうかのフラグ
	bool	sunderflag;							//電流が流れるフラグ
	int		sunderalpha;						//電流のα値
	int		blockalpha;							//ブロックのα値
	bool	sunderstep;							//明度がどちらにむかっていくか
	int		sundercount;						//明度が低くなるまでの間
	bool	lightline;							//今通電すればどこが消えるのか確認
	bool	turnflag;							//ターン中か
	int		nextturn;							//回転後の向きは
	bool	turndirection;						//どっちむきの回転か
};
//ステージ１マスの構造体
struct STAGE
{
	BLOCK	block;
	float	x,y;
};