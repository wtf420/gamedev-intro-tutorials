#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "mario.h"
#include "debug.h"

#define ID_ANI_LUIGI_IDLE_RIGHT 99100
#define ID_ANI_LUIGI_IDLE_LEFT 99101

#define ID_ANI_LUIGI_IDLE_HOLD_RIGHT 99110
#define ID_ANI_LUIGI_IDLE_HOLD_LEFT 99111

#define ID_ANI_LUIGI_WALKING_RIGHT 99200
#define ID_ANI_LUIGI_WALKING_LEFT 99201

#define ID_ANI_LUIGI_WALKRUN_HOLD_RIGHT 99420
#define ID_ANI_LUIGI_WALKRUN_HOLD_LEFT 99421

#define ID_ANI_LUIGI_JUMP_RIGHT 99500
#define ID_ANI_LUIGI_JUMP_LEFT 99501

#define ID_ANI_LUIGI_FALL_RIGHT 99600
#define ID_ANI_LUIGI_FALL_LEFT 99601

class CTSMario : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;

	int isSitting, level;
	int acceptKeyboardInput = 0;
	int currentani = -1;
public:
	CTSMario(float x, float y) : CGameObject(x, y) {
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsColliable() { return 0; }

	void StartPBA(int pba, int step);
	void GetPBA(int step);
};

class CTSLuigi : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;

	int isSitting, level;
	int acceptKeyboardInput = 0;
	int currentani = -1;
public:
	CTSLuigi(float x, float y) : CGameObject(x, y) {
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsColliable() { return 0; }

	void StartPBA(int pba, int step);
	void GetPBA(int step);
};

class CTSLeaf : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG timer2 = 0;
	ULONGLONG PbaTimer, timetillnextPba;

	int currentani = -1;
public:
	CTSLeaf(float x, float y) : CGameObject(x, y) {
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsColliable() { return 0; }

	void StartPBA(int pba, int step);
	void GetPBA(int step);
};

class CTSMushroom : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;
public:
	CTSMushroom(float x, float y) : CGameObject(x, y) {
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsColliable() { return 0; }

	void StartPBA(int pba, int step);
	void GetPBA(int step);
};

class CTSGoomba : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;

	int currentani = -1;
public:
	CTSGoomba(float x, float y) : CGameObject(x, y) {
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsColliable() { return 0; }

	void StartPBA(int pba, int step);
	void GetPBA(int step);
};

class CTSKoopas : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	float speed;
	ULONGLONG PbaTimer, timetillnextPba;

	int currentani = -1;
public:
	CTSKoopas(float x, float y, float s) : CGameObject(x, y) {
		speed = s;
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsColliable() { return 0; }

	void StartPBA(int pba, int step);
	void GetPBA(int step);
};

class CTSTurtleShell : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;

	int currentani = -1;
public:
	CTSTurtleShell(float x, float y) : CGameObject(x, y) {
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsColliable() { return 0; }

	void StartPBA(int pba, int step);
	void GetPBA(int step);
};

class CTSBackground : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;

	int ani = -1;
public:
	CTSBackground(float x, float y, int k) : CGameObject(x, y) {
		ani = k;
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsColliable() { return 0; }

	void StartPBA(int pba, int step);
	void GetPBA(int step);
};

