//=============================================================================
//
// �e���ˏ��� [bullet.h]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_BULLET		(10)	// �e�ő吔

#define	BULLET_SIZE		(10.0f)				// �����蔻��̑傫��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4	mtxWorld;		// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	rot;			// �p�x
	XMFLOAT3	scl;			// �X�P�[��

	MATERIAL	material;		// �}�e���A��
	float		s;				//���`��ԗp�p�����[�^��

	float		spd;			// �ړ���
	float		size;			// �����蔻��̑傫��



	int			shadowIdx;		// �eID
	BOOL		use;			// �g�p���Ă��邩�ǂ���
	bool		load;
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F


} BULLET;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);

BULLET *GetBullet(void);
