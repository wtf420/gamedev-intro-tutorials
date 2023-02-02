#include "worldmap.h"
#include "debug.h"

void CWMario::Render()
{
	CAnimations::GetInstance()->Get(ID_MARIO_ANI_WORLD_SMALL)->Render(x, y);
	//RenderBoundingBox();
}

void CWMario::Enter()
{
	if (currentPba == -1 && nodeIsLevel[currentnode] == 1)
	{
		CGame::GetInstance()->InitiateSwitchScene(3);
	}
}

int CWMario::FindPossibleNode(int direction)
{
	DebugOut(L"\n");
	if (direction == DIRECTION_UP)
	{
		for (int i = 0; i < 21; i++)
			if (matrix[currentnode][i] == 1 && (GetNodeCoorY(i) < GetNodeCoorY(currentnode))) {
				DebugOut(L"\nFOUND POSSIBLE NODE %i: %f, %f\n", i, GetNodeCoorX(i), GetNodeCoorY(i));
				return i;
			}
	}
	else
	if (direction == DIRECTION_DOWN)
	{
		for (int i = 0; i < 21; i++)
			if (matrix[currentnode][i] == 1 && (GetNodeCoorY(i) > GetNodeCoorY(currentnode))) {
				DebugOut(L"\nFOUND POSSIBLE NODE %i: %f, %f\n", i, GetNodeCoorX(i), GetNodeCoorY(i));
				return i;
			}
	}

	else
	if (direction == DIRECTION_LEFT)
	{
		for (int i = 0; i < 21; i++)
			if (matrix[currentnode][i] == 1 && (GetNodeCoorX(i) < GetNodeCoorX(currentnode))) {
				DebugOut(L"\nFOUND POSSIBLE NODE %i: %f, %f\n", i, GetNodeCoorX(i), GetNodeCoorY(i));
				return i;
			}
	}
	else
	if (direction == DIRECTION_RIGHT)
	{
		for (int i = 0; i < 21; i++)
			if (matrix[currentnode][i] == 1 && (GetNodeCoorX(i) > GetNodeCoorX(currentnode))) {
				DebugOut(L"\nFOUND POSSIBLE NODE %i: %f, %f\n", i, GetNodeCoorX(i), GetNodeCoorY(i));
				return i;
			}
	}
	return -1;
}

void CWMario::MoveToNode(int n)
{
	if (n == -1) return; else
	{
		nextnode = n;
		DebugOut(L"MOVING FROM: %i TO %i, Current coors: %f, %f To %f, %f\n", 
			currentnode, nextnode, this->x, this->y, GetNodeCoorX(nextnode), GetNodeCoorY(nextnode));
		if (GetNodeCoorY(nextnode) > GetNodeCoorY(currentnode))
		{
			DebugOut(L"DIRECTION_DOWN\n");
			StartPBA(DIRECTION_DOWN, 1);
		} else
		if (GetNodeCoorY(nextnode) < GetNodeCoorY(currentnode)) {
			DebugOut(L"DIRECTION_UP\n");
			StartPBA(DIRECTION_UP, 1);
		}
		else
		if (GetNodeCoorX(nextnode) > GetNodeCoorX(currentnode)) {
			DebugOut(L"DIRECTION_RIGHT\n");
			StartPBA(DIRECTION_RIGHT, 1);
		}
		else
		if (GetNodeCoorX(nextnode) < GetNodeCoorX(currentnode)) {
			DebugOut(L"DIRECTION_LEFT\n");
			StartPBA(DIRECTION_LEFT, 1);
		}
	}
}

void CWMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	nodeCoorY[6] = -93.0f;
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		PbaTimer += dt;
		//condition to switch step / pba
		if (currentPba == DIRECTION_DOWN || currentPba == DIRECTION_UP)
		{
			if (abs(GetNodeCoorY(nextnode) - this->y) < 5.0f)
			{
				this->y = GetNodeCoorY(nextnode);
				DebugOut(L"REACHED DESTINATION Y: %i TO %i, Current coors: %f, %f To %f, %f\n",
					currentnode, nextnode, this->x, this->y, GetNodeCoorX(nextnode), GetNodeCoorY(nextnode));
				PbaStep++;
				StartPBA(currentPba, PbaStep);
			}
		} else
		{
			if (abs(GetNodeCoorX(nextnode) - this->x) < 5.0f)
			{ 
				this->x = GetNodeCoorX(nextnode);
				DebugOut(L"REACHED DESTINATION X: %i TO %i, Current coors: %f, %f To %f, %f\n",
					currentnode, nextnode, this->x, this->y, GetNodeCoorX(nextnode), GetNodeCoorY(nextnode));
				PbaStep++;
				StartPBA(currentPba, PbaStep);
			}
		}
	}
	else
	{
		//insert normal update here
	}
}

