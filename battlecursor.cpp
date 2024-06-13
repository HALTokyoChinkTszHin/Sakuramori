//=============================================================================
//
// バトルカーソル処理 [battlecursor.h]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "model.h"

#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "playerknight.h"

#include "enemy.h"
#include "enemyskeletons.h"

#include "battle.h"
#include "battlecursor.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(2)				// テクスチャの数

#define	CURSOR_WIDTH			(8.0f)			// 頂点サイズ
#define	CURSOR_HEIGHT			(12.0f)			// 頂点サイズ

#define	MAX_BATTLECURSOR	(1)			// カーソル最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	scl;			// スケール
	MATERIAL	material;		// マテリアル
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	BOOL		use;			// 使用しているかどうか
	BOOL		texNo;			// テクスチャ番号

} BATTLECURSOR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattleCursor(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static BATTLECURSOR					g_BattleCursor[MAX_BATTLECURSOR];	// カーソルワーク
static BOOL					g_bAlpaTest;		// アルファテストON/OFF

XMFLOAT3 moving_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);

static char* g_TextureName[] =
{
	"data/TEXTURE/Cursor_Moving.png",
	"data/TEXTURE/Cursor_Target.png",
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattleCursor(void)
{
	MakeVertexBattleCursor();

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


	// カーソルワークの初期化
	for(int nCntCursor = 0; nCntCursor < MAX_BATTLECURSOR; nCntCursor++)
	{
		ZeroMemory(&g_BattleCursor[nCntCursor].material, sizeof(g_BattleCursor[nCntCursor].material));
		g_BattleCursor[nCntCursor].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_BattleCursor[nCntCursor].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_BattleCursor[nCntCursor].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_BattleCursor[nCntCursor].fWidth = CURSOR_WIDTH;
		g_BattleCursor[nCntCursor].fHeight = CURSOR_HEIGHT;
		g_BattleCursor[nCntCursor].use = FALSE;
		g_BattleCursor[nCntCursor].texNo = 0;


	}


	g_bAlpaTest = TRUE;
	//g_nAlpha = 0x0;
	// カーソルの設定
	SetBattleCursor(XMFLOAT3(0.0f, 0.0f, 0.0f), 60.0f, 90.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattleCursor(void)
{
	for(int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if(g_Texture[nCntTex] != NULL)
		{// テクスチャの解放
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if(g_VertexBuffer != NULL)
	{// 頂点バッファの解放
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattleCursor(void)
{
	ENEMY* enemy = GetEnemySkeletons();		// エネミーのポインターを初期化
	PLAYER* knight = GetPlayerKnight();	// プレイヤー（騎士）のポインターを初期化

	// エネミーが全部死亡したら状態遷移
	int enemy_count = 0;
	for (int i = 0; i < MAX_BATTLE_ENEMY; i++)
	{
		if (enemy[i].use == FALSE) continue;
		enemy_count++;
	}

	// エネミーが０匹？
	if (enemy_count == 0)
	{
		SetTurn(TURN_WIN);
	}

	int player_count = 0;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (knight->use == FALSE) continue;
		player_count++;
	}

	// エネミーが０匹？
	if (player_count == 0)
	{
		SetTurn(TURN_GAMEOVER);
	}

	for(int nCntCursor = 0; nCntCursor < MAX_BATTLECURSOR; nCntCursor++)
	{
		if(g_BattleCursor[nCntCursor].use)
		{

			int g_Turn = GetTurn();

			switch (g_Turn)
			{
			case TURN_START:

				break;

			case TURN_ACTION:

				g_BattleCursor[nCntCursor].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
				g_BattleCursor[nCntCursor].texNo = 0;

				if (GetKeyboardTrigger(DIK_L))
				{
					// 攻撃
					SetTurn(TURN_TARGET);

				}
				if (GetKeyboardTrigger(DIK_K))
				{
					// 移動
					SetTurn(TURN_MOVING);
				}
				if (GetKeyboardTrigger(DIK_I))
				{
					// 防御
					SetTurn(TURN_DEFEND);

				}
				if (GetKeyboardTrigger(DIK_J))
				{
					// 逃走
					SetTurn(TURN_RUN);

				}


				break;

			case TURN_MOVING:

				// 移動処理
				if (GetKeyboardPress(DIK_A))
				{
					g_BattleCursor[nCntCursor].pos.x -= 1.0f;
				}
				if (GetKeyboardPress(DIK_D))
				{
					g_BattleCursor[nCntCursor].pos.x += 1.0f;
				}
				if (GetKeyboardPress(DIK_W))
				{
					g_BattleCursor[nCntCursor].pos.z += 1.0f;
				}
				if (GetKeyboardPress(DIK_S))
				{
					g_BattleCursor[nCntCursor].pos.z -= 1.0f;
				}

#ifdef _DEBUG
				if (GetKeyboardPress(DIK_R))
				{
					g_BattleCursor[nCntCursor].pos.z = g_BattleCursor[nCntCursor].pos.x = 0.0f;
				}
#endif

				if (GetKeyboardTrigger(DIK_P))
				{
					//if ((player->pos.x != g_BattleCursor[nCntCursor].pos.x) &&
					//	(player->pos.z != g_BattleCursor[nCntCursor].pos.z))// 線形補間を実行する？
					//{
					//	for (int i = 0; i < 100; i++)
					//	{
					//		PlayerMoving(g_BattleCursor[nCntCursor].pos, 100.0f);


					//	}
					//}

					moving_pos = g_BattleCursor[nCntCursor].pos;

					//if ((player->pos.x == g_BattleCursor[nCntCursor].pos.x) &&
					//	(player->pos.z == g_BattleCursor[nCntCursor].pos.z))// 線形補間を止める？
					//{
					//	SetTurn(TURN_ENEMY);

					//}
				}

				// 移動範囲外の表示
				{
					float moving_distance = VectorDistance(knight->pos, g_BattleCursor[nCntCursor].pos);

					if (moving_distance >= MOVING_RANGE)
					{
						g_BattleCursor[nCntCursor].texNo = 1;
					}
					else if (moving_distance < MOVING_RANGE)
					{
						g_BattleCursor[nCntCursor].texNo = 0;
					}
				}

				if (GetKeyboardTrigger(DIK_O))
				{

					SetTurn(TURN_ACTION);
				}



				break;

			case TURN_TARGET:

				g_BattleCursor[nCntCursor].pos = enemy->pos;
				g_BattleCursor[nCntCursor].pos.y += enemy->size * 4;
				g_BattleCursor[nCntCursor].texNo = 1;

				if (GetKeyboardTrigger(DIK_P))
				{
					SetTurn(TURN_ATTACK);

				}

				if (GetKeyboardTrigger(DIK_O))
				{
					g_BattleCursor[nCntCursor].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
					g_BattleCursor[nCntCursor].texNo = 0;
					SetTurn(TURN_ACTION);
				}

				break;

			case TURN_ATTACK:

				break;

			case TURN_DEFEND:

				if (GetKeyboardTrigger(DIK_P))
				{

				}

				if (GetKeyboardTrigger(DIK_O))
				{

					SetTurn(TURN_ACTION);
				}
				break;

			case TURN_RUN:

				if (GetKeyboardTrigger(DIK_P))
				{
					SetMode(MODE_GAME);
				}

				if (GetKeyboardTrigger(DIK_O))
				{

					SetTurn(TURN_ACTION);
				}

				break;

			case TURN_ENEMY:
				break;

			case TURN_WIN:
				if (GetKeyboardTrigger(DIK_RETURN))
				{
					SetMode(MODE_GAME);
				}

				break;

			case TURN_GAMEOVER:
				if (GetKeyboardTrigger(DIK_RETURN))
				{
					SetMode(MODE_GAME);
				}



				break;


				// ゲーム終了時の処理
			case TURN_MAX:
				break;
			}

		}

	}


#ifdef _DEBUG
	// アルファテストON/OFF
	if(GetKeyboardTrigger(DIK_F1))
	{
		g_bAlpaTest = g_bAlpaTest ? FALSE: TRUE;
	}

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattleCursor(void)
{
	// αテスト設定
	if (g_bAlpaTest == TRUE)
	{
		// αテストを有効に
		SetAlphaTestEnable(TRUE);
	}

	// ライティングを無効
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for(int i = 0; i < MAX_BATTLECURSOR; i++)
	{
		if(g_BattleCursor[i].use)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			// 正方行列（直交行列）を転置行列させて逆行列を作ってる版(速い)
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];


			// スケールを反映
			mtxScl = XMMatrixScaling(g_BattleCursor[i].scl.x, g_BattleCursor[i].scl.y, g_BattleCursor[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_BattleCursor[i].pos.x, g_BattleCursor[i].pos.y, g_BattleCursor[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);


			// マテリアル設定
			SetMaterial(g_BattleCursor[i].material);

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BattleCursor[i].texNo]);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ライティングを有効に
	SetLightEnable(TRUE);

	// αテストを無効に
	SetAlphaTestEnable(FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBattleCursor(void)
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

	float fWidth = CURSOR_WIDTH;
	float fHeight = CURSOR_HEIGHT;

	// 頂点座標の設定
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, 0.0f, 0.0f);

	// 法線の設定
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 拡散光の設定
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}

//=============================================================================
// カーソルのパラメータをセット
//=============================================================================
int SetBattleCursor(XMFLOAT3 pos, float fWidth, float fHeight, XMFLOAT4 col)
{
	int nIdxCursor = -1;

	for(int nCntCursor = 0; nCntCursor < MAX_BATTLECURSOR; nCntCursor++)
	{
		if(!g_BattleCursor[nCntCursor].use)
		{
			g_BattleCursor[nCntCursor].pos = pos;
			g_BattleCursor[nCntCursor].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
			g_BattleCursor[nCntCursor].fWidth = fWidth;
			g_BattleCursor[nCntCursor].fHeight = fHeight;
			g_BattleCursor[nCntCursor].use = TRUE;

			nIdxCursor = nCntCursor;

			break;
		}
	}

	return nIdxCursor;
}




//=============================================================================
// 移動座標の設定
//=============================================================================
void SetMovingPos(XMFLOAT3 pos)
{

	moving_pos = pos;	// 次のモードをセットしている


}

//=============================================================================
// 移動座標の取得
//=============================================================================
XMFLOAT3 GetMovingPos(void)
{
	return moving_pos;
}


