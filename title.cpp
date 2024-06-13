//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "sound.h"

#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(5)				// テクスチャの数

#define TEXTURE_WIDTH_LOGO			(236 * 2)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(66 * 2)			// 
#define ANGLE_MOVE					(360.0f / TEXTURE_HEIGHT_LOGO)

#define MAGIC_CIRCLE_ROT			(0.01f)			//魔法陣の回転速度

#define	LEAF_SIZE_X		(20.0f)		// 葉っぱサイズ
#define	LEAF_SIZE_Y		(20.0f)		// 葉っぱサイズ

#define	RIPPLES_SIZE_X		(20.0f)		// 葉っぱサイズ
#define	RIPPLES_SIZE_Y		(20.0f)		// 葉っぱサイズ




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/title_bg_000.png",
	"data/TEXTURE/title_sakura.png",
	"data/TEXTURE/title_ripples.jpg",
	"data/TEXTURE/title_SAKURAMORI.png",
	"data/TEXTURE/title_START.png",
	//"data/TEXTURE/title_NEW_GAME.png",
	//"data/TEXTURE/title_TUTORIAL.png",
	//"data/TEXTURE/title_EXIT.png",
	//"data/TEXTURE/title_OPTIONS.png",
	//"data/TEXTURE/title_EXIT.png",
	//"data/TEXTURE/title_option_cursor.png",
	//"data/TEXTURE/leaf.png",






};


static BOOL						g_Use;									// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;								// 幅と高さ
static XMFLOAT3					g_Pos;									// ポリゴンの座標
static int						g_TexNo;								// テクスチャ番号

static TITLE_LOGO				g_TitleLogo[TEXTURE_HEIGHT_LOGO];		//ロゴ構造体
float							startAngle = 0;

static TITLE_PARTICLE			g_Sakura[MAX_LEAF];						// 桜構造体
static TITLE_PARTICLE			g_Ripples[MAX_RIPPLES];					// 波構造体



static BOOL						g_Load = FALSE;

//float magic1_rot = 0;													//魔法陣の回転用
//float magic2_rot = 0;													//魔法陣の回転用

BOOL	startUse;											//スタートの表示フラグ
int		startCnt = 0;										//点滅用
//int		showCnt = 0;										//連続防止用

float	startTranasparency = 1.0f;							//点滅用

//static TITLE_OPTION	g_TitleOptions[TITLE_MAX];				//タイトルのオプション構造体
//BOOL	optionsUse;											//オプションの表示用
//
//XMFLOAT3	cursorPos;										// カーソルの座標
//BOOL		cursorUse;										// true:使っている  false:未使用
//float		cursorW, cursorH;								// 幅と高さ
//int			cursorTexNo;									// テクスチャ番号
//BOOL		cursorPosition;									// カーソルの位置




