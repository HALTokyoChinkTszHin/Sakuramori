//=============================================================================
//
// モデル処理 [player.cpp]
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

#define PLAYER_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define PLAYER_OFFSET_Y		(10.0f)							// プレイヤーの足元をあわせる


#define TEXTURE_MAX			(1)								// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexRange(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static float		roty = 0.0f;

static LIGHT		g_Light;

static ID3D11Buffer* g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TextureName[] =
{
	"data/TEXTURE/circular_blue.png",
};

static RANGE		moving_Range;					




//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	// プレイヤー（騎士）の初期化
	InitPlayerKnight();

	MakeVertexRange();

	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}



	// 移動範囲の初期化
	{
		ZeroMemory(&moving_Range.material, sizeof(moving_Range.material));
		moving_Range.material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

		moving_Range.pos = { 0.0f, 0.0f, 0.0f };
		moving_Range.rot = { 0.0f, 0.0f, 0.0f };
		moving_Range.scl = { 10.0f, 10.0f, 10.0f };
		moving_Range.use = FALSE;

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// プレイヤー（騎士）の終了処理
	UninitPlayerKnight();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{// テクスチャの解放
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if (g_VertexBuffer != NULL)
	{// 頂点バッファの解放
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA* cam = GetCamera();
	ENEMY* skeletons = GetEnemySkeletons();		// エネミーのポインターを初期化
	PLAYER* knight = GetPlayerKnight();

	if (knight->use == TRUE)
	{
		// プレイヤー（騎士）の更新処理
		UpdatePlayerKnight();

	}

	int g_Mode = GetMode();
	int g_Turn = GetTurn();

	switch (g_Mode)
	{
	case MODE_TITLE:
		break;

	case MODE_GAME:
		// 騎士
		if (knight->use == TRUE)
		{
			// 移動処理
			if (GetKeyboardPress(DIK_A))
			{
				knight->movSpd = PLAYER_VALUE_MOVE;
				knight->motion = MOTIONMOVE;
				knight->dir = CHAR_DIR_LEFT;

			}
			if (GetKeyboardPress(DIK_D))
			{
				knight->movSpd = PLAYER_VALUE_MOVE;
				knight->motion = MOTIONMOVE;
				knight->dir = CHAR_DIR_RIGHT;

			}
			if (GetKeyboardPress(DIK_W))
			{
				knight->movSpd = PLAYER_VALUE_MOVE;
				knight->motion = MOTIONMOVE;
				knight->dir = CHAR_DIR_INSIDE;

			}
			if (GetKeyboardPress(DIK_S))
			{
				knight->movSpd = PLAYER_VALUE_MOVE;
				knight->motion = MOTIONMOVE;
				knight->dir = CHAR_DIR_OUTSIDE;

			}

			// 弾発射処理
			if (GetKeyboardTrigger(DIK_SPACE))
			{
				XMFLOAT3 pos = knight->pos;
				XMFLOAT3 rot = knight->rot;

				rot.z -= XM_PI / 3;


				SetBullet(pos, rot);
				knight->motion = MOTIONATTACK;

			}


		}



#ifdef _DEBUG
		if (GetKeyboardPress(DIK_R))
		{
		}
#endif



		break;


	case MODE_BATTLE:

		if (g_Turn == TURN_MOVING)
		{
			moving_Range.use = TRUE;
		}
		else
		{
			moving_Range.use = FALSE;
		}

		switch (g_Turn)
		{
		case TURN_START:

			break;

		case TURN_PLAYER:

			break;


		case TURN_ACTION:

			break;

		case TURN_MOVING:

			moving_Range.pos = knight->pos;
			moving_Range.pos.y = 0.0f;


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
			break;

		// ターン終了時の処理
		case TURN_MAX:

			break;
			
		}


		break;

		// ゲーム終了時の処理
	case MODE_MAX:
		break;
	}


#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("knight X:%f Y:%f Z:%f\n", knight->pos.x, knight->pos.y, knight->pos.z);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	PLAYER* knight = GetPlayerKnight();

	if (knight->use == TRUE)
	{
		// プレイヤー（騎士）の描画処理
		DrawPlayerKnight();

	}


	// カリング無効
	SetCullingMode(CULL_MODE_NONE);


	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	{// 移動範囲の描画
		if (moving_Range.use == TRUE)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(moving_Range.scl.x, moving_Range.scl.y, moving_Range.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(moving_Range.rot.x, moving_Range.rot.y, moving_Range.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(moving_Range.pos.x, moving_Range.pos.y + 1.0f, moving_Range.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&moving_Range.mtxWorld, mtxWorld);


			// マテリアル設定
			SetMaterial(moving_Range.material);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// αテストを有効に
	SetAlphaTestEnable(FALSE);

	// ライティングを無効
	SetLightEnable(TRUE);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}



//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexRange(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = 16.0f;
	float fHeight = 16.0f;

	// 頂点座標の設定
	vertex[0].Position = { -fWidth / 2.0f, 0.0f,  fHeight / 2.0f };
	vertex[1].Position = { fWidth / 2.0f, 0.0f,  fHeight / 2.0f };
	vertex[2].Position = { -fWidth / 2.0f, 0.0f, -fHeight / 2.0f };
	vertex[3].Position = { fWidth / 2.0f, 0.0f, -fHeight / 2.0f };

	// 法線の設定
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// 拡散光の設定
	vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

	// テクスチャ座標の設定
	vertex[0].TexCoord = { 0.0f, 0.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}



//=============================================================================
// プレイヤーの向き変更
//=============================================================================
void changeDirection(float* roty, int targetDir, float rotationSpeed) 
{
	float diff;
	int sign;

	// 目標の角度
	float targetAngle;

	switch (targetDir) {
	case CHAR_DIR_INSIDE:

		targetAngle = XM_PI;

		break;
	case CHAR_DIR_RIGHT:

		targetAngle = -XM_PI / 2;

		break;
	case CHAR_DIR_OUTSIDE:

		targetAngle = 0.0f;

		break;
	case CHAR_DIR_LEFT:

		targetAngle = XM_PI / 2;

		break;
	}

	// 現在の角度から目標の角度までの差を計算
	diff = targetAngle - *roty;

	// 差を-πからπの範囲に制限
	while (diff > XM_PI) {
		diff -= 2 * XM_PI;
	}
	while (diff <= -XM_PI) {
		diff += 2 * XM_PI;
	}

	// 回転量が小さすぎる場合、直接目標の角度に設定
	if (fabs(diff) < rotationSpeed) {
		*roty = targetAngle;
	}
	else {
		// 回転方向を決定するために符号を取得
		sign = (diff > 0) ? 1 : -1;
		// 回転量を加える
		*roty += sign * rotationSpeed;
	}

	// rotyが[0, 2π)の範囲内に収まるように調整
	if (*roty >= 2 * XM_PI)
		*roty -= 2 * XM_PI;
	else if (*roty < 0.0f)
		*roty += 2 * XM_PI;
}
