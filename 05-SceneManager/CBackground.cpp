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
