#include "CBackground.h"

void CBackground::Render()
{
	LPSPRITE sprite = CSprites::GetInstance()->Get(SpriteID);
	sprite->Draw(x, y);
}

void CBackground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	LPSPRITE sprite = CSprites::GetInstance()->Get(SpriteID);
	l = x - sprite->Width() / 2;
	t = y - sprite->Heigth() / 2;
	r = l + sprite->Width();
	b = t + sprite->Heigth();
}

//********************************************************************************************
//
//********************************************************************************************

void CAnimatedBackground::Render()
{
	CAnimations::GetInstance()->Get(AniID)->Render(x, y);
}

void CAnimatedBackground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BACKGROUND_BBOX_WIDTH / 2;
	t = y - BACKGROUND_BBOX_HEIGHT / 2;
	r = l + BACKGROUND_BBOX_WIDTH;
	b = t + BACKGROUND_BBOX_HEIGHT;
}

//********************************************************************************
//
//********************************************************************************

void CBackgroundFX::Render()
{
	CAnimations::GetInstance()->Get(AniID)->Render(x, y);
	//RenderBoundingBox();
}

void CBackgroundFX::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

void CBackgroundFX::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetPopupPBA(step);
		break;
	default:
		break;
	}
}

void CBackgroundFX::GetPopupPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = 1;
		PbaTimer = 0;

		timetillnextPba = time;
		vx = 0;
		vy = 0;
		break;
	}
	//end of PBA
	default:
		Delete();
		break;
	}
}

void CBackgroundFX::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - 2 / 2;
	t = y - 2 / 2;
	r = l + 2;
	b = t + 2;
}
