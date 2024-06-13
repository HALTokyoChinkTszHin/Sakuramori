//=============================================================================
//
// ���H���� [maze.cpp]
// Author : �K�q���i�`�F���Y�[�V�F���j
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "collision.h"
#include "camera.h"

#include "enemy.h"
#include "enemyskeletons.h"

#include "player.h"
#include "playerknight.h"

#include "maze.h"

#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_MAZE_WALL			"data/MODEL/Maze/mazeWall.obj"		// �ǂݍ��ރ��f����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MAZE			maze_wallWidth[MAX_MAZE_WALL_Y][MAX_MAZE_WALL_X - 1];				// �������̕�
static MAZE			maze_wallLength[MAX_MAZE_WALL_Y - 1][MAX_MAZE_WALL_X];				// �c�����̕�

int mazeWallWidthUse[MAX_MAZE_WALL_Y][MAX_MAZE_WALL_X - 1]{
	//0 1 2 3 4 5 6 7 8 9|0 1 2 
	  1,1,1,1,1,1,1,1,1,1,1,1,1,	//0
	  0,0,0,1,1,1,1,0,1,0,0,1,0,	//1
	  0,0,0,0,0,0,1,1,0,1,0,0,1,	//2
	  1,1,1,0,1,1,1,1,1,1,1,1,0,	//3
	  0,0,1,1,1,0,0,0,1,0,0,0,0,	//4
	  0,1,0,0,1,1,0,1,1,1,0,0,0,	//5
	  0,0,1,1,1,1,1,0,1,0,0,0,0,	//6
	  0,0,1,0,0,0,1,0,0,1,0,0,1,	//7
	  0,1,0,0,0,1,0,0,0,0,1,1,0,	//8
	  0,0,0,1,1,0,1,1,1,1,0,1,1,	//9
	  0,1,1,0,1,0,0,1,1,1,1,0,0,	//10
	  1,1,1,0,0,1,1,0,0,0,0,1,0,	//11
	  1,0,0,1,1,1,1,1,0,1,1,1,0,	//12
	  0,0,0,1,1,0,0,0,0,0,1,0,1,	//13
	  0,1,0,0,0,0,0,1,0,1,1,1,1,	//14
	  1,0,1,1,0,0,1,0,0,0,0,0,0,	//15
	  0,0,0,1,1,1,0,1,1,0,0,0,0,	//16
	  0,1,0,0,0,0,1,1,0,0,0,0,0,	//17
	  1,1,1,1,1,1,1,1,1,1,1,1,1,	//18
};

int mazeWallLengthUse[MAX_MAZE_WALL_Y - 1][MAX_MAZE_WALL_X]{
	//0 1 2 3 4 5 6 7 8 9|0 1 2 3
	  1,0,0,0,0,0,0,0,1,0,1,0,0,1,	//0
	  1,0,0,1,0,1,0,0,1,0,1,1,0,1,	//1
	  1,0,0,1,1,0,0,0,0,1,0,1,0,1,	//2
	  1,0,0,1,0,0,1,0,1,0,1,0,1,1,	//3
	  1,1,1,0,1,0,0,1,0,0,1,1,1,1,	//4
	  1,1,0,1,0,0,0,1,0,0,1,1,1,1,	//5
	  1,1,1,0,0,1,0,1,1,0,1,1,0,1,	//6
	  1,1,0,1,1,1,0,1,1,1,0,1,0,1,	//7
	  1,0,1,1,1,0,1,0,1,0,1,0,0,1,	//8
	  1,1,0,1,0,0,1,0,0,0,0,0,0,1,	//9
	  1,0,0,0,1,0,1,0,1,1,0,1,1,1,	//10
	  1,0,1,0,1,0,0,0,1,0,1,0,0,1,	//11
	  1,0,1,0,0,0,1,0,1,1,0,1,0,1,	//12
	  1,1,1,1,0,1,1,1,1,1,0,0,0,1,	//13
	  1,0,1,0,1,1,1,0,1,1,0,0,0,1,	//14
	  1,1,1,0,0,1,0,1,0,0,0,0,0,1,	//15
	  1,1,0,1,1,0,1,0,0,1,0,0,0,1,	//16
	  1,0,0,1,0,1,0,0,0,1,0,0,0,1,	//17
};

