#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define ID_MARIO_ANI_WORLD_SMALL	12090
#define ID_MARIO_ANI_WORLD_BIG	0
#define ID_MARIO_ANI_WORLD_RACCOON	0

#define WORLD_MARIO_BBOX_WIDTH 16
#define WORLD_MARIO_BBOX_HEIGHT 16

#define ID_ANI_WORLD_ENEMY_MOVE_LEFT 12030	
#define ID_ANI_WORLD_ENEMY_MOVE_RIGHT 12040	

#define WORLD_ENEMY_BBOX_WIDTH 16
#define WORLD_ENEMY_BBOX_HEIGHT 16

#define DIRECTION_UP 1
#define DIRECTION_DOWN	2
#define DIRECTION_LEFT 3
#define DIRECTION_RIGHT	4

class CWMario : public CGameObject {
	int currentPba = -1;
	int PbaStep = 0;
	ULONGLONG PbaTimer, timetillnextPba;

	float nodeCoorX[21] = { -88, -56, -56, -24, 8, 40, 72,
												8, 40, 72,
								-56, -24, 8,
								-56, -24, 8, 40, 72,
								-56, -24, 8};

	float nodeCoorY[21] = {-61, -61, -93, -93, -93, -93,-93,
												-61, -61, -61,
								-29, -29, -29,
								3, 3, 3, 3, 3,
								35, 35, 35};

	int nodeIsLevel[21] = { 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1};

	int matrix[21][21];

	int currentnode = 0;
	int nextnode = -1;
public:
	bool acceptKeyboardInput = 1;

	CWMario(float x, float y) : CGameObject(x, y) {
		for (int i = 0; i < 21; i++)
			for (int j = 0; j < 21; j++)
			{
				matrix[i][j] = 0;
			}
		matrix[0][1] = 1; matrix[1][0] = 1;
		matrix[1][2] = 1; matrix[2][1] = 1;
		matrix[2][3] = 1; matrix[3][2] = 1;
		matrix[3][4] = 1; matrix[4][3] = 1;
		matrix[4][5] = 1; matrix[5][4] = 1;
			matrix[4][7] = 1; matrix[7][4] = 1;
		matrix[5][6] = 1; matrix[6][5] = 1;
		matrix[9][6] = 1; matrix[6][9] = 1;

		matrix[7][8] = 1; matrix[8][7] = 1;
			matrix[7][12] = 1; matrix[12][7] = 1;
		matrix[8][9] = 1; matrix[9][8] = 1;

		matrix[10][11] = 1; matrix[11][10] = 1;
			matrix[10][13] = 1; matrix[13][10] = 1;
		matrix[11][12] = 1; matrix[12][11] = 1;

			matrix[13][18] = 1; matrix[18][13] = 1;
		matrix[14][15] = 1; matrix[15][14] = 1;
		matrix[15][16] = 1; matrix[16][15] = 1;
			matrix[15][20] = 1; matrix[20][15] = 1;
		matrix[16][17] = 1; matrix[17][16] = 1;

		matrix[18][19] = 1; matrix[18][18] = 1;
		matrix[19][20] = 1; matrix[20][19] = 1;
		currentnode = 0;
		nextnode = -1;

		for (int j = 0; j < 21; j++)
		{
			DebugOut(L"Node %i has x, y = %f, %f \n", j, GetNodeCoorX(j), GetNodeCoorY(j));
		}
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	int FindPossibleNode(int direction);
	void MoveToNode(int n);
	float GetNodeCoorX(int n) { return nodeCoorX[n]; }
	float GetNodeCoorY(int n) { return nodeCoorY[n]; }
	void Enter();

	void StartPBA(int pba, int step);
	void GetMoveLeftPBA(int step);
	void GetMoveRightPBA(int step);
	void GetMoveUpPBA(int step);
	void GetMoveDownPBA(int step);
};

class CWEnemy : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;
public:
	CWEnemy(float x, float y) : CGameObject(x, y) {
		StartPBA(1,1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	void StartPBA(int pba, int step);
	void GetMoveLeftPBA(int step);
	void GetMoveRightPBA(int step);

	int GetAniId();
};
