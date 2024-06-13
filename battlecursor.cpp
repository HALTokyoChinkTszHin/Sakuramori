//=============================================================================
//
// �o�g���J�[�\������ [battlecursor.h]
// Author : �K�q���i�`�F���Y�[�V�F���j
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(2)				// �e�N�X�`���̐�

#define	CURSOR_WIDTH			(8.0f)			// ���_�T�C�Y
#define	CURSOR_HEIGHT			(12.0f)			// ���_�T�C�Y

#define	MAX_BATTLECURSOR	(1)			// �J�[�\���ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	scl;			// �X�P�[��
	MATERIAL	material;		// �}�e���A��
	float		fWidth;			// ��
	float		fHeight;		// ����
	BOOL		use;			// �g�p���Ă��邩�ǂ���
	BOOL		texNo;			// �e�N�X�`���ԍ�

} BATTLECURSOR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBattleCursor(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static BATTLECURSOR					g_BattleCursor[MAX_BATTLECURSOR];	// �J�[�\�����[�N
static BOOL					g_bAlpaTest;		// �A���t�@�e�X�gON/OFF

XMFLOAT3 moving_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);

static char* g_TextureName[] =
{
	"data/TEXTURE/Cursor_Moving.png",
	"data/TEXTURE/Cursor_Target.png",
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBattleCursor(void)
{
	MakeVertexBattleCursor();

	// �e�N�X�`������
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


	// �J�[�\�����[�N�̏�����
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
	// �J�[�\���̐ݒ�
	SetBattleCursor(XMFLOAT3(0.0f, 0.0f, 0.0f), 60.0f, 90.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBattleCursor(void)
{
	for(int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if(g_Texture[nCntTex] != NULL)
		{// �e�N�X�`���̉��
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if(g_VertexBuffer != NULL)
	{// ���_�o�b�t�@�̉��
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBattleCursor(void)
{
	ENEMY* enemy = GetEnemySkeletons();		// �G�l�~�[�̃|�C���^�[��������
	PLAYER* knight = GetPlayerKnight();	// �v���C���[�i�R�m�j�̃|�C���^�[��������

	// �G�l�~�[���S�����S�������ԑJ��
	int enemy_count = 0;
	for (int i = 0; i < MAX_BATTLE_ENEMY; i++)
	{
		if (enemy[i].use == FALSE) continue;
		enemy_count++;
	}

	// �G�l�~�[���O�C�H
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

	// �G�l�~�[���O�C�H
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
					// �U��
					SetTurn(TURN_TARGET);

				}
				if (GetKeyboardTrigger(DIK_K))
				{
					// �ړ�
					SetTurn(TURN_MOVING);
				}
				if (GetKeyboardTrigger(DIK_I))
				{
					// �h��
					SetTurn(TURN_DEFEND);

				}
				if (GetKeyboardTrigger(DIK_J))
				{
					// ����
					SetTurn(TURN_RUN);

				}


				break;

			case TURN_MOVING:

				// �ړ�����
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
					//	(player->pos.z != g_BattleCursor[nCntCursor].pos.z))// ���`��Ԃ����s����H
					//{
					//	for (int i = 0; i < 100; i++)
					//	{
					//		PlayerMoving(g_BattleCursor[nCntCursor].pos, 100.0f);


					//	}
					//}

					moving_pos = g_BattleCursor[nCntCursor].pos;

					//if ((player->pos.x == g_BattleCursor[nCntCursor].pos.x) &&
					//	(player->pos.z == g_BattleCursor[nCntCursor].pos.z))// ���`��Ԃ��~�߂�H
					//{
					//	SetTurn(TURN_ENEMY);

					//}
				}

				// �ړ��͈͊O�̕\��
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


				// �Q�[���I�����̏���
			case TURN_MAX:
				break;
			}

		}

	}


#ifdef _DEBUG
	// �A���t�@�e�X�gON/OFF
	if(GetKeyboardTrigger(DIK_F1))
	{
		g_bAlpaTest = g_bAlpaTest ? FALSE: TRUE;
	}

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattleCursor(void)
{
	// ���e�X�g�ݒ�
	if (g_bAlpaTest == TRUE)
	{
		// ���e�X�g��L����
		SetAlphaTestEnable(TRUE);
	}

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for(int i = 0; i < MAX_BATTLECURSOR; i++)
	{
		if(g_BattleCursor[i].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			// �����s��i�����s��j��]�u�s�񂳂��ċt�s�������Ă��(����)
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];


			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_BattleCursor[i].scl.x, g_BattleCursor[i].scl.y, g_BattleCursor[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_BattleCursor[i].pos.x, g_BattleCursor[i].pos.y, g_BattleCursor[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);


			// �}�e���A���ݒ�
			SetMaterial(g_BattleCursor[i].material);

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BattleCursor[i].texNo]);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// ���e�X�g�𖳌���
	SetAlphaTestEnable(FALSE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBattleCursor(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = CURSOR_WIDTH;
	float fHeight = CURSOR_HEIGHT;

	// ���_���W�̐ݒ�
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 2.0f, 0.0f, 0.0f);

	// �@���̐ݒ�
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// �g�U���̐ݒ�
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}

//=============================================================================
// �J�[�\���̃p�����[�^���Z�b�g
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
// �ړ����W�̐ݒ�
//=============================================================================
void SetMovingPos(XMFLOAT3 pos)
{

	moving_pos = pos;	// ���̃��[�h���Z�b�g���Ă���


}

//=============================================================================
// �ړ����W�̎擾
//=============================================================================
XMFLOAT3 GetMovingPos(void)
{
	return moving_pos;
}


