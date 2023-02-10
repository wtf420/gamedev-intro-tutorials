#include "Myth.h"
#include "Debug.h"
#include "Rewards.h"

CMyth::CMyth(float x, float y, int t) : CGameObject(x, y) {
	mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	type = t;
	touched = false;
	originalY = y;
	maxY = y - 8;
	maxvy = 0;
	ay = 0;
}

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
			if (type == TYPE_MUSHROOM || type == TYPE_1UP)
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
	case TYPE_1UP:
		Spawn1Up();
		break;
	default:
		break;
	}
}

void CMyth::SpawnCoin()
{
	CRCoin* coin = new CRCoin(x, y);
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(coin);
	coin->Start();

}

void CMyth::Spawn1Up()
{
	CR1UpMushroom* up = new CR1UpMushroom(x, y);
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(up);
	up->Start();

}

void CMyth::SpawnMushroom()
{
	if (mario->GetLevel() == MARIO_LEVEL_SMALL)
	{
		CRMushroom* mushroom = new CRMushroom(x, y);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(mushroom);
		mushroom->Start();
	}
	else
	{
		CRLeaf* leaf = new CRLeaf(x, y);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(leaf);
		leaf->Start();
	}
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
