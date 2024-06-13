//=============================================================================
//
// バトルUI処理 [battleUI.h]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HP_MAX			(999)		// スコアの最大値
#define HP_DIGIT		(3)			// 桁数

// UI
enum {
	UI_START,
	UI_WIN,
	UI_GAMEOVER,

	UI_TURN_NONE,
	UI_TURN_FIRST,
	UI_TURN_SECOND,
	UI_TURN_THIRD,
	UI_TURN_FOURTH,
	UI_TURN_FIFTH,

	UI_HP_ICON_KNIGHT,
	UI_HP_NONE_KNIGHT,
	UI_HP_RED_KNIGHT,
	UI_HP_GREEN_KNIGHT,

	UI_OPTION,

	UI_SELECT,

	UI_MAX
};

// 画像
enum {
	IMAGE_START,
	IMAGE_WIN,
	IMAGE_GAMEOVER,

	IMAGE_TURN_NONE,
	IMAGE_TURN_KNIGHT,
	IMAGE_TURN_SKELETONS,

	IMAGE_ICON_KNIGHT,
	IMAGE_HP_NONE,
	IMAGE_HP_RED,
	IMAGE_HP_GREEN,

	IMAGE_COMMAND,
	IMAGE_CONFIRM,


	UI_IMAGE_MAX
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct UI
{
	BOOL					use;						// TRUE:使っている  FALSE:未使用
	float					w, h;					// 幅と高さ
	XMFLOAT3				pos;						// ポリゴンの座標
	int						texNo;					// テクスチャ番号

};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattleUI(void);
void UninitBattleUI(void);
void UpdateBattleUI(void);
void DrawBattleUI(void);


