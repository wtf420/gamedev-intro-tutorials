#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "PlayScene.h"

#define ID_TEX_BACKGROUND -10
#define HUD_WIDTH 152
#define HUD_BBOX_WIDTH 152
#define HUD_BBOX_HEIGHT 28

#define ID_SPRITE_SCORE	110000
#define ID_SPRITE_SCORE_100	(ID_SPRITE_SCORE + 100)
#define ID_SPRITE_SCORE_200	(ID_SPRITE_SCORE + 200)
#define ID_SPRITE_SCORE_400	(ID_SPRITE_SCORE + 300)
#define ID_SPRITE_SCORE_800	(ID_SPRITE_SCORE + 400)
#define ID_SPRITE_SCORE_1000	(ID_SPRITE_SCORE + 1000)
#define ID_SPRITE_SCORE_2000	(ID_SPRITE_SCORE + 2000)
#define ID_SPRITE_SCORE_4000	(ID_SPRITE_SCORE + 4000)
#define ID_SPRITE_SCORE_8000	(ID_SPRITE_SCORE + 8000)
#define ID_SPRITE_SCORE_1UP	(ID_SPRITE_SCORE - 1)

class CHud : public CGameObject {
	LPGAMEOBJECT mario;
public:
	float worldId, timer, coinCount, livesCount, score;
	CHud(float x, float y) : CGameObject(x, y) {
		mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		worldId = 1;
		timer = 300;
		coinCount = 0;
		score = 0;
		livesCount = 3;
	}
	void Render();
	void Update(DWORD dt) {
		if (dynamic_cast<CMario*>(mario))
		{
			float timescale;
			CGame::GetInstance()->GetTimeScale(timescale);
			timer -= dt / 1000.0f * timescale;
			if (timer <= 0.0f && mario->GetState() != MARIO_STATE_DIE)
			{
				mario->SetState(MARIO_STATE_DIE);
			}
		}
		else
			timer = 0;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int GetNumberSpriteID(int i);
	void SetCoinCount(int i);
	void SetScore(int i);
	void SetLives(int i);
	void RenderNumber(float a, float b, int i, int maxdigit);
	void RenderPowerMeter(float a, float b, int p);
};

class ScoreFX : public CGameObject {
	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;

	int amount;
public:
	ScoreFX(float x, float y, int number) : CGameObject(x, y) {
		amount = number;
		StartPBA(1, 1);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	void StartPBA(int pba, int step);
	void GetPopupPBA(int step);

	int GetAniId();
};
