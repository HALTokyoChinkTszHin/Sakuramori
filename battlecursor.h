//=============================================================================
//
// �o�g���J�[�\������ [battlecursor.h]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#pragma once



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBattleCursor(void);
void UninitBattleCursor(void);
void UpdateBattleCursor(void);
void DrawBattleCursor(void);

int SetBattleCursor(XMFLOAT3 pos, float fWidth, float fHeight, XMFLOAT4 col);

void SetMovingPos(XMFLOAT3 pos);
XMFLOAT3 GetMovingPos(void);

