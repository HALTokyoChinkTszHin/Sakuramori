//=============================================================================
//
// 迷路処理 [maze.h]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MAZE_WALL_X		(14)					// 壁の横数
#define MAX_MAZE_WALL_Y		(19)					// 壁の縦数

#define	WALL_SIZE_X		(10.0f)				// 当たり判定の大きさ
#define	WALL_SIZE_Y		(30.0f)				// 当たり判定の大きさ
#define	WALL_SIZE_Z		(5.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct MAZE
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)

	BOOL				use;
	BOOL				vision;

	BOOL				load;

	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	float				movSpd;				// 移動スピード
	float				size;				// 当たり判定の大きさ
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMaze(void);
void UninitMaze(void);
void UpdateMaze(void);
void DrawMaze(void);

MAZE* GetMazeWallWidth();
MAZE* GetMazeWallLength();

