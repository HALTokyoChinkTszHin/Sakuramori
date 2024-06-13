//=============================================================================
//
// バトル画面処理 [battle.h]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#pragma once





//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum {
	TURN_START,
	TURN_PLAYER,
	TURN_ACTION,
	TURN_MOVING,
	TURN_TARGET,
	TURN_ATTACK,
	TURN_DEFEND,
	TURN_RUN,
	TURN_ENEMY,
	TURN_WIN,
	TURN_GAMEOVER,




	TURN_MAX
};




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattle(void);
void UninitBattle(void);
void UpdateBattle(void);
void DrawBattle(void);

void SetTurn(int mode);
int GetTurn(void);
