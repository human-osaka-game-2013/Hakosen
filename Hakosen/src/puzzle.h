#include "graphic.h"
#define STAGE_WIDTH		5			//�X�e�[�W�̃u���b�N���i���j
#define STAGE_HEIGHT	4			//�X�e�[�W�̃u���b�N���i�c�j			

#define BLOCK_WIDTH	100.f		//�u���b�N�̉���
#define BLOCK_HEIGHT	100.f		//�u���b�N�̏c��

#define STAGE_LEFT	40.f		//�X�e�[�W�̍��[�̍��W
#define STAGE_UP		60.f		//�X�e�[�W�̏�[�̍��W
#define STAGE2_LEFT	740.f		//2p����
#define STAGE2_UP		60.f
#define STAGE_SIZE_X	BLOCK_WIDTH * STAGE_WIDTH	//�X�e�[�W�̃u���b�N�����񂾂Ƃ��̉��̒���
#define STAGE_SIZE_Y	BLOCK_HEIGHT * STAGE_HEIGHT	//�c�̒���

#define CURSOR_WIDTH	100.f		//�J�[�\���̉���
#define CURSOR_HEIGHT	100.f		//�J�[�\���̏c��

#define CHARA_SIZE_X	200.f		//�L�����摜�̒���(���j
#define CHARA_SIZE_Y	200.f		//�L�����摜�̒���(�c�j
#define CHARA_LEFT	40.f		//�L�����摜�̍��[���W
#define CHARA_UP		510.f		//�L�����摜�̏�[���W
#define CHARA2_LEFT	WIDTH - CHARA_LEFT - CHARA_SIZE_X		//2p
#define CHARA2_UP		510.f


#define HPGAUGE_LEFT	316.f		//�g�o�Q�[�W�̍��[
#define HPGAUGE_UP		610.f		//�g�o�Q�[�W�̏�[
#define HPGAUGE2_LEFT	644.f		//2p
#define HPGAUGE2_UP		610.f		//2p
#define HPGAUGE_WIDTH	320.f		//�g�o�Q�[�W�̉�����
#define	HPGAUGE_HEIGHT	80.f		//�g�o�Q�[�W�̏c����
#define HP_MAX			100.f
#define ALLDELDAMAGE	10.f		//�S�����̂Ƃ��̃_���[�W

#define SUNDERCOUNT		60			//�d���̖��x��������n�߂�܂ł̎���
#define SUNDERDEL		10			//�d���̖��x��������X�s�[�h

#define BOX_WIDTH		580.f
#define BOX_HEIGHT		520.f

#define BOX_LEFT		STAGE_LEFT - 70.f
#define BOX_UP			STAGE_UP - 70.f

#define DROPSPEED		10.f			//�u���b�N���������Ƃ��̗����A�j���[�V�������x
#define CHANGESPEED		15.f			//��Ռ����X�s�[�h
#define TURNSPEED		20.f			//�u���b�N��]���x
#define CURSORSPEED		20.f			//�J�[�\���ړ����x
//[��{�l+�A����*C_DAMAGE+P_DAMAGE]*��Փx
#ifdef _DEBUG
#define N_DAMAGE		5.f			//�_���[�W�̊�{�l�@
#else
#define N_DAMAGE		5			//�_���[�W�̊�{�l�@
#endif
#define C_DAMAGE		2.f			//�`�F�C���_���[�W
#define P_DAMAGE		1.f			//���o�͈ˑ��̃_���[�W

#define HPREDUCESPEED	0.5f

#define NEXTBLOCK_WIDTH		50.f
#define NEXTBLOCK_HEIGHT	50.f
#define NEXTBLOCK_LENGS		100.f
#define	NEXTBLOCK_X			60.f
#define NEXTBLOCK2_X		760.f
#define NEXTBLOCK_Y			25.f
//�����w�G�t�F�N�g
#define MAJIC_SPEED 10.f
#define MAJIC_MAX 5
#define MAJIC_LENGTH 50.f
#define MAJIC_SCALE 0.4f

