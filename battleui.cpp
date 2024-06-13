//=============================================================================
//
// �o�g��UI���� [battleUI.cpp]
// Author : �K�q���i�`�F���Y�[�V�F���j
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX					(UI_IMAGE_MAX)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

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

static UI		battle_UI[UI_MAX];						// �v���C���[



static BOOL						g_Load = FALSE;

int								ui_Time;				// ���Ԍv�Z�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBattleUI(void)
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

	ui_Time = 0;			// ���Ԃ̏�����

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// UI(�퓬�J�n)�̏�����
	battle_UI[UI_START].use = TRUE;
	battle_UI[UI_START].w = 200;
	battle_UI[UI_START].h = 100;
	battle_UI[UI_START].pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };
	battle_UI[UI_START].texNo = IMAGE_START;

	// UI(WIN)�̏�����
	battle_UI[UI_WIN].use = FALSE;
	battle_UI[UI_WIN].w = 200;
	battle_UI[UI_WIN].h = 100;
	battle_UI[UI_WIN].pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };
	battle_UI[UI_WIN].texNo = IMAGE_WIN;

	// UI(GAMEOVER)�̏�����
	battle_UI[UI_GAMEOVER].use = FALSE;
	battle_UI[UI_GAMEOVER].w = 200;
	battle_UI[UI_GAMEOVER].h = 100;
	battle_UI[UI_GAMEOVER].pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };
	battle_UI[UI_GAMEOVER].texNo = IMAGE_GAMEOVER;

	// UI(�s����)�̏�����
	battle_UI[UI_TURN_NONE].use = FALSE;
	battle_UI[UI_TURN_NONE].w = 500;
	battle_UI[UI_TURN_NONE].h = 80;
	battle_UI[UI_TURN_NONE].pos = { 890.0f, 100.0f, 0.0f };
	battle_UI[UI_TURN_NONE].texNo = IMAGE_TURN_NONE;

	// �s����(1)
	battle_UI[UI_TURN_FIRST].use = FALSE;
	battle_UI[UI_TURN_FIRST].w = 150;
	battle_UI[UI_TURN_FIRST].h = 100;
	battle_UI[UI_TURN_FIRST].pos = { 740.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_FIRST].texNo = IMAGE_TURN_KNIGHT;

	// �s����(2)
	battle_UI[UI_TURN_SECOND].use = FALSE;
	battle_UI[UI_TURN_SECOND].w = 150;
	battle_UI[UI_TURN_SECOND].h = 100;
	battle_UI[UI_TURN_SECOND].pos = { 800.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_SECOND].texNo = IMAGE_TURN_SKELETONS;

	// �s����(3)
	battle_UI[UI_TURN_THIRD].use = FALSE;
	battle_UI[UI_TURN_THIRD].w = 150;
	battle_UI[UI_TURN_THIRD].h = 100;
	battle_UI[UI_TURN_THIRD].pos = { 860.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_THIRD].texNo = IMAGE_TURN_KNIGHT;

	// �s����(4)
	battle_UI[UI_TURN_FOURTH].use = FALSE;
	battle_UI[UI_TURN_FOURTH].w = 150;
	battle_UI[UI_TURN_FOURTH].h = 100;
	battle_UI[UI_TURN_FOURTH].pos = { 920.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_FOURTH].texNo = IMAGE_TURN_SKELETONS;

	// �s����(5)
	battle_UI[UI_TURN_FIFTH].use = FALSE;
	battle_UI[UI_TURN_FIFTH].w = 150;
	battle_UI[UI_TURN_FIFTH].h = 100;
	battle_UI[UI_TURN_FIFTH].pos = { 980.0f, 50.0f, 0.0f };
	battle_UI[UI_TURN_FIFTH].texNo = IMAGE_TURN_KNIGHT;



	// UI(�R�m�̃A�C�R��)�̏�����
	battle_UI[UI_HP_ICON_KNIGHT].use = FALSE;
	battle_UI[UI_HP_ICON_KNIGHT].w = 150;
	battle_UI[UI_HP_ICON_KNIGHT].h = 150;
	battle_UI[UI_HP_ICON_KNIGHT].pos = { 70.0f, 450.0f, 0.0f };
	battle_UI[UI_HP_ICON_KNIGHT].texNo = IMAGE_ICON_KNIGHT;

	// UI(HP)�̏�����
	// ���~���̃Q�[�W
	battle_UI[UI_HP_NONE_KNIGHT].use = FALSE;
	battle_UI[UI_HP_NONE_KNIGHT].w = 380;
	battle_UI[UI_HP_NONE_KNIGHT].h = 20;
	battle_UI[UI_HP_NONE_KNIGHT].pos = { 100.0f, 500.0f, 0.0f };
	battle_UI[UI_HP_NONE_KNIGHT].texNo = IMAGE_HP_NONE;

	// HP30%�ȉ��̃Q�[�W
	battle_UI[UI_HP_RED_KNIGHT].use = FALSE;
	battle_UI[UI_HP_RED_KNIGHT].w = 380;
	battle_UI[UI_HP_RED_KNIGHT].h = 20;
	battle_UI[UI_HP_RED_KNIGHT].pos = battle_UI[UI_HP_NONE_KNIGHT].pos;
	battle_UI[UI_HP_RED_KNIGHT].texNo = IMAGE_HP_RED;

	// HP30%�ȏ�̃Q�[�W
	battle_UI[UI_HP_GREEN_KNIGHT].use = FALSE;
	battle_UI[UI_HP_GREEN_KNIGHT].w = 380;
	battle_UI[UI_HP_GREEN_KNIGHT].h = 20;
	battle_UI[UI_HP_GREEN_KNIGHT].pos = battle_UI[UI_HP_NONE_KNIGHT].pos;
	battle_UI[UI_HP_GREEN_KNIGHT].texNo = IMAGE_HP_GREEN;



	// UI(�퓬�R�}���h)�̏�����
	battle_UI[UI_OPTION].use = FALSE;
	battle_UI[UI_OPTION].w = 270;
	battle_UI[UI_OPTION].h = 250;
	battle_UI[UI_OPTION].pos = { 800.0f, 300.0f, 0.0f };
	battle_UI[UI_OPTION].texNo = IMAGE_COMMAND;

	// UI(����|�b�^�[)�̏�����
	battle_UI[UI_SELECT].use = FALSE;
	battle_UI[UI_SELECT].w = 100;
	battle_UI[UI_SELECT].h = 60;
	battle_UI[UI_SELECT].pos = { 880.0f, 480.0f, 0.0f };
	battle_UI[UI_SELECT].texNo = IMAGE_CONFIRM;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateBattleUI(void)
{
	PLAYER* knight = GetPlayerKnight();	// �v���C���[�i�R�m�j�̃|�C���^�[��������

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
		// �s�����̕ύX
		battle_UI[UI_TURN_FIRST].texNo = IMAGE_TURN_SKELETONS;
		battle_UI[UI_TURN_SECOND].texNo = IMAGE_TURN_KNIGHT;

		break;

		// �Q�[���I�����̏���
	case TURN_MAX:

		for (int i = 0; i < UI_MAX; i++)
		{
			battle_UI[i].use = FALSE;
		}

		break;
	}

