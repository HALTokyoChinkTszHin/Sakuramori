//=============================================================================
//
// エネミーモデル処理 [enemyskeletons.cpp]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "collision.h"
#include "camera.h"

#include "enemy.h"
#include "enemyskeletons.h"

#include "player.h"
#include "playerknight.h"

#include "battle.h"

#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_SKELETONS			"data/MODEL/enemy/skeletons_nomal.obj"		// 読み込むモデル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Skeletons[MAX_SKELETONS];				// スケルトン

int g_Skeletons_load = 0;

XMFLOAT3 originalPos[MAX_SKELETONS];

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl0[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(300.0f, ENEMY_OFFSET_Y,  20.0f), XMFLOAT3(0.0f, -XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, 20.0f), XMFLOAT3(0.0f, XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2},

};

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl1[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-600.0f, ENEMY_OFFSET_Y,  50.0f), XMFLOAT3(0.0f, XM_PI , 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-600.0f, ENEMY_OFFSET_Y, -450.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl2[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(370.0f, ENEMY_OFFSET_Y,  -250.0f), XMFLOAT3(0.0f, XM_PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(370.0f, ENEMY_OFFSET_Y, -550.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl3[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(40.0f, ENEMY_OFFSET_Y, 100.0f), XMFLOAT3(0.0f, XM_PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(40.0f, ENEMY_OFFSET_Y, 450.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl4[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, ENEMY_OFFSET_Y,  -550.0f), XMFLOAT3(0.0f, -XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-250.0f, ENEMY_OFFSET_Y, -550.0f), XMFLOAT3(0.0f, XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_g_Skeletons_MoveTbl0,
	g_g_Skeletons_MoveTbl1,
	g_g_Skeletons_MoveTbl2,
	g_g_Skeletons_MoveTbl3,
	g_g_Skeletons_MoveTbl4,

};



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemySkeletons(void)
{
	int g_Mode = GetMode();

	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		LoadModel(MODEL_SKELETONS, &g_Skeletons[i].model);
		g_Skeletons[i].load = TRUE;

		g_Skeletons[i].pos = XMFLOAT3(10.0f + i * 30.0f, ENEMY_OFFSET_Y, 150.0f);
		g_Skeletons[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Skeletons[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// パラメータの設定
		g_Skeletons[i].hpMax = 20;
		g_Skeletons[i].hp = g_Skeletons[i].hpMax;
		g_Skeletons[i].str = 10;
		g_Skeletons[i].def = 10;
		g_Skeletons[i].spd = 10;

		g_Skeletons[i].movSpd = 0.0f;			// 移動スピードクリア
		g_Skeletons[i].size = ENEMY_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Skeletons[i].model, &g_Skeletons[i].diffuse[0]);

		XMFLOAT3 pos = g_Skeletons[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Skeletons[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		
		g_Skeletons[i].vision = FALSE;			// 視野範囲をクリア
		g_Skeletons[i].lost = FALSE;			// プレイヤーを見失った状態を初期化

		g_Skeletons[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Skeletons[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Skeletons[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		g_Skeletons[i].use = TRUE;			// TRUE:生きてる

	}

	 //0番だけ線形補間で動かしてみる
	g_Skeletons[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Skeletons[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	g_Skeletons[0].tblMax = sizeof(g_g_Skeletons_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	//1番だけ線形補間で動かしてみる
	g_Skeletons[1].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Skeletons[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
	g_Skeletons[1].tblMax = sizeof(g_g_Skeletons_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	//2番だけ線形補間で動かしてみる
	g_Skeletons[2].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Skeletons[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
	g_Skeletons[2].tblMax = sizeof(g_g_Skeletons_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	//3番だけ線形補間で動かしてみる
	g_Skeletons[3].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Skeletons[3].tblNo = 3;		// 再生するアニメデータの先頭アドレスをセット
	g_Skeletons[3].tblMax = sizeof(g_g_Skeletons_MoveTbl3) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	//4番だけ線形補間で動かしてみる
	g_Skeletons[4].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Skeletons[4].tblNo = 4;		// 再生するアニメデータの先頭アドレスをセット
	g_Skeletons[4].tblMax = sizeof(g_g_Skeletons_MoveTbl4) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット



	if (g_Mode == MODE_BATTLE)
	{
		for (int i = 0; i < MAX_SKELETONS; i++)
		{
			g_Skeletons[i].use = FALSE;			// TRUE:生きてる

		}

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemySkeletons(void)
{

	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		if (g_Skeletons[i].load)
		{
			UnloadModel(&g_Skeletons[i].model);
			g_Skeletons[i].load = FALSE;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemySkeletons(void)
{
	PLAYER* knight = GetPlayerKnight();		// プレイヤー（騎士）のポインターを初期化
	int g_Mode = GetMode();
	int g_Turn = GetTurn();

	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		if (g_Skeletons[i].use == TRUE)		// このエネミーが使われている？
		{// Yes
			if (g_Skeletons[i].hp <= 0)
			{
				g_Skeletons[i].use = FALSE;

			}

			// モードによって処理を分ける
			switch (g_Mode)
			{
			case MODE_TITLE:		// タイトル画面の描画
				break;

			case MODE_GAME:			// ゲーム画面の描画
				// 元の位置を記録する
				originalPos[i] = g_Skeletons[i].pos;

				if (g_Skeletons[i].vision == TRUE)
				{
					// プレイヤーを見つけている場合の処理
					// ホーミングの移動処理を行う
					XMVECTOR epos = XMLoadFloat3(&g_Skeletons[i].pos);
					XMVECTOR vec = (XMLoadFloat3(&knight->pos) - epos) * 0.01f;				//自分とプレイヤーの差分を求める
					epos += vec;
					XMStoreFloat3(&g_Skeletons[i].pos, epos);
					float angle = atan2f(g_Skeletons[i].pos.x - knight->pos.x, g_Skeletons[i].pos.z - knight->pos.z);							//(yの差分、ⅹの差分)その差分を使って角度を求めている
					g_Skeletons[i].rot.y = angle;
					g_Skeletons[i].pos.y = ENEMY_OFFSET_Y;

				}
				else
				{
					// プレイヤーを見失った場合の処理
					if (!g_Skeletons[i].lost) 
					{
						// プレイヤーを見失った状態にする
						g_Skeletons[i].lost = TRUE;

						// 元の位置に戻るために時間を初期化する
						g_Skeletons[i].time = 0.0f;
					}

					// 敵キャラクターが元の位置に戻る処理
					INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Skeletons[i].tblNo];
					XMVECTOR nowPosVector = XMLoadFloat3(&g_Skeletons[i].pos);								// XMVECTORへ変換
					XMVECTOR originalPosVector = XMLoadFloat3(&originalPos[i]) - nowPosVector;			// XYZ移動量を計算している
					float moveSpeed = 0.1f;
					originalPosVector *= moveSpeed;														// 現在の移動量を計算している

					// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
					XMStoreFloat3(&g_Skeletons[i].pos, nowPosVector + originalPosVector);

					// 元の位置に戻ったかどうかの判定
					float distanceThreshold = 0.1f; // 元の位置に戻ったと判定する距離の閾値
					if (XMVectorGetX(XMVector3LengthSq(originalPosVector - XMLoadFloat3(&g_Skeletons[i].pos))) < distanceThreshold * distanceThreshold) 
					{
						// 元の位置に戻ったと判定された場合
						g_Skeletons[i].lost = false; // プレイヤーを見失った状態を解除

					}

					// 徘徊の移動処理
					if (g_Skeletons[i].tblMax > 0)	// 線形補間を実行する？
					{	// 線形補間の処理
						int nowNo = (int)g_Skeletons[i].time;			// 整数分であるテーブル番号を取り出している
						int maxNo = g_Skeletons[i].tblMax;				// 登録テーブル数を数えている
						int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
						INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Skeletons[i].tblNo];	// 行動テーブルのアドレスを取得

						XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
						XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
						XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

						XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
						XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
						XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

						float nowTime = g_Skeletons[i].time - nowNo;	// 時間部分である少数を取り出している

						Pos *= nowTime;								// 現在の移動量を計算している
						//Rot *= nowTime;								// 現在の回転量を計算している
						Scl *= nowTime;								// 現在の拡大率を計算している

						// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
						XMStoreFloat3(&g_Skeletons[i].pos, nowPos + Pos);

						// 計算して求めた回転量を現在の移動テーブルに足している
						XMStoreFloat3(&g_Skeletons[i].rot, nowRot + Rot);

						// 計算して求めた拡大率を現在の移動テーブルに足している
						XMStoreFloat3(&g_Skeletons[i].scl, nowScl + Scl);

						// frameを使て時間経過処理をする
						g_Skeletons[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
						if ((int)g_Skeletons[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
						{
							g_Skeletons[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
						}


					}

				}


			// 視野の計算
			{
				// エネミーの向く方向
				XMFLOAT3 View01 = XMFLOAT3(0.0f, 0.0f, 0.0f);		

				View01.x = -cosf(g_Skeletons[i].rot.y - XM_PIDIV2)/* * 50*/;
				View01.z = sinf(g_Skeletons[i].rot.y - XM_PIDIV2) /** 50*/;

				XMVECTOR	v0 = XMLoadFloat3(&View01);
				XMVECTOR	v1 = XMLoadFloat3(&g_Skeletons[i].pos);
				XMVECTOR	v2 = XMLoadFloat3(&knight->pos) - v1;

				v0 = XMVector3Normalize(v0);
				v2 = XMVector3Normalize(v2);

				float dis = VectorDistance(g_Skeletons[i].pos, knight->pos);

				float dot = dotProduct(&v0, &v2);
				dot = dot > 1 ? 1:dot;
				float pAngel = acosf(dot);
				float aAngel = pAngel * 360 / (XM_PI * 2);

				if ((aAngel <= VIEW_ANGLE / 2) && (dis < VIEW_DISTANCE))
				{
					g_Skeletons[i].vision = TRUE;			// 視野範囲内にキャラあり

				}
				else 
				{
					g_Skeletons[i].vision = FALSE;			// 視野範囲内にキャラなし

				}
			}
				break;

			case MODE_BATTLE:		// バトル画面の描画

				// バトルのターンによって行動
				switch (g_Turn)
				{
				case TURN_START:

					break;

				case TURN_PLAYER:

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
					// 線形補間の処理
				{	// プレイヤーに近く
					BOOL moveStop = FALSE;

					if (moveStop == FALSE)
					{

						XMVECTOR nowPos = XMLoadFloat3(&g_Skeletons[0].pos);	// XMVECTORへ変換

						XMVECTOR Pos = XMLoadFloat3(&knight->pos) - nowPos;	// XYZ移動量を計算している

						Pos *= g_Skeletons[0].time;								// 現在の移動量を計算している

						// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
						XMStoreFloat3(&g_Skeletons[0].pos, nowPos + Pos);

						float angle = atan2f(g_Skeletons[0].pos.x - knight->pos.x, g_Skeletons[0].pos.z - knight->pos.z);							//(yの差分、ⅹの差分)その差分を使って角度を求めている
						g_Skeletons[0].rot.y = angle;

						// frameを使て時間経過処理をする
						g_Skeletons[0].time += 1.0f / 100.0f;	// 時間を進めている

						if (g_Skeletons[0].time >= 1.0f)
						{
							g_Skeletons[0].time = 0.0f;
						}

					}

					// 敵とプレイヤーキャラ
					for (int i = 0; i < MAX_SKELETONS; i++)
					{
						//敵の有効フラグをチェックする
						if (g_Skeletons[i].use == FALSE)
							continue;

						//BCの当たり判定
						if (CollisionBC(g_Skeletons[0].pos, knight->pos, g_Skeletons[0].size * 2, knight->size * 2))
						{
							// エネミーの近くに止まる
							moveStop = TRUE;

						}
					}

					if (moveStop == TRUE)
					{
						moveStop = FALSE;
						SetPlayerKnightDamage(2);
						SetTurn(TURN_ACTION);

					}

				}

					break;

					// ゲーム終了時の処理
				case TURN_MAX:


					break;

				}

				break;

			}

			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Skeletons[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Skeletons[i].shadowIdx, pos);
		}
		else
		{
			switch (g_Mode)
			{
			case MODE_BATTLE:		// バトル画面の描画
				ReleaseShadow(g_Skeletons[i].shadowIdx);

				break;
			}
		}
	}




#ifdef _DEBUG

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	// モデルの色を変更できるよ！半透明にもできるよ。
	//	for (int j = 0; j < g_Skeletons[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Skeletons[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// モデルの色を元に戻している
	//	for (int j = 0; j < g_Skeletons[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Skeletons[0].model, j, g_Skeletons[0].diffuse[j]);
	//	}
	//}
#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemySkeletons(void)
{

	CAMERA* cam = GetCamera();
	int g_Mode = GetMode();


	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		if (g_Skeletons[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Skeletons[i].scl.x, g_Skeletons[i].scl.y, g_Skeletons[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Skeletons[i].rot.x, g_Skeletons[i].rot.y + XM_PI, g_Skeletons[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Skeletons[i].pos.x, g_Skeletons[i].pos.y, g_Skeletons[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Skeletons[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Skeletons[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY * GetEnemySkeletons()
{
	return &g_Skeletons[0];
}



//=============================================================================
// スケルトンにダメージ
//=============================================================================
void SetEnemySkeletonsDamage(float damge)
{
	g_Skeletons[0].hp -= damge;
}


//=============================================================================
// エネミーを線形補間で座標posにtimeフレイム内に移動
//=============================================================================
void EnemySkeletonsMoving(XMFLOAT3 pos, float time)
{	// 線形補間の処理

	XMVECTOR nowPos = XMLoadFloat3(&g_Skeletons[0].pos);	// XMVECTORへ変換

	XMVECTOR Pos = XMLoadFloat3(&pos) - nowPos;	// XYZ移動量を計算している

	Pos *= g_Skeletons[0].time;								// 現在の移動量を計算している

	// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
	XMStoreFloat3(&g_Skeletons[0].pos, nowPos + Pos);

	float angle = atan2f(g_Skeletons[0].pos.x - pos.x, g_Skeletons[0].pos.z - pos.z);							//(yの差分、ⅹの差分)その差分を使って角度を求めている
	g_Skeletons[0].rot.y = angle;

	// frameを使て時間経過処理をする
	g_Skeletons[0].time += 1.0f / time;	// 時間を進めている

	if (g_Skeletons[0].time >= 1.0f)
	{
		g_Skeletons[0].time = 0.0f;
	}

}
