#include "hud.h"

void CHud::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_HUD)->Render(x, y);

	//RenderBoundingBox();
}

void CHud::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - HUD_BBOX_WIDTH / 2;
	t = y - HUD_BBOX_HEIGHT / 2;
	r = l + HUD_BBOX_WIDTH;
	b = t + HUD_BBOX_HEIGHT;
}
