#include "hud.h"

void CHud::Render()
//This also works as update.
{
	float l = x - HUD_BBOX_WIDTH / 2.0f;
	float t = y - HUD_BBOX_HEIGHT / 2.0f;

	LPTEXTURE back = CTextures::GetInstance()->Get(ID_TEX_BACKGROUND);
	CGame::GetInstance()->Draw(
		CGame::GetInstance()->GetBackBufferWidth() / 2 + 1,
		CGame::GetInstance()->GetBackBufferHeight() - (HUD_BBOX_HEIGHT + 27.0f) / 2.0f,
		back, nullptr, 1.0f,
		CGame::GetInstance()->GetBackBufferWidth(), 
		HUD_BBOX_HEIGHT + 27);

	LPSPRITE hudsprite = CSprites::GetInstance()->Get(ID_SPRITE_HUD);
	hudsprite->Draw(x, y);

	RenderNumber(l + 38.0f, t + 8.0f, worldId, 1);
	RenderNumber(l + 30.0f, t + 16.0f, livesCount, 2);
	RenderNumber(l + 132.0f, t + 8.0f, coinCount, 2);
	RenderNumber(l + 52.0f, t + 16.0f, score, 7);
	RenderNumber(l + 125.0f, t + 16.0f, timer, 3);
	if (dynamic_cast<CMario*>(mario))
		RenderPowerMeter(l + 52.0f, t + 8.0f, dynamic_cast<CMario*>(mario)->power / MARIO_POWER_LIMIT * 7);
	else 
		RenderPowerMeter(l + 52.0f, t + 8.0f, 0);
}

void CHud::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - HUD_BBOX_WIDTH / 2;
	t = y - HUD_BBOX_HEIGHT / 2;
	r = l + HUD_BBOX_WIDTH;
	b = t + HUD_BBOX_HEIGHT;
}

void CHud::RenderNumber(float a, float b, int i, int maxdigit)
{
	if (i > pow(10, maxdigit) - 1) return;
	//set pos to where the center of the number is.
	a += 4.0f; b += 3.0f;

	vector<int> digits;
	if (i == 0)
	{
		digits.push_back(0);
	} else
	while (i != 0)
	{
		digits.push_back(i % 10);
		i = i / 10;
	}

	LPSPRITE numbersprite;
	for (int i = maxdigit - 1; i >= 0; i--)
	{
		if (i > (digits.size() - 1))
		{
			numbersprite = CSprites::GetInstance()->Get(GetNumberSpriteID(0));
			numbersprite->Draw(a, b);
			a += 8.0f;
		}
		else
		{
			numbersprite = CSprites::GetInstance()->Get(GetNumberSpriteID(digits[i]));
			numbersprite->Draw(a, b);
			a += 8.0f;
		}
	}
}

int CHud::GetNumberSpriteID(int i)
{
	switch (i) {
	case 1: 
		return ID_SPRITE_NUMBER_1;
		break;
	case 2:
		return ID_SPRITE_NUMBER_2;
		break;
	case 3:
		return ID_SPRITE_NUMBER_3;
		break;
	case 4:
		return ID_SPRITE_NUMBER_4;
		break;
	case 5:
		return ID_SPRITE_NUMBER_5;
		break;
	case 6:
		return ID_SPRITE_NUMBER_6;
		break;
	case 7:
		return ID_SPRITE_NUMBER_7;
		break;
	case 8:
		return ID_SPRITE_NUMBER_8;
		break;
	case 9:
		return ID_SPRITE_NUMBER_9;
		break;
	default:
		return ID_SPRITE_NUMBER_0;
		break;
	}
}

void CHud::SetCoinCount(int i)
{
	coinCount = i;
}

void CHud::SetScore(int i)
{
	score = i;
}
void CHud::SetLives(int i)
{
	livesCount = i;
}

void CHud::RenderPowerMeter(float a, float b, int p)
{
	a += 4.0f; b += 3.0f;
	LPSPRITE powerArrow;
	for (int i = 1; i < 7; i++)
	{
		if (i <= p)
		{
			powerArrow = CSprites::GetInstance()->Get(ID_SPRITE_POWER_METER_WHITE);
			powerArrow->Draw(a, b);
			a += 8.0f;
		}
		else
		{
			powerArrow = CSprites::GetInstance()->Get(ID_SPRITE_POWER_METER_BLACK);
			powerArrow->Draw(a, b);
			a += 8.0f;
		}
	}

	a += 5.0f;
	if (p == 7)
	{
		powerArrow = CSprites::GetInstance()->Get(ID_SPRITE_POWER_WHITE);
		powerArrow->Draw(a, b);
	}
	else
	{
		powerArrow = CSprites::GetInstance()->Get(ID_SPRITE_POWER_BLACK);
		powerArrow->Draw(a, b);
	}
}

//********************************************************************************
//
//********************************************************************************

void ScoreFX::Render()
{
	LPSPRITE sprite = CSprites::GetInstance()->Get(GetAniId());
	sprite->Draw(x, y);
	//RenderBoundingBox();
}

int ScoreFX::GetAniId()
{
	return ID_SPRITE_SCORE + amount;
}

void ScoreFX::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (currentPba != -1)
	{
		x += vx * dt;
		y += vy * dt;

		PbaTimer += dt;
		//condition to switch step / pba
		if (PbaTimer >= timetillnextPba)
		{
			PbaStep++;
			StartPBA(currentPba, PbaStep);
		}
	}
	else
	{
		//insert normal update here
	}
}

void ScoreFX::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetPopupPBA(step);
		break;
	default:
		break;
	}
}

void ScoreFX::GetPopupPBA(int step)
{
	switch (step)
	{
	case 1:
	{
		PbaStep = 1;
		PbaTimer = 0;

		timetillnextPba = 500;
		vx = 0;
		vy = -0.05f;
		break;
	}
	//end of PBA
	default:
		Delete();
		break;
	}
}

void ScoreFX::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - 18.0f / 2;
	t = y - 10.0f / 2;
	r = l + 18.0f;
	b = t + 10.0f;
}

