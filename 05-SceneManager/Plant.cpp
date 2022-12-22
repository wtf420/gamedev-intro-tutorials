#include "Plant.h"
#include "PlayScene.h"
#include "debug.h"
#include <string>

void CPlantPipe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Render();
	CCollision::GetInstance()->Process(this, dt, coObjects);

	std::string stddrivestring = "to_string(a) + "; " + to_string(absy())";

	std::wstring widedrivestring = std::wstring(stddrivestring.begin(), stddrivestring.end());

	const wchar_t* TargetDrive = widedrivestring.c_str();
	DebugOutTitle(TargetDrive);
}

void CPlantPipe::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_PLANT_TUBE)->Render(x, y);
	//RenderBoundingBox();
}

void CPlantPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PLANT_TUBE_BBOX_WIDTH / 2;
	t = y - PLANT_TUBE_BBOX_HEIGHT / 2;
	r = l + PLANT_TUBE_BBOX_WIDTH;
	b = t + PLANT_TUBE_BBOX_HEIGHT;
}

/********************************************************************************************/

void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (CanAttack())
		Attack();

	if (isActive)
	{
		//reached max
		if (y <= maxY && vy < 0)
		{
			y = maxY;
			attacking = GetTickCount64();
			vy = 0;
			plantbullet->Reset(this->x, this->maxY);
		} // attack
		else if (GetTickCount64() - attacking > PLANT_ATTACK_TIME && vy == 0)
		{
			lastattack = GetTickCount64();
			vy = PLANT_UP_SPEED_Y;
			plantbullet->Shoot();
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
	plantbullet->Delete();
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
	animations->Get(ID_ANI_PLANT)->Render(x, y);
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
	std::string stddrivestring = to_string(x) + "; " + to_string(y);

	std::wstring widedrivestring = std::wstring(stddrivestring.begin(), stddrivestring.end());

	const wchar_t* TargetDrive = widedrivestring.c_str();
	DebugOutTitle(TargetDrive);
	Render();
	x += vx * dt;
	y += vy * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPlantBullet::Shoot()
{
	float mx, my;
	mario->GetPosition(mx, my);

	float dis = sqrt(abs(mx - this->x) * abs(mx - this->x) + abs(mx - this->y) * abs(mx - this->y));
	vx = (mx - x) / ( dis / PLANT_BULLET_SPEED);
	vy = (my - y) / (dis / PLANT_BULLET_SPEED);
}

void CPlantBullet::Reset(float px, float py)
{
	vx = 0.0f;
	vy = 0.0f;
	x = px;
	y = py;
}

void CPlantBullet::Reset2()
{
	x = oriX;
	y = oriY;
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