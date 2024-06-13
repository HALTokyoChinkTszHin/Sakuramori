//=============================================================================
//
// バトルUI処理 [battleUI.cpp]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"

#include "battleui.h"
#include "sprite.h"
#include "model.h"
#include "player.h"
#include "playerknight.h"

#include "battle.h"
#include "battlecursor.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX					(UI_IMAGE_MAX)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/UI_START.png",
	"data/TEXTURE/UI_Win.png",
	"data/TEXTURE/UI_GameOver.png",

	"data/TEXTURE/UI_NEXT.png",
	"data/TEXTURE/UI_turn_knight.png",
	"data/TEXTURE/UI_turn_skeletons.png",

	"data/TEXTURE/UI_Icon_knight.png",
	"data/TEXTURE/UI_HPNONE.png",
	"data/TEXTURE/UI_HPRED.png",
	"data/TEXTURE/UI_HPGREEN.png",

	"data/TEXTURE/UI_Command.png",
	"data/TEXTURE/UI_Confirm.png",



};

static UI		battle_UI[UI_MAX];						// プレイヤー



static BOOL						g_Load = FALSE;

int								ui_Time;				// 時間計算用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattleUI(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	ui_Time = 0;			// 時間の初期化

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// UI(戦闘開始)の初期化
	battle_UI[UI_START].use = TRUE;
	battle_UI[UI_START].w = 200;
	battle_UI[UI_START].h = 100;
	battle_UI[UI_START].pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };
	battle_UI[UI_START].texNo = IMAGE_START;

	// UI(WIN)の初期化
	battle_UI[UI_WIN].use = FALSE;
	battle_UI[UI_WIN].w = 200;
	battle_UI[UI_WIN].h = 100;
	battle_UI[UI_WIN].pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };
	battle_UI[UI_WIN].texNo = IMAGE_WIN;

	// UI(GAMEOVER)の初期化
	battle_UI[UI_GAMEOVER].use = FALSE;
	battle_UI[UI_GAMEOVER].w = 200;
	battle_UI[UI_GAMEOVER].h = 100;
	battle_UI[UI_GAMEOVER].pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };
	battle_UI[UI_GAMEOVER].texNo = IMAGE_GAMEOVER;

	// UI(行動順)の初期化
	battle_UI[UI_TURN_NONE].use = FALSE;
	battle_UI[UI_TURN_NONE].w = 500;
	battle_UI[UI_TURN_NONE].h = 80;
	battle_UI[UI_TURN_NONE].pos = { 890.0f, 100.0f, 0.0f };
	battle_UI[UI_TURN_NONE].texNo = IMAGE_TURN_NONE;

	// 行動順(1)
	battle_UI[UI_TURN_FIRST].use = FALSE;
	battle_UI[UI_TURN_FIRST].w = 150;
	battle_UI[UI_TURN_FIRST].h = 100;
	battle_UI[UI_TURN_FIRST].pos = { 740.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_FIRST].texNo = IMAGE_TURN_KNIGHT;

	// 行動順(2)
	battle_UI[UI_TURN_SECOND].use = FALSE;
	battle_UI[UI_TURN_SECOND].w = 150;
	battle_UI[UI_TURN_SECOND].h = 100;
	battle_UI[UI_TURN_SECOND].pos = { 800.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_SECOND].texNo = IMAGE_TURN_SKELETONS;

	// 行動順(3)
	battle_UI[UI_TURN_THIRD].use = FALSE;
	battle_UI[UI_TURN_THIRD].w = 150;
	battle_UI[UI_TURN_THIRD].h = 100;
	battle_UI[UI_TURN_THIRD].pos = { 860.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_THIRD].texNo = IMAGE_TURN_KNIGHT;

	// 行動順(4)
	battle_UI[UI_TURN_FOURTH].use = FALSE;
	battle_UI[UI_TURN_FOURTH].w = 150;
	battle_UI[UI_TURN_FOURTH].h = 100;
	battle_UI[UI_TURN_FOURTH].pos = { 920.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_FOURTH].texNo = IMAGE_TURN_SKELETONS;

	// 行動順(5)
	battle_UI[UI_TURN_FIFTH].use = FALSE;
	battle_UI[UI_TURN_FIFTH].w = 150;
	battle_UI[UI_TURN_FIFTH].h = 100;
	battle_UI[UI_TURN_FIFTH].pos = { 980.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_FIFTH].texNo = IMAGE_TURN_KNIGHT;



	// UI(騎士のアイコン)の初期化
	battle_UI[UI_HP_ICON_KNIGHT].use = FALSE;
	battle_UI[UI_HP_ICON_KNIGHT].w = 150;
	battle_UI[UI_HP_ICON_KNIGHT].h = 150;
	battle_UI[UI_HP_ICON_KNIGHT].pos = { 70.0f, 450.0f, 0.0f };
	battle_UI[UI_HP_ICON_KNIGHT].texNo = IMAGE_ICON_KNIGHT;

	// UI(HP)の初期化
	// 下敷きのゲージ
	battle_UI[UI_HP_NONE_KNIGHT].use = FALSE;
	battle_UI[UI_HP_NONE_KNIGHT].w = 380;
	battle_UI[UI_HP_NONE_KNIGHT].h = 20;
	battle_UI[UI_HP_NONE_KNIGHT].pos = { 100.0f, 500.0f, 0.0f };
	battle_UI[UI_HP_NONE_KNIGHT].texNo = IMAGE_HP_NONE;

	// HP30%以下のゲージ
	battle_UI[UI_HP_RED_KNIGHT].use = FALSE;
	battle_UI[UI_HP_RED_KNIGHT].w = 380;
	battle_UI[UI_HP_RED_KNIGHT].h = 20;
	battle_UI[UI_HP_RED_KNIGHT].pos = battle_UI[UI_HP_NONE_KNIGHT].pos;
	battle_UI[UI_HP_RED_KNIGHT].texNo = IMAGE_HP_RED;

	// HP30%以上のゲージ
	battle_UI[UI_HP_GREEN_KNIGHT].use = FALSE;
	battle_UI[UI_HP_GREEN_KNIGHT].w = 380;
	battle_UI[UI_HP_GREEN_KNIGHT].h = 20;
	battle_UI[UI_HP_GREEN_KNIGHT].pos = battle_UI[UI_HP_NONE_KNIGHT].pos;
	battle_UI[UI_HP_GREEN_KNIGHT].texNo = IMAGE_HP_GREEN;



	// UI(戦闘コマンド)の初期化
	battle_UI[UI_OPTION].use = FALSE;
	battle_UI[UI_OPTION].w = 270;
	battle_UI[UI_OPTION].h = 250;
	battle_UI[UI_OPTION].pos = { 800.0f, 300.0f, 0.0f };
	battle_UI[UI_OPTION].texNo = IMAGE_COMMAND;

	// UI(決定ポッター)の初期化
	battle_UI[UI_SELECT].use = FALSE;
	battle_UI[UI_SELECT].w = 100;
	battle_UI[UI_SELECT].h = 60;
	battle_UI[UI_SELECT].pos = { 880.0f, 480.0f, 0.0f };
	battle_UI[UI_SELECT].texNo = IMAGE_CONFIRM;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattleUI(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattleUI(void)
{
	PLAYER* knight = GetPlayerKnight();	// プレイヤー（騎士）のポインターを初期化

	int g_Turn = GetTurn();

	switch (g_Turn)
	{
	case TURN_START:
		ui_Time++;

		if (ui_Time > 100)
		{
			battle_UI[UI_START].use = FALSE;
			ui_Time = 0;
			SetTurn(TURN_ACTION);

		}
		break;

	case TURN_PLAYER:

		break;

	case TURN_ACTION:
		battle_UI[UI_TURN_FIRST].texNo = IMAGE_TURN_KNIGHT;
		battle_UI[UI_TURN_SECOND].texNo = IMAGE_TURN_SKELETONS;

		for (int i = 0; i < UI_MAX; i++)
		{
			battle_UI[i].use = TRUE;

		}

		battle_UI[UI_START].use = FALSE;
		battle_UI[UI_SELECT].use = FALSE;
		battle_UI[UI_WIN].use = FALSE;
		battle_UI[UI_GAMEOVER].use = FALSE;



		break;

	case TURN_MOVING:
		battle_UI[UI_OPTION].use = FALSE;
		battle_UI[UI_SELECT].use = TRUE;





		break;

	case TURN_TARGET:
		battle_UI[UI_OPTION].use = FALSE;
		battle_UI[UI_SELECT].use = TRUE;

		break;

	case TURN_ATTACK:
		battle_UI[UI_SELECT].use = FALSE;

		break;

	case TURN_DEFEND:
		battle_UI[UI_OPTION].use = FALSE;
		battle_UI[UI_SELECT].use = TRUE;

		break;

	case TURN_RUN:
		battle_UI[UI_OPTION].use = FALSE;
		battle_UI[UI_SELECT].use = TRUE;

		break;

	case TURN_WIN:
		for (int i = 0; i < UI_MAX; i++)
		{
			battle_UI[i].use = FALSE;

		}

		battle_UI[UI_WIN].use = TRUE;

		break;

	case TURN_GAMEOVER:
		for (int i = 0; i < UI_MAX; i++)
		{
			battle_UI[i].use = FALSE;

		}

		battle_UI[UI_GAMEOVER].use = TRUE;


		break;

	case TURN_ENEMY:
		// 行動順の変更
		battle_UI[UI_TURN_FIRST].texNo = IMAGE_TURN_SKELETONS;
		battle_UI[UI_TURN_SECOND].texNo = IMAGE_TURN_KNIGHT;

		break;

		// ゲーム終了時の処理
	case TURN_MAX:

		for (int i = 0; i < UI_MAX; i++)
		{
			battle_UI[i].use = FALSE;
		}

		break;
	}

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattleUI(void)
{
	PLAYER* knight = GetPlayerKnight();	// プレイヤー（騎士）のポインターを初期化

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// UI を描画
	for (int i = 0; i < UI_MAX; i++)
	{
		int hp_Knight_Start = UI_HP_NONE_KNIGHT;
		int hp_Knight_End = UI_HP_GREEN_KNIGHT;
		if ((i >= hp_Knight_Start) && (i <= hp_Knight_End))

			continue;


		if(battle_UI[i].use == TRUE)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[battle_UI[i].texNo]);


			// スコアの位置やテクスチャー座標を反映
			float px = battle_UI[i].pos.x;			// スコアの表示位置X
			float py = battle_UI[i].pos.y;			// スコアの表示位置Y
			float pw = battle_UI[i].w;				// スコアの表示幅
			float ph = battle_UI[i].h;				// スコアの表示高さ

			float tw = 1.0f;		// テクスチャの幅
			float th = 1.0f;		// テクスチャの高さ
			float tx = 0.0f;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}


	}

	// 騎士のHPゲージ
	{
		// 枠
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[battle_UI[UI_HP_NONE_KNIGHT].texNo]);


		// スコアの位置やテクスチャー座標を反映
		float px = battle_UI[UI_HP_NONE_KNIGHT].pos.x;			// スコアの表示位置X
		float py = battle_UI[UI_HP_NONE_KNIGHT].pos.y;			// スコアの表示位置Y
		float pw = battle_UI[UI_HP_NONE_KNIGHT].w;				// スコアの表示幅
		float ph = battle_UI[UI_HP_NONE_KNIGHT].h;				// スコアの表示高さ

		float tw = 1.0f;		// テクスチャの幅
		float th = 1.0f;		// テクスチャの高さ
		float tx = 0.0f;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);



		// 赤
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[battle_UI[UI_HP_RED_KNIGHT].texNo]);


		// スコアの位置やテクスチャー座標を反映
		px = battle_UI[UI_HP_RED_KNIGHT].pos.x;			// スコアの表示位置X
		py = battle_UI[UI_HP_RED_KNIGHT].pos.y;			// スコアの表示位置Y
		pw = battle_UI[UI_HP_RED_KNIGHT].w;				// スコアの表示幅
		ph = battle_UI[UI_HP_RED_KNIGHT].h;				// スコアの表示高さ

		tw = 1.0f;		// テクスチャの幅
		th = 1.0f;		// テクスチャの高さ
		tx = 0.0f;			// テクスチャの左上X座標
		ty = 0.0f;			// テクスチャの左上Y座標

		// プレイヤーのHPに合わせてHPゲージの変更
		pw = pw * (float)(knight->hp / knight->hpMax);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);



		// 30%以下
		if (knight->hp / knight->hpMax > 0.3)
		{
			// 緑
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[battle_UI[UI_HP_GREEN_KNIGHT].texNo]);


			// スコアの位置やテクスチャー座標を反映
			px = battle_UI[UI_HP_GREEN_KNIGHT].pos.x;			// スコアの表示位置X
			py = battle_UI[UI_HP_GREEN_KNIGHT].pos.y;			// スコアの表示位置Y
			pw = battle_UI[UI_HP_GREEN_KNIGHT].w;				// スコアの表示幅
			ph = battle_UI[UI_HP_GREEN_KNIGHT].h;				// スコアの表示高さ

			tw = 1.0f;		// テクスチャの幅
			th = 1.0f;		// テクスチャの高さ
			tx = 0.0f;			// テクスチャの左上X座標
			ty = 0.0f;			// テクスチャの左上Y座標

			// プレイヤーのHPに合わせてHPゲージの変更
			pw = pw * (float)(knight->hp / knight->hpMax);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}


	}


}


