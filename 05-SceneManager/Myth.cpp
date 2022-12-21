#include "Myth.h"
#include "Debug.h"
#include "Rewards.h"

void CMyth::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > maxY && ay < 0) vy += ay * dt; else
		if (y <= maxY && ay < 0)
		{
			y = maxY;
			maxvy = UP_SPEED_Y;
			ay = ACCEL_SPEED_Y;
			vy = 0;
		}
	if (y < originalY && ay > 0) vy += ay * dt; else
		if (y >= originalY && ay > 0)
		{
			y = originalY;
			ay = 0;
			vy = 0;
			if (type == TYPE_MUSHROOM)
				Reward();
		}

	vy += ay * dt;
	y += vy * dt;
}

void CMyth::Interact()
{
	if (!touched) {
		maxvy = -UP_SPEED_Y;
		ay = -ACCEL_SPEED_Y;
		if (type == TYPE_COIN)
			Reward();
	}
	touched = true;
}

void CMyth::Reward()
{
	
	switch (type)
	{
	case TYPE_COIN:
		SpawnCoin();
		break;
	case TYPE_MUSHROOM:
		SpawnMushroom();
		break;
	}
}

void CMyth::SpawnCoin()
{
	CRCoin* r = dynamic_cast<CRCoin*>(reward);
	r->Start();
}

void CMyth::SpawnMushroom()
{
	CRMushroom* r = dynamic_cast<CRMushroom*>(reward);
	r->Start();
}

void CMyth::Render()
{
	if (!touched)
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_MYTH)->Render(x, y);
	}
	else
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_MYTH_DIED)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CMyth::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MYTH_BBOX_WIDTH / 2;
	t = y - MYTH_BBOX_HEIGHT / 2;
	r = l + MYTH_BBOX_WIDTH;
	b = t + MYTH_BBOX_HEIGHT;
}
