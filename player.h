//=============================================================================
//
// ���f������ [player.h]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_PLAYER			(1)							
#define	PLAYER_SIZE			(5.0f)							// �����蔻��̑傫��

#define MOVING_RANGE		(75)		

#define PLAYER_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(10.0f)							// �v���C���[�̑��������킹��


#define	PLAYER_VALUE_MOVE			(2.0f)							// �ړ���

#define	ROTATION_SPEED		(0.2f)							// ��]�ʑ��x


#define PLAYER_PARTS_MAX	(7)								// �v���C���[�̃p�[�c�̐�

enum
{
	// �v���C���[�̃p�[�c�̐�
	PARTHEAD,
	PARTARM_L,
	PARTARM_R,
	PARTLEG_L,
	PARTLEG_R,
	PARTSWORD,
	PARTSHIELD,

	PARTMAX

};

enum {
	//����
	MOTIONSTAY,
	MOTIONMOVE,
	MOTIONATTACK,
	MOTIONDEFENSE,
	//MOTIONJUMP,


	MOTIONMAX

};


enum
{
	//����
	CHAR_DIR_INSIDE,
	CHAR_DIR_RIGHT,
	CHAR_DIR_OUTSIDE,
	CHAR_DIR_LEFT,

	CHAR_DIR_MAX
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			originPos;			// �ړ��O�̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	float				movSpd;				// �ړ��X�s�[�h
	
	BOOL				load;
	DX11_MODEL			model;				// ���f�����

	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�
	int					dir;				// �����i0:�� 1:�E 2:�� 3:���j

	BOOL				use;

	// �v���C���[�̃p�����[�^
	float				hpMax;				// HP�̍ő�l
	float				hp;					// HP�̎c��
	float				str;				// �U����
	float				def;				// �h���
	float				spd;				// ���x
	float				size;				// �����蔻��͈̔�

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��
	int					motion;				// �v���C���[�̓���

	// �e�́ANULL�A�q���͐e�̃A�h���X������
	PLAYER				*parent;			// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X

	// �N�H�[�^�j�I��
	XMFLOAT4			Quaternion;

	XMFLOAT3			UpVector;			// �����������Ă��鏊

};


struct RANGE
{
	XMFLOAT4X4	mtxWorld;		// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	rot;			// �p�x
	XMFLOAT3	scl;			// �X�P�[��
	MATERIAL	material;		// �}�e���A��
	BOOL		use;			// �g�p���Ă��邩�ǂ���

};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

void changeDirection(float* roty, int targetDir, float rotationSpeed);
