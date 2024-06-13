//=============================================================================
//
// �G�l�~�[���f������ [enemy.h]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BATTLE_ENEMY		(5)					// �G�l�~�[�̐�

#define	ENEMY_SIZE		(5.0f)				// �����蔻��̑傫��

#define	ENEMY_VALUE_MOVE	(5.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define ENEMY_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define ENEMY_OFFSET_Y		(0.0f)							// �G�l�~�[�̑��������킹��

#define VIEW_ANGLE			(90)							// ����p
#define VIEW_DISTANCE		(100)							// ����̋���

enum
{
	// �퓬AI�s��
	ENEMY_BATTLE_MOVE,
	ENEMY_BATTLE_ATTACK,
	ENEMY_BATTLE_SKILL,

	ENEMY_BATTLE_MAX
};
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ENEMY
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	BOOL				use;
	BOOL				load;

	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	// �G�l�~�[�̃p�����[�^
	float				hpMax;				// HP�̍ő�l
	float				hp;					// HP�̎c��
	float				str;				// �U����
	float				def;				// �h���
	float				spd;				// ���x

	float				movSpd;				// �ړ��X�s�[�h
	float				size;				// �����蔻��̑傫��
	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	BOOL				vision;				// ����͈͓��ɃL�������݁H
	BOOL				lost;				// ����͈͓��ɃL�������������H

	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��

};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);