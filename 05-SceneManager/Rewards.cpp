#include "Rewards.h"
#include "Debug.h"
#include <string>
#include "PlayScene.h"

void CRCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//If falling up pass limit
	if (y <= maxY && vy < 0)
	{
		y = maxY;
		vy = COIN_GRAVITY;
	}
	else //If falling down pass limit
		if (y >= originalY && vy > 0)
		{
			y = originalY;
			vy = 0;
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->AddCoin();
			mario->AddScore(this->x, this->y, 200);
			isDeleted = true;
		}

	y += vy * dt;
}

void CRCoin::Start()
{
	vy = -COIN_UP_SPEED_Y;
}

void CRCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_REWARD_COIN)->Render(x, y);
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
			collidable = 1;
		}

	vy += ay * dt;
	vx += ax * dt;

	vector<LPGAMEOBJECT> coObjects2;
	for (int k = 0; k < coObjects->size(); k++)
	{
		CPlatformOneWay* u = dynamic_cast<CPlatformOneWay*>(coObjects->at(k));
		float top = 0;
		if (u) top = u->GetTopPosition();
		if ((!u) || (u == currentPlatform) || (u && top >= y))
		{
			coObjects2.push_back(coObjects->at(k));
		}
	}
	CGameObject::Update(dt, &coObjects2);
	CCollision::GetInstance()->Process(this, dt, &coObjects2);
}

void CRMushroom::OnNoCollision(DWORD dt)
{
	currentPlatform = NULL;
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
	if (e->ny <= 0) currentPlatform = e->obj;

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

/*******************************************************************
							LEAF
/********************************************************************/

void CRLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//If falling right pass limit
	if (x > maxX && vx > 0)
	{
		x = maxX;
		vx = -LEAF_SPEED_X;
	}
	else //If falling left pass limit
		if (x < originalX && vx < 0)
		{
			x = originalX;
			vx = LEAF_SPEED_X;
		}

	//If falling up pass limit
	if (y <= maxY && vy < 0)
	{
		vx = LEAF_SPEED_X;
		y = maxY;
		vy = LEAF_GRAVITY;
		collidable = 1;
	} else //If falling down pass limit
	if (y >= originalY && vy > 0)
	{
		y = originalY;
		vy = 0;
		isDeleted = true;
	}

	x += vx * dt;
	y += vy * dt;
}

void CRLeaf::Start()
{
	vx = 0;
	vy = -LEAF_UP_SPEED_Y;
}

void CRLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_LEAF)->Render(x, y);
}

void CRLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_MYTH_BBOX_WIDTH / 2;
	t = y - COIN_MYTH_BBOX_HEIGHT / 2;
	r = l + COIN_MYTH_BBOX_WIDTH;
	b = t + COIN_MYTH_BBOX_HEIGHT;
}

/*******************************************************************
							Power block
/********************************************************************/
void PowerBlock::Render()
{
	if (collidable)
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_POWERBLOCK)->Render(x, y);
	}
	else
	{
		LPSPRITE sprite = CSprites::GetInstance()->Get(ID_POWERBLOCK_DIED);
		sprite->Draw(x, y);
	}
}

void PowerBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}