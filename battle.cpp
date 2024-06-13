//=============================================================================
//
// �o�g����ʏ��� [battle.cpp]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"

#include "battle.h"
#include "battleui.h"
#include "battlecursor.h"

#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "playerknight.h"


#include "enemy.h"
#include "enemyskeletons.h"

#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "bullet.h"

#include "collision.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// �|�[�YON/OFF

int g_Turn = TURN_START;		// �o�g���J�n��ݒ�


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBattle(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	g_Turn = TURN_START;

	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 150, 13.0f, 13.0f);

	// ���C�g��L����	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	// �ǂ̏�����
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// ��(�����p�̔�����)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// �e�̏�����
	InitBullet();

	// �J�[�\���̏�����
	InitBattleCursor();

	// �X�R�A�̏�����
	InitBattleUI();

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_battle);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBattle(void)
{
	// �X�R�A�̏I������
	UninitBattleUI();

	// �J�[�\���̏I������
	UninitBattleCursor();

	// �e�̏I������
	UninitBullet();

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
void UpdateBattle(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	g_bPause = g_bPause ? FALSE : TRUE;
	//}


#endif

	if(g_bPause == FALSE)
		return;

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();

	// �Ǐ����̍X�V
	UpdateMeshWall();

	// �e�̍X�V����
	UpdateBullet();

	// �e�̍X�V����
	UpdateShadow();

	// �J�[�\���̍X�V����
	UpdateBattleCursor();

	// �X�R�A�̍X�V����
	UpdateBattleUI();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBattle0(void)
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

	// �ǂ̕`�揈��
	DrawMeshWall();

	// �J�[�\���̕`�揈��
	DrawBattleCursor();

	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// UI�̕`�揈��
	DrawBattleUI();


	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);
}


void DrawBattle(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// �v���C���[���_
	pos = GetPlayerKnight()->pos;
	pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���

	SetCameraAT(pos);
	SetCamera();

	switch (g_Turn)
	{
	case TURN_START:

		break;

	case TURN_PLAYER:
		// �v���C���[���_
		pos = GetPlayerKnight()->pos;
		pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���

		SetCameraAT(pos);
		SetCamera();

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
		// �G�l�~�[���_
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���

		SetCameraAT(pos);
		SetCamera();

		break;

		// �Q�[���I�����̏���
	case TURN_MAX:

		break;

	}

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawBattle0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawBattle0();

		// �G�l�~�[���_
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawBattle0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawBattle0();

		// �G�l�~�[���_
		pos = GetEnemySkeletons()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawBattle0();
		break;

	}

}




//=============================================================================
// �^�[���̐ݒ�
//=============================================================================
void SetTurn(int turn)
{

	g_Turn = turn;	// ���̃��[�h���Z�b�g���Ă���


}

//=============================================================================
// �^�[���̎擾
//=============================================================================
int GetTurn(void)
{
	return g_Turn;
}
