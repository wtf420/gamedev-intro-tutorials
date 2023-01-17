#pragma once
#include "GameObject.h"
#include "Mario.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f

#define GOOMBA_JUMP_SPEED_Y		0.3f
#define GOOMBA_LEAP_SPEED_Y		0.4f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500
#define GOOMBA_JUMp_COOLDOWN 2000

#define GOOMBA_STATE_SUPER_WALKING 50
#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_FLYING 300

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_SUPERWALK 5100
#define ID_ANI_GOOMBA_FLY 5200

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	CMario *mario;
	ULONGLONG die_start;
	CGameObject* currentPlatform;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y);
	virtual void SetState(int state);
};

class CSuperGoomba : public CGameObject
{
protected:
	float ax;
	float ay;
	int jumpCount;

	CMario* mario;
	ULONGLONG die_start, jump_start;
	CGameObject* currentPlatform;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual bool canJump();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CSuperGoomba(float x, float y);
	virtual void SetState(int state);
};