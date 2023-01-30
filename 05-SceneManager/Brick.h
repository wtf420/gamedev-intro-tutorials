#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_BRICK_1	20001	
#define ID_BRICK_2	20002
#define ID_BRICK_3	20003	
#define ID_BRICK_DIED 69000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject {
public:
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class CBrickWithCoin : public CGameObject {
public:
	CBrickWithCoin(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void Attacked();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class CBrickWithP : public CGameObject {
	bool attacked;
public:
	CBrickWithP(float x, float y) : CGameObject(x, y) {
		attacked = false;
	}
	void Render();
	void Update(DWORD dt) {}
	void Attacked();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};