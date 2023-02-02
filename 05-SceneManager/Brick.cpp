#include "Brick.h"
#include "PlayScene.h"
#include "Coin.h"
#include "Rewards.h"

void CBrick::Render()
{
	LPSPRITE sprite = CSprites::GetInstance()->Get(ID_BRICK_2);
	sprite->Draw(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

//**********************************************
//	BRICK WITH COIN
//**********************************************

void CBrickWithCoin::Render()
{
	LPSPRITE sprite = CSprites::GetInstance()->Get(ID_BRICK_2);
	sprite->Draw(x, y);
	//RenderBoundingBox();
}

void CBrickWithCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrickWithCoin::Attacked()
{
	Delete();
	CCoin* coin = new CCoin(this->x, this->y);
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(coin);
}

//**********************************************
//	BRICK WITH POWERBLOCK
//**********************************************
void CBrickWithP::Render()
{
	if (!attacked)
	{
		LPSPRITE sprite = CSprites::GetInstance()->Get(ID_BRICK_2);
		sprite->Draw(x, y);
	}
	else
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_BRICK_DIED)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CBrickWithP::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBrickWithP::Attacked()
{
	if (!attacked)
	{
		attacked = true;
		PowerBlock* p = new PowerBlock(this->x, this->y - 16.0f);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(p);
	}
}