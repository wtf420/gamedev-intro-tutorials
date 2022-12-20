#include "CBackground.h"

void CBackground::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BACKGROUND)->Render(x, y);

	//RenderBoundingBox();
}

void CBackground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BACKGROUND_BBOX_WIDTH / 2;
	t = y - BACKGROUND_BBOX_HEIGHT / 2;
	r = l + BACKGROUND_BBOX_WIDTH;
	b = t + BACKGROUND_BBOX_HEIGHT;
}
