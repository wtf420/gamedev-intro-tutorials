#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_REWARD_COIN 62000

#define COIN_MYTH_BBOX_WIDTH 16
#define COIN_MYTH_BBOX_HEIGHT 16


#define COIN_UP_SPEED_Y	0.2f
#define COIN_GRAVITY 0.2f
#define COIN_MAX_Y 48.0f

#define ID_ANI_MUSHROOM 63000

#define MUSHROOM_MYTH_BBOX_WIDTH 16
#define MUSHROOM_MYTH_BBOX_HEIGHT 16

#define MUSHROOM_UP_SPEED_Y		0.05f
#define MUSHROOM_ACCEL_SPEED_Y	0.005f
#define MUSHROOM_MAX_Y 16.0f

#define ID_ANI_LEAF 64000

#define LEAF_MYTH_BBOX_WIDTH 16
#define LEAF_MYTH_BBOX_HEIGHT 14

#define LEAF_UP_SPEED_Y	0.1f
#define LEAF_GRAVITY 0.02f
#define LEAF_MAX_Y 48.0f
#define LEAF_SPEED_X 0.05f
#define LEAF_MAX_X 36.0f

#define ID_ANI_POWERBLOCK 68000
#define ID_POWERBLOCK_DIED 62004

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

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	void Start();
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

//*********************************************************************

class CRMushroom : public CGameObject {
public:
	int collidable;
	float originalY, maxY, maxvy;
	float ax, ay;				// acceleration on y
	CGameObject* currentPlatform;

	CRMushroom(float x, float y) : CGameObject(x, y) {
		originalY = y;
		maxY = y - MUSHROOM_MAX_Y;
		maxvy = 0;
		ax = 0;
		ay = 0;
		collidable = 0;
		currentPlatform = NULL;
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

//*********************************************************************

class CRLeaf : public CGameObject {
public:
	int collidable;
	float originalX, originalY, maxY, maxX;

	CRLeaf(float x, float y) : CGameObject(x, y) {
		collidable = 0;
		originalY = y;
		originalX = x;
		vx = 0;
		vy = 0;
		maxY = y - LEAF_MAX_Y;
		maxX = x + LEAF_MAX_X;
	}

	virtual int IsCollidable() { return collidable; };
	virtual int IsBlocking() { return 0; }
	void Start();
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

//************************************
class PowerBlock : public CGameObject {
public:
	int collidable;

	PowerBlock(float x, float y) : CGameObject(x, y) {
		collidable = 1;
	}

	virtual int IsBlocking() { return collidable; }
	virtual int IsCollidable() { return collidable; };
	void Start()
	{
		y = y + 8 - 3;
		collidable = 0;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};
