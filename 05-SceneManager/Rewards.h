#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 62000

#define COIN_MYTH_BBOX_WIDTH 16
#define COIN_MYTH_BBOX_HEIGHT 16

#define COIN_UP_SPEED_Y		0.05f
#define COIN_ACCEL_SPEED_Y	0.005f
#define COIN_MAX_Y 100.0f

#define ID_ANI_MUSHROOM 63000

#define MUSHROOM_MYTH_BBOX_WIDTH 16
#define MUSHROOM_MYTH_BBOX_HEIGHT 16

#define MUSHROOM_UP_SPEED_Y		0.05f
#define MUSHROOM_ACCEL_SPEED_Y	0.005f
#define MUSHROOM_MAX_Y 16.0f

#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_WALKING_SPEED 0.05f

class CRCoin : public CGameObject {
public:
	float originalY, maxY, maxvy;
	float ay;				// acceleration on y

	CRCoin(float x, float y) : CGameObject(x, y) {
		originalY = y;
		maxY = y - COIN_MAX_Y;
		maxvy = 0;
		ay = 0;
	}

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	void Start();
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

//*********************************************************************

class CRMushroom : public CGameObject {
public:
	bool collidable;
	float originalY, maxY, maxvy;
	float ax, ay;				// acceleration on y

	CRMushroom(float x, float y) : CGameObject(x, y) {
		originalY = y;
		maxY = y - MUSHROOM_MAX_Y;
		maxvy = 0;
		ax = 0;
		ay = 0;
		collidable = false;
	}

	virtual int IsCollidable() { return collidable; };
	virtual int IsBlocking() { return 0; }
	void Start();
	void Render();
	void Died();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};
