//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 銭子軒（チェンズーシェン）
// 
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_creator,	// BGM0
	SOUND_LABEL_BGM_title,	// BGM1
	SOUND_LABEL_BGM_field,	// BGM2
	SOUND_LABEL_BGM_battle,	// BGM2
	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