//�U���G�t�F�N�g
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
//�v���g�^�C�v�錾
void PuzzleInit();		//�p�Y����ʏ�����
void PuzzleControl();	//�p�Y����ʏ���
void PuzzleDraw();		//�p�Y����ʏ���
bool CheckAttack(int,bool,bool);	//�ʓd�`�F�b�N
bool BlockDrop( int  );	//�u���b�N���Ŏ��̃u���b�N����
void AddBlock( int );
void BlockReset( int );	//�u���b�N����
void ChangeAnime();

bool GetPlayerDeath(int);
int GetPlayerType( int _pn );

//�v���C���[�\����
struct PLAYER_INFO
{
	D3DXVECTOR2		Picturepos;		//�L�����摜�p���W
	int				iCursor[2];		//�J�[�\�������񉽍s�ɂ��邩
	int				iGocursor[2];	//���̃J�[�\���̏ꏊ
	int				cursordirection;//�J�[�\�����ǂ����Ɉړ�����̂�
	bool			cursormoveflag;	//�J�[�\���ړ������ǂ����̃t���O
	float			fHp;			//�v���C���[�̂g�o
	bool			bAttackflag;	//���d�����ǂ����̃t���O
	float			fCursor_X;		//�J�[�\���̂w���W(�`��p�j
	float			fCursor_Y;		//�J�[�\���̂x���W(�`��p�j
	float			fPlayer_X;		//�v���C���[�摜�̍��W
	float			fPlayer_Y;		
	int				charatype;		//�j�����{�b�g�ǂꂩ
	int				charastate;		//�����@�U���@�_���[�W��Ԃǂꂩ
	int				chain;			//�A����
	float			givedamage;		//�^����_���[�W�ۑ�
	int				difficult;		//��Փx�@�Ì��@���h�@���h�@�^�_���{���������
	int				animetime;
	float			animewidth;
	int				goinganime;		//�ǂ̉摜�ɂȂ낤�Ƃ��Ă���̂���ۑ�
	int				animestep;		//�A�j���[�V�����Ǘ��p
	int				blockchangeflag;//�u���b�N����ւ��A�j���[�V�����p
	int				nextset;
	bool			turnflag;
	float			goingHP;		//�g�o�ǂ��܂łւ邩
	bool			HPreduceflag;
	float			BOXPOS_X;
	float			BOXPOS_Y;
	float			BOXMOZI_X;
	float			BOXMOZI_Y;
};
//�u���b�N�\����
struct BLOCK
{
	bool	runflag;
	int		type;								//�u���b�N�̌^
	int		direction;							//�u���b�N�̌���
	int		root[4];							//�u���b�N�̐i�s�\����
	float	angle;								//�u���b�N�̊p�x
	int		pos[STAGE_WIDTH][STAGE_HEIGHT];		//�u���b�N�̃X�e�[�W��̏ꏊ
	bool	lightflag;							//���̃u���b�N���d���̗����u���b�N���ǂ���
	int		dropcount;							//�u���b�N���������Ƃ����i������̂��̃J�E���g
	bool	dropflag;							//�ǂ̃u���b�N��������̂��̃t���O
	D3DXVECTOR2 goalpos;						//�ǂ��܂ŗ�����̂�
	bool	moveflag;							//�����Ă邩�ǂ����̃t���O
	bool	sunderflag;							//�d���������t���O
	int		sunderalpha;						//�d���̃��l
	int		blockalpha;							//�u���b�N�̃��l
	bool	sunderstep;							//���x���ǂ���ɂނ����Ă�����
	int		sundercount;						//���x���Ⴍ�Ȃ�܂ł̊�
	bool	lightline;							//���ʓd����΂ǂ���������̂��m�F
	bool	turnflag;							//�^�[������
	int		nextturn;							//��]��̌�����
	bool	turndirection;						//�ǂ����ނ��̉�]��
};
//�X�e�[�W�P�}�X�̍\����
struct STAGE
{
	BLOCK	block;
	float	x,y;
};