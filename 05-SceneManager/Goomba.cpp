#include "Goomba.h"
#include "Koopas.h"
#include "Platform.h"
#include "Playscene.h"
#include "debug.h"
#include <string>

CGoomba::CGoomba(float x, float y):CGameObject(x, y)
{
	mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	SetState(0);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	currentPlatform = NULL;

	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CKoopas*>(e->obj))
	{
		if (dynamic_cast<CKoopas*>(e->obj)->GetState() == KOOPAS_STATE_MOVING_SHELL)
			this->SetState(GOOMBA_STATE_DIE);
		return;
	}

	if (!e->obj->IsBlocking()) return; 
	if (e->ny <= 0) currentPlatform = e->obj;
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (this->state == 0)
	{
		float mx, my;
		mario->GetPosition(mx, my);
		if (abs(mx - this->x) <= 300)
		{
			this->ax = 0;
			this->ay = GOOMBA_GRAVITY;
			die_start = -1;
			SetState(GOOMBA_STATE_WALKING);
		}

		return;
	}

	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}

	vector<LPGAMEOBJECT> coObjects2;
	for (int k = 0; k < coObjects->size(); k++)
	{
		CPlatformOneWay* u = dynamic_cast<CPlatformOneWay*>(coObjects->at(k));
		float ux, uy = 0.0f;
		if (u) u->GetPosition(ux, uy);
		if (!u || u == currentPlatform || (u && uy >= y))
		{
			coObjects2.push_back(coObjects->at(k));
		}
	}
	CGameObject::Update(dt, &coObjects2);
	CCollision::GetInstance()->Process(this, dt, &coObjects2);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
	}
}

//****************************************************************************************************************

CSuperGoomba::CSuperGoomba(float x, float y) :CGameObject(x, y)
{
	mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	SetState(0);
	jump_start = -1;
	jumpCount = 0;
}

void CSuperGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CSuperGoomba::OnNoCollision(DWORD dt)
{
	currentPlatform = NULL;

	x += vx * dt;
	y += vy * dt;
};

void CSuperGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CKoopas*>(e->obj))
	{
		if (dynamic_cast<CKoopas*>(e->obj)->GetState() == KOOPAS_STATE_MOVING_SHELL)
			this->SetState(GOOMBA_STATE_DIE);
		return;
	}

	if (!e->obj->IsBlocking()) return;
	if (e->ny <= 0) currentPlatform = e->obj;
	if (dynamic_cast<CSuperGoomba*>(e->obj)) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		if (state == GOOMBA_STATE_FLYING)
			state = GOOMBA_STATE_WALKING;
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

bool CSuperGoomba::canJump()
{
	if (GetTickCount64() - jump_start > GOOMBA_JUMp_COOLDOWN)
	{
		return true;
	}
	return false;
}

void CSuperGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	std::string stddrivestring = to_string(canJump());

	std::wstring widedrivestring = std::wstring(stddrivestring.begin(), stddrivestring.end());

	const wchar_t* TargetDrive = widedrivestring.c_str();
	DebugOutTitle(TargetDrive);

	if (this->state == 0)
	{
		float mx, my;
		mario->GetPosition(mx, my);
		if (abs(mx - this->x) <= 300)
		{
			this->ax = 0;
			this->ay = GOOMBA_GRAVITY;
			die_start = -1;
			SetState(GOOMBA_STATE_WALKING);
		}
		return;
	}

	if (state == GOOMBA_STATE_WALKING && canJump())
	{
		if (jumpCount < 2)
		{
			jump_start = GetTickCount64();
			jumpCount++;
			vy = -GOOMBA_JUMP_SPEED_Y;
		} else 
		{
			jump_start = GetTickCount64();
			jumpCount = 0;
			vy = -GOOMBA_LEAP_SPEED_Y;
			SetState(GOOMBA_STATE_FLYING);
		}
	}

	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	vector<LPGAMEOBJECT> coObjects2;
	for (int k = 0; k < coObjects->size(); k++)
	{
		CPlatformOneWay* u = dynamic_cast<CPlatformOneWay*>(coObjects->at(k));
		float ux, uy = 0.0f;
		if (u) u->GetPosition(ux, uy);
		if (!u || u == currentPlatform || (u && uy >= y))
		{
			coObjects2.push_back(coObjects->at(k));
		}
	}
	CGameObject::Update(dt, &coObjects2);
	CCollision::GetInstance()->Process(this, dt, &coObjects2);

}


void CSuperGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_SUPERWALK;
	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	} else
		if (state == GOOMBA_STATE_FLYING)
		{
			aniId = ID_ANI_GOOMBA_FLY;
		}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CSuperGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_FLYING:
		break;
	}
}

