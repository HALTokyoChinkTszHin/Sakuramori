//=============================================================================
//
// �o�g����ʏ��� [battle.h]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#pragma once





//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum {
	TURN_START,
	TURN_PLAYER,
	TURN_ACTION,
	TURN_MOVING,
	TURN_TARGET,
	TURN_ATTACK,
	TURN_DEFEND,
	TURN_RUN,
	TURN_ENEMY,
	TURN_WIN,
	TURN_GAMEOVER,




	TURN_MAX
};




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBattle(void);
void UninitBattle(void);
void UpdateBattle(void);
void DrawBattle(void);

void SetTurn(int mode);
int GetTurn(void);
