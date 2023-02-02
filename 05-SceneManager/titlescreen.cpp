#include "titlescreen.h"
#include "debug.h"

void CTSMario::Render()
{
	CAnimations::GetInstance()->Get(GetAniId())->Render(x, y);
	//RenderBoundingBox();
}

int CTSMario::GetAniId()
{
	return -1;
}

void CTSMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

void CTSMario::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		//GetMoveLeftPBA(step);
		break;
	case 2:
		currentPba = 2;
		PbaStep = 1;
		PbaTimer = 0;
		//GetMoveRightPBA(step);
		break;
	default:
		break;
	}
}

void CTSMario::GetPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = 1;
		PbaTimer = 0;

		timetillnextPba = 800;

		vx = 0.0f;
		vy = 0;
		break;
	}
	//end of PBA
	default:
		PbaStep = 0;
		currentPba = -1;
		acceptKeyboardInput = 1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
}

void CTSMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}