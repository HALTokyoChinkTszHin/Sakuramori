//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "playerknight.h"

#include "enemy.h"
#include "enemyskeletons.h"

#include "maze.h"

#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "bullet.h"
#include "score.h"
#include "collision.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// �|�[�YON/OFF


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 150, 13.0f, 13.0f);

	// ���C�g��L����	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	// ���H�̏�����
	InitMaze();

	// �e�̏�����
	InitBullet();

	// �X�R�A�̏�����
	InitScore();

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_field);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �X�R�A�̏I������
	UninitScore();

	// �e�̏I������
	UninitBullet();

	// ���H�̏I������
	UninitMaze();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitShadow();

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}


#endif

	if(g_bPause == FALSE)
		return;

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();

	// ���H�̍X�V����
	UpdateMaze();

	// �Ǐ����̍X�V
	UpdateMeshWall();

	// �e�̍X�V����
	UpdateBullet();

	// �e�̍X�V����
	UpdateShadow();

	// �����蔻�菈��
	CheckHit();

	// �X�R�A�̍X�V����
	UpdateScore();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame0(void)
{
	// 3D�̕���`�悷�鏈��
	// �n�ʂ̕`�揈��
	DrawMeshField();

	// �e�̕`�揈��
	DrawShadow();

	// �G�l�~�[�̕`�揈��
	DrawEnemy();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �e�̕`�揈��
	DrawBullet();

	// ���H�̕`�揈��
	DrawMaze();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// �X�R�A�̕`�揈��
	DrawScore();


	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);
}


void DrawGame(void)
{
	XMFLOAT3 pos;

	PLAYER* knight = GetPlayerKnight();

	if (knight->use == TRUE)
	{
		// �R�m���_
		XMFLOAT3 pos = knight->pos;
		pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
		SetCameraAT(pos);
		SetCamera();
	}

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif


	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	ENEMY *skeletons = GetEnemySkeletons();		// �G�l�~�[�̃|�C���^�[��������
	PLAYER *knight = GetPlayerKnight();	// �v���C���[�̃|�C���^�[��������
	MAZE* mazeWidth = GetMazeWallWidth();	// �v���C���[�̃|�C���^�[��������
	MAZE* mazeLength = GetMazeWallLength();	// �v���C���[�̃|�C���^�[��������

	BULLET *bullet = GetBullet();	// �e�̃|�C���^�[��������

	// �G�ƃv���C���[�L����
	for (int i = 0; i < MAX_SKELETONS; i++)
	{
		//�G�̗L���t���O���`�F�b�N����
		if (skeletons[i].use == FALSE)
			continue;

		XMFLOAT3 pos_player = knight->pos;
		pos_player.y = 0.0f;
		XMFLOAT3 pos_enemy = skeletons[i].pos;
		pos_enemy.y = 0.0f;

		//BC�̓����蔻��
		if (CollisionBC(pos_player, pos_enemy, knight->size, skeletons[i].size))
		{
			//// �o�g���V�[���ɓ���
			SetFade(FADE_OUT, MODE_BATTLE);
		}

	}


	// �v���C���[�̒e�ƓG
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (bullet[i].use == FALSE)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_SKELETONS; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (skeletons[j].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(bullet[i].pos, skeletons[j].pos, bullet[i].size, skeletons[j].size))
			{
				// �����������疢�g�p�ɖ߂�
				bullet[i].use = FALSE;
				ReleaseShadow(bullet[i].shadowIdx);

				// �o�g���V�[���ɓ���
				SetFade(FADE_OUT, MODE_BATTLE);
			}
		}

	}


}