void CWMario::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - WORLD_MARIO_BBOX_WIDTH / 2;
	t = y - WORLD_MARIO_BBOX_HEIGHT / 2;
	r = l + WORLD_MARIO_BBOX_WIDTH;
	b = t + WORLD_MARIO_BBOX_HEIGHT;
}

void CWMario::StartPBA(int pba, int step)
{
	DebugOut(L"Start PBA %i, step %i, current: %i \n", pba, step, currentPba);
	switch (pba)
	{
		case 1:
		{
			currentPba = 1;
			PbaStep = 1;
			PbaTimer = 0;
			GetMoveUpPBA(step);
			break;
		}
		case 2:
		{
			currentPba = 2;
			PbaStep = 1;
			PbaTimer = 0;
			GetMoveDownPBA(step);
			break;
		}
		case 3:
		{
			currentPba = 3;
			PbaStep = 1;
			PbaTimer = 0;
			GetMoveLeftPBA(step);
			break;
		}
		case 4:
		{
			currentPba = 4;
			PbaStep = 1;
			PbaTimer = 0;
			GetMoveRightPBA(step);
			break;
		}
	default:
		break;
	}
}

void CWMario::GetMoveLeftPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = 1;
		PbaTimer = 0;

		timetillnextPba = 800;
		acceptKeyboardInput = 0;
		vx = -0.1f;
		vy = 0;
		break;
	}
	//end of PBA
	default:
		PbaStep = 0;
		currentPba = -1;
		currentnode = nextnode;
		nextnode = -1;
		acceptKeyboardInput = 1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
}

void CWMario::GetMoveRightPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = 1;
		PbaTimer = 0;

		timetillnextPba = 800;
		acceptKeyboardInput = 0;
		vx = 0.1f;
		vy = 0;
		break;
	}
	//end of PBA
	default:
		PbaStep = 0;
		currentPba = -1;
		currentnode = nextnode;
		nextnode = -1;
		acceptKeyboardInput = 1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
}

void CWMario::GetMoveUpPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = 1;
		PbaTimer = 0;

		timetillnextPba = 800;
		acceptKeyboardInput = 0;
		vx = 0;
		vy = -0.1f;
		break;
	}
	//end of PBA
	default:
		PbaStep = 0;
		currentPba = -1;
		currentnode = nextnode;
		nextnode = -1;
		acceptKeyboardInput = 1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
}

void CWMario::GetMoveDownPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = 1;
		PbaTimer = 0;

		timetillnextPba = 800;
		acceptKeyboardInput = 0;
		vx = 0;
		vy = 0.1f;
		break;
	}
	//end of PBA
	default:
		PbaStep = 0;
		currentPba = -1;
		currentnode = nextnode;
		nextnode = -1;
		acceptKeyboardInput = 1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
}

//********************************************************************************
//
//********************************************************************************

void CWEnemy::Render()
{
	CAnimations::GetInstance()->Get(GetAniId())->Render(x, y);
	//RenderBoundingBox();
}

int CWEnemy::GetAniId()
{
	switch (currentPba)
	{
	case 1:
		return ID_ANI_WORLD_ENEMY_MOVE_LEFT;
		break;
	case 2:
		return ID_ANI_WORLD_ENEMY_MOVE_RIGHT;
		break;
	default:
		return ID_ANI_WORLD_ENEMY_MOVE_LEFT;
		break;
	}
}

void CWEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		PbaTimer += dt;
		//condition to switch step / pba
		if (PbaTimer >= timetillnextPba)
		{
			PbaStep++;
			StartPBA(currentPba, PbaStep);
		}
	}
	else
	{
		//insert normal update here
	}
}

void CWEnemy::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetMoveLeftPBA(step);
		break;
	case 2:
		currentPba = 2;
		PbaStep = 1;
		PbaTimer = 0;
		GetMoveRightPBA(step);
		break;
	default:
		break;
	}
}

void CWEnemy::GetMoveLeftPBA(int step)
{
	switch (step)
	{
		case 1:
		{
			PbaStep = 1;
			PbaTimer = 0;

			timetillnextPba = 800;
			vx = -0.025f;
			vy = 0;
			break;
		}
	//end of PBA
		default:
			StartPBA(2, 1);
			break;
	}
}

void CWEnemy::GetMoveRightPBA(int step)
{
	switch (step)
	{
		case 1:
		{
			PbaStep = 1;
			PbaTimer = 0;

			timetillnextPba = 800;
			vx = 0.025f;
			vy = 0;
			break;
		}
		//end of PBA
		default:
			StartPBA(1, 1);
			break;
	}
}

void CWEnemy::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - WORLD_ENEMY_BBOX_WIDTH / 2;
	t = y - WORLD_ENEMY_BBOX_HEIGHT / 2;
	r = l + WORLD_ENEMY_BBOX_WIDTH;
	b = t + WORLD_ENEMY_BBOX_HEIGHT;
}
