#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_HUD 120000
#define HUD_WIDTH 152
#define HUD_BBOX_WIDTH 152
#define HUD_BBOX_HEIGHT 28

class CHud : public CGameObject {
public:
	CHud(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};
