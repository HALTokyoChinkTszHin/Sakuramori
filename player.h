//=============================================================================
//
// モデル処理 [player.h]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_PLAYER			(1)							
#define	PLAYER_SIZE			(5.0f)							// 当たり判定の大きさ

#define MOVING_RANGE		(75)		

#define PLAYER_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(10.0f)							// プレイヤーの足元をあわせる


#define	PLAYER_VALUE_MOVE			(2.0f)							// 移動量

#define	ROTATION_SPEED		(0.2f)							// 回転量速度


#define PLAYER_PARTS_MAX	(7)								// プレイヤーのパーツの数

enum
{
	// プレイヤーのパーツの数
	PARTHEAD,
	PARTARM_L,
	PARTARM_R,
	PARTLEG_L,
	PARTLEG_R,
	PARTSWORD,
	PARTSHIELD,

	PARTMAX

};

enum {
	//動作
	MOTIONSTAY,
	MOTIONMOVE,
	MOTIONATTACK,
	MOTIONDEFENSE,
	//MOTIONJUMP,


	MOTIONMAX

};


enum
{
	//方向
	CHAR_DIR_INSIDE,
	CHAR_DIR_RIGHT,
	CHAR_DIR_OUTSIDE,
	CHAR_DIR_LEFT,

	CHAR_DIR_MAX
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PLAYER
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			originPos;			// 移動前の位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)

	float				movSpd;				// 移動スピード
	
	BOOL				load;
	DX11_MODEL			model;				// モデル情報

	int					shadowIdx;			// 影のインデックス番号
	int					dir;				// 向き（0:上 1:右 2:下 3:左）

	BOOL				use;

	// プレイヤーのパラメータ
	float				hpMax;				// HPの最大値
	float				hp;					// HPの残量
	float				str;				// 攻撃力
	float				def;				// 防御力
	float				spd;				// 速度
	float				size;				// 当たり判定の範囲

	// 階層アニメーション用のメンバー変数
	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数
	int					motion;				// プレイヤーの動作

	// 親は、NULL、子供は親のアドレスを入れる
	PLAYER				*parent;			// 自分が親ならNULL、自分が子供なら親のplayerアドレス

	// クォータニオン
	XMFLOAT4			Quaternion;

	XMFLOAT3			UpVector;			// 自分が立っている所

};


struct RANGE
{
	XMFLOAT4X4	mtxWorld;		// ワールドマトリックス
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	rot;			// 角度
	XMFLOAT3	scl;			// スケール
	MATERIAL	material;		// マテリアル
	BOOL		use;			// 使用しているかどうか

};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void changeDirection(float* roty, int targetDir, float rotationSpeed);
