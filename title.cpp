//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "sound.h"

#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(5)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(236 * 2)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(66 * 2)			// 
#define ANGLE_MOVE					(360.0f / TEXTURE_HEIGHT_LOGO)

#define MAGIC_CIRCLE_ROT			(0.01f)			//���@�w�̉�]���x

#define	LEAF_SIZE_X		(20.0f)		// �t���σT�C�Y
#define	LEAF_SIZE_Y		(20.0f)		// �t���σT�C�Y

#define	RIPPLES_SIZE_X		(20.0f)		// �t���σT�C�Y
#define	RIPPLES_SIZE_Y		(20.0f)		// �t���σT�C�Y




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

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


static BOOL						g_Use;									// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;								// ���ƍ���
static XMFLOAT3					g_Pos;									// �|���S���̍��W
static int						g_TexNo;								// �e�N�X�`���ԍ�

static TITLE_LOGO				g_TitleLogo[TEXTURE_HEIGHT_LOGO];		//���S�\����
float							startAngle = 0;

static TITLE_PARTICLE			g_Sakura[MAX_LEAF];						// ���\����
static TITLE_PARTICLE			g_Ripples[MAX_RIPPLES];					// �g�\����



static BOOL						g_Load = FALSE;

//float magic1_rot = 0;													//���@�w�̉�]�p
//float magic2_rot = 0;													//���@�w�̉�]�p

BOOL	startUse;											//�X�^�[�g�̕\���t���O
int		startCnt = 0;										//�_�ŗp
//int		showCnt = 0;										//�A���h�~�p

float	startTranasparency = 1.0f;							//�_�ŗp

//static TITLE_OPTION	g_TitleOptions[TITLE_MAX];				//�^�C�g���̃I�v�V�����\����
//BOOL	optionsUse;											//�I�v�V�����̕\���p
//
//XMFLOAT3	cursorPos;										// �J�[�\���̍��W
//BOOL		cursorUse;										// true:�g���Ă���  false:���g�p
//float		cursorW, cursorH;								// ���ƍ���
//int			cursorTexNo;									// �e�N�X�`���ԍ�
//BOOL		cursorPosition;									// �J�[�\���̈ʒu




