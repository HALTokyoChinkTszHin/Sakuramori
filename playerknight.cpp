//=============================================================================
//
// モデル処理 [playerknight.cpp]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "playerknight.h"
#include "fade.h"
#include "shadow.h"
#include "bullet.h"
#include "debugproc.h"
#include "meshfield.h"
#include "collision.h"

#include "enemy.h"
#include "enemyskeletons.h"


#include "battle.h"
#include "battlecursor.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_KNIGHT				"data/MODEL/knight/knight_body.obj"				// 読み込むモデル名
#define	MODEL_KNIGHT_HEAD			"data/MODEL/knight/knight_head.obj"				// 読み込むモデル名
#define	MODEL_KNIGHT_ARM_LEFT		"data/MODEL/knight/knight_arm_l.obj"			// 読み込むモデル名
#define	MODEL_KNIGHT_ARM_RIGHT		"data/MODEL/knight/knight_arm_r.obj"			// 読み込むモデル名
#define	MODEL_KNIGHT_LEG_LEFT		"data/MODEL/knight/knight_leg_l.obj"			// 読み込むモデル名
#define	MODEL_KNIGHT_LEG_RIGHT		"data/MODEL/knight/knight_leg_r.obj"			// 読み込むモデル名
#define	MODEL_KNIGHT_SWORD			"data/MODEL/knight/knight_sword.obj"			// 読み込むモデル名
#define	MODEL_KNIGHT_SHIELD			"data/MODEL/knight/knight_shield.obj"			// 読み込むモデル名







//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER		g_Knight;						// プレイヤー

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// プレイヤーのパーツ用

static float		roty = 0.0f;

static LIGHT		g_Light;

// プレイヤーの階層アニメーションデータ
// プレイヤーの待機中のアニメデータ
static INTERPOLATION_DATA stay_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA stay_tbl_arm_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA stay_tbl_arm_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA stay_tbl_leg_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA stay_tbl_leg_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA stay_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA stay_tbl_shield[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};









 
// プレイヤーの移動中のアニメデータ
static INTERPOLATION_DATA move_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA move_tbl_arm_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA move_tbl_arm_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA move_tbl_leg_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 6, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 6, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA move_tbl_leg_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 6, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 6, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA move_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA move_tbl_shield[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};








// プレイヤーの攻撃中のアニメデータ
static INTERPOLATION_DATA attack_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA attack_tbl_arm_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },


};

static INTERPOLATION_DATA attack_tbl_arm_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 4, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 100 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 100 },

};


static INTERPOLATION_DATA attack_tbl_leg_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA attack_tbl_leg_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA attack_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA attack_tbl_shield[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};






// プレイヤーの防御中のアニメデータ
static INTERPOLATION_DATA defense_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA defense_tbl_arm_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 4, -XM_PI / 2, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 4, -XM_PI / 2, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },


};

static INTERPOLATION_DATA defense_tbl_arm_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },

};


static INTERPOLATION_DATA defense_tbl_leg_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA defense_tbl_leg_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA defense_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA defense_tbl_shield[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	stay_tbl_head,
	stay_tbl_arm_l,
	stay_tbl_arm_r,
	stay_tbl_leg_l,
	stay_tbl_leg_r,
	stay_tbl_sword,
	stay_tbl_shield,

	

	move_tbl_head,
	move_tbl_arm_l,
	move_tbl_arm_r,
	move_tbl_leg_l,
	move_tbl_leg_r,
	move_tbl_sword,
	move_tbl_shield,



	attack_tbl_head,
	attack_tbl_arm_l,
	attack_tbl_arm_r,
	attack_tbl_leg_l,
	attack_tbl_leg_r,
	attack_tbl_sword,
	attack_tbl_shield,



	defense_tbl_head,
	defense_tbl_arm_l,
	defense_tbl_arm_r,
	defense_tbl_leg_l,
	defense_tbl_leg_r,
	defense_tbl_sword,
	defense_tbl_shield,


};






