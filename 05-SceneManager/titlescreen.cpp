#include "titlescreen.h"
#include "mario.h"
#include "goomba.h"
#include "koopas.h"
#include "rewards.h"
#include "CBackground.h"
#include "Playscene.h"
#include "debug.h"

void CTSBackground::Render()
{
	if (ani != -1)
		CAnimations::GetInstance()->Get(ani)->Render(x, y);
}

void CTSBackground::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

void CTSBackground::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetPBA(step);
		break;
	default:
		break;
	}
}

void CTSBackground::GetPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		//chay xuong
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2100;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 2:
	{
		//chay xuong
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2100;
		vx = 0.0f;
		vy = 0.06f;
		break;
	}
	case 3:
	{
		//dung im
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2300;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	default:
		PbaStep = 0;
		currentPba = -1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
}

void CTSBackground::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = 200;
	top = 200;
	right = 200;
	bottom = 200;
}

//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************

void CTSTurtleShell::Render()
{
	if (currentani != -1)
		CAnimations::GetInstance()->Get(currentani)->Render(x, y);
	//RenderBoundingBox();
}

void CTSTurtleShell::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		if (y > 173.0f)
			y = 173.0f;

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

void CTSTurtleShell::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentani = ID_ANI_KOOPAS_SHELL;
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetPBA(step);
		break;
	default:
		break;
	}
}

void CTSTurtleShell::GetPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2500;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 2:
	{
		//bat dau roi
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2100;
		vx = 0.0f;
		vy = 0.08f;
		break;
	}
	case 3:
	{
		//nay
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 250;
		vx = 0.04375;
		vy = -0.044f;
		break;
	}
	case 4:
	{
		//nay xuong
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 250;
		vx = 0.04375f;
		vy = 0.15f;
		break;
	}
	case 5:
	{
		//cham dat
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 4050;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 6:
	{
		//bi da sang phai
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 700;
		vx = 0.1f;
		vy = 0.0f;
		break;
	}
	case 7:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 200;
		vx = -0.07f;
		vy = 0.0f;
		break;
	}
	case 8:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 500;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 9:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1000;
		vx = -0.2f;
		vy = 0.0f;
		break;
	}
	default:
		PbaStep = 0;
		currentPba = -1;
		vx = 0.0f;
		vy = 0.0f;
		Delete();
		break;
	}
}

void CTSTurtleShell::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************

void CTSGoomba::Render()
{
	if (currentani != -1)
		CAnimations::GetInstance()->Get(currentani)->Render(x, y);
	//RenderBoundingBox();
}

void CTSGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		if (y > 173.0f)
			y = 173.0f;

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

void CTSGoomba::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentani = ID_ANI_GOOMBA_WALKING;
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetPBA(step);
		break;
	default:
		break;
	}
}

void CTSGoomba::GetPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2500;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 2:
	{
		//bat dau roi
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1000;
		vx = 0.0f;
		vy = 0.193f;
		break;
	}
	case 3:
	{
		//cham dat & dung im
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 3200;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 4:
	{
		//chay qua trai
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 300;
		vx = -0.06f;
		vy = 0.0f;
		break;
	}
	case 5:
	{
		//ded
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 300;
		currentani = ID_ANI_GOOMBA_DIE;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	//end of PBA
	default:
		PbaStep = 0;
		currentPba = -1;
		vx = 0.0f;
		vy = 0.0f;
		Delete();
		break;
	}
}

void CTSGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************

void CTSKoopas::Render()
{
	if (currentani != -1)
		CAnimations::GetInstance()->Get(currentani)->Render(x, y);
	//RenderBoundingBox();
}

void CTSKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		if (y > 173.0f)
			y = 173.0f;

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

void CTSKoopas::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentani = ID_ANI_KOOPAS_WALKING_RIGHT;
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetPBA(step);
		break;
	default:
		break;
	}
}

void CTSKoopas::GetPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 10000;
		vx = speed;
		vy = 0.0f;
		break;
	}
	default:
		PbaStep = 0;
		currentPba = -1;
		vx = 0.0f;
		vy = 0.0f;
		Delete();
		break;
	}
}

void CTSKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************

void CTSMushroom::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM)->Render(x, y);
	//RenderBoundingBox();
}

void CTSMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		if (y > 173.0f)
			y = 173.0f;

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

void CTSMushroom::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetPBA(step);
		break;
	default:
		break;
	}
}

