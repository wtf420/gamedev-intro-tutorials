#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define BACKGROUND_WIDTH 2816
#define BACKGROUND_BBOX_WIDTH 2816
#define BACKGROUND_BBOX_HEIGHT 626

class CBackground : public CGameObject {
	int SpriteID;
public:
	CBackground(float x, float y, int k) : CGameObject(x, y) {
		SpriteID = k;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