//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_TexNo = 0;

	startUse = TRUE;
	//showCnt = 0;

	//�I�v�V�����̏�����
	//optionsUse = FALSE;

	//for (int i = 0; i < TITLE_MAX; i++)
	//{
	//	g_TitleOptions[i].pos = XMFLOAT3(700.0f, 220.0f + i * 60.0f, 0.0f);	// ���S�_����\��
	//	g_TitleOptions[i].w = 210.0f;
	//	g_TitleOptions[i].h = 70.0f;
	//	g_TitleOptions[i].texNo = 5 + i;

	//}

	//g_TitleOptions[TITLE_EXIT].w = 120.0f;


	////�J�[�\���̏�����
	//cursorPos = XMFLOAT3(580.0f, 220.0f, 0.0f);
	//cursorUse = FALSE;
	//cursorW = 48.0f;
	//cursorH = 48.0f;
	//cursorTexNo = 9;
	//cursorPosition = TITLE_START;



	for (int i = 0; i < TEXTURE_HEIGHT_LOGO; i++)
	{
		// �ϐ��̏�����(���S�̉摜)
		g_TitleLogo[i].use = TRUE;
		g_TitleLogo[i].pos = XMFLOAT3(SCREEN_WIDTH / 2, 75.0f + i, 0.0f);	// ���S�_����\��
		g_TitleLogo[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ��]��
		g_TitleLogo[i].w = TEXTURE_WIDTH_LOGO;
		g_TitleLogo[i].h = 1;
		g_TitleLogo[i].texNo = 3;

		g_TitleLogo[i].countAnim = 0;
		g_TitleLogo[i].patternAnim = 0;

		g_TitleLogo[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �ړ���

	}

	// ���̏�����
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

	// �g��̏�����
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


	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_title);


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if (g_Load == FALSE) return;

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	//�e�N�X�`���̉��
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
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	//�X�^�[�g�̏���
	{	
		//�L�[���͏���
		if ((GetKeyboardTrigger(DIK_RETURN)) && (startUse == TRUE))
		{// Enter��������A�X�^�[�g�̕\����؂�ւ���
			SetFade(FADE_OUT, MODE_GAME);

		}

		// �Q�[���p�b�h�œ��͏���
		if ((IsButtonTriggered(0, BUTTON_START)) && (startUse == TRUE))
		{// Enter��������A�X�^�[�g�̕\����؂�ւ���
			SetFade(FADE_OUT, MODE_GAME);

		}

		////�A���h�~
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

	//// �J�[�\���̈ړ�����
	//if (cursorUse == TRUE)
	//{
	//	//�L�[���͏���
	//	if (GetKeyboardTrigger(DIK_S))
	//	{
	//		cursorPosition = (++cursorPosition + TITLE_MAX) % TITLE_MAX;
	//	}
	//	else if (GetKeyboardTrigger(DIK_W))
	//	{
	//		cursorPosition = (--cursorPosition + TITLE_MAX) % TITLE_MAX;
	//	}
	//	else if (GetKeyboardTrigger(DIK_RETURN))
	//	{// Enter��������A�J�[�\���̈ʒu�ɂ���ď�������
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

	//	// �Q�[���p�b�h�œ��͏���
	//	if (IsButtonTriggered(0, BUTTON_UP))
	//	{
	//		cursorPosition = (++cursorPosition + TITLE_MAX) % TITLE_MAX;
	//	}
	//	else if (IsButtonTriggered(0, BUTTON_DOWN))
	//	{
	//		cursorPosition = (--cursorPosition + TITLE_MAX) % TITLE_MAX;
	//	}
	//	else if (IsButtonTriggered(0, BUTTON_START))
	//	{// Enter��������A�X�e�[�W��؂�ւ���
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

	//	//�J�[�\���̈ʒu��ϊ�
	//	cursorPos.y = g_TitleOptions[cursorPosition].pos.y;			// �|���S���̍��W

	//}


	//���S�̍X�V
	{	//���X�^�[����
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

	//�X�^�[�g�����̍X�V
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


	//���̍X�V
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
			{// �g�p��
				g_Sakura[nCntLeaf].pos.x += g_Sakura[nCntLeaf].move.x;
				g_Sakura[nCntLeaf].pos.y += g_Sakura[nCntLeaf].move.y;


				g_Sakura[nCntLeaf].nLife--;
				if (g_Sakura[nCntLeaf].nLife == 0)
				{
					g_Sakura[nCntLeaf].use = FALSE;

				}
				else if (g_Sakura[nCntLeaf].nLife <= 80)

				{
					// ���l�ݒ�
					g_Sakura[nCntLeaf].material.Diffuse.w -= 0.05f;
					if (g_Sakura[nCntLeaf].material.Diffuse.w < 0.0f)
					{
						g_Sakura[nCntLeaf].material.Diffuse.w = 0.0f;
					}

				}
			}
		}
	}



	//�g��̍X�V
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
		{// �g�p��


			g_Ripples[nCntRipples].nLife--;
			g_Ripples[nCntRipples].fSizeX++;
			g_Ripples[nCntRipples].fSizeY++;

			if (g_Ripples[nCntRipples].nLife == 0)
			{
				g_Ripples[nCntRipples].use = FALSE;

			}
			else if (g_Ripples[nCntRipples].nLife <= 80)

			{
				// ���l�ݒ�
				g_Ripples[nCntRipples].material.Diffuse.w -= 0.02f;
				if (g_Ripples[nCntRipples].material.Diffuse.w < 0.0f)
				{
					g_Ripples[nCntRipples].material.Diffuse.w = 0.0f;
				}

			}

		}


	}

		

	

