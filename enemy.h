//=============================================================================
//
// エネミーモデル処理 [enemy.h]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BATTLE_ENEMY		(5)					// エネミーの数

#define	ENEMY_SIZE		(5.0f)				// 当たり判定の大きさ

#define	ENEMY_VALUE_MOVE	(5.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define ENEMY_OFFSET_Y		(0.0f)							// エネミーの足元をあわせる

#define VIEW_ANGLE			(90)							// 視野角
#define VIEW_DISTANCE		(100)							// 視野の距離

enum
{
	// 戦闘AI行動
	ENEMY_BATTLE_MOVE,
	ENEMY_BATTLE_ATTACK,
	ENEMY_BATTLE_SKILL,

	ENEMY_BATTLE_MAX
};
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)

	BOOL				use;
	BOOL				load;

	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	// エネミーのパラメータ
	float				hpMax;				// HPの最大値
	float				hp;					// HPの残量
	float				str;				// 攻撃力
	float				def;				// 防御力
	float				spd;				// 速度

	float				movSpd;				// 移動スピード
	float				size;				// 当たり判定の大きさ
	int					shadowIdx;			// 影のインデックス番号

	BOOL				vision;				// 視野範囲内にキャラ存在？
	BOOL				lost;				// 視野範囲内にキャラ見失った？

	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数

};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);