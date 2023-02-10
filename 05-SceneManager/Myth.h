#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#include "Rewards.h"
#include "Mario.h"
#include "Playscene.h"

#define ID_ANI_MYTH 61000
#define ID_ANI_MYTH_DIED 69000
#define MYTH_WIDTH 16
#define MYTH_BBOX_WIDTH 16
#define MYTH_BBOX_HEIGHT 16

#define TYPE_COIN 1
#define TYPE_MUSHROOM 2
#define TYPE_1UP 3

#define UP_SPEED_Y		0.1f
#define ACCEL_SPEED_Y	0.002f

class CMyth : public CGameObject {
public:
	int type;
	float originalY, maxY, maxvy;
	float ay;				// acceleration on y 
	bool touched;
	CMario* mario;

	CMyth(float x, float y, int t);
	void Interact();
	void Render();
	void Reward();
	void SpawnCoin();
	void SpawnMushroom();
	void Spawn1Up();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};