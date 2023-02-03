#include "Plant.h"
#include "PlayScene.h"
#include "debug.h"
#include <string>
#include "CBackground.h"

/********************************************************************************************/

void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (CanAttack())
		Attack();

	//First version ever of the PBA system, not fixing it because its working perfectly
	if (isActive)
	{
		//reached max
		if (y <= maxY && vy < 0)
		{
			y = maxY;
			attacking = GetTickCount64();
			vy = 0;
		} // attack
		else if (GetTickCount64() - attacking > PLANT_ATTACK_TIME && vy == 0)
		{
			lastattack = GetTickCount64();
			vy = PLANT_UP_SPEED_Y;
			CPlantBullet* bullet = new CPlantBullet(this->x, this->y);
			((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(bullet);
		}
		else //reached min
			if (y >= originalY && vy > 0)
			{
				isActive = 0;
				lastattack = GetTickCount64();
				collidable = 0;
				y = originalY;
				vy = 0;
			}
	}

	y += vy * dt;
}

void CPlant::Attack()
{
	isActive = 1;
	vy = -PLANT_UP_SPEED_Y;
	collidable = 1;
}
void CPlant::Die()
{
	CBackgroundFX* fx = new CBackgroundFX(this->x, this->y, ID_ANI_POP_SMOKE_FX, 1000);
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(fx);
	isDeleted = true;
}

bool CPlant::CanAttack()
{
	if (isActive)
		return false;
	float mx, my;
	mario->GetPosition(mx, my);
	if (abs(mx - this->x) <= 30 || abs(mx - this->x) >= 150)
		return false;
	if (GetTickCount64() - lastattack < PLANT_ATTACK_COOLDOWN)
		return false;
	return true;
}

void CPlant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(GetAniId())->Render(x, y);
}

int CPlant::GetAniId()
{
	float mx, my;
	mario->GetPosition(mx, my);

	if (mx < this->x)
	{
		if (my > this->y)
			return ID_ANI_PLANT_LEFT_DOWN; else
			return ID_ANI_PLANT_LEFT_UP;
	}
	else
	{
		if (my > this->y)
			return ID_ANI_PLANT_RIGHT_DOWN; else
			return ID_ANI_PLANT_RIGHT_UP;
	}
}

void CPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PLANT_MYTH_BBOX_WIDTH / 2;
	t = y - PLANT_MYTH_BBOX_HEIGHT / 2;
	r = l + PLANT_MYTH_BBOX_WIDTH;
	b = t + PLANT_MYTH_BBOX_HEIGHT;
}

//**************************************************************************************

void CPlantBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isInsideCamera()) Delete(); 
	else
	{
		Render();
		float timeScale;
		CGame::GetInstance()->GetTimeScale(timeScale);
		if (timeScale == 0.0f)
		{
			if (!timeStopped)
			{
				timeStopped = true;
				lastvx = vx;
				lastvy = vy;
			}
			vx = 0;
			vy = 0;
		}
		else
		{
			if (timeStopped)
			{
				vx = lastvx;
				vy = lastvy;
				timeStopped = false;
			}
		}
		x += vx * dt;
		y += vy * dt;
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

bool CPlantBullet::isInsideCamera()
{
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float w = CGame::GetInstance()->GetBackBufferWidth() / 2.0f;
	float h = CGame::GetInstance()->GetBackBufferHeight() / 2.0f;
	cx += w;
	cy += h;
	//Once the bullet is out of camera it will stop updating position, so we plan ahead
	if ((l + 1.0f > cx + w) || (t + 1.0f > cy + w) || (r - 1.0f < cx - w) || (b - 1.0f < cy - w))
		return false;
	return true;
}

void CPlantBullet::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_PLANT_BULLET)->Render(x, y);
	//RenderBoundingBox();
}

void CPlantBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PLANT_BULLET_BBOX_WIDTH / 2;
	t = y - PLANT_BULLET_BBOX_HEIGHT / 2;
	r = l + PLANT_BULLET_BBOX_WIDTH;
	b = t + PLANT_BULLET_BBOX_HEIGHT;
}

/********************************************************************************************/

void CPlant2::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (CanAttack())
		Attack();

	//First version ever of the PBA system, not fixing it because its working perfectly
	if (isActive)
	{
		//reached max
		if (y <= maxY && vy < 0)
		{
			y = maxY;
			attacking = GetTickCount64();
			vy = 0;
		} // attack
		else if (GetTickCount64() - attacking > PLANT_ATTACK_TIME && vy == 0)
		{
			lastattack = GetTickCount64();
			vy = PLANT_UP_SPEED_Y;
		}
		else //reached min
			if (y >= originalY && vy > 0)
			{
				isActive = 0;
				lastattack = GetTickCount64();
				collidable = 0;
				y = originalY;
				vy = 0;
			}
	}

	y += vy * dt;
}

void CPlant2::Attack()
{
	isActive = 1;
	vy = -PLANT_UP_SPEED_Y;
	collidable = 1;
}
void CPlant2::Die()
{
	CBackgroundFX* fx = new CBackgroundFX(this->x, this->y, ID_ANI_POP_SMOKE_FX, 1000);
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->Add(fx);
	isDeleted = true;
}

bool CPlant2::CanAttack()
{
	if (isActive)
		return false;
	float mx, my;
	mario->GetPosition(mx, my);
	if (abs(mx - this->x) <= 30 || abs(mx - this->x) >= 150)
		return false;
	if (GetTickCount64() - lastattack < PLANT_ATTACK_COOLDOWN)
		return false;
	return true;
}

void CPlant2::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_PLANT_2)->Render(x, y);
}

void CPlant2::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PLANT_MYTH_BBOX_WIDTH / 2;
	t = y - PLANT_MYTH_BBOX_HEIGHT / 2;
	r = l + PLANT_MYTH_BBOX_WIDTH;
	b = t + PLANT_MYTH_BBOX_HEIGHT;
}