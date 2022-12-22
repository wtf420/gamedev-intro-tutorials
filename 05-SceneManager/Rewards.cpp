#include "Rewards.h"
#include "Debug.h"
#include "PlayScene.h"

void CRCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > maxY && ay < 0) vy += ay * dt; else
		if (y <= maxY && ay < 0)
		{
			y = maxY;
			maxvy = COIN_UP_SPEED_Y;
			ay = COIN_ACCEL_SPEED_Y;
			vy = 0;
		}
	if (y < originalY && ay > 0) vy += ay * dt; else
		if (y >= originalY && ay > 0)
		{
			y = originalY;
			ay = 0;
			vy = 0;
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->AddCoin();
			isDeleted = true;
		}

	y += vy * dt;
}

void CRCoin::Start()
{
	maxvy = -COIN_UP_SPEED_Y;
	ay = -COIN_ACCEL_SPEED_Y;
}

void CRCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);
}

void CRCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_MYTH_BBOX_WIDTH / 2;
	t = y - COIN_MYTH_BBOX_HEIGHT / 2;
	r = l + COIN_MYTH_BBOX_WIDTH;
	b = t + COIN_MYTH_BBOX_HEIGHT;
}

/*
* **************************************************
*					MUSHROOM
* **************************************************
*/

void CRMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > maxY && vy < 0) vy += ay * dt; else
		if (y <= maxY && vy < 0)
		{
			this->ay = MUSHROOM_GRAVITY;
			this->ax = 0;
			vy = 0;
			vx = -MUSHROOM_WALKING_SPEED;
			collidable = true;
		}

	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CRMushroom::Died()
{
	isDeleted = true;
};

void CRMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CRMushroom*>(e->obj)) return;
	if (dynamic_cast<CMario*>(e->obj)) {
		isDeleted = true;
		LPGAME game = CGame::GetInstance();
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->OnCollisionWithMushroom();
	}

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CRMushroom::Start()
{
	vy = -MUSHROOM_UP_SPEED_Y;
	maxvy = 0;
	ay = 0;
	ax = 0;
}

void CRMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM)->Render(x, y);
}

void CRMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_MYTH_BBOX_WIDTH / 2;
	t = y - COIN_MYTH_BBOX_HEIGHT / 2;
	r = l + COIN_MYTH_BBOX_WIDTH;
	b = t + COIN_MYTH_BBOX_HEIGHT;
}