//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
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


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// 変数の初期化
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_TexNo = 0;

	startUse = TRUE;
	//showCnt = 0;

	//オプションの初期化
	//optionsUse = FALSE;

	//for (int i = 0; i < TITLE_MAX; i++)
	//{
	//	g_TitleOptions[i].pos = XMFLOAT3(700.0f, 220.0f + i * 60.0f, 0.0f);	// 中心点から表示
	//	g_TitleOptions[i].w = 210.0f;
	//	g_TitleOptions[i].h = 70.0f;
	//	g_TitleOptions[i].texNo = 5 + i;

	//}

	//g_TitleOptions[TITLE_EXIT].w = 120.0f;


	////カーソルの初期化
	//cursorPos = XMFLOAT3(580.0f, 220.0f, 0.0f);
	//cursorUse = FALSE;
	//cursorW = 48.0f;
	//cursorH = 48.0f;
	//cursorTexNo = 9;
	//cursorPosition = TITLE_START;



	for (int i = 0; i < TEXTURE_HEIGHT_LOGO; i++)
	{
		// 変数の初期化(ロゴの画像)
		g_TitleLogo[i].use = TRUE;
		g_TitleLogo[i].pos = XMFLOAT3(SCREEN_WIDTH / 2, 75.0f + i, 0.0f);	// 中心点から表示
		g_TitleLogo[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 回転率
		g_TitleLogo[i].w = TEXTURE_WIDTH_LOGO;
		g_TitleLogo[i].h = 1;
		g_TitleLogo[i].texNo = 3;

		g_TitleLogo[i].countAnim = 0;
		g_TitleLogo[i].patternAnim = 0;

		g_TitleLogo[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 移動量

	}

	// 桜の初期化
	for (int nCntLeaf = 0; nCntLeaf < MAX_LEAF; nCntLeaf++)
	{
		float fAngle, fLength;

		fAngle = (float)(rand() % 628 - 314) / 100.0f;
		fLength = (float)(rand() % (int)(0.15 * 200) / 100.0f - 0.15);

		g_Sakura[nCntLeaf].pos = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		g_Sakura[nCntLeaf].rot = XMFLOAT3(1.0f, 0.0f, 0.0f);
		g_Sakura[nCntLeaf].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Sakura[nCntLeaf].move = XMFLOAT3(sinf(fAngle) * fLength, -0.1f, cosf(fAngle) * fLength);
		g_Sakura[nCntLeaf].texNo = 1;


		ZeroMemory(&g_Sakura[nCntLeaf].material, sizeof(g_Sakura[nCntLeaf].material));
		g_Sakura[nCntLeaf].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_Sakura[nCntLeaf].fSizeX = LEAF_SIZE_X;
		g_Sakura[nCntLeaf].fSizeY = LEAF_SIZE_Y;
		g_Sakura[nCntLeaf].nLife = rand() % 75;
		g_Sakura[nCntLeaf].use = FALSE;
	}

	// 波紋の初期化
	for (int nCntRipples = 0; nCntRipples < MAX_RIPPLES; nCntRipples++)
	{
		g_Ripples[nCntRipples].pos = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		g_Ripples[nCntRipples].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Ripples[nCntRipples].scale = XMFLOAT3(10.0f, 10.0f, 10.0f);
		g_Ripples[nCntRipples].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Ripples[nCntRipples].texNo = 2;


		ZeroMemory(&g_Ripples[nCntRipples].material, sizeof(g_Ripples[nCntRipples].material));
		g_Ripples[nCntRipples].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_Ripples[nCntRipples].fSizeX = RIPPLES_SIZE_X;
		g_Ripples[nCntRipples].fSizeY = RIPPLES_SIZE_Y;
		g_Ripples[nCntRipples].nLife = rand() % 75;
		g_Ripples[nCntRipples].use = FALSE;
	}


	// BGM再生
	PlaySound(SOUND_LABEL_BGM_title);


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_Load == FALSE) return;

	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	//テクスチャの解放
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
void UpdateTitle(void)
{
	//スタートの処理
	{	
		//キー入力処理
		if ((GetKeyboardTrigger(DIK_RETURN)) && (startUse == TRUE))
		{// Enter押したら、スタートの表示を切り替える
			SetFade(FADE_OUT, MODE_GAME);

		}

		// ゲームパッドで入力処理
		if ((IsButtonTriggered(0, BUTTON_START)) && (startUse == TRUE))
		{// Enter押したら、スタートの表示を切り替える
			SetFade(FADE_OUT, MODE_GAME);

		}

		////連続防止
		//if (startUse == FALSE)
		//{
		//	showCnt++;
		//	if (showCnt == 10)
		//	{
		//		optionsUse = TRUE;
		//		cursorUse = TRUE;

		//	}
		//}
	}

	//// カーソルの移動処理
	//if (cursorUse == TRUE)
	//{
	//	//キー入力処理
	//	if (GetKeyboardTrigger(DIK_S))
	//	{
	//		cursorPosition = (++cursorPosition + TITLE_MAX) % TITLE_MAX;
	//	}
	//	else if (GetKeyboardTrigger(DIK_W))
	//	{
	//		cursorPosition = (--cursorPosition + TITLE_MAX) % TITLE_MAX;
	//	}
	//	else if (GetKeyboardTrigger(DIK_RETURN))
	//	{// Enter押したら、カーソルの位置によって処理する
	//		switch (cursorPosition)
	//		{
	//		case TITLE_START:

	//			SetFade(FADE_OUT, MODE_GAME);

	//			break;

	//		case TITLE_TUTORIAL:

	//			SetFade(FADE_OUT, MODE_TUTORIAL);

	//			break;

	//		//case TITLE_OPTIONS:

	//		//	SetBack(MODE_TITLE);

	//		//	SetFade(FADE_OUT, MODE_OPTION);

	//		//	break;


	//		case TITLE_EXIT:

	//			exit(-1);

	//			break;
	//		}

	//	}

	//	// ゲームパッドで入力処理
	//	if (IsButtonTriggered(0, BUTTON_UP))
	//	{
	//		cursorPosition = (++cursorPosition + TITLE_MAX) % TITLE_MAX;
	//	}
	//	else if (IsButtonTriggered(0, BUTTON_DOWN))
	//	{
	//		cursorPosition = (--cursorPosition + TITLE_MAX) % TITLE_MAX;
	//	}
	//	else if (IsButtonTriggered(0, BUTTON_START))
	//	{// Enter押したら、ステージを切り替える
	//		switch (cursorPosition)
	//		{
	//		case TITLE_START:

	//			SetFade(FADE_OUT, MODE_GAME);

	//			break;

	//		case TITLE_TUTORIAL:

	//			SetFade(FADE_OUT, MODE_TUTORIAL);

	//			break;

	//		//case TITLE_OPTIONS:
	//		//	SetBack(MODE_TITLE);

	//		//	SetFade(FADE_OUT, MODE_OPTION);

	//		//	break;


	//		case TITLE_EXIT:

	//			exit(-1);

	//			break;
	//		}

	//	}

	//	//カーソルの位置を変換
	//	cursorPos.y = g_TitleOptions[cursorPosition].pos.y;			// ポリゴンの座標

	//}


	//ロゴの更新
	{	//ラスター処理
		for (int i = 0; i < TEXTURE_HEIGHT_LOGO; i++)
		{

			if (g_TitleLogo[i].use == TRUE)
			{
				float DrawAngle = ANGLE_MOVE * i + startAngle;
				//float radian = (XM_PI / 180) * DrawAngle;
				float radian = XMConvertToRadians(DrawAngle);
				g_TitleLogo[i].pos.x = SCREEN_WIDTH / 2 + (sinf(radian) * 5);
			}


		}

		startAngle += ANGLE_MOVE;

		if (startAngle > 360.0f)
		{
			startAngle -= 360.0f;

		}
	}

	//スタート文字の更新
	{
		startCnt++;

		if ((startCnt % 15 == 0) && (startCnt % 30 != 0))
		{
			startTranasparency -= 0.5f;
		}

		if (startCnt % 30 == 0)
		{
			startTranasparency += 0.5f;
			
		}

		if (startCnt == 100)
		{
			startCnt = 0;
		}
	}


	//桜の更新
	{
		for (int nCntLeaf = 0; nCntLeaf < MAX_LEAF; nCntLeaf++)
		{

			if (!g_Sakura[nCntLeaf].use)
			{
				XMFLOAT3 pos;
				XMFLOAT3 move;
				XMFLOAT3 scale;
				XMFLOAT3 rot;

				float fAngle, fLength;
				float fSize;
				int nLife;
				int texNo;


				pos = g_Sakura[nCntLeaf].pos;
				scale = XMFLOAT3(1.5f, 1.5f, 1.5f);
				rot = XMFLOAT3(1.0f, 0.0f, 0.0f);


				fAngle = (float)(rand() % 628 - 314) / 100.0f;
				fLength = (float)(rand() % (int)(0.15 * 200) / 100.0f - 0.15);
				move.x = sinf(fAngle) * fLength;
				move.y = 0.2f;
				move.z = cosf(fAngle) * fLength;

				nLife = rand() % 100 + 300;
				texNo = 1;

				fSize = (float)(rand() % 10 + 20);
				pos.x = (float)(rand() % SCREEN_WIDTH);
				pos.y = (float)(rand() % SCREEN_HEIGHT + SCREEN_HEIGHT / 2);

				g_Sakura[nCntLeaf].pos = pos;
				g_Sakura[nCntLeaf].rot = rot;
				g_Sakura[nCntLeaf].scale = scale;
				g_Sakura[nCntLeaf].move = move;
				g_Sakura[nCntLeaf].texNo = texNo;
				g_Sakura[nCntLeaf].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f);
				g_Sakura[nCntLeaf].fSizeX = fSize;
				g_Sakura[nCntLeaf].fSizeY = fSize;
				g_Sakura[nCntLeaf].nLife = nLife;
				g_Sakura[nCntLeaf].use = TRUE;



				break;
			}


			if (g_Sakura[nCntLeaf].use)
			{// 使用中
				g_Sakura[nCntLeaf].pos.x += g_Sakura[nCntLeaf].move.x;
				g_Sakura[nCntLeaf].pos.y += g_Sakura[nCntLeaf].move.y;


				g_Sakura[nCntLeaf].nLife--;
				if (g_Sakura[nCntLeaf].nLife == 0)
				{
					g_Sakura[nCntLeaf].use = FALSE;

				}
				else if (g_Sakura[nCntLeaf].nLife <= 80)

				{
					// α値設定
					g_Sakura[nCntLeaf].material.Diffuse.w -= 0.05f;
					if (g_Sakura[nCntLeaf].material.Diffuse.w < 0.0f)
					{
						g_Sakura[nCntLeaf].material.Diffuse.w = 0.0f;
					}

				}
			}
		}
	}



	//波紋の更新
	for (int nCntRipples = 0; nCntRipples < MAX_RIPPLES; nCntRipples++)
	{

		if (!g_Ripples[nCntRipples].use)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			XMFLOAT3 scale;
			XMFLOAT3 rot;

			float fSize;
			int nLife;
			int texNo;


			pos = g_Ripples[nCntRipples].pos;
			scale = XMFLOAT3(10.0f, 10.0f, 10.0f);
			rot = XMFLOAT3(XM_PI / 2, 0.0f, 0.0f);
			move = XMFLOAT3(0.0f, 0.0f, 0.0f);

			pos.x = (float)(rand() % SCREEN_WIDTH);
			pos.y = (float)(SCREEN_HEIGHT - rand() % 100);



			nLife = rand() % 200 + 200;
			texNo = 2;

			fSize = (float)(rand() % 10 + 20);

			g_Ripples[nCntRipples].pos = pos;
			g_Ripples[nCntRipples].rot = rot;
			g_Ripples[nCntRipples].scale = scale;
			g_Ripples[nCntRipples].move = move;
			g_Ripples[nCntRipples].texNo = texNo;
			g_Ripples[nCntRipples].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f);
			g_Ripples[nCntRipples].fSizeX = fSize;
			g_Ripples[nCntRipples].fSizeY = fSize;
			g_Ripples[nCntRipples].nLife = nLife;
			g_Ripples[nCntRipples].use = TRUE;



			break;
		}

		if (g_Ripples[nCntRipples].use)
		{// 使用中


			g_Ripples[nCntRipples].nLife--;
			g_Ripples[nCntRipples].fSizeX++;
			g_Ripples[nCntRipples].fSizeY++;

			if (g_Ripples[nCntRipples].nLife == 0)
			{
				g_Ripples[nCntRipples].use = FALSE;

			}
			else if (g_Ripples[nCntRipples].nLife <= 80)

			{
				// α値設定
				g_Ripples[nCntRipples].material.Diffuse.w -= 0.02f;
				if (g_Ripples[nCntRipples].material.Diffuse.w < 0.0f)
				{
					g_Ripples[nCntRipples].material.Diffuse.w = 0.0f;
				}

			}

		}


	}

		

	

