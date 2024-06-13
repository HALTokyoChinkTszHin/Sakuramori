//=============================================================================
//
// ���f������ [playerknight.cpp]
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
#include "fade.h"
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
#define	MODEL_KNIGHT				"data/MODEL/knight/knight_body.obj"				// �ǂݍ��ރ��f����
#define	MODEL_KNIGHT_HEAD			"data/MODEL/knight/knight_head.obj"				// �ǂݍ��ރ��f����
#define	MODEL_KNIGHT_ARM_LEFT		"data/MODEL/knight/knight_arm_l.obj"			// �ǂݍ��ރ��f����
#define	MODEL_KNIGHT_ARM_RIGHT		"data/MODEL/knight/knight_arm_r.obj"			// �ǂݍ��ރ��f����
#define	MODEL_KNIGHT_LEG_LEFT		"data/MODEL/knight/knight_leg_l.obj"			// �ǂݍ��ރ��f����
#define	MODEL_KNIGHT_LEG_RIGHT		"data/MODEL/knight/knight_leg_r.obj"			// �ǂݍ��ރ��f����
#define	MODEL_KNIGHT_SWORD			"data/MODEL/knight/knight_sword.obj"			// �ǂݍ��ރ��f����
#define	MODEL_KNIGHT_SHIELD			"data/MODEL/knight/knight_shield.obj"			// �ǂݍ��ރ��f����







//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Knight;						// �v���C���[

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p

static float		roty = 0.0f;

static LIGHT		g_Light;

// �v���C���[�̊K�w�A�j���[�V�����f�[�^
// �v���C���[�̑ҋ@���̃A�j���f�[�^
static INTERPOLATION_DATA stay_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA stay_tbl_arm_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA stay_tbl_arm_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA stay_tbl_leg_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA stay_tbl_leg_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA stay_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA stay_tbl_shield[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};









 
// �v���C���[�̈ړ����̃A�j���f�[�^
static INTERPOLATION_DATA move_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA move_tbl_arm_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA move_tbl_arm_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA move_tbl_leg_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 6, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 6, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA move_tbl_leg_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 6, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 6, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA move_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA move_tbl_shield[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};








// �v���C���[�̍U�����̃A�j���f�[�^
static INTERPOLATION_DATA attack_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA attack_tbl_arm_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },


};

static INTERPOLATION_DATA attack_tbl_arm_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 4, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 100 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 2, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 100 },

};


static INTERPOLATION_DATA attack_tbl_leg_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA attack_tbl_leg_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA attack_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA attack_tbl_shield[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};






// �v���C���[�̖h�䒆�̃A�j���f�[�^
static INTERPOLATION_DATA defense_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA defense_tbl_arm_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 4, -XM_PI / 2, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 4, -XM_PI / 2, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },


};

static INTERPOLATION_DATA defense_tbl_arm_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 50 },

};


static INTERPOLATION_DATA defense_tbl_leg_l[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA defense_tbl_leg_r[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA defense_tbl_sword[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};

static INTERPOLATION_DATA defense_tbl_shield[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },

};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	stay_tbl_head,
	stay_tbl_arm_l,
	stay_tbl_arm_r,
	stay_tbl_leg_l,
	stay_tbl_leg_r,
	stay_tbl_sword,
	stay_tbl_shield,

	

	move_tbl_head,
	move_tbl_arm_l,
	move_tbl_arm_r,
	move_tbl_leg_l,
	move_tbl_leg_r,
	move_tbl_sword,
	move_tbl_shield,



	attack_tbl_head,
	attack_tbl_arm_l,
	attack_tbl_arm_r,
	attack_tbl_leg_l,
	attack_tbl_leg_r,
	attack_tbl_sword,
	attack_tbl_shield,



	defense_tbl_head,
	defense_tbl_arm_l,
	defense_tbl_arm_r,
	defense_tbl_leg_l,
	defense_tbl_leg_r,
	defense_tbl_sword,
	defense_tbl_shield,


};