#ifdef _DEBUG	// �f�o�b�O����\������
	//PrintDebugProc("Player:�� �� �� ���@Space\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �^�C�g���̔w�i��`��
	{

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	//// �^�C�g���̖��@�w��`��
	//{

	//	// �e�N�X�`���ݒ�
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		////���@�w1�̈ʒu��e�N�X�`���[���W�𔽉f
		//float px = 80.0f;	// ���@�w1�̕\���ʒuX
		//float py = 85.0f;	// ���@�w1�̕\���ʒuY
		//float pw = 150.0f;		// ���@�w1�̕\����
		//float ph = 150.0f;		// ���@�w1�̕\������

		//float tw = 1.0f;	// �e�N�X�`���̕�
		//float th = 1.0f;	// �e�N�X�`���̍���
		//float tx = 0.0f;	// �e�N�X�`���̍���X���W
		//float ty = 0.0f;	// �e�N�X�`���̍���Y���W

		//// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		//	XMFLOAT4(1.0f,1.0f, 1.0f, 1.0f),
		//	magic1_rot);

		//// �|���S���`��
		//GetDeviceContext()->Draw(4, 0);


	//	// �e�N�X�`���ݒ�
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

	//	//���@�w2�̈ʒu��e�N�X�`���[���W�𔽉f
	//	px = 80.0f;		// ���@�w2�\���ʒuX
	//	py = 85.0f;		// ���@�w2�\���ʒuY
	//	pw = 75.0f;		// ���@�w2�\����
	//	ph = 75.0f;		// ���@�w2�\������

	//	tw = 1.0f;		// �e�N�X�`���̕�
	//	th = 1.0f;		// �e�N�X�`���̍���
	//	tx = 0.0f;		// �e�N�X�`���̍���X���W
	//	ty = 0.0f;		// �e�N�X�`���̍���Y���W

	//	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
	//		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//		magic2_rot);

	//	// �|���S���`��
	//	GetDeviceContext()->Draw(4, 0);

	//}

	// ����`��
	{

		// �e�N�X�`���ݒ�
		for (int nCntLeaf = 0; nCntLeaf < MAX_LEAF; nCntLeaf++)
		{
			if (g_Sakura[nCntLeaf].use == TRUE)
			{

				// ���Z�����ɐݒ�
				SetBlendState(BLEND_MODE_ADD);

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Sakura[nCntLeaf].texNo]);

				float px = g_Sakura[nCntLeaf].pos.x;	// ���@�w1�̕\���ʒuX
				float py = g_Sakura[nCntLeaf].pos.y;	// ���@�w1�̕\���ʒuY
				float pw = g_Sakura[nCntLeaf].fSizeX;		// ���@�w1�̕\����
				float ph = g_Sakura[nCntLeaf].fSizeY;		// ���@�w1�̕\������

				float tw = 1.0f;	// �e�N�X�`���̕�
				float th = 1.0f;	// �e�N�X�`���̍���
				float tx = 0.0f;	// �e�N�X�`���̍���X���W
				float ty = 0.0f;	// �e�N�X�`���̍���Y���W


				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					g_Sakura[nCntLeaf].material.Diffuse,
					g_Sakura[nCntLeaf].rot.x);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);


				// �ʏ�u�����h�ɖ߂�
				SetBlendState(BLEND_MODE_ALPHABLEND);



			}
		}

	}

	// �g���`��
	{

		// �e�N�X�`���ݒ�
		for (int nCntRipples = 0; nCntRipples < MAX_LEAF; nCntRipples++)
		{
			if (g_Ripples[nCntRipples].use == TRUE)
			{

				// ���Z�����ɐݒ�
				SetBlendState(BLEND_MODE_ADD);

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Ripples[nCntRipples].texNo]);

				float px = g_Ripples[nCntRipples].pos.x;	// ���@�w1�̕\���ʒuX
				float py = g_Ripples[nCntRipples].pos.y;	// ���@�w1�̕\���ʒuY
				float pw = g_Ripples[nCntRipples].fSizeX;		// ���@�w1�̕\����
				float ph = g_Ripples[nCntRipples].fSizeY;		// ���@�w1�̕\������

				float tw = 1.0f;	// �e�N�X�`���̕�
				float th = 1.0f;	// �e�N�X�`���̍���
				float tx = 0.0f;	// �e�N�X�`���̍���X���W
				float ty = 0.0f;	// �e�N�X�`���̍���Y���W


				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					g_Ripples[nCntRipples].material.Diffuse,
					g_Ripples[nCntRipples].rot.x);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);


				// �ʏ�u�����h�ɖ߂�
				SetBlendState(BLEND_MODE_ALPHABLEND);



			}
		}

	}


	// �^�C�g���̃��S��`��
	{
		for (int i = 0; i < TEXTURE_HEIGHT_LOGO; i++)
		{
			if (g_TitleLogo[i].use == TRUE)		// ���̃v���C���[���g���Ă���H
			{									// Yes
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TitleLogo[i].texNo]);

				//�^�C�g���̃��S�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_TitleLogo[i].pos.x;	// �^�C�g���̃��S�̕\���ʒuX
				float py = g_TitleLogo[i].pos.y;	// �^�C�g���̃��S�̕\���ʒuY
				float pw = g_TitleLogo[i].w;		// �^�C�g���̃��S�̕\����
				float ph = g_TitleLogo[i].h;		// �^�C�g���̃��S�̕\������

				float tw = 1.0f;								// �e�N�X�`���̕�
				float th = 1.0f / TEXTURE_HEIGHT_LOGO;			// �e�N�X�`���̍���
				float tx = 0.0f;								// �e�N�X�`���̍���X���W
				float ty = 1.0f / TEXTURE_HEIGHT_LOGO * i;		// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
					g_TitleLogo[i].rot.z);

				// �|���S���`��o
				GetDeviceContext()->Draw(4, 0);
			}


		}

	}

	//�X�^�[�g��`��
	{
		if (startUse == TRUE)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

			//�X�^�[�g�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = SCREEN_WIDTH/2;	// �X�^�[�g�̕\���ʒuX
			float py = 350.0f;	// �X�^�[�g�̕\���ʒuY
			float pw = 300.0f;		// �X�^�[�g�̕\����
			float ph = 100.0f;		// �X�^�[�g�̕\������

			float tw = 1.0f;	// �e�N�X�`���̕�
			float th = 1.0f;	// �e�N�X�`���̍���
			float tx = 0.0f;	// �e�N�X�`���̍���X���W
			float ty = 0.0f;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, startTranasparency),
				0.0f);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}

	}



	// �^�C�g���̃I�v�V������`��
	//{
	//	for (int i = 0; i < TITLE_MAX; i++)
	//	{
	//		if (optionsUse == TRUE)
	//		{
	//			// �e�N�X�`���ݒ�
	//			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TitleOptions[i].texNo]);

	//			//�X�^�[�g�̈ʒu��e�N�X�`���[���W�𔽉f
	//			float px = g_TitleOptions[i].pos.x;	// �I�v�V�����̕\���ʒuX
	//			float py = g_TitleOptions[i].pos.y;	// �I�v�V�����̕\���ʒuY
	//			float pw = g_TitleOptions[i].w;		// �I�v�V�����̕\����
	//			float ph = g_TitleOptions[i].h;		// �I�v�V�����̕\������

	//			float tw = 1.0f;	// �e�N�X�`���̕�
	//			float th = 1.0f;	// �e�N�X�`���̍���
	//			float tx = 0.0f;	// �e�N�X�`���̍���X���W
	//			float ty = 0.0f;	// �e�N�X�`���̍���Y���W

	//			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
	//				XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
	//				0.0f);

	//			// �|���S���`��
	//			GetDeviceContext()->Draw(4, 0);

	//		}

	//	}


	//}

	////�J�[�\����`��
	//{
	//	if (cursorUse == TRUE)
	//	{
	//		// �e�N�X�`���ݒ�
	//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[cursorTexNo]);

	//		//�X�^�[�g�̈ʒu��e�N�X�`���[���W�𔽉f
	//		float px = cursorPos.x;	// �X�^�[�g�̕\���ʒuX
	//		float py = cursorPos.y;	// �X�^�[�g�̕\���ʒuY
	//		float pw = cursorW;		// �X�^�[�g�̕\����
	//		float ph = cursorH;		// �X�^�[�g�̕\������

	//		float tw = 1.0f;	// �e�N�X�`���̕�
	//		float th = 1.0f;	// �e�N�X�`���̍���
	//		float tx = 0.0f;	// �e�N�X�`���̍���X���W
	//		float ty = 0.0f;	// �e�N�X�`���̍���Y���W

	//		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
	//			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//			0.0f);

	//		// �|���S���`��
	//		GetDeviceContext()->Draw(4, 0);

	//	}

	//}


}