#ifdef _DEBUG	// デバッグ情報を表示する
	//PrintDebugProc("Player:↑ → ↓ ←　Space\n");

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
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

	// タイトルの背景を描画
	{

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	//// タイトルの魔法陣を描画
	//{

	//	// テクスチャ設定
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		////魔法陣1の位置やテクスチャー座標を反映
		//float px = 80.0f;	// 魔法陣1の表示位置X
		//float py = 85.0f;	// 魔法陣1の表示位置Y
		//float pw = 150.0f;		// 魔法陣1の表示幅
		//float ph = 150.0f;		// 魔法陣1の表示高さ

		//float tw = 1.0f;	// テクスチャの幅
		//float th = 1.0f;	// テクスチャの高さ
		//float tx = 0.0f;	// テクスチャの左上X座標
		//float ty = 0.0f;	// テクスチャの左上Y座標

		//// １枚のポリゴンの頂点とテクスチャ座標を設定
		//SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		//	XMFLOAT4(1.0f,1.0f, 1.0f, 1.0f),
		//	magic1_rot);

		//// ポリゴン描画
		//GetDeviceContext()->Draw(4, 0);


	//	// テクスチャ設定
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

	//	//魔法陣2の位置やテクスチャー座標を反映
	//	px = 80.0f;		// 魔法陣2表示位置X
	//	py = 85.0f;		// 魔法陣2表示位置Y
	//	pw = 75.0f;		// 魔法陣2表示幅
	//	ph = 75.0f;		// 魔法陣2表示高さ

	//	tw = 1.0f;		// テクスチャの幅
	//	th = 1.0f;		// テクスチャの高さ
	//	tx = 0.0f;		// テクスチャの左上X座標
	//	ty = 0.0f;		// テクスチャの左上Y座標

	//	// １枚のポリゴンの頂点とテクスチャ座標を設定
	//	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
	//		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//		magic2_rot);

	//	// ポリゴン描画
	//	GetDeviceContext()->Draw(4, 0);

	//}

	// 桜を描画
	{

		// テクスチャ設定
		for (int nCntLeaf = 0; nCntLeaf < MAX_LEAF; nCntLeaf++)
		{
			if (g_Sakura[nCntLeaf].use == TRUE)
			{

				// 加算合成に設定
				SetBlendState(BLEND_MODE_ADD);

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Sakura[nCntLeaf].texNo]);

				float px = g_Sakura[nCntLeaf].pos.x;	// 魔法陣1の表示位置X
				float py = g_Sakura[nCntLeaf].pos.y;	// 魔法陣1の表示位置Y
				float pw = g_Sakura[nCntLeaf].fSizeX;		// 魔法陣1の表示幅
				float ph = g_Sakura[nCntLeaf].fSizeY;		// 魔法陣1の表示高さ

				float tw = 1.0f;	// テクスチャの幅
				float th = 1.0f;	// テクスチャの高さ
				float tx = 0.0f;	// テクスチャの左上X座標
				float ty = 0.0f;	// テクスチャの左上Y座標


				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					g_Sakura[nCntLeaf].material.Diffuse,
					g_Sakura[nCntLeaf].rot.x);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);


				// 通常ブレンドに戻す
				SetBlendState(BLEND_MODE_ALPHABLEND);



			}
		}

	}

	// 波紋を描画
	{

		// テクスチャ設定
		for (int nCntRipples = 0; nCntRipples < MAX_LEAF; nCntRipples++)
		{
			if (g_Ripples[nCntRipples].use == TRUE)
			{

				// 加算合成に設定
				SetBlendState(BLEND_MODE_ADD);

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Ripples[nCntRipples].texNo]);

				float px = g_Ripples[nCntRipples].pos.x;	// 魔法陣1の表示位置X
				float py = g_Ripples[nCntRipples].pos.y;	// 魔法陣1の表示位置Y
				float pw = g_Ripples[nCntRipples].fSizeX;		// 魔法陣1の表示幅
				float ph = g_Ripples[nCntRipples].fSizeY;		// 魔法陣1の表示高さ

				float tw = 1.0f;	// テクスチャの幅
				float th = 1.0f;	// テクスチャの高さ
				float tx = 0.0f;	// テクスチャの左上X座標
				float ty = 0.0f;	// テクスチャの左上Y座標


				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					g_Ripples[nCntRipples].material.Diffuse,
					g_Ripples[nCntRipples].rot.x);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);


				// 通常ブレンドに戻す
				SetBlendState(BLEND_MODE_ALPHABLEND);



			}
		}

	}


	// タイトルのロゴを描画
	{
		for (int i = 0; i < TEXTURE_HEIGHT_LOGO; i++)
		{
			if (g_TitleLogo[i].use == TRUE)		// このプレイヤーが使われている？
			{									// Yes
				// テクスチャ設定
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TitleLogo[i].texNo]);

				//タイトルのロゴの位置やテクスチャー座標を反映
				float px = g_TitleLogo[i].pos.x;	// タイトルのロゴの表示位置X
				float py = g_TitleLogo[i].pos.y;	// タイトルのロゴの表示位置Y
				float pw = g_TitleLogo[i].w;		// タイトルのロゴの表示幅
				float ph = g_TitleLogo[i].h;		// タイトルのロゴの表示高さ

				float tw = 1.0f;								// テクスチャの幅
				float th = 1.0f / TEXTURE_HEIGHT_LOGO;			// テクスチャの高さ
				float tx = 0.0f;								// テクスチャの左上X座標
				float ty = 1.0f / TEXTURE_HEIGHT_LOGO * i;		// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
					g_TitleLogo[i].rot.z);

				// ポリゴン描画o
				GetDeviceContext()->Draw(4, 0);
			}


		}

	}

	//スタートを描画
	{
		if (startUse == TRUE)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

			//スタートの位置やテクスチャー座標を反映
			float px = SCREEN_WIDTH/2;	// スタートの表示位置X
			float py = 350.0f;	// スタートの表示位置Y
			float pw = 300.0f;		// スタートの表示幅
			float ph = 100.0f;		// スタートの表示高さ

			float tw = 1.0f;	// テクスチャの幅
			float th = 1.0f;	// テクスチャの高さ
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, startTranasparency),
				0.0f);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}

	}



	// タイトルのオプションを描画
	//{
	//	for (int i = 0; i < TITLE_MAX; i++)
	//	{
	//		if (optionsUse == TRUE)
	//		{
	//			// テクスチャ設定
	//			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TitleOptions[i].texNo]);

	//			//スタートの位置やテクスチャー座標を反映
	//			float px = g_TitleOptions[i].pos.x;	// オプションの表示位置X
	//			float py = g_TitleOptions[i].pos.y;	// オプションの表示位置Y
	//			float pw = g_TitleOptions[i].w;		// オプションの表示幅
	//			float ph = g_TitleOptions[i].h;		// オプションの表示高さ

	//			float tw = 1.0f;	// テクスチャの幅
	//			float th = 1.0f;	// テクスチャの高さ
	//			float tx = 0.0f;	// テクスチャの左上X座標
	//			float ty = 0.0f;	// テクスチャの左上Y座標

	//			// １枚のポリゴンの頂点とテクスチャ座標を設定
	//			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
	//				XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
	//				0.0f);

	//			// ポリゴン描画
	//			GetDeviceContext()->Draw(4, 0);

	//		}

	//	}


	//}

	////カーソルを描画
	//{
	//	if (cursorUse == TRUE)
	//	{
	//		// テクスチャ設定
	//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[cursorTexNo]);

	//		//スタートの位置やテクスチャー座標を反映
	//		float px = cursorPos.x;	// スタートの表示位置X
	//		float py = cursorPos.y;	// スタートの表示位置Y
	//		float pw = cursorW;		// スタートの表示幅
	//		float ph = cursorH;		// スタートの表示高さ

	//		float tw = 1.0f;	// テクスチャの幅
	//		float th = 1.0f;	// テクスチャの高さ
	//		float tx = 0.0f;	// テクスチャの左上X座標
	//		float ty = 0.0f;	// テクスチャの左上Y座標

	//		// １枚のポリゴンの頂点とテクスチャ座標を設定
	//		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
	//			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//			0.0f);

	//		// ポリゴン描画
	//		GetDeviceContext()->Draw(4, 0);

	//	}

	//}


}

