//=============================================================================
//
// �o�g��UI���� [battleUI.h]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HP_MAX			(999)		// �X�R�A�̍ő�l
#define HP_DIGIT		(3)			// ����

// UI
enum {
	UI_START,
	UI_WIN,
	UI_GAMEOVER,

	UI_TURN_NONE,
	UI_TURN_FIRST,
	UI_TURN_SECOND,
	UI_TURN_THIRD,
	UI_TURN_FOURTH,
	UI_TURN_FIFTH,

	UI_HP_ICON_KNIGHT,
	UI_HP_NONE_KNIGHT,
	UI_HP_RED_KNIGHT,
	UI_HP_GREEN_KNIGHT,

	UI_OPTION,

	UI_SELECT,

	UI_MAX
};

// �摜
enum {
	IMAGE_START,
	IMAGE_WIN,
	IMAGE_GAMEOVER,

	IMAGE_TURN_NONE,
	IMAGE_TURN_KNIGHT,
	IMAGE_TURN_SKELETONS,

	IMAGE_ICON_KNIGHT,
	IMAGE_HP_NONE,
	IMAGE_HP_RED,
	IMAGE_HP_GREEN,

	IMAGE_COMMAND,
	IMAGE_CONFIRM,


	UI_IMAGE_MAX
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct UI
{
	BOOL					use;						// TRUE:�g���Ă���  FALSE:���g�p
	float					w, h;					// ���ƍ���
	XMFLOAT3				pos;						// �|���S���̍��W
	int						texNo;					// �e�N�X�`���ԍ�

};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBattleUI(void);
void UninitBattleUI(void);
void UpdateBattleUI(void);
void DrawBattleUI(void);


