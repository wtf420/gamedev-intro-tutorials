#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_POP_SMOKE_FX 990

#define BACKGROUND_WIDTH 0
#define BACKGROUND_BBOX_WIDTH 16
#define BACKGROUND_BBOX_HEIGHT 16

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

class CAnimatedBackground : public CGameObject {
	int AniID;
public:
	CAnimatedBackground(float x, float y, int k) : CGameObject(x, y) {
		AniID = k;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

class CBackgroundFX : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;

	int time;
	int AniID;
public:
	CBackgroundFX(float x, float y, int k, float time) : CGameObject(x, y) {
		AniID = k;
		this->time = time;
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	void StartPBA(int pba, int step);
	void GetPopupPBA(int step);
};

