//=============================================================================
//
// バトルカーソル処理 [battlecursor.h]
// Author : 銭子軒（チェンズーシェン）
//
//=============================================================================
#pragma once



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattleCursor(void);
void UninitBattleCursor(void);
void UpdateBattleCursor(void);
void DrawBattleCursor(void);

int SetBattleCursor(XMFLOAT3 pos, float fWidth, float fHeight, XMFLOAT4 col);

void SetMovingPos(XMFLOAT3 pos);
XMFLOAT3 GetMovingPos(void);

