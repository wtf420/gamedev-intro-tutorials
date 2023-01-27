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

class CHud : public CGameObject {
	CMario* mario;
public:
	float worldId, timer, coinCount, livesCount, score;
	CHud(float x, float y) : CGameObject(x, y) {
		mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		worldId = 1;
		timer = 420;
		coinCount = 69;
		score = 177013;
		livesCount = 3;
	}
	void Render();
	void Update(DWORD dt) {
		timer -= dt / 1000.0f;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int GetNumberSpriteID(int i);
	void SetCoinCount(int i);
	void RenderNumber(float a, float b, int i, int maxdigit);
	void RenderPowerMeter(float a, float b, int p);
};
