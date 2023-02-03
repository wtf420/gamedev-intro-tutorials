#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"
#include "Debug.h"

#define ID_ANI_PLANT 8100
#define ID_ANI_PLANT_LEFT_DOWN (ID_ANI_PLANT + 0)
#define ID_ANI_PLANT_LEFT_UP (ID_ANI_PLANT + 1)
#define ID_ANI_PLANT_RIGHT_DOWN (ID_ANI_PLANT + 2)
#define ID_ANI_PLANT_RIGHT_UP (ID_ANI_PLANT + 3)

#define ID_ANI_PLANT_2 8300

#define PLANT_MYTH_BBOX_WIDTH 16
#define PLANT_MYTH_BBOX_HEIGHT 24

#define PLANT_UP_SPEED_Y	0.02f
#define PLANT_MAX_Y 26.0f

#define PLANT_ATTACK_TIME	2500
#define PLANT_ATTACK_COOLDOWN	2500

#define ID_ANI_PLANT_BULLET 8200
#define PLANT_BULLET_SPEED 0.05f
#define PLANT_BULLET_BBOX_WIDTH 8
#define PLANT_BULLET_BBOX_HEIGHT 8

class CPlantBullet : public CGameObject {
public:
	CMario* mario;
	CPlantBullet(float x, float y) : CGameObject(x, y) {
		mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float mx, my;
		mario->GetPosition(mx, my);

		float dis = sqrt(abs(mx - this->x) * abs(mx - this->x) + abs(my - this->y) * abs(my - this->y));
		float t = dis / PLANT_BULLET_SPEED;
		vx = (mx - this->x) / t;
		vy = (my - this->y) / t;
		DebugOut(L"vx: %f, vy: %f\n", dis, dis);
	}
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	void Render();
	bool isInsideCamera();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

class CPlant : public CGameObject {
public:
	ULONGLONG lastattack, attacking;
	int collidable, isActive;
	float originalY, maxY, maxX;
	CMario* mario;



	CPlant(float x, float y) : CGameObject(x, y) {
		mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		lastattack = 0; attacking = 0; isActive = 0;
		collidable = 0;
		originalY = y;
		vx = 0;
		vy = 0;
		maxY = y - PLANT_MAX_Y;
		Attack();
	}

	virtual int IsCollidable() { return collidable; };
	virtual int IsBlocking() { return 0; }
	int GetAniId();
	void Attack();
	void Die();
	bool CanAttack();
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class CPlant2 : public CGameObject {
public:
	ULONGLONG lastattack, attacking;
	int collidable, isActive;
	float originalY, maxY, maxX;
	CMario* mario;



	CPlant2(float x, float y) : CGameObject(x, y) {
		mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		lastattack = 0; attacking = 0; isActive = 0;
		collidable = 0;
		originalY = y;
		vx = 0;
		vy = 0;
		maxY = y - PLANT_MAX_Y;
		Attack();
	}

	virtual int IsCollidable() { return collidable; };
	virtual int IsBlocking() { return 0; }
	void Attack();
	void Die();
	bool CanAttack();
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};


