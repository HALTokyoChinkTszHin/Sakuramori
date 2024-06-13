//=============================================================================
//
// ���f������ [player.cpp]
// Author : �K�q���i�`�F���Y�[�V�F���j
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
// �}�N����`
//*****************************************************************************

#define PLAYER_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(10.0f)							// �v���C���[�̑��������킹��


#define TEXTURE_MAX			(1)								// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexRange(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static float		roty = 0.0f;

static LIGHT		g_Light;

static ID3D11Buffer* g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TextureName[] =
{
	"data/TEXTURE/circular_blue.png",
};

static RANGE		moving_Range;					




//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	// �v���C���[�i�R�m�j�̏�����
	InitPlayerKnight();

	MakeVertexRange();

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



	// �ړ��͈͂̏�����
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
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// �v���C���[�i�R�m�j�̏I������
	UninitPlayerKnight();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{// �e�N�X�`���̉��
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if (g_VertexBuffer != NULL)
	{// ���_�o�b�t�@�̉��
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA* cam = GetCamera();
	ENEMY* skeletons = GetEnemySkeletons();		// �G�l�~�[�̃|�C���^�[��������
	PLAYER* knight = GetPlayerKnight();

	if (knight->use == TRUE)
	{
		// �v���C���[�i�R�m�j�̍X�V����
		UpdatePlayerKnight();

	}

	int g_Mode = GetMode();
	int g_Turn = GetTurn();

	switch (g_Mode)
	{
	case MODE_TITLE:
		break;

	case MODE_GAME:
		// �R�m
		if (knight->use == TRUE)
		{
			// �ړ�����
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

			// �e���ˏ���
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

		// �^�[���I�����̏���
		case TURN_MAX:

			break;
			
		}


		break;

		// �Q�[���I�����̏���
	case MODE_MAX:
		break;
	}


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("knight X:%f Y:%f Z:%f\n", knight->pos.x, knight->pos.y, knight->pos.z);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	PLAYER* knight = GetPlayerKnight();

	if (knight->use == TRUE)
	{
		// �v���C���[�i�R�m�j�̕`�揈��
		DrawPlayerKnight();

	}


	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);


	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	{// �ړ��͈͂̕`��
		if (moving_Range.use == TRUE)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(moving_Range.scl.x, moving_Range.scl.y, moving_Range.scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(moving_Range.rot.x, moving_Range.rot.y, moving_Range.rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(moving_Range.pos.x, moving_Range.pos.y + 1.0f, moving_Range.pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&moving_Range.mtxWorld, mtxWorld);


			// �}�e���A���ݒ�
			SetMaterial(moving_Range.material);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ���e�X�g��L����
	SetAlphaTestEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(TRUE);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}



//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexRange(void)
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

	float fWidth = 16.0f;
	float fHeight = 16.0f;

	// ���_���W�̐ݒ�
	vertex[0].Position = { -fWidth / 2.0f, 0.0f,  fHeight / 2.0f };
	vertex[1].Position = { fWidth / 2.0f, 0.0f,  fHeight / 2.0f };
	vertex[2].Position = { -fWidth / 2.0f, 0.0f, -fHeight / 2.0f };
	vertex[3].Position = { fWidth / 2.0f, 0.0f, -fHeight / 2.0f };

	// �@���̐ݒ�
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// �g�U���̐ݒ�
	vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

	// �e�N�X�`�����W�̐ݒ�
	vertex[0].TexCoord = { 0.0f, 0.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}



//=============================================================================
// �v���C���[�̌����ύX
//=============================================================================
void changeDirection(float* roty, int targetDir, float rotationSpeed) 
{
	float diff;
	int sign;

	// �ڕW�̊p�x
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

	// ���݂̊p�x����ڕW�̊p�x�܂ł̍����v�Z
	diff = targetAngle - *roty;

	// ����-�΂���΂͈̔͂ɐ���
	while (diff > XM_PI) {
		diff -= 2 * XM_PI;
	}
	while (diff <= -XM_PI) {
		diff += 2 * XM_PI;
	}

	// ��]�ʂ�����������ꍇ�A���ږڕW�̊p�x�ɐݒ�
	if (fabs(diff) < rotationSpeed) {
		*roty = targetAngle;
	}
	else {
		// ��]���������肷�邽�߂ɕ������擾
		sign = (diff > 0) ? 1 : -1;
		// ��]�ʂ�������
		*roty += sign * rotationSpeed;
	}

	// roty��[0, 2��)�͈͓̔��Ɏ��܂�悤�ɒ���
	if (*roty >= 2 * XM_PI)
		*roty -= 2 * XM_PI;
	else if (*roty < 0.0f)
		*roty += 2 * XM_PI;
}
