//=============================================================================
//
// �e���ˏ��� [bullet.cpp]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"

#include "shadow.h"
#include "bullet.h"
#include "enemy.h"
#include "enemyskeletons.h"

#include "collision.h"
#include "score.h"





//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BULLET				"data/MODEL/knight/knight_slash.obj"				// �ǂݍ��ރ��f����


#define	BULLET_SPEED		(2.0f)			// �e�̈ړ��X�s�[�h


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BULLET						g_Bullet[MAX_BULLET];	// �e���[�N

int g_Bullet_load = 0;

BOOL change = TRUE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{


	// �e���[�N�̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		LoadModel(MODEL_BULLET, &g_Bullet[nCntBullet].model);
		g_Bullet[nCntBullet].load = true;


		ZeroMemory(&g_Bullet[nCntBullet].material, sizeof(g_Bullet[nCntBullet].material));
		g_Bullet[nCntBullet].material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

		g_Bullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		g_Bullet[nCntBullet].rot = { 0.0f, 0.0f, 0.0f };
		g_Bullet[nCntBullet].scl = { 1.0f, 1.0f, 1.0f };
		g_Bullet[nCntBullet].spd = BULLET_SPEED;
		g_Bullet[nCntBullet].size = BULLET_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Bullet[nCntBullet].model, &g_Bullet[nCntBullet].diffuse[0]);

		g_Bullet[nCntBullet].use = FALSE;

	}


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_Bullet[nCntBullet].load)
		{
			UnloadModel(&g_Bullet[nCntBullet].model);
			g_Bullet[nCntBullet].load = false;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	ENEMY* skeletons = GetEnemySkeletons();

	for (int i = 0; i < MAX_BULLET; i++)
	{


		if (g_Bullet[i].use == TRUE)
		{

			// �e�̈ړ�����
			//����
			{
				g_Bullet[i].pos.x -= sinf(g_Bullet[i].rot.y) * g_Bullet[i].spd;
				g_Bullet[i].pos.z -= cosf(g_Bullet[i].rot.y) * g_Bullet[i].spd;

			}


			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_Bullet[i].shadowIdx, XMFLOAT3(g_Bullet[i].pos.x, 0.1f, g_Bullet[i].pos.z));


			// �t�B�[���h�̊O�ɏo����e����������
			if (g_Bullet[i].pos.x < MAP_LEFT
				|| g_Bullet[i].pos.x > MAP_RIGHT
				|| g_Bullet[i].pos.z < MAP_DOWN
				|| g_Bullet[i].pos.z > MAP_TOP)
			{
				g_Bullet[i].use = FALSE;
				ReleaseShadow(g_Bullet[i].shadowIdx);
			}

		}
	}

#ifdef _DEBUG



#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;


	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullet[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Bullet[i].scl.x, g_Bullet[i].scl.y, g_Bullet[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Bullet[i].rot.x, g_Bullet[i].rot.y + XM_PI, g_Bullet[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Bullet[i].pos.x, g_Bullet[i].pos.y, g_Bullet[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Bullet[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Bullet[i].model);
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

}


//=============================================================================
// �e�̃p�����[�^���Z�b�g
//=============================================================================
int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot)
{
	int nIdxBullet = -1;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_Bullet[nCntBullet].use)
		{
			g_Bullet[nCntBullet].pos = pos;
			g_Bullet[nCntBullet].rot = rot;
			g_Bullet[nCntBullet].scl = { 1.0f, 1.0f, 1.0f };
			g_Bullet[nCntBullet].use = TRUE;

			// �e�̐ݒ�
			g_Bullet[nCntBullet].shadowIdx = CreateShadow(g_Bullet[nCntBullet].pos, 0.5f, 0.5f);

			nIdxBullet = nCntBullet;

		//	PlaySound(SOUND_LABEL_SE_shot000);

			break;
		}
	}

	return nIdxBullet;
}

//=============================================================================
// �e�̎擾
//=============================================================================
BULLET *GetBullet(void)
{
	return &(g_Bullet[0]);
}
