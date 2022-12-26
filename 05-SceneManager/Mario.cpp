#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopas.h"
#include "Coin.h"
#include "Portal.h"
#include "Platform.h"
#include "Myth.h"
#include "Rewards.h"
#include "Plant.h"

#include "Collision.h"
#include <string>

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;
	
	if (abs(this->vx) > MARIO_WALKING_SPEED)
	{
		power++;
		if (power > MARIO_POWER_LIMIT)
			power = MARIO_POWER_LIMIT;
	}
	else
	{
		power--;
		if (power < 0)
			power = 0;
	}

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount64() - attack_start > MARIO_ATTACK_TIME)
	{
		attack_start = 0;
		attacking = 0;
	}
	if (GetTickCount64() - raccoonSpamJump_start <= MARIO_RACCOONSPAMJUMP_TIME)
	{
		maxVy = MARIO_MAX_GRAVITY;
		if (vy > maxVy) vy = maxVy;
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
	CCollision::GetInstance()->Process(this, dt, &coObjects2);

	std::string stddrivestring = to_string(absx()) + " " + to_string(absy());
	std::wstring widedrivestring = std::wstring(stddrivestring.begin(), stddrivestring.end());
	const wchar_t* TargetDrive = widedrivestring.c_str();
	DebugOutTitle(TargetDrive);
}

void CMario::OnNoCollision(DWORD dt)
{
	isOnPlatform = false;
	currentPlatform = NULL;
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		isOnPlatform = true;
		currentPlatform = e->obj;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CMyth*>(e->obj))
	{
		if (e->ny > 0)
		{
			CMyth* myth = dynamic_cast<CMyth*>(e->obj);
			myth->Interact();
		}
	}
	else if (dynamic_cast<CPlant*>(e->obj))
		OnCollisionWithPlant(e);
	else if (dynamic_cast<CPlantBullet*>(e->obj))
		OnCollisionWithBullet(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CSuperGoomba*>(e->obj))
		OnCollisionWithSuperGoomba(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CRLeaf*>(e->obj) || dynamic_cast<CRMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (attacking)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE) goomba->SetState(GOOMBA_STATE_DIE);
		} else
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level == MARIO_LEVEL_RACCOON)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else
				if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithSuperGoomba(LPCOLLISIONEVENT e)
{
	CSuperGoomba* goomba = dynamic_cast<CSuperGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (attacking)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE) goomba->SetState(GOOMBA_STATE_DIE);
		}
		else
			if (untouchable == 0)
			{
				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					if (level == MARIO_LEVEL_RACCOON)
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
					else
						if (level == MARIO_LEVEL_BIG)
						{
							level = MARIO_LEVEL_SMALL;
							StartUntouchable();
						}
						else
						{
							DebugOut(L">>> Mario DIE >>> \n");
							SetState(MARIO_STATE_DIE);
						}
				}
			}
	}
}

