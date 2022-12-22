#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"

#define ID_ANI_PLANT_TUBE 8000
#define PLANT_TUBE_BBOX_WIDTH 16
#define PLANT_TUBE_BBOX_HEIGHT 16

#define ID_ANI_PLANT 8100

#define PLANT_MYTH_BBOX_WIDTH 16
#define PLANT_MYTH_BBOX_HEIGHT 24

#define PLANT_UP_SPEED_Y	0.02f
#define PLANT_MAX_Y 26.0f

#define PLANT_ATTACK_TIME	2500
#define PLANT_ATTACK_COOLDOWN	2500

#define ID_ANI_PLANT_BULLET 8200
#define PLANT_BULLET_SPEED 0.1f
#define PLANT_BULLET_BBOX_WIDTH 9
#define PLANT_BULLET_BBOX_HEIGHT 9

class CPlantPipe : public CGameObject {
public:
	CPlantPipe(float x, float y) : CGameObject(x, y) {}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

class CPlantBullet : public CGameObject {
public:
	CMario* mario;
	float oriX, oriY;
	CPlantBullet(float x, float y) : CGameObject(x, y) {
		mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		vx = 0;
		vy = 0;
		oriX = x; oriY = y;
	}
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	void Render();
	void Shoot();
	void Reset(float px, float py);
	void Reset2();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

class CPlant : public CGameObject {
public:
	ULONGLONG lastattack, attacking;
	int collidable, isActive;
	float originalY, maxY, maxX;
	CMario* mario;
	CPlantBullet* plantbullet;

	CPlant(float x, float y, CPlantBullet* pb) : CGameObject(x, y) {
		mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		lastattack = 0; attacking = 0; isActive = 0;
		collidable = 0;
		originalY = y;
		vx = 0;
		vy = 0;
		maxY = y - PLANT_MAX_Y;
		plantbullet = pb;
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