//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayerKnight(void)
{
	g_Knight.load = TRUE;
	LoadModel(MODEL_KNIGHT, &g_Knight.model);

	int g_Mode = GetMode();
	if (g_Mode == MODE_GAME)
	{
		g_Knight.pos = XMFLOAT3(-600.0f, PLAYER_OFFSET_Y + 50.0f, -600.0f);
	}
	else if (g_Mode == MODE_BATTLE)
	{
		g_Knight.pos = XMFLOAT3(10.0f, PLAYER_OFFSET_Y + 50.0f, 0.0f);
	}


	g_Knight.originPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_Knight.rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Knight.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_Knight.movSpd = 0.0f;			// �ړ��X�s�[�h�N���A
	g_Knight.dir = CHAR_DIR_OUTSIDE;


	g_Knight.use = TRUE;			// TRUE:�����Ă�

	// �p�����[�^�̐ݒ�
	g_Knight.hpMax = 100;
	g_Knight.hp = g_Knight.hpMax;

	g_Knight.str = 20;
	g_Knight.def = 20;
	g_Knight.spd = 20;


	g_Knight.size = PLAYER_SIZE;	// �����蔻��̑傫��

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Knight.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Knight.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	g_Knight.time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
	g_Knight.motion = MOTIONSTAY;

	// �L�[�����������̃v���C���[�̌���
	roty = 0.0f;

	g_Knight.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������


	// �K�w�A�j���[�V�����̏�����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = FALSE;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		// �e�q�֌W
		g_Parts[i].parent = &g_Knight;		// �� �����ɐe�̃A�h���X������
	//	g_Parts[�r].parent= &g_Knight;		// �r��������e�͖{�́i�v���C���[�j
	//	g_Parts[��].parent= &g_Paerts[�r];	// �w���r�̎q���������ꍇ�̗�

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		g_Parts[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Parts[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Parts[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
		g_Parts[i].load = FALSE;
	}

	g_Parts[PARTHEAD].use = TRUE;
	g_Parts[PARTHEAD].parent = &g_Knight;	// �e���Z�b�g
	g_Parts[PARTHEAD].tblNo = PARTHEAD;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[PARTHEAD].tblMax = sizeof(move_tbl_head) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[PARTHEAD].load = TRUE;
	LoadModel(MODEL_KNIGHT_HEAD, &g_Parts[PARTHEAD].model);

	g_Parts[PARTARM_L].use = TRUE;
	g_Parts[PARTARM_L].parent = &g_Knight;	// �e���Z�b�g
	g_Parts[PARTARM_L].tblNo = PARTARM_L;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[PARTARM_L].tblMax = sizeof(move_tbl_arm_l) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[PARTARM_L].load = TRUE;
	LoadModel(MODEL_KNIGHT_ARM_LEFT, &g_Parts[PARTARM_L].model);

	g_Parts[PARTARM_R].use = TRUE;
	g_Parts[PARTARM_R].parent = &g_Knight;	// �e���Z�b�g
	g_Parts[PARTARM_R].tblNo = PARTARM_R;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[PARTARM_R].tblMax = sizeof(move_tbl_arm_r) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[PARTARM_R].load = TRUE;
	LoadModel(MODEL_KNIGHT_ARM_RIGHT, &g_Parts[PARTARM_R].model);

	g_Parts[PARTLEG_L].use = TRUE;
	g_Parts[PARTLEG_L].parent = &g_Knight;	// �e���Z�b�g
	g_Parts[PARTLEG_L].tblNo = PARTLEG_L;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[PARTLEG_L].tblMax = sizeof(move_tbl_leg_l) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[PARTLEG_L].load = TRUE;
	LoadModel(MODEL_KNIGHT_LEG_LEFT, &g_Parts[PARTLEG_L].model);

	g_Parts[PARTLEG_R].use = TRUE;
	g_Parts[PARTLEG_R].parent = &g_Knight;	// �e���Z�b�g
	g_Parts[PARTLEG_R].tblNo = PARTLEG_R;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[PARTLEG_R].tblMax = sizeof(move_tbl_leg_r) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[PARTLEG_R].load = TRUE;
	LoadModel(MODEL_KNIGHT_LEG_RIGHT, &g_Parts[PARTLEG_R].model);

	g_Parts[PARTSWORD].use = TRUE;
	g_Parts[PARTSWORD].parent = &g_Parts[PARTARM_R];	// �e���Z�b�g
	g_Parts[PARTSWORD].tblNo = PARTSWORD;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[PARTSWORD].tblMax = sizeof(move_tbl_sword) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[PARTSWORD].load = TRUE;
	LoadModel(MODEL_KNIGHT_SWORD, &g_Parts[PARTSWORD].model);

	g_Parts[PARTSHIELD].use = TRUE;
	g_Parts[PARTSHIELD].parent = &g_Parts[PARTARM_L];	// �e���Z�b�g
	g_Parts[PARTSHIELD].tblNo = PARTSHIELD;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[PARTSHIELD].tblMax = sizeof(move_tbl_shield) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Parts[PARTSHIELD].load = TRUE;
	LoadModel(MODEL_KNIGHT_SHIELD, &g_Parts[PARTSHIELD].model);


	// �N�H�[�^�j�I���̏�����
	XMStoreFloat4(&g_Knight.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayerKnight(void)
{
	// ���f���̉������
	if (g_Knight.load == TRUE)
	{
		UnloadModel(&g_Knight.model);
		g_Knight.load = FALSE;
	}

	// �p�[�c�̉������
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load == TRUE)
		{
			// �p�[�c�̉������
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = FALSE;
		}
	}



}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayerKnight(void)
{
	CAMERA *cam = GetCamera();
	ENEMY* skeletons = GetEnemySkeletons();		// �G�l�~�[�̃|�C���^�[��������

	//g_Knight.movSpd *= 0.7f;
	int g_Mode = GetMode();
	int g_Turn = GetTurn();

	switch (g_Mode)
	{
	case MODE_TITLE:
		break;

	case MODE_GAME:

		// �Q�[���N���A
		if ((g_Knight.pos.x > 80) && (g_Knight.pos.z > 360))
		{
			SetFade(FADE_OUT, MODE_TITLE);

		}

		// �ړ��O�̍��W��ۑ�
		g_Knight.originPos = g_Knight.pos;

		// ������ύX
		changeDirection(&roty, g_Knight.dir, ROTATION_SPEED);

		// Key���͂���������ړ���������
		if (g_Knight.movSpd > 0.0f)
		{	// �����������Ƀv���C���[���ړ�������
			// �����������Ƀv���C���[���������Ă��鏊
			g_Knight.rot.y = roty + cam->rot.y;

			g_Knight.pos.x -= sinf(g_Knight.rot.y) * g_Knight.movSpd;
			g_Knight.pos.z -= cosf(g_Knight.rot.y) * g_Knight.movSpd;
		}

		g_Knight.movSpd *= 0.5f;

		if ((g_Knight.movSpd < PLAYER_VALUE_MOVE / 4) && (g_Knight.motion != MOTIONATTACK))
		{
			g_Knight.motion = MOTIONSTAY;
		}

		break;


	case MODE_BATTLE:
		switch (g_Turn)
		{
		case TURN_START:
			g_Knight.motion = MOTIONSTAY;

			break;

		case TURN_PLAYER:

			break;


		case TURN_ACTION:
			g_Knight.motion = MOTIONSTAY;

			break;

		case TURN_MOVING:
			// �ړ�����
			{
				XMFLOAT3 moving_pos = GetMovingPos();

				float moving_distance = VectorDistance(g_Knight.pos, moving_pos);

				if (moving_distance < MOVING_RANGE)
				{

					g_Knight.motion = MOTIONMOVE;

					if (moving_pos.x != 0.0)	// ���`��Ԃ����s����H
					{	// ���`��Ԃ̏���

						XMVECTOR nowPos = XMLoadFloat3(&g_Knight.pos);	// XMVECTOR�֕ϊ�

						XMVECTOR Pos = XMLoadFloat3(&moving_pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���

						Pos *= g_Knight.time;								// ���݂̈ړ��ʂ��v�Z���Ă���

						// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
						XMStoreFloat3(&g_Knight.pos, nowPos + Pos);

						float angle = atan2f(g_Knight.pos.x - moving_pos.x, g_Knight.pos.z - moving_pos.z);							//(y�̍����A�I�̍���)���̍������g���Ċp�x�����߂Ă���
						g_Knight.rot.y = angle;

						// frame���g�Ď��Ԍo�ߏ���������
						g_Knight.time += 1.0f / 100.0f;	// ���Ԃ�i�߂Ă���

						if (g_Knight.time >= 1.0f)
						{

							if ((g_Knight.pos.x == moving_pos.x) &&
								(g_Knight.pos.z == moving_pos.z))// ���`��Ԃ��~�߂�H
							{
								SetMovingPos(XMFLOAT3(0.0f, 0.0f, 0.0f));

								SetTurn(TURN_ENEMY);

							}

							g_Knight.time = 0.0f;
						}

					}

				}



			}


				break;

		case TURN_TARGET:
			break;

		case TURN_ATTACK:
			// ���`��Ԃ̏���
		{	// �G�l�~�[�ɋ߂�
			BOOL moveStop = FALSE;

			if (moveStop == FALSE)
			{

				XMVECTOR nowPos = XMLoadFloat3(&g_Knight.pos);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&skeletons[0].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���

				Pos *= g_Knight.time;								// ���݂̈ړ��ʂ��v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Knight.pos, nowPos + Pos);

				float angle = atan2f(g_Knight.pos.x - skeletons[0].pos.x, g_Knight.pos.z - skeletons[0].pos.z);							//(y�̍����A�I�̍���)���̍������g���Ċp�x�����߂Ă���
				g_Knight.rot.y = angle;

				// frame���g�Ď��Ԍo�ߏ���������
				g_Knight.time += 1.0f / 100.0f;	// ���Ԃ�i�߂Ă���

				if (g_Knight.time >= 1.0f)
				{
					g_Knight.time = 0.0f;
				}

			}

			// �G�ƃv���C���[�L����
			for (int i = 0; i < MAX_SKELETONS; i++)
			{
				//�G�̗L���t���O���`�F�b�N����
				if (skeletons[i].use == FALSE)
					continue;

				//BC�̓����蔻��
				if (CollisionBC(g_Knight.pos, skeletons[i].pos, g_Knight.size * 2, skeletons[i].size * 2))
				{
					// �G�l�~�[�̋߂��Ɏ~�܂�
					moveStop = TRUE;

				}
			}

			if (moveStop == TRUE)
			{
				moveStop = FALSE;

				g_Knight.motion = MOTIONATTACK;
				SetEnemySkeletonsDamage(5);
				SetTurn(TURN_ENEMY);

			}

		}
			break;

		case TURN_DEFEND:
			g_Knight.motion = MOTIONDEFENSE;

			break;

		case TURN_RUN:
			break;

		case TURN_ENEMY:
			break;

			// �Q�[���I�����̏���
		case TURN_MAX:

			break;
			
		}


		break;

		// �Q�[���I�����̏���
	case MODE_MAX:
		break;
	}




	// ���C�L���X�g���đ����̍��������߂�
	XMFLOAT3 HitPosition;		// ��_
	XMFLOAT3 Normal;			// �Ԃ������|���S���̖@���x�N�g���i�����j
	BOOL ans = RayHitField(g_Knight.pos, &HitPosition, &Normal);
	if (ans)
	{
		g_Knight.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
	}
	else
	{
		g_Knight.pos.y = PLAYER_OFFSET_Y;
		Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Knight.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Knight.shadowIdx, pos);


	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// �g���Ă���Ȃ珈������
		if ((g_Parts[i].use == TRUE) && (g_Parts[i].tblMax > 0))
		{	
			//�A�j���[�V�����̕ϊ�
			for (int i = 0; i < PARTMAX; i++)
			{
				g_Parts[i].tblNo = i + PARTMAX * g_Knight.motion;
			}

			// ���`��Ԃ̏���
			int nowNo = (int)g_Parts[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
			int maxNo = g_Parts[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
			int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
			INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

			float nowTime = g_Parts[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

			Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
			Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

			// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

			// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

			// frame���g�Ď��Ԍo�ߏ���������
			g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
			if ((int)g_Parts[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
			{
				g_Parts[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
			}

		}

	}


	//// �|�C���g���C�g�̃e�X�g
	//{
	//	LIGHT *light = GetLightData(1);
	//	XMFLOAT3 pos = g_Knight.pos;
	//	pos.y += 20.0f;

	//	light->Position = pos;
	//	light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Type = LIGHT_TYPE_POINT;
	//	light->Enable = TRUE;
	//	SetLightData(1, light);
	//}



	//////////////////////////////////////////////////////////////////////
	// �p������
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_Knight.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Knight.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	//nvx = vx / len;
	angle = asinf(len);

	//quat = XMQuaternionIdentity();

//	quat = XMQuaternionRotationAxis(nvx, angle);
	quat = XMQuaternionRotationNormal(nvx, angle);


	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Knight.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_Knight.Quaternion, quat);




#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_Knight.pos.x, g_Knight.pos.y, g_Knight.pos.z, Normal.y);
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayerKnight(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Knight.scl.x, g_Knight.scl.y, g_Knight.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Knight.rot.x, g_Knight.rot.y + XM_PI, g_Knight.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �N�H�[�^�j�I���𔽉f
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Knight.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Knight.pos.x, g_Knight.pos.y, g_Knight.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Knight.mtxWorld, mtxWorld);


	//// �����̐ݒ�
	//SetFuchi(1);

	// ���f���`��
	DrawModel(&g_Knight.model);



	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ��
			// g_Knight.mtxWorld���w���Ă���
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������
		if (g_Parts[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);


		// ���f���`��
		DrawModel(&g_Parts[i].model);

	}

	//SetFuchi(0);

	// ���e�X�g��L����
	SetAlphaTestEnable(TRUE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);


}

//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayerKnight(void)
{
	return &g_Knight;
}






//=============================================================================
// �v���C���[�Ƀ_���[�W
//=============================================================================
void SetPlayerKnightDamage(float damge)
{
	g_Knight.hp -= damge;
}



