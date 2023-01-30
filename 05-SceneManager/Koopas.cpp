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

	if (this->state != KOOPAS_STATE_CARRIED)
	{
		x += vx * dt;
		y += vy * dt;
	}
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->ny <= 0) currentPlatform = e->obj;
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
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
				float ux, uy;
				u->GetPosition(ux, uy);
				if (x < ux || x > ux + u->length)
					vx = -vx;
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

}


void CKoopas::Render()
{
	int aniId = ID_ANI_KOOPAS_WALKING;
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_MOVING_SHELL || state == KOOPAS_STATE_CARRIED)
	{
		aniId = ID_ANI_KOOPAS_SHELL;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
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
				float ux, uy;
				u->GetPosition(ux, uy);
				if (x < ux || x > ux + u->length)
					vx = -vx;
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
		aniId = ID_ANI_KOOPAS_WALKING;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
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
