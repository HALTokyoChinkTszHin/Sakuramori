//=============================================================================
//
// 弾発射処理 [bullet.h]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET		(10)	// 弾最大数

#define	BULLET_SIZE		(10.0f)				// 当たり判定の大きさ

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4	mtxWorld;		// ワールドマトリックス
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	rot;			// 角度
	XMFLOAT3	scl;			// スケール

	MATERIAL	material;		// マテリアル
	float		s;				//線形補間用パラメータｓ

	float		spd;			// 移動量
	float		size;			// 当たり判定の大きさ



	int			shadowIdx;		// 影ID
	BOOL		use;			// 使用しているかどうか
	bool		load;
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色


} BULLET;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);

BULLET *GetBullet(void);
