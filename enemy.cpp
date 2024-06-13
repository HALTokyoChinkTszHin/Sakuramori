//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
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
#define TEXTURE_MAX			(2)								// テクスチャの数
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexHPGauge(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_BATTLE_ENEMY];				// バトルのエネミー

int g_Enemy_load = 0;


static ID3D11Buffer* g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TextureName[] =
{
	"data/TEXTURE/UI_HPNONE.png",
	"data/TEXTURE/UI_HPGREEN.png",
};

MATERIAL	materialHPGauge;		// マテリアル

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	MakeVertexHPGauge();
	int g_Mode = GetMode();


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

	InitEnemySkeletons();

	ENEMY* skeletons = GetEnemySkeletons();	// スケルトンのポインターを初期化

	if (g_Mode == MODE_BATTLE)
	{
		skeletons[0].use = TRUE;

	}
		

	ZeroMemory(&materialHPGauge, sizeof(materialHPGauge));
	materialHPGauge.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	UninitEnemySkeletons();

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
void UpdateEnemy(void)
{
	PLAYER* knight = GetPlayerKnight();	// プレイヤー（騎士）のポインターを初期化
	ENEMY* skeletons = GetEnemySkeletons();	// スケルトンのポインターを初期化

	int g_Mode = GetMode();
	int g_Turn = GetTurn();

	UpdateEnemySkeletons();


#ifdef _DEBUG

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	// モデルの色を変更できるよ！半透明にもできるよ。
	//	for (int j = 0; j < g_Enemy[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Enemy[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// モデルの色を元に戻している
	//	for (int j = 0; j < g_Enemy[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Enemy[0].model, j, g_Enemy[0].diffuse[j]);
	//	}
	//}
#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{

	CAMERA* cam = GetCamera();
	int g_Mode = GetMode();

	DrawEnemySkeletons();

}


//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexHPGauge(void)
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

	float fWidth = 25.0f;
	float fHeight = 1.0f;

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
