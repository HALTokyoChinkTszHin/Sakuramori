//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "playerknight.h"

#include "enemy.h"
#include "enemyskeletons.h"

#include "maze.h"

#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "bullet.h"
#include "score.h"
#include "collision.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// ポーズON/OFF


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 150, 13.0f, 13.0f);

	// ライトを有効化	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	// 迷路の初期化
	InitMaze();

	// 弾の初期化
	InitBullet();

	// スコアの初期化
	InitScore();

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_field);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// スコアの終了処理
	UninitScore();

	// 弾の終了処理
	UninitBullet();

	// 迷路の終了処理
	UninitMaze();

	// 壁の終了処理
	UninitMeshWall();

	// 地面の終了処理
	UninitMeshField();

	// エネミーの終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// 影の終了処理
	UninitShadow();

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}


#endif

	if(g_bPause == FALSE)
		return;

	// 地面処理の更新
	UpdateMeshField();

	// プレイヤーの更新処理
	UpdatePlayer();

	// エネミーの更新処理
	UpdateEnemy();

	// 迷路の更新処理
	UpdateMaze();

	// 壁処理の更新
	UpdateMeshWall();

	// 弾の更新処理
	UpdateBullet();

	// 影の更新処理
	UpdateShadow();

	// 当たり判定処理
	CheckHit();

	// スコアの更新処理
	UpdateScore();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame0(void)
{
	// 3Dの物を描画する処理
	// 地面の描画処理
	DrawMeshField();

	// 影の描画処理
	DrawShadow();

	// エネミーの描画処理
	DrawEnemy();

	// プレイヤーの描画処理
	DrawPlayer();

	// 弾の描画処理
	DrawBullet();

	// 迷路の描画処理
	DrawMaze();

	// 壁の描画処理
	DrawMeshWall();

	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);

	// スコアの描画処理
	DrawScore();


	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);
}


void DrawGame(void)
{
	XMFLOAT3 pos;

	PLAYER* knight = GetPlayerKnight();

	if (knight->use == TRUE)
	{
		// 騎士視点
		XMFLOAT3 pos = knight->pos;
		pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
		SetCameraAT(pos);
		SetCamera();
	}

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif


	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	ENEMY *skeletons = GetEnemySkeletons();		// エネミーのポインターを初期化
	PLAYER *knight = GetPlayerKnight();	// プレイヤーのポインターを初期化
	MAZE* mazeWidth = GetMazeWallWidth();	// プレイヤーのポインターを初期化
	MAZE* mazeLength = GetMazeWallLength();	// プレイヤーのポインターを初期化

	BULLET *bullet = GetBullet();	// 弾のポインターを初期化

	// 敵とプレイヤーキャラ
	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		//敵の有効フラグをチェックする
		if (skeletons[i].use == FALSE)
			continue;

		XMFLOAT3 pos_player = knight->pos;
		pos_player.y = 0.0f;
		XMFLOAT3 pos_enemy = skeletons[i].pos;
		pos_enemy.y = 0.0f;

		//BCの当たり判定
		if (CollisionBC(pos_player, pos_enemy, knight->size, skeletons[i].size))
		{
			//// バトルシーンに入る
			SetFade(FADE_OUT, MODE_BATTLE);
		}

	}


	// プレイヤーの弾と敵
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//弾の有効フラグをチェックする
		if (bullet[i].use == FALSE)
			continue;

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_SKELETONS; j++)
		{
			//敵の有効フラグをチェックする
			if (skeletons[j].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(bullet[i].pos, skeletons[j].pos, bullet[i].size, skeletons[j].size))
			{
				// 当たったから未使用に戻す
				bullet[i].use = FALSE;
				ReleaseShadow(bullet[i].shadowIdx);

				// バトルシーンに入る
				SetFade(FADE_OUT, MODE_BATTLE);
			}
		}

	}


}


