#pragma once
#include "GameObject.h"
#include "Mario.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SHELL_SPEED 0.2f


#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 28
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_DIE_TIMEOUT 500
#define KOOPAS_RESPAWN_TIMEOUT 3000

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_SHELL 300
#define KOOPAS_STATE_MOVING_SHELL 350
#define KOOPAS_STATE_CARRIED 400

#define ID_ANI_KOOPAS_WALKING 7100
#define ID_ANI_KOOPAS_SHELL 7200

class CKoopas : public CGameObject
{
protected:
	float ax;
	float ay;
	float lastvx;

	CMario* mario;
	ULONGLONG die_start;
	CGameObject* currentPlatform;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopas(float x, float y);
	virtual void SetState(int state);
};