int maze_wallWidth_load = 0;





//=============================================================================
// ����������
//=============================================================================
HRESULT InitMaze(void)
{
	// �������̕�
	for (int y = 0; y < MAX_MAZE_WALL_Y; y++)
	{
		for (int x = 0; x < MAX_MAZE_WALL_X - 1; x++)
		{
			
			LoadModel(MODEL_MAZE_WALL, &maze_wallWidth[y][x].model);
			maze_wallWidth[y][x].load = TRUE;

			maze_wallWidth[y][x].pos = XMFLOAT3(MAP_LEFT + x * 80.0f, 0.0f, MAP_DOWN + y * 80.0f);
			maze_wallWidth[y][x].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			maze_wallWidth[y][x].scl = XMFLOAT3(2.0f, 2.0f, 2.0f);

			maze_wallWidth[y][x].movSpd = 0.0f;			// �ړ��X�s�[�h�N���A
			maze_wallWidth[y][x].size = WALL_SIZE_X;	// �����蔻��̑傫��

			// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
			GetModelDiffuse(&maze_wallWidth[y][x].model, &maze_wallWidth[y][x].diffuse[0]);

			maze_wallWidth[y][x].use = mazeWallWidthUse[y][x];			// TRUE:�g�p����
			maze_wallWidth[y][x].vision = TRUE;						// TRUE:�`�悷��
		}

	}


	// �c�����̕�
	for (int y = 0; y < MAX_MAZE_WALL_Y - 1; y++)
	{
		for (int x = 0; x < MAX_MAZE_WALL_X; x++)
		{
			LoadModel(MODEL_MAZE_WALL, &maze_wallLength[y][x].model);
			maze_wallLength[y][x].load = TRUE;

			maze_wallLength[y][x].pos = XMFLOAT3(MAP_LEFT - 80.0f+ x * 80.0f, 0.0f, MAP_DOWN + y * 80.0f);
			maze_wallLength[y][x].rot = XMFLOAT3(0.0f, XM_PI * 0.5, 0.0f);
			maze_wallLength[y][x].scl = XMFLOAT3(2.0f, 2.0f, 2.0f);

			maze_wallLength[y][x].movSpd = 0.0f;			// �ړ��X�s�[�h�N���A
			maze_wallLength[y][x].size = WALL_SIZE_X;	// �����蔻��̑傫��

			// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
			GetModelDiffuse(&maze_wallLength[y][x].model, &maze_wallLength[y][x].diffuse[0]);

			maze_wallLength[y][x].use = mazeWallLengthUse[y][x];			// TRUE:�g�p����
			maze_wallLength[y][x].vision = TRUE;							// TRUE:�`�悷��
		}

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMaze(void)
{
	for (int y = 0; y < MAX_MAZE_WALL_Y; y++)
	{
		for (int x = 0; x < MAX_MAZE_WALL_X - 1; x++)
		{
			if (maze_wallWidth[y][x].load)
			{
				UnloadModel(&maze_wallWidth[y][x].model);
				maze_wallWidth[y][x].load = FALSE;
			}

		}

	}


	for (int y = 0; y < MAX_MAZE_WALL_Y - 1; y++)
	{
		for (int x = 0; x < MAX_MAZE_WALL_X; x++)
		{
			if (maze_wallLength[y][x].load)
			{
				UnloadModel(&maze_wallLength[y][x].model);
				maze_wallLength[y][x].load = FALSE;
			}

		}

	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMaze(void)
{
	PLAYER* knight = GetPlayerKnight();		// �v���C���[�i�R�m�j�̃|�C���^�[��������
	int g_Mode = GetMode();

	if (knight->use == TRUE)
	{
		// �v���C���[�ƕ�(������)
		for (int y = 0; y < MAX_MAZE_WALL_Y; y++)
		{
			for (int x = 0; x < MAX_MAZE_WALL_X - 1; x++)
			{
				//�ǂ̗L���t���O���`�F�b�N����
				if (maze_wallWidth[y][x].use == FALSE)
					continue;

				XMFLOAT3 pos_player = knight->pos;
				pos_player.y = 0.0f;

				XMFLOAT3 pos_wallWidth = maze_wallWidth[y][x].pos;

				//BC�̓����蔻��(�ǂɂԂ�����)
				if (CollisionBC(pos_player, pos_wallWidth, PLAYER_SIZE, maze_wallWidth[y][x].size))
				{
					knight->pos = knight->originPos;
				}

				//BC�̓����蔻��(������ǃ`�F�b�N)
				if (CollisionBC(pos_player, pos_wallWidth, PLAYER_SIZE * 50, maze_wallWidth[y][x].size))
				{
					maze_wallWidth[y][x].vision = TRUE;
				}
				else
				{
					maze_wallWidth[y][x].vision = FALSE;
				}

			}
		}

		// �v���C���[�ƕ�(������)
		for (int y = 0; y < MAX_MAZE_WALL_Y - 1; y++)
		{
			for (int x = 0; x < MAX_MAZE_WALL_X; x++)
			{
				//�ǂ̗L���t���O���`�F�b�N����
				if (maze_wallLength[y][x].use == FALSE)
					continue;

				XMFLOAT3 pos_player = knight->pos;
				pos_player.y = 0.0f;

				XMFLOAT3 pos_wallLength = maze_wallLength[y][x].pos;

				//BC�̓����蔻��(�ǂɂԂ�����)
				if (CollisionBC(pos_player, pos_wallLength, PLAYER_SIZE, maze_wallLength[y][x].size))
				{
					knight->pos = knight->originPos;
				}

				//BC�̓����蔻��(������ǃ`�F�b�N)
				if (CollisionBC(pos_player, pos_wallLength, PLAYER_SIZE * 50, maze_wallLength[y][x].size))
				{
					maze_wallLength[y][x].vision = TRUE;
				}
				else
				{
					maze_wallLength[y][x].vision = FALSE;
				}

			}
		}


	}



#ifdef _DEBUG

#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMaze(void)
{

	CAMERA* cam = GetCamera();
	int g_Mode = GetMode();


	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int y = 0; y < MAX_MAZE_WALL_Y; y++)
	{
		for (int x = 0; x < MAX_MAZE_WALL_X - 1; x++)
		{
			if (maze_wallWidth[y][x].use == FALSE) continue;

			if (maze_wallWidth[y][x].vision == FALSE) continue;

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(maze_wallWidth[y][x].scl.x, maze_wallWidth[y][x].scl.y, maze_wallWidth[y][x].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(maze_wallWidth[y][x].rot.x, maze_wallWidth[y][x].rot.y + XM_PI, maze_wallWidth[y][x].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(maze_wallWidth[y][x].pos.x, maze_wallWidth[y][x].pos.y, maze_wallWidth[y][x].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&maze_wallWidth[y][x].mtxWorld, mtxWorld);


			// ���f���`��
			DrawModel(&maze_wallWidth[y][x].model);

		}
	}

	for (int y = 0; y < MAX_MAZE_WALL_Y - 1; y++)
	{
		for (int x = 0; x < MAX_MAZE_WALL_X; x++)
		{
			if (maze_wallLength[y][x].use == FALSE) continue;

			if (maze_wallLength[y][x].vision == FALSE) continue;

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(maze_wallLength[y][x].scl.x, maze_wallLength[y][x].scl.y, maze_wallLength[y][x].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(maze_wallLength[y][x].rot.x, maze_wallLength[y][x].rot.y + XM_PI, maze_wallLength[y][x].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(maze_wallLength[y][x].pos.x, maze_wallLength[y][x].pos.y, maze_wallLength[y][x].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&maze_wallLength[y][x].mtxWorld, mtxWorld);


			// ���f���`��
			DrawModel(&maze_wallLength[y][x].model);

		}
	}


	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �������̕ǂ̎擾
//=============================================================================
MAZE * GetMazeWallWidth()
{
	return &maze_wallWidth[0][0];
}

//=============================================================================
// �c�����̕ǂ̎擾
//=============================================================================
MAZE* GetMazeWallLength()
{
	return &maze_wallWidth[0][0];
}

