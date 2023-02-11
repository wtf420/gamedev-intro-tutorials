#include "Koopas.h"
#include "PlayScene.h"
#include "Platform.h"
#include "Myth.h"

CKoopas::CKoopas(float x, float y) :CGameObject(x, y)
{
	mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = -1;
	lastvx = 0.0f;
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_MOVING_SHELL)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
}

void CKoopas::OnNoCollision(DWORD dt)
{
	currentPlatform = NULL;
	isOnPlatform = 0;

	if (this->state != KOOPAS_STATE_CARRIED)
	{
		x += vx * dt;
		y += vy * dt;
	}
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->ny <= 0) {
		currentPlatform = e->obj;
		isOnPlatform = 1;
	}
	if (dynamic_cast<CKoopas*>(e->obj)) return;
	if (dynamic_cast<CMyth*>(e->obj) && this->state == KOOPAS_STATE_MOVING_SHELL)
		dynamic_cast<CMyth*>(e->obj)->Interact();
	if (dynamic_cast<CBrickWithCoin*>(e->obj))
	{
		if (this->state == KOOPAS_STATE_MOVING_SHELL)
		e->obj->Delete();
	}

	if (e->ny != 0)
	{
		if (whatever == 1 && state == KOOPAS_STATE_SHELL)
		{
			whatever = 0;
			vx = 0;
		}
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float timeScale;
	CGame::GetInstance()->GetTimeScale(timeScale);

	if (this->y > 200 && this->y < 230)
	{
		this->Delete();
	}

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
		vy += ay * dt;
		vx += ax * dt;
	}
	if ((state == KOOPAS_STATE_SHELL) && (GetTickCount64() - die_start > KOOPAS_RESPAWN_TIMEOUT))
	{
		SetState(KOOPAS_STATE_WALKING);
		//isDeleted = true;
		//return;
	}

	vector<LPGAMEOBJECT> coObjects2;
	for (int k = 0; k < coObjects->size(); k++)
	{
		CPlatformOneWay* u = dynamic_cast<CPlatformOneWay*>(coObjects->at(k));
		float top = 0;
		if (u) top = u->GetTopPosition();
		if ((!u) || (u == currentPlatform) || (u && top >= y))
		{
			coObjects2.push_back(coObjects->at(k));
		}
	}
	CGameObject::Update(dt, &coObjects2);
	CCollision::GetInstance()->Process(this, dt, &coObjects2);

	if (isOnPlatform == 1)
	{
		if (state == KOOPAS_STATE_WALKING && currentPlatform != NULL)
		{
			float l, t, r, b;
			currentPlatform->GetBoundingBox(l, t, r, b);
			if (this->x - KOOPAS_BBOX_WIDTH / 4 < l || this->x + KOOPAS_BBOX_WIDTH / 4 > r)
				vx = -vx;
		}
	}
}


void CKoopas::Render()
{
	int aniId = 0;
	if (vx < 0) aniId = ID_ANI_KOOPAS_WALKING_LEFT; else
		aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_MOVING_SHELL || state == KOOPAS_STATE_CARRIED)
	{
		aniId = ID_ANI_KOOPAS_SHELL;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopas::Attacked()
{
	this->SetState(KOOPAS_STATE_SHELL);
	whatever = 1;
	float mx, my;
	mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetPosition(mx, my);

	if (mx > x)
		vx = -KOOPAS_WALKING_SPEED; else
		vx = KOOPAS_WALKING_SPEED;
	vy = -0.4f;
	ay = KOOPAS_GRAVITY;
}

void CKoopas::SetState(int state)
{
	switch (state)
	{
	case KOOPAS_STATE_SHELL:
		die_start = GetTickCount64();
		lastvx = vx;
		if (this->state == KOOPAS_STATE_WALKING)
		{
			y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2 - 1;
		}
		vx = 0;
		vy = 0;
		ax = 0;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_WALKING:
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2 + 1.0f;
		vx = 0;
		vx = -KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_GRAVITY;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_MOVING_SHELL:
		y -= 1.0f;
		float mx, my;
		mario->GetPosition(mx, my);
		if (mx < this->x) vx = KOOPAS_SHELL_SPEED;
		else vx = -KOOPAS_SHELL_SPEED;
		vy = -KOOPAS_GRAVITY;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_CARRIED:
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPAS_STATE_DIE:
		isDeleted = true;
		break;
	}
	CGameObject::SetState(state);
}

/*
*************************************** SUPER KOOPAS **************************************************
*/

CSuperKoopas::CSuperKoopas(float x, float y) :CGameObject(x, y)
{
	mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = -1;
	lastvx = 0.0f;
	SetState(KOOPAS_STATE_FLYING);
}

void CSuperKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_MOVING_SHELL)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
}

void CSuperKoopas::OnNoCollision(DWORD dt)
{
	currentPlatform = NULL;

	if (this->state != KOOPAS_STATE_CARRIED)
	{
		x += vx * dt;
		y += vy * dt;
	}
};

void CSuperKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->ny <= 0) currentPlatform = e->obj;
	if (dynamic_cast<CSuperKoopas*>(e->obj)) return;
	if (dynamic_cast<CMyth*>(e->obj) && this->state == KOOPAS_STATE_MOVING_SHELL)
		dynamic_cast<CMyth*>(e->obj)->Interact();

	if (e->ny != 0)
	{
		if (whatever == 1 && state == KOOPAS_STATE_SHELL)
		{
			whatever = 0;
			vx = 0;
		}
		if (this->state == KOOPAS_STATE_FLYING)
			vy = -KOOPAS_JUMP_SPEED_Y;
		else
			vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CSuperKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->state == KOOPAS_STATE_CARRIED)
	{
		float mx, my, mvx, mvy;
		mario->GetPosition(mx, my);
		mario->GetSpeed(mvx, mvy);
		if (mvx >= 0)
		{
			x = mx + KOOPAS_BBOX_WIDTH;
			y = my;
		}
		else
		{
			x = mx - KOOPAS_BBOX_WIDTH;
			y = my;
		}
		if (!mario->isHolding)
			this->SetState(KOOPAS_STATE_MOVING_SHELL);
	}
	else
	{
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
			vy += ay * dt;
			vx += ax * dt;
		}

		if (state == KOOPAS_STATE_WALKING && currentPlatform)
		{
			if (dynamic_cast<CPlatformOneWay*>(currentPlatform))
			{
				CPlatformOneWay* u = dynamic_cast<CPlatformOneWay*>(currentPlatform);
				float left = u->GetLeftPosition();
				if (x < left || x > left + u->length)
				{
					vx = -vx;
				}
			} else if (dynamic_cast<CPlatform3*>(currentPlatform))
			{
				CPlatform3* u = dynamic_cast<CPlatform3*>(currentPlatform);
				float left = u->GetLeftPosition();
				if (x < left || x > left + u->length)
				{
					vx = -vx;
				}
			}
			else if (dynamic_cast<CPlatform2*>(currentPlatform))
			{
				CPlatform2* u = dynamic_cast<CPlatform2*>(currentPlatform);
				float ux, uy;
				u->GetPosition(ux, uy);
				if (x < ux || x > ux + u->length)
					vx = -vx;
			}
			else if (dynamic_cast<CPlatform*>(currentPlatform))
			{
				CPlatform* u = dynamic_cast<CPlatform*>(currentPlatform);
				float ux, uy, l;
				l = u->length * 16.0f;
				u->GetPosition(ux, uy);
				if (x < ux - l / 2 || x > ux + l / 2)
					vx = -vx;
			}
		}

		if ((state == KOOPAS_STATE_SHELL) && (GetTickCount64() - die_start > KOOPAS_RESPAWN_TIMEOUT))
		{
			SetState(KOOPAS_STATE_WALKING);
			//isDeleted = true;
			//return;
		}

		vector<LPGAMEOBJECT> coObjects2;
		for (int k = 0; k < coObjects->size(); k++)
		{
			CPlatformOneWay* u = dynamic_cast<CPlatformOneWay*>(coObjects->at(k));
			float top = 0;
			if (u) top = u->GetTopPosition();
			if ((!u) || (u == currentPlatform) || (u && top >= y))
			{
				coObjects2.push_back(coObjects->at(k));
			}
		}
		CGameObject::Update(dt, &coObjects2);
		CCollision::GetInstance()->Process(this, dt, &coObjects2);
	}
}


void CSuperKoopas::Render()
{
	int aniId = ID_ANI_KOOPAS_FLYING;
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_MOVING_SHELL || state == KOOPAS_STATE_CARRIED)
	{
		aniId = ID_ANI_KOOPAS_SHELL;
	}
	else if (state == KOOPAS_STATE_WALKING)
	{
		if (vx < 0) aniId = ID_ANI_KOOPAS_WALKING_LEFT; else
			aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CSuperKoopas::Attacked()
{
	this->SetState(KOOPAS_STATE_SHELL);
	whatever = 1;
	float mx, my;
	mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->GetPosition(mx, my);

	if (mx > x)
		vx = -KOOPAS_WALKING_SPEED; else
		vx = KOOPAS_WALKING_SPEED;
	vy = -0.5f;
	ay = KOOPAS_GRAVITY;
}

void CSuperKoopas::SetState(int state)
{
	switch (state)
	{
	case KOOPAS_STATE_SHELL:
		die_start = GetTickCount64();
		lastvx = vx;
		if (this->state == KOOPAS_STATE_WALKING)
		{
			y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		}
		y -= 1;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPAS_STATE_WALKING:
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2 + 1.0f;
		vx = 0;
		vx = -KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_GRAVITY;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_FLYING:
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2 + 1.0f;
		vx = 0;
		vx = -KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_GRAVITY;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_MOVING_SHELL:
		y -= 1.0f;
		float mx, my;
		mario->GetPosition(mx, my);
		if (mx < this->x) vx = KOOPAS_SHELL_SPEED;
		else vx = -KOOPAS_SHELL_SPEED;
		vy = -KOOPAS_GRAVITY;
		ay = KOOPAS_GRAVITY;
		break;
	case KOOPAS_STATE_DIE:
		isDeleted = true;
		break;
	}
	CGameObject::SetState(state);
}
