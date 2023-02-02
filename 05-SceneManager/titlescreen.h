#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "mario.h"
#include "debug.h"

#define ID_MARIO_STATE_LOOKING_UP 696969

class CTSMario : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;

	int isSitting, level;
	int acceptKeyboardInput = 0;
public:
	CTSMario(float x, float y) : CGameObject(x, y) {
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	void StartPBA(int pba, int step);
	void GetPBA(int step);

	int GetAniId();
};