void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

	// jump on top >> kill KOOPAS and deflect a bit 
	if (e->ny < 0)
	{
		if (koopas->GetState() == KOOPAS_STATE_WALKING)
		{
			koopas->SetState(KOOPAS_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		} else
			if (koopas->GetState() == KOOPAS_STATE_SHELL)
			{
				koopas->SetState(KOOPAS_STATE_MOVING_SHELL);
			} else
				if (koopas->GetState() == KOOPAS_STATE_MOVING_SHELL)
				{
					koopas->SetState(KOOPAS_STATE_SHELL);
				}
	}
	else // hit by KOOPAS
	{
		if (attacking)
		{
			if (koopas->GetState() == KOOPAS_STATE_WALKING) koopas->SetState(KOOPAS_STATE_DIE);
		} else
		if (untouchable == 0)
		{
			if (koopas->GetState() != KOOPAS_STATE_SHELL && koopas->GetState() != KOOPAS_STATE_CARRIED)
			{
				if (level == MARIO_LEVEL_RACCOON)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else
					if (level == MARIO_LEVEL_BIG)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			} else
			if (isHolding)
			{
				koopas->SetState(KOOPAS_STATE_CARRIED);
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	AddCoin();
}

void CMario::OnCollisionWithPlant(LPCOLLISIONEVENT e)
{
	CPlant* plant = dynamic_cast<CPlant*>(e->obj);
	if (attacking)
	{
		plant->Die();
	}
	else
		if (untouchable == 0)
		{
			if (level == MARIO_LEVEL_RACCOON)
			{
				level = MARIO_LEVEL_BIG;
				StartUntouchable();
			}
			else
				if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
}

void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e)
{
	CPlantBullet* bullet = dynamic_cast<CPlantBullet*>(e->obj);
	if (attacking)
	{
		bullet->Reset2();
	}
	else
		if (untouchable == 0)
		{
			if (level == MARIO_LEVEL_RACCOON)
			{
				level = MARIO_LEVEL_BIG;
				StartUntouchable();
			}
			else
				if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CRMushroom* m = dynamic_cast<CRMushroom*>(e->obj);
	CRLeaf* l = dynamic_cast<CRLeaf*>(e->obj);

	if (m) {
		if (m->collidable)
		{
			e->obj->Delete();
			if (level == MARIO_LEVEL_SMALL)
				SetLevel(MARIO_LEVEL_BIG); else
				if (level == MARIO_LEVEL_BIG)
					SetLevel(MARIO_LEVEL_RACCOON);
		}
	} else
	if (l) {
		if (l->collidable)
		{
			e->obj->Delete();
			if (level == MARIO_LEVEL_SMALL)
				SetLevel(MARIO_LEVEL_BIG); else
				if (level == MARIO_LEVEL_BIG)
					SetLevel(MARIO_LEVEL_RACCOON);
		}
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdRaccoon()
{
	int aniId = -1;
	if (attacking)
	{
		if (nx >= 0)
			aniId = ID_ANI_MARIO_RACCOON_ATTACK_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACCOON_ATTACK_LEFT;
		return aniId;
	}
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;

	return aniId;
}


void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_RACCOON)
		aniId = GetAniIdRaccoon();
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
}

void CMario::SetHold(int h)
{
	if (isHolding != h)
		isHolding = h;
}

void CMario::SetState(int state)
{
	if (isNoclipping)
	{
		ay = 0;
		switch (state)
		{
		case MARIO_STATE_WALKING_RIGHT:
			maxVx = MARIO_WALKING_SPEED;
			ax = MARIO_ACCEL_WALK_X;
			nx = 1;
			break;
		case MARIO_STATE_WALKING_LEFT:
			maxVx = -MARIO_WALKING_SPEED;
			ax = -MARIO_ACCEL_WALK_X;
			nx = -1;
			break;
		case MARIO_STATE_WALKING_DOWN:
			maxVy = MARIO_WALKING_SPEED;
			ay = MARIO_ACCEL_WALK_X;
			break;
		case MARIO_STATE_WALKING_UP:
			maxVx = -MARIO_WALKING_SPEED;
			ay = -MARIO_ACCEL_WALK_X;
			break;
		case MARIO_STATE_IDLE:
			ax = 0.0f;
			vx = 0.0f;
			ay = 0.0f;
			vy = 0.0f;
			break;
		}
	}
	else
	{
		ay = MARIO_GRAVITY;
		// DIE is the end state, cannot be changed! 
		if (this->state == MARIO_STATE_DIE) return;

		switch (state)
		{
		case MARIO_STATE_ATTACK:
			if (level == MARIO_LEVEL_RACCOON)
			{
				attack_start = GetTickCount64();
				attacking = 1;
				untouchable_start = 0;
				untouchable = 0;
				if (!isOnPlatform && vy <= 0.05f)
				{
					vy += -MARIO_JUMP_SPEED_Y / 2.0f;
				}
			}
			break;
		case MARIO_STATE_RUNNING_RIGHT:
			if (isSitting) break;
			maxVx = MARIO_RUNNING_SPEED;
			ax = MARIO_ACCEL_RUN_X;
			nx = 1;
			break;
		case MARIO_STATE_RUNNING_LEFT:
			if (isSitting) break;
			maxVx = -MARIO_RUNNING_SPEED;
			ax = -MARIO_ACCEL_RUN_X;
			nx = -1;
			break;
		case MARIO_STATE_WALKING_RIGHT:
			if (isSitting) break;
			maxVx = MARIO_WALKING_SPEED;
			ax = MARIO_ACCEL_WALK_X;
			nx = 1;
			break;
		case MARIO_STATE_WALKING_LEFT:
			if (isSitting) break;
			maxVx = -MARIO_WALKING_SPEED;
			ax = -MARIO_ACCEL_WALK_X;
			nx = -1;
			break;
		case MARIO_STATE_JUMP:
			if (isSitting) break;
			if (isOnPlatform)
			{
				if (abs(this->vx) == MARIO_RUNNING_SPEED)
					vy = -MARIO_JUMP_RUN_SPEED_Y;
				else
					vy = -MARIO_JUMP_SPEED_Y;
			}
			else if (this->level == MARIO_LEVEL_RACCOON)
			{
				raccoonSpamJump_start = GetTickCount64();
				if (power == MARIO_POWER_LIMIT)
				{
					ay = 0;
					vy = -MARIO_JUMP_SPEED_Y / 1.5;
				}
			}
			break;

		case MARIO_STATE_RELEASE_JUMP:
			if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 4;
			break;

		case MARIO_STATE_SIT:
			if (isOnPlatform && level != MARIO_LEVEL_SMALL)
			{
				state = MARIO_STATE_IDLE;
				isSitting = true;
				vx = 0; vy = 0.0f;
				y += MARIO_SIT_HEIGHT_ADJUST;
			}else if (this->state == MARIO_LEVEL_RACCOON)
			{
				vy = MARIO_GRAVITY / 8;
			}
			break;

		case MARIO_STATE_SIT_RELEASE:
			if (isSitting)
			{
				isSitting = false;
				state = MARIO_STATE_IDLE;
				y -= MARIO_SIT_HEIGHT_ADJUST;
			}
			break;

		case MARIO_STATE_IDLE:
			ax = 0.0f;
			vx = 0.0f;
			break;

		case MARIO_STATE_DIE:
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			vx = 0;
			ax = 0;
			break;
		}

		CGameObject::SetState(state);

	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else if (attacking)
		{
			left = x - MARIO_ATTACK_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_ATTACK_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

void CMario::ToggleNoclip()
{
	isNoclipping = !isNoclipping;
}