//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayerKnight(void)
{
	g_Knight.load = TRUE;
	LoadModel(MODEL_KNIGHT, &g_Knight.model);

	int g_Mode = GetMode();
	if (g_Mode == MODE_GAME)
	{
		g_Knight.pos = XMFLOAT3(-600.0f, PLAYER_OFFSET_Y + 50.0f, -600.0f);
	}
	else if (g_Mode == MODE_BATTLE)
	{
		g_Knight.pos = XMFLOAT3(10.0f, PLAYER_OFFSET_Y + 50.0f, 0.0f);
	}


	g_Knight.originPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_Knight.rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Knight.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_Knight.movSpd = 0.0f;			// 移動スピードクリア
	g_Knight.dir = CHAR_DIR_OUTSIDE;


	g_Knight.use = TRUE;			// TRUE:生きてる

	// パラメータの設定
	g_Knight.hpMax = 100;
	g_Knight.hp = g_Knight.hpMax;

	g_Knight.str = 20;
	g_Knight.def = 20;
	g_Knight.spd = 20;


	g_Knight.size = PLAYER_SIZE;	// 当たり判定の大きさ

	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_Knight.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Knight.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号

	g_Knight.time = 0.0f;			// 線形補間用のタイマーをクリア
	g_Knight.motion = MOTIONSTAY;

	// キーを押した時のプレイヤーの向き
	roty = 0.0f;

	g_Knight.parent = NULL;			// 本体（親）なのでNULLを入れる


	// 階層アニメーションの初期化
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// 親子関係
		g_Parts[i].parent = &g_Knight;		// ← ここに親のアドレスを入れる
	//	g_Parts[腕].parent= &g_Knight;		// 腕だったら親は本体（プレイヤー）
	//	g_Parts[手].parent= &g_Paerts[腕];	// 指が腕の子供だった場合の例

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Parts[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Parts[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		// パーツの読み込みはまだしていない
		g_Parts[i].load = FALSE;
	}

	g_Parts[PARTHEAD].use = TRUE;
	g_Parts[PARTHEAD].parent = &g_Knight;	// 親をセット
	g_Parts[PARTHEAD].tblNo = PARTHEAD;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[PARTHEAD].tblMax = sizeof(move_tbl_head) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[PARTHEAD].load = TRUE;
	LoadModel(MODEL_KNIGHT_HEAD, &g_Parts[PARTHEAD].model);

	g_Parts[PARTARM_L].use = TRUE;
	g_Parts[PARTARM_L].parent = &g_Knight;	// 親をセット
	g_Parts[PARTARM_L].tblNo = PARTARM_L;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[PARTARM_L].tblMax = sizeof(move_tbl_arm_l) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[PARTARM_L].load = TRUE;
	LoadModel(MODEL_KNIGHT_ARM_LEFT, &g_Parts[PARTARM_L].model);

	g_Parts[PARTARM_R].use = TRUE;
	g_Parts[PARTARM_R].parent = &g_Knight;	// 親をセット
	g_Parts[PARTARM_R].tblNo = PARTARM_R;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[PARTARM_R].tblMax = sizeof(move_tbl_arm_r) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[PARTARM_R].load = TRUE;
	LoadModel(MODEL_KNIGHT_ARM_RIGHT, &g_Parts[PARTARM_R].model);

	g_Parts[PARTLEG_L].use = TRUE;
	g_Parts[PARTLEG_L].parent = &g_Knight;	// 親をセット
	g_Parts[PARTLEG_L].tblNo = PARTLEG_L;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[PARTLEG_L].tblMax = sizeof(move_tbl_leg_l) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[PARTLEG_L].load = TRUE;
	LoadModel(MODEL_KNIGHT_LEG_LEFT, &g_Parts[PARTLEG_L].model);

	g_Parts[PARTLEG_R].use = TRUE;
	g_Parts[PARTLEG_R].parent = &g_Knight;	// 親をセット
	g_Parts[PARTLEG_R].tblNo = PARTLEG_R;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[PARTLEG_R].tblMax = sizeof(move_tbl_leg_r) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[PARTLEG_R].load = TRUE;
	LoadModel(MODEL_KNIGHT_LEG_RIGHT, &g_Parts[PARTLEG_R].model);

	g_Parts[PARTSWORD].use = TRUE;
	g_Parts[PARTSWORD].parent = &g_Parts[PARTARM_R];	// 親をセット
	g_Parts[PARTSWORD].tblNo = PARTSWORD;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[PARTSWORD].tblMax = sizeof(move_tbl_sword) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[PARTSWORD].load = TRUE;
	LoadModel(MODEL_KNIGHT_SWORD, &g_Parts[PARTSWORD].model);

	g_Parts[PARTSHIELD].use = TRUE;
	g_Parts[PARTSHIELD].parent = &g_Parts[PARTARM_L];	// 親をセット
	g_Parts[PARTSHIELD].tblNo = PARTSHIELD;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[PARTSHIELD].tblMax = sizeof(move_tbl_shield) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Parts[PARTSHIELD].load = TRUE;
	LoadModel(MODEL_KNIGHT_SHIELD, &g_Parts[PARTSHIELD].model);


	// クォータニオンの初期化
	XMStoreFloat4(&g_Knight.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayerKnight(void)
{
	// モデルの解放処理
	if (g_Knight.load == TRUE)
	{
		UnloadModel(&g_Knight.model);
		g_Knight.load = FALSE;
	}

	// パーツの解放処理
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// パーツの解放処理
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}



}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayerKnight(void)
{
	CAMERA *cam = GetCamera();
	ENEMY* skeletons = GetEnemySkeletons();		// エネミーのポインターを初期化

	//g_Knight.movSpd *= 0.7f;
	int g_Mode = GetMode();
	int g_Turn = GetTurn();

	switch (g_Mode)
	{
	case MODE_TITLE:
		break;

	case MODE_GAME:

		// ゲームクリア
		if ((g_Knight.pos.x > 80) && (g_Knight.pos.z > 360))
		{
			SetFade(FADE_OUT, MODE_TITLE);

		}

		// 移動前の座標を保存
		g_Knight.originPos = g_Knight.pos;

		// 向きを変更
		changeDirection(&roty, g_Knight.dir, ROTATION_SPEED);

		// Key入力があったら移動処理する
		if (g_Knight.movSpd > 0.0f)
		{	// 押した方向にプレイヤーを移動させる
			// 押した方向にプレイヤーを向かせている所
			g_Knight.rot.y = roty + cam->rot.y;

			g_Knight.pos.x -= sinf(g_Knight.rot.y) * g_Knight.movSpd;
			g_Knight.pos.z -= cosf(g_Knight.rot.y) * g_Knight.movSpd;
		}

		g_Knight.movSpd *= 0.5f;

		if ((g_Knight.movSpd < PLAYER_VALUE_MOVE / 4) && (g_Knight.motion != MOTIONATTACK))
		{
			g_Knight.motion = MOTIONSTAY;
		}

		break;


	case MODE_BATTLE:
		switch (g_Turn)
		{
		case TURN_START:
			g_Knight.motion = MOTIONSTAY;

			break;

		case TURN_PLAYER:

			break;


		case TURN_ACTION:
			g_Knight.motion = MOTIONSTAY;

			break;

		case TURN_MOVING:
			// 移動処理
			{
				XMFLOAT3 moving_pos = GetMovingPos();

				float moving_distance = VectorDistance(g_Knight.pos, moving_pos);

				if (moving_distance < MOVING_RANGE)
				{

					g_Knight.motion = MOTIONMOVE;

					if (moving_pos.x != 0.0)	// 線形補間を実行する？
					{	// 線形補間の処理

						XMVECTOR nowPos = XMLoadFloat3(&g_Knight.pos);	// XMVECTORへ変換

						XMVECTOR Pos = XMLoadFloat3(&moving_pos) - nowPos;	// XYZ移動量を計算している

						Pos *= g_Knight.time;								// 現在の移動量を計算している

						// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
						XMStoreFloat3(&g_Knight.pos, nowPos + Pos);

						float angle = atan2f(g_Knight.pos.x - moving_pos.x, g_Knight.pos.z - moving_pos.z);							//(yの差分、ⅹの差分)その差分を使って角度を求めている
						g_Knight.rot.y = angle;

						// frameを使て時間経過処理をする
						g_Knight.time += 1.0f / 100.0f;	// 時間を進めている

						if (g_Knight.time >= 1.0f)
						{

							if ((g_Knight.pos.x == moving_pos.x) &&
								(g_Knight.pos.z == moving_pos.z))// 線形補間を止める？
							{
								SetMovingPos(XMFLOAT3(0.0f, 0.0f, 0.0f));

								SetTurn(TURN_ENEMY);

							}

							g_Knight.time = 0.0f;
						}

					}

				}



			}


				break;

		case TURN_TARGET:
			break;

		case TURN_ATTACK:
			// 線形補間の処理
		{	// エネミーに近く
			BOOL moveStop = FALSE;

			if (moveStop == FALSE)
			{

				XMVECTOR nowPos = XMLoadFloat3(&g_Knight.pos);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&skeletons[0].pos) - nowPos;	// XYZ移動量を計算している

				Pos *= g_Knight.time;								// 現在の移動量を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Knight.pos, nowPos + Pos);

				float angle = atan2f(g_Knight.pos.x - skeletons[0].pos.x, g_Knight.pos.z - skeletons[0].pos.z);							//(yの差分、ⅹの差分)その差分を使って角度を求めている
				g_Knight.rot.y = angle;

				// frameを使て時間経過処理をする
				g_Knight.time += 1.0f / 100.0f;	// 時間を進めている

				if (g_Knight.time >= 1.0f)
				{
					g_Knight.time = 0.0f;
				}

			}

			// 敵とプレイヤーキャラ
			for (int i = 0; i < MAX_SKELETONS; i++)
			{
				//敵の有効フラグをチェックする
				if (skeletons[i].use == FALSE)
					continue;

				//BCの当たり判定
				if (CollisionBC(g_Knight.pos, skeletons[i].pos, g_Knight.size * 2, skeletons[i].size * 2))
				{
					// エネミーの近くに止まる
					moveStop = TRUE;

				}
			}

			if (moveStop == TRUE)
			{
				moveStop = FALSE;

				g_Knight.motion = MOTIONATTACK;
				SetEnemySkeletonsDamage(5);
				SetTurn(TURN_ENEMY);

			}

		}
			break;

		case TURN_DEFEND:
			g_Knight.motion = MOTIONDEFENSE;

			break;

		case TURN_RUN:
			break;

		case TURN_ENEMY:
			break;

			// ゲーム終了時の処理
		case TURN_MAX:

			break;
			
		}


		break;

		// ゲーム終了時の処理
	case MODE_MAX:
		break;
	}




	// レイキャストして足元の高さを求める
	XMFLOAT3 HitPosition;		// 交点
	XMFLOAT3 Normal;			// ぶつかったポリゴンの法線ベクトル（向き）
	BOOL ans = RayHitField(g_Knight.pos, &HitPosition, &Normal);
	if (ans)
	{
		g_Knight.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
	}
	else
	{
		g_Knight.pos.y = PLAYER_OFFSET_Y;
		Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos = g_Knight.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Knight.shadowIdx, pos);


	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// 使われているなら処理する
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tblMax > 0))
		{	
			//アニメーションの変換
			for (int i = 0; i < PARTMAX; i++)
			{
				g_Parts[i].tblNo = i + PARTMAX * g_Knight.motion;
			}

			// 線形補間の処理
			int nowNo = (int)g_Parts[i].time;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Parts[i].tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
			INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// 行動テーブルのアドレスを取得

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

			float nowTime = g_Parts[i].time - nowNo;	// 時間部分である少数を取り出している

			Pos *= nowTime;								// 現在の移動量を計算している
			Rot *= nowTime;								// 現在の回転量を計算している
			Scl *= nowTime;								// 現在の拡大率を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
			XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

			// 計算して求めた回転量を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

			// 計算して求めた拡大率を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

			// frameを使て時間経過処理をする
			g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
			if ((int)g_Parts[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
			{
				g_Parts[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
			}

		}

	}


	//// ポイントライトのテスト
	//{
	//	LIGHT *light = GetLightData(1);
	//	XMFLOAT3 pos = g_Knight.pos;
	//	pos.y += 20.0f;

	//	light->Position = pos;
	//	light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Type = LIGHT_TYPE_POINT;
	//	light->Enable = TRUE;
	//	SetLightData(1, light);
	//}



	//////////////////////////////////////////////////////////////////////
	// 姿勢制御
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_Knight.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Knight.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	//nvx = vx / len;
	angle = asinf(len);

	//quat = XMQuaternionIdentity();

//	quat = XMQuaternionRotationAxis(nvx, angle);
	quat = XMQuaternionRotationNormal(nvx, angle);


	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Knight.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_Knight.Quaternion, quat);




#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_Knight.pos.x, g_Knight.pos.y, g_Knight.pos.z, Normal.y);
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayerKnight(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Knight.scl.x, g_Knight.scl.y, g_Knight.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Knight.rot.x, g_Knight.rot.y + XM_PI, g_Knight.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// クォータニオンを反映
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Knight.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Knight.pos.x, g_Knight.pos.y, g_Knight.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Knight.mtxWorld, mtxWorld);


	//// 縁取りの設定
	//SetFuchi(1);

	// モデル描画
	DrawModel(&g_Knight.model);



	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ↑
			// g_Knight.mtxWorldを指している
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する
		if (g_Parts[i].use == FALSE) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);


		// モデル描画
		DrawModel(&g_Parts[i].model);

	}

	//SetFuchi(0);

	// αテストを有効に
	SetAlphaTestEnable(TRUE);

	// ライティングを無効
	SetLightEnable(FALSE);


}

//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayerKnight(void)
{
	return &g_Knight;
}






//=============================================================================
// プレイヤーにダメージ
//=============================================================================
void SetPlayerKnightDamage(float damge)
{
	g_Knight.hp -= damge;
}



