//=============================================================================
//
// バトル画面処理 [battle.cpp]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"

#include "battle.h"
#include "battleui.h"
#include "battlecursor.h"

#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "playerknight.h"


#include "enemy.h"
#include "enemyskeletons.h"

#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "bullet.h"

#include "collision.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// ポーズON/OFF

int g_Turn = TURN_START;		// バトル開始を設定


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	g_Turn = TURN_START;

	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 150, 13.0f, 13.0f);

	// ライトを有効化	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	// 壁の初期化
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// 壁(裏側用の半透明)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// 弾の初期化
	InitBullet();

	// カーソルの初期化
	InitBattleCursor();

	// スコアの初期化
	InitBattleUI();

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_battle);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle(void)
{
	// スコアの終了処理
	UninitBattleUI();

	// カーソルの終了処理
	UninitBattleCursor();

	// 弾の終了処理
	UninitBullet();

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
void UpdateBattle(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	g_bPause = g_bPause ? FALSE : TRUE;
	//}


#endif

	if(g_bPause == FALSE)
		return;

	// 地面処理の更新
	UpdateMeshField();

	// プレイヤーの更新処理
	UpdatePlayer();

	// エネミーの更新処理
	UpdateEnemy();

	// 壁処理の更新
	UpdateMeshWall();

	// 弾の更新処理
	UpdateBullet();

	// 影の更新処理
	UpdateShadow();

	// カーソルの更新処理
	UpdateBattleCursor();

	// スコアの更新処理
	UpdateBattleUI();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle0(void)
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

	// 壁の描画処理
	DrawMeshWall();

	// カーソルの描画処理
	DrawBattleCursor();

	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);

	// UIの描画処理
	DrawBattleUI();


	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);
}


void DrawBattle(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// プレイヤー視点
	pos = GetPlayerKnight()->pos;
	pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている

	SetCameraAT(pos);
	SetCamera();

	switch (g_Turn)
	{
	case TURN_START:

		break;

	case TURN_PLAYER:
		// プレイヤー視点
		pos = GetPlayerKnight()->pos;
		pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている

		SetCameraAT(pos);
		SetCamera();

		break;


	case TURN_ACTION:

		break;

	case TURN_MOVING:

		break;

	case TURN_TARGET:
		break;

	case TURN_ATTACK:
		break;

	case TURN_DEFEND:

		break;

	case TURN_RUN:
		break;

	case TURN_ENEMY:
		// エネミー視点
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている

		SetCameraAT(pos);
		SetCamera();

		break;

		// ゲーム終了時の処理
	case TURN_MAX:

		break;

	}

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawBattle0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawBattle0();

		// エネミー視点
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawBattle0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawBattle0();

		// エネミー視点
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawBattle0();
		break;

	}

}




//=============================================================================
// ターンの設定
//=============================================================================
void SetTurn(int turn)
{

	g_Turn = turn;	// 次のモードをセットしている


}

//=============================================================================
// ターンの取得
//=============================================================================
int GetTurn(void)
{
	return g_Turn;
}