#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattleUI(void)
{
	PLAYER* knight = GetPlayerKnight();	// �v���C���[�i�R�m�j�̃|�C���^�[��������

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

	// UI ��`��
	for (int i = 0; i < UI_MAX; i++)
	{
		int hp_Knight_Start = UI_HP_NONE_KNIGHT;
		int hp_Knight_End = UI_HP_GREEN_KNIGHT;
		if ((i >= hp_Knight_Start) && (i <= hp_Knight_End))

			continue;


		if(battle_UI[i].use == TRUE)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[battle_UI[i].texNo]);


			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = battle_UI[i].pos.x;			// �X�R�A�̕\���ʒuX
			float py = battle_UI[i].pos.y;			// �X�R�A�̕\���ʒuY
			float pw = battle_UI[i].w;				// �X�R�A�̕\����
			float ph = battle_UI[i].h;				// �X�R�A�̕\������

			float tw = 1.0f;		// �e�N�X�`���̕�
			float th = 1.0f;		// �e�N�X�`���̍���
			float tx = 0.0f;			// �e�N�X�`���̍���X���W
			float ty = 0.0f;			// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}


	}

	// �R�m��HP�Q�[�W
	{
		// �g
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[battle_UI[UI_HP_NONE_KNIGHT].texNo]);


		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = battle_UI[UI_HP_NONE_KNIGHT].pos.x;			// �X�R�A�̕\���ʒuX
		float py = battle_UI[UI_HP_NONE_KNIGHT].pos.y;			// �X�R�A�̕\���ʒuY
		float pw = battle_UI[UI_HP_NONE_KNIGHT].w;				// �X�R�A�̕\����
		float ph = battle_UI[UI_HP_NONE_KNIGHT].h;				// �X�R�A�̕\������

		float tw = 1.0f;		// �e�N�X�`���̕�
		float th = 1.0f;		// �e�N�X�`���̍���
		float tx = 0.0f;			// �e�N�X�`���̍���X���W
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);



		// ��
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[battle_UI[UI_HP_RED_KNIGHT].texNo]);


		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		px = battle_UI[UI_HP_RED_KNIGHT].pos.x;			// �X�R�A�̕\���ʒuX
		py = battle_UI[UI_HP_RED_KNIGHT].pos.y;			// �X�R�A�̕\���ʒuY
		pw = battle_UI[UI_HP_RED_KNIGHT].w;				// �X�R�A�̕\����
		ph = battle_UI[UI_HP_RED_KNIGHT].h;				// �X�R�A�̕\������

		tw = 1.0f;		// �e�N�X�`���̕�
		th = 1.0f;		// �e�N�X�`���̍���
		tx = 0.0f;			// �e�N�X�`���̍���X���W
		ty = 0.0f;			// �e�N�X�`���̍���Y���W

		// �v���C���[��HP�ɍ��킹��HP�Q�[�W�̕ύX
		pw = pw * (float)(knight->hp / knight->hpMax);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);



		// 30%�ȉ�
		if (knight->hp / knight->hpMax > 0.3)
		{
			// ��
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[battle_UI[UI_HP_GREEN_KNIGHT].texNo]);


			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			px = battle_UI[UI_HP_GREEN_KNIGHT].pos.x;			// �X�R�A�̕\���ʒuX
			py = battle_UI[UI_HP_GREEN_KNIGHT].pos.y;			// �X�R�A�̕\���ʒuY
			pw = battle_UI[UI_HP_GREEN_KNIGHT].w;				// �X�R�A�̕\����
			ph = battle_UI[UI_HP_GREEN_KNIGHT].h;				// �X�R�A�̕\������

			tw = 1.0f;		// �e�N�X�`���̕�
			th = 1.0f;		// �e�N�X�`���̍���
			tx = 0.0f;			// �e�N�X�`���̍���X���W
			ty = 0.0f;			// �e�N�X�`���̍���Y���W

			// �v���C���[��HP�ɍ��킹��HP�Q�[�W�̕ύX
			pw = pw * (float)(knight->hp / knight->hpMax);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}


	}


}


