//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_LEAF		(30)		// �p�[�e�B�N���ő吔
#define	MAX_RIPPLES		(5)		// �p�[�e�B�N���ő吔



struct TITLE_LOGO
{
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	rot;			// �|���S���̉�]��
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	float		countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			texNo;			// �e�N�X�`���ԍ�
	XMFLOAT3	move;			// �ړ����x

};

struct TITLE_OPTION
{
	XMFLOAT3	pos;			// �|���S���̍��W
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�

};

struct  TITLE_PARTICLE
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scale;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	float			fSizeX;			// ��
	float			fSizeY;			// ����
	int				nLife;			// ����
	BOOL			use;			// �g�p���Ă��邩�ǂ���

	int				texNo;

};

enum
{
	TITLE_START,
	TITLE_TUTORIAL,
	//TITLE_OPTIONS,
	TITLE_EXIT,

	TITLE_MAX,
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
