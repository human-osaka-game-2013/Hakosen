#include "graphic.h"
#include "game.h"
#include "input.h"
#include "scene.h"
#include"puzzle.h"
#include"Result.h"
#include "words.h"

CUSTOMVERTEX game[4];
bool ResultFlag = false;


//	メインゲームの初期化
void GameInit ()
{
	PuzzleInit();

	WordInit ();

	ResultFlag = false;
}

//	メインゲームの処理
void GameControl ()
{
	if(!ResultFlag)
		PuzzleControl();
	else 
		ChangeScene(RESULT_SCENE);

	for(int i = 0 ; i < 2 ;i++)
		if( GetPlayerDeath(i) )
			ResultFlag = true;
}

//	メインゲームの描画
void GameDraw ()
{
	PuzzleDraw();

	WordDraw ();
}