void CTSMushroom::GetPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2500;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 2:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1000;
		vx = 0.0f;
		vy = 0.223f;
		break;
	}
	case 3:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1000;
		vx = -0.05f;
		vy = 0.0f;
		break;
	}
	//end of PBA
	default:
		PbaStep = 0;
		currentPba = -1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
}

void CTSMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************

void CTSLeaf::Render()
{
	if (currentani != -1)
		CAnimations::GetInstance()->Get(currentani)->Render(x, y);
	//RenderBoundingBox();
}

void CTSLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"Step %i, timer %f, ani %i\n", currentPba, PbaTimer, currentani);
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		if (y > 165.0f)
			y = 165.0f;

		PbaTimer += dt;
		if (PbaStep == 2)
		{
			//DebugOut(L"%f \n", timer2);
			timer2 += dt;
			if (timer2 >= 680)
			{
				vx = -vx;
				timer2 = 0;
			}
		}
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

void CTSLeaf::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetPBA(step);
		break;
	default:
		break;
	}
}

void CTSLeaf::GetPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2500;
		currentani = ID_ANI_LEAF;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 2:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 3400;
		timer2 = 0;
		vx = 0.0735f;
		vy = 0.035f;
		break;
	}
	//end of PBA
	default:
		currentani = -1;
		PbaStep = 0;
		currentPba = -1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
}

void CTSLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************

void CTSLuigi::Render()
{
	if (currentani != -1)
		CAnimations::GetInstance()->Get(currentani)->Render(x, y);
	//RenderBoundingBox();
}

void CTSLuigi::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"Step %i, timer %f, ani %i\n", currentPba, PbaTimer, currentani);
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		if (y > 165.0f)
			y = 165.0f;

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

void CTSLuigi::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetPBA(step);
		break;
	default:
		break;
	}
}

void CTSLuigi::GetPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1200;
		currentani = ID_ANI_LUIGI_IDLE_RIGHT;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 2:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 400;
		currentani = ID_ANI_LUIGI_WALKING_RIGHT;
		vx = 0.07f;
		vy = 0.0f;


		break;
	}
	case 3:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 250;
		currentani = ID_ANI_LUIGI_JUMP_RIGHT;
		vx = 0.07f;
		vy = -0.2f;


		break;
	}
	case 4:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 200;
		currentani = ID_ANI_LUIGI_FALL_RIGHT;
		vx = 0.07f;
		vy = 0.2f;


		break;
	}
	case 5:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 400;
		currentani = ID_ANI_LUIGI_JUMP_RIGHT;
		vx = 0.07f;
		vy = -0.2f;


		break;
	}
	case 6:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 450;
		currentani = ID_ANI_LUIGI_FALL_RIGHT;
		vx = 0.07f;
		vy = 0.2f;


		break;
	}
	case 7:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 800;
		currentani = ID_ANI_LUIGI_WALKING_RIGHT;
		vx = 0.07f;
		vy = 0.0f;


		break;
	}
	case 8:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 6150;
		currentani = -1;
		vx = 0.0f;
		vy = 0.0f;


		break;
	}
	case 9:
	{
		PbaStep = step;
		PbaTimer = 0;

		x = 310.0f;
		timetillnextPba = 200;
		currentani = ID_ANI_LUIGI_WALKRUN_HOLD_LEFT;
		vx = -0.07f;
		vy = 0.0f;


		break;
	}
	case 10:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 500;
		currentani = ID_ANI_LUIGI_IDLE_HOLD_LEFT;
		vx = 0.0f;
		vy = 0.0f;


		break;
	}
	case 11:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 500;
		currentani = ID_ANI_LUIGI_IDLE_LEFT;
		vx =0.0f;
		vy = 0.0f;


		break;
	}
	case 12:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 200;
		currentani = ID_ANI_LUIGI_WALKING_RIGHT;
		vx = 0.1f;
		vy = 0.0f;


		break;
	}
	default:
		PbaStep = 0;
		currentani = -1;
		currentPba = -1;
		acceptKeyboardInput = 1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
}

void CTSLuigi::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MARIO_SMALL_BBOX_WIDTH / 2;
	top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
	right = left + MARIO_SMALL_BBOX_WIDTH;
	bottom = top + MARIO_SMALL_BBOX_HEIGHT;
}

//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************
//*********************************************************************************************

void CTSMario::Render()
{
	if (currentani != -1)
	CAnimations::GetInstance()->Get(currentani)->Render(x, y);
	//RenderBoundingBox();
}

void CTSMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"Step %i, timer %f, ani %i\n", currentPba, PbaTimer, currentani);
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		if (y > 165.0f)
			y = 165.0f;

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
		GetPBA(step);
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
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1200;
		currentani = ID_ANI_MARIO_IDLE_LEFT;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 2:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1000;
		currentani = ID_ANI_MARIO_RUNNING_LEFT;
		vx = -0.07f;
		vy = 0.0f;


		break;
	}
	case 3:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1800;
		currentani = ID_ANI_MARIO_SIT_LEFT;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 4:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 500;
		currentani = ID_ANI_MARIO_IDLE_LEFT;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 5:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 100;
		currentani = ID_ANI_MARIO_TITLESCREEN_1;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 6:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 50;
		currentani = ID_ANI_MARIO_IDLE_LEFT;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 7:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 500;
		currentani = ID_ANI_MARIO_TITLESCREEN_2;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 8:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 750;
		currentani = ID_ANI_MARIO_JUMP_LEFT;
		vx = 0.0f;
		vy = -0.1f;
		break;
	}
	case 9:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 200;
		currentani = ID_ANI_MARIO_LEVEL_UPDOWN_BIG;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 10:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1100;
		currentani = ID_ANI_MARIO_RACCOON_FALL_JS_LEFT;
		vx = -0.04375f;
		vy = 0.06f;
		break;
	}
	case 11:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 250;
		currentani = ID_ANI_MARIO_RACCOON_JUMP_LEFT;
		vx = -0.04375;
		vy = -0.044f;
		break;
	}
	case 12:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 250;
		currentani = ID_ANI_MARIO_RACCOON_FALL_JS_LEFT;
		vx = -0.04375f;
		vy = 0.088f;
		break;
	}
	case 13:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 50;
		currentani = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 14:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1400;
		currentani = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
		vx = 0.05f;
		vy = 0.0f;
		break;
	}
	case 15:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 150;
		currentani = ID_ANI_MARIO_RACCOON_KICK_RIGHT;
		vx = 0.05f;
		vy = 0.0f;

		//add two more bushes so mario will have sth to run into and disappear 
		break;
	}
	case 16:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 600;
		currentani = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
		vx = 0.05f;
		vy = 0.0f;
		break;
	}
	case 17:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 500;
		currentani = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
		vx = 0.0f;
		vy = 0.0f;
		break;
	}
	case 18:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2000;
		currentani = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
		vx = -0.14f;
		vy = 0.0f;
		break;
	}
	case 19:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2000;
		currentani = -1;
		vx = 0.0f;
		vy = 0.0f;

		CBackground* select = new CBackground(200, 160, 190009);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(select);

		CTSKoopas * koopas1 = new CTSKoopas(90, 165, 0.02f);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(koopas1);

		CBackground* bush1 = new CBackground(104, 148, 190005);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(bush1);
		CBackground* bush2 = new CBackground(296, 132, 190006);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(bush2);
		break;
	}
	case 20:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 2000;
		currentani = -1;
		vx = 0.0f;
		vy = 0.0f;

		CTSKoopas* koopas1 = new CTSKoopas(90, 165, 0.02f);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(koopas1);

		CBackground* bush1 = new CBackground(104, 148, 190005);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(bush1);
		CBackground* bush2 = new CBackground(296, 132, 190006);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(bush2);
		break;
	}
	case 21:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 7000;
		currentani = -1;
		vx = 0.0f;
		vy = 0.0f;

		CTSKoopas* koopas1 = new CTSKoopas(90, 165, 0.02f);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(koopas1);

		CBackground* bush1 = new CBackground(104, 148, 190005);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(bush1);
		CBackground* bush2 = new CBackground(296, 132, 190006);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(bush2);
		break;
	}
	case 22:
	{
		PbaStep = step;
		PbaTimer = 0;

		timetillnextPba = 1000;
		currentani = -1;
		vx = 0.0f;
		vy = 0.0f;

		CTSKoopas* koopas1 = new CTSKoopas(90, 165, 0.05f);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(koopas1);

		CBackground* bush1 = new CBackground(104, 148, 190005);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(bush1);
		CBackground* bush2 = new CBackground(296, 132, 190006);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(bush2);
	}

	//end of PBA
	default:
		PbaStep = 0;
		currentani = -1;
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

//****************************************************************************************************
//
//****************************************************************************************************