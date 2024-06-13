//=============================================================================
//
// �G�l�~�[���f������ [enemyskeletons.cpp]
// Author : �K�q���i�`�F���Y�[�V�F���j
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
// �}�N����`
//*****************************************************************************
#define	MODEL_SKELETONS			"data/MODEL/enemy/skeletons_nomal.obj"		// �ǂݍ��ރ��f����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Skeletons[MAX_SKELETONS];				// �X�P���g��

int g_Skeletons_load = 0;

XMFLOAT3 originalPos[MAX_SKELETONS];

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl0[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(300.0f, ENEMY_OFFSET_Y,  20.0f), XMFLOAT3(0.0f, -XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, 20.0f), XMFLOAT3(0.0f, XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2},

};

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl1[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-600.0f, ENEMY_OFFSET_Y,  50.0f), XMFLOAT3(0.0f, XM_PI , 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-600.0f, ENEMY_OFFSET_Y, -450.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl2[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(370.0f, ENEMY_OFFSET_Y,  -250.0f), XMFLOAT3(0.0f, XM_PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(370.0f, ENEMY_OFFSET_Y, -550.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl3[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(40.0f, ENEMY_OFFSET_Y, 100.0f), XMFLOAT3(0.0f, XM_PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(40.0f, ENEMY_OFFSET_Y, 450.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA g_g_Skeletons_MoveTbl4[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, ENEMY_OFFSET_Y,  -550.0f), XMFLOAT3(0.0f, -XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-250.0f, ENEMY_OFFSET_Y, -550.0f), XMFLOAT3(0.0f, XM_PI / 2, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_g_Skeletons_MoveTbl0,
	g_g_Skeletons_MoveTbl1,
	g_g_Skeletons_MoveTbl2,
	g_g_Skeletons_MoveTbl3,
	g_g_Skeletons_MoveTbl4,

};



//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemySkeletons(void)
{
	int g_Mode = GetMode();

	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		LoadModel(MODEL_SKELETONS, &g_Skeletons[i].model);
		g_Skeletons[i].load = TRUE;

		g_Skeletons[i].pos = XMFLOAT3(10.0f + i * 30.0f, ENEMY_OFFSET_Y, 150.0f);
		g_Skeletons[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Skeletons[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// �p�����[�^�̐ݒ�
		g_Skeletons[i].hpMax = 20;
		g_Skeletons[i].hp = g_Skeletons[i].hpMax;
		g_Skeletons[i].str = 10;
		g_Skeletons[i].def = 10;
		g_Skeletons[i].spd = 10;

		g_Skeletons[i].movSpd = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Skeletons[i].size = ENEMY_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Skeletons[i].model, &g_Skeletons[i].diffuse[0]);

		XMFLOAT3 pos = g_Skeletons[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Skeletons[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		
		g_Skeletons[i].vision = FALSE;			// ����͈͂��N���A
		g_Skeletons[i].lost = FALSE;			// �v���C���[������������Ԃ�������

		g_Skeletons[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Skeletons[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Skeletons[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		g_Skeletons[i].use = TRUE;			// TRUE:�����Ă�

	}

	 //0�Ԃ������`��Ԃœ������Ă݂�
	g_Skeletons[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Skeletons[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Skeletons[0].tblMax = sizeof(g_g_Skeletons_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	//1�Ԃ������`��Ԃœ������Ă݂�
	g_Skeletons[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Skeletons[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Skeletons[1].tblMax = sizeof(g_g_Skeletons_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	//2�Ԃ������`��Ԃœ������Ă݂�
	g_Skeletons[2].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Skeletons[2].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Skeletons[2].tblMax = sizeof(g_g_Skeletons_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	//3�Ԃ������`��Ԃœ������Ă݂�
	g_Skeletons[3].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Skeletons[3].tblNo = 3;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Skeletons[3].tblMax = sizeof(g_g_Skeletons_MoveTbl3) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	//4�Ԃ������`��Ԃœ������Ă݂�
	g_Skeletons[4].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Skeletons[4].tblNo = 4;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Skeletons[4].tblMax = sizeof(g_g_Skeletons_MoveTbl4) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g



	if (g_Mode == MODE_BATTLE)
	{
		for (int i = 0; i < MAX_SKELETONS; i++)
		{
			g_Skeletons[i].use = FALSE;			// TRUE:�����Ă�

		}

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemySkeletons(void)
{

	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		if (g_Skeletons[i].load)
		{
			UnloadModel(&g_Skeletons[i].model);
			g_Skeletons[i].load = FALSE;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemySkeletons(void)
{
	PLAYER* knight = GetPlayerKnight();		// �v���C���[�i�R�m�j�̃|�C���^�[��������
	int g_Mode = GetMode();
	int g_Turn = GetTurn();

	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		if (g_Skeletons[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{// Yes
			if (g_Skeletons[i].hp <= 0)
			{
				g_Skeletons[i].use = FALSE;

			}

			// ���[�h�ɂ���ď����𕪂���
			switch (g_Mode)
			{
			case MODE_TITLE:		// �^�C�g����ʂ̕`��
				break;

			case MODE_GAME:			// �Q�[����ʂ̕`��
				// ���̈ʒu���L�^����
				originalPos[i] = g_Skeletons[i].pos;

				if (g_Skeletons[i].vision == TRUE)
				{
					// �v���C���[�������Ă���ꍇ�̏���
					// �z�[�~���O�̈ړ��������s��
					XMVECTOR epos = XMLoadFloat3(&g_Skeletons[i].pos);
					XMVECTOR vec = (XMLoadFloat3(&knight->pos) - epos) * 0.01f;				//�����ƃv���C���[�̍��������߂�
					epos += vec;
					XMStoreFloat3(&g_Skeletons[i].pos, epos);
					float angle = atan2f(g_Skeletons[i].pos.x - knight->pos.x, g_Skeletons[i].pos.z - knight->pos.z);							//(y�̍����A�I�̍���)���̍������g���Ċp�x�����߂Ă���
					g_Skeletons[i].rot.y = angle;
					g_Skeletons[i].pos.y = ENEMY_OFFSET_Y;

				}
				else
				{
					// �v���C���[�����������ꍇ�̏���
					if (!g_Skeletons[i].lost) 
					{
						// �v���C���[������������Ԃɂ���
						g_Skeletons[i].lost = TRUE;

						// ���̈ʒu�ɖ߂邽�߂Ɏ��Ԃ�����������
						g_Skeletons[i].time = 0.0f;
					}

					// �G�L�����N�^�[�����̈ʒu�ɖ߂鏈��
					INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Skeletons[i].tblNo];
					XMVECTOR nowPosVector = XMLoadFloat3(&g_Skeletons[i].pos);								// XMVECTOR�֕ϊ�
					XMVECTOR originalPosVector = XMLoadFloat3(&originalPos[i]) - nowPosVector;			// XYZ�ړ��ʂ��v�Z���Ă���
					float moveSpeed = 0.1f;
					originalPosVector *= moveSpeed;														// ���݂̈ړ��ʂ��v�Z���Ă���

					// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
					XMStoreFloat3(&g_Skeletons[i].pos, nowPosVector + originalPosVector);

					// ���̈ʒu�ɖ߂������ǂ����̔���
					float distanceThreshold = 0.1f; // ���̈ʒu�ɖ߂����Ɣ��肷�鋗����臒l
					if (XMVectorGetX(XMVector3LengthSq(originalPosVector - XMLoadFloat3(&g_Skeletons[i].pos))) < distanceThreshold * distanceThreshold) 
					{
						// ���̈ʒu�ɖ߂����Ɣ��肳�ꂽ�ꍇ
						g_Skeletons[i].lost = false; // �v���C���[������������Ԃ�����

					}

					// �p�j�̈ړ�����
					if (g_Skeletons[i].tblMax > 0)	// ���`��Ԃ����s����H
					{	// ���`��Ԃ̏���
						int nowNo = (int)g_Skeletons[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
						int maxNo = g_Skeletons[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
						int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
						INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Skeletons[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

						XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
						XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
						XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

						XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
						XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
						XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

						float nowTime = g_Skeletons[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

						Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
						//Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
						Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

						// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
						XMStoreFloat3(&g_Skeletons[i].pos, nowPos + Pos);

						// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
						XMStoreFloat3(&g_Skeletons[i].rot, nowRot + Rot);

						// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
						XMStoreFloat3(&g_Skeletons[i].scl, nowScl + Scl);

						// frame���g�Ď��Ԍo�ߏ���������
						g_Skeletons[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
						if ((int)g_Skeletons[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
						{
							g_Skeletons[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
						}


					}

				}


			// ����̌v�Z
			{
				// �G�l�~�[�̌�������
				XMFLOAT3 View01 = XMFLOAT3(0.0f, 0.0f, 0.0f);		

				View01.x = -cosf(g_Skeletons[i].rot.y - XM_PIDIV2)/* * 50*/;
				View01.z = sinf(g_Skeletons[i].rot.y - XM_PIDIV2) /** 50*/;

				XMVECTOR	v0 = XMLoadFloat3(&View01);
				XMVECTOR	v1 = XMLoadFloat3(&g_Skeletons[i].pos);
				XMVECTOR	v2 = XMLoadFloat3(&knight->pos) - v1;

				v0 = XMVector3Normalize(v0);
				v2 = XMVector3Normalize(v2);

				float dis = VectorDistance(g_Skeletons[i].pos, knight->pos);

				float dot = dotProduct(&v0, &v2);
				dot = dot > 1 ? 1:dot;
				float pAngel = acosf(dot);
				float aAngel = pAngel * 360 / (XM_PI * 2);

				if ((aAngel <= VIEW_ANGLE / 2) && (dis < VIEW_DISTANCE))
				{
					g_Skeletons[i].vision = TRUE;			// ����͈͓��ɃL��������

				}
				else 
				{
					g_Skeletons[i].vision = FALSE;			// ����͈͓��ɃL�����Ȃ�

				}
			}
				break;

			case MODE_BATTLE:		// �o�g����ʂ̕`��

				// �o�g���̃^�[���ɂ���čs��
				switch (g_Turn)
				{
				case TURN_START:

					break;

				case TURN_PLAYER:

					break;

				case TURN_ACTION:

					break;

				case TURN_MOVING:
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
					// ���`��Ԃ̏���
				{	// �v���C���[�ɋ߂�
					BOOL moveStop = FALSE;

					if (moveStop == FALSE)
					{

						XMVECTOR nowPos = XMLoadFloat3(&g_Skeletons[0].pos);	// XMVECTOR�֕ϊ�

						XMVECTOR Pos = XMLoadFloat3(&knight->pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���

						Pos *= g_Skeletons[0].time;								// ���݂̈ړ��ʂ��v�Z���Ă���

						// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
						XMStoreFloat3(&g_Skeletons[0].pos, nowPos + Pos);

						float angle = atan2f(g_Skeletons[0].pos.x - knight->pos.x, g_Skeletons[0].pos.z - knight->pos.z);							//(y�̍����A�I�̍���)���̍������g���Ċp�x�����߂Ă���
						g_Skeletons[0].rot.y = angle;

						// frame���g�Ď��Ԍo�ߏ���������
						g_Skeletons[0].time += 1.0f / 100.0f;	// ���Ԃ�i�߂Ă���

						if (g_Skeletons[0].time >= 1.0f)
						{
							g_Skeletons[0].time = 0.0f;
						}

					}

					// �G�ƃv���C���[�L����
					for (int i = 0; i < MAX_SKELETONS; i++)
					{
						//�G�̗L���t���O���`�F�b�N����
						if (g_Skeletons[i].use == FALSE)
							continue;

						//BC�̓����蔻��
						if (CollisionBC(g_Skeletons[0].pos, knight->pos, g_Skeletons[0].size * 2, knight->size * 2))
						{
							// �G�l�~�[�̋߂��Ɏ~�܂�
							moveStop = TRUE;

						}
					}

					if (moveStop == TRUE)
					{
						moveStop = FALSE;
						SetPlayerKnightDamage(2);
						SetTurn(TURN_ACTION);

					}

				}

					break;

					// �Q�[���I�����̏���
				case TURN_MAX:


					break;

				}

				break;

			}

			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Skeletons[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Skeletons[i].shadowIdx, pos);
		}
		else
		{
			switch (g_Mode)
			{
			case MODE_BATTLE:		// �o�g����ʂ̕`��
				ReleaseShadow(g_Skeletons[i].shadowIdx);

				break;
			}
		}
	}




#ifdef _DEBUG

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	// ���f���̐F��ύX�ł����I�������ɂ��ł����B
	//	for (int j = 0; j < g_Skeletons[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Skeletons[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// ���f���̐F�����ɖ߂��Ă���
	//	for (int j = 0; j < g_Skeletons[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Skeletons[0].model, j, g_Skeletons[0].diffuse[j]);
	//	}
	//}
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemySkeletons(void)
{

	CAMERA* cam = GetCamera();
	int g_Mode = GetMode();


	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		if (g_Skeletons[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Skeletons[i].scl.x, g_Skeletons[i].scl.y, g_Skeletons[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Skeletons[i].rot.x, g_Skeletons[i].rot.y + XM_PI, g_Skeletons[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Skeletons[i].pos.x, g_Skeletons[i].pos.y, g_Skeletons[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Skeletons[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Skeletons[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY * GetEnemySkeletons()
{
	return &g_Skeletons[0];
}



//=============================================================================
// �X�P���g���Ƀ_���[�W
//=============================================================================
void SetEnemySkeletonsDamage(float damge)
{
	g_Skeletons[0].hp -= damge;
}


//=============================================================================
// �G�l�~�[����`��Ԃō��Wpos��time�t���C�����Ɉړ�
//=============================================================================
void EnemySkeletonsMoving(XMFLOAT3 pos, float time)
{	// ���`��Ԃ̏���

	XMVECTOR nowPos = XMLoadFloat3(&g_Skeletons[0].pos);	// XMVECTOR�֕ϊ�

	XMVECTOR Pos = XMLoadFloat3(&pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���

	Pos *= g_Skeletons[0].time;								// ���݂̈ړ��ʂ��v�Z���Ă���

	// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
	XMStoreFloat3(&g_Skeletons[0].pos, nowPos + Pos);

	float angle = atan2f(g_Skeletons[0].pos.x - pos.x, g_Skeletons[0].pos.z - pos.z);							//(y�̍����A�I�̍���)���̍������g���Ċp�x�����߂Ă���
	g_Skeletons[0].rot.y = angle;

	// frame���g�Ď��Ԍo�ߏ���������
	g_Skeletons[0].time += 1.0f / time;	// ���Ԃ�i�߂Ă���

	if (g_Skeletons[0].time >= 1.0f)
	{
		g_Skeletons[0].time = 0.0f;
	}

}
