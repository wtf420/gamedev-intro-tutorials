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
	if (currentPba != -1)
	{
		vy += ay * dt;
		vx += ax * dt;

		x += vx * dt;
		y += vy * dt;

		PbaTimer += dt;
		if (PbaTimer >= timetillnextPba)
		{
			PbaStep++;
			StartPBA(currentPba, PbaStep);
		}
	}
	else
	{
		vy += ay * dt;
		vx += ax * dt;

		if (abs(vx) > abs(maxVx)) vx = maxVx;

		if (abs(this->vx) > MARIO_WALKING_SPEED && isOnPlatform)
		{
			power++;
			if (power > MARIO_POWER_LIMIT)
				power = MARIO_POWER_LIMIT;
		}
		else if (isOnPlatform)
		{
			power--;
			if (power < 0)
				power = 0;
		}

		// reset untouchable timer if untouchable time has passed
		if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
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
		if (isWarping && dynamic_cast<CWarpPipe*>(currentPlatform))
		{
			CWarpPipe* warppipe = dynamic_cast<CWarpPipe*>(currentPlatform);
			SetPosition(warppipe->exitX, warppipe->exitY);
		}

		if (attacking)
		{
			vector<LPGAMEOBJECT>* attack = Attack(10.0f, 0.0f);
			for (int i = 0; i < attack->size(); i++)
			{
				if (dynamic_cast<CBrick*>(attack->at(i)))
					attack->at(i)->Delete();
				else if (dynamic_cast<CPlant*>(attack->at(i)))
				{
					CPlant* plant = dynamic_cast<CPlant*>(attack->at(i));
					plant->Die();
				}
				else if (dynamic_cast<CPlantBullet*>(attack->at(i)))
					attack->at(i)->Delete();
				else if (dynamic_cast<CGoomba*>(attack->at(i)))
				{
					if (attack->at(i)->GetState() != GOOMBA_STATE_DIE) attack->at(i)->SetState(GOOMBA_STATE_ATTACKED);
				}
				else if (dynamic_cast<CSuperGoomba*>(attack->at(i)))
				{
					if (attack->at(i)->GetState() != GOOMBA_STATE_DIE) attack->at(i)->SetState(GOOMBA_STATE_ATTACKED);
				}
				else if (dynamic_cast<CKoopas*>(attack->at(i)))
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(attack->at(i));
					koopas->Attacked();
				}
				else if (dynamic_cast<CSuperKoopas*>(attack->at(i)))
				{
					CSuperKoopas* koopas = dynamic_cast<CSuperKoopas*>(attack->at(i));
					koopas->Attacked();
				}
				else if (dynamic_cast<CBrickWithCoin*>(attack->at(i)))
				{
					CBrickWithCoin* brick = dynamic_cast<CBrickWithCoin*>(attack->at(i));
					brick->Delete();
				}
				else if (dynamic_cast<CBrickWithP*>(attack->at(i)))
				{
					CBrickWithP* brick = dynamic_cast<CBrickWithP*>(attack->at(i));
					brick->Attacked();
				}
			}
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
		CCollision::GetInstance()->Process(this, dt, &coObjects2);

		std::string stddrivestring = to_string(this->x) + " | " + to_string(this->y);
		std::wstring widedrivestring = std::wstring(stddrivestring.begin(), stddrivestring.end());
		const wchar_t* TargetDrive = widedrivestring.c_str();
		DebugOutTitle(TargetDrive);
	}
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

	if (isWarping && dynamic_cast<CWarpPipe*>(e->obj))
	{
		CWarpPipe* warppipe = dynamic_cast<CWarpPipe*>(currentPlatform);
		SetPosition(warppipe->exitX, warppipe->exitY);
	}
	if (dynamic_cast<CMyth*>(e->obj))
	{
		if (e->ny > 0)
		{
			CMyth* myth = dynamic_cast<CMyth*>(e->obj);
			myth->Interact();
		}
	}

	else if (dynamic_cast<PowerBlock*>(e->obj))
		OnCollisionWithPowerBlock(e);
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
	else if (dynamic_cast<CSuperKoopas*>(e->obj))
		OnCollisionWithSuperKoopas(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CRLeaf*>(e->obj) || dynamic_cast<CRMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

void CMario::StartPBA(int pba, int step)
{
	switch (pba)
	{
	case 1:
		currentPba = 1;
		PbaStep = 1;
		PbaTimer = 0;
		GetDiePBA(step);
		break;
	case 2:
		currentPba = 2;
		PbaStep = 1;
		PbaTimer = 0;
		break;
	default:
		break;
	}
}

void CMario::GetDiePBA(int step)
{
	switch (step)
	{
	case 1:
	{
		acceptKeyboardInput = 0;
		PbaStep = 1;
		PbaTimer = 0;

		colliable = 0;
		CGame::GetInstance()->SetTimeScale(0.0f);
		timetillnextPba = 500;
		vx = 0;
		vy = 0;
		ax = 0;
		ay = 0;
		break;
	}
	case 2:
	{
		PbaStep = 2;
		PbaTimer = 0;
		vx = 0;
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		ax = 0;
		ay = MARIO_GRAVITY;
		timetillnextPba = 3000;
		break;
	}
	default:
		break;
	}
}

vector<LPGAMEOBJECT>* CMario::Attack(float rangeX, float rangeY)
{
	float ml, mt, mr, mb;
	this->GetBoundingBox(ml, mt, mr, mb);

	vector<LPGAMEOBJECT> objects = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetObjects();
	vector<LPGAMEOBJECT>* results = new vector<LPGAMEOBJECT>();

	for (int i = 0; i < objects.size(); i++)
		if (!dynamic_cast<CMario*>(objects[i]) && (objects[i]->IsCollidable() || objects[i]->IsBlocking()))
		{
			float l, t, r, b;
			objects[i]->GetBoundingBox(l, t, r, b);

			if ((l > mr + rangeX) || (t > mb + rangeY) || (r < ml - rangeX) || (b < mt - rangeY))
			{

			} else

				results->push_back(objects[i]);
		}
	return results;
}


void CMario::OnCollisionWithPowerBlock(LPCOLLISIONEVENT e)
{
	PowerBlock* pblock = dynamic_cast<PowerBlock*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (pblock->collidable)
		{
			pblock->Start();
			vector<LPGAMEOBJECT>* objects = ((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetObjectsInCamera();
			for (int i = 0; i < objects->size(); i++)
				if (dynamic_cast<CBrickWithCoin*>(objects->at(i)))
				{
					dynamic_cast<CBrickWithCoin*>(objects->at(i))->Attacked();
				}
		}
	}
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
		if (untouchable == 0 || attacking == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_ATTACKED)
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
		if (goomba->GetState() == GOOMBA_STATE_SUPER_WALKING)
		{
			goomba->SetState(GOOMBA_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		} else
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0 || attacking == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_ATTACKED)
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
		if (koopas->GetState() == KOOPAS_STATE_SHELL)
		{
			koopas->SetState(KOOPAS_STATE_MOVING_SHELL);
		}
		else
		if (untouchable == 0 || attacking == 0)
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

void CMario::OnCollisionWithSuperKoopas(LPCOLLISIONEVENT e)
{
	CSuperKoopas* Koopas = dynamic_cast<CSuperKoopas*>(e->obj);

	// jump on top >> kill Koopas and deflect a bit 
	if (e->ny < 0)
	{
		if (Koopas->GetState() == KOOPAS_STATE_FLYING)
		{
			Koopas->SetState(KOOPAS_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else
			if (Koopas->GetState() == KOOPAS_STATE_SHELL)
			{
				Koopas->SetState(KOOPAS_STATE_MOVING_SHELL);
			}
			else
				if (Koopas->GetState() == KOOPAS_STATE_MOVING_SHELL)
				{
					Koopas->SetState(KOOPAS_STATE_SHELL);
				}
	}
	else // hit by Koopas
	{
		if (Koopas->GetState() == KOOPAS_STATE_SHELL)
		{
			Koopas->SetState(KOOPAS_STATE_MOVING_SHELL);
		}
		else
		if (untouchable == 0 || attacking == 0)
		{
			if (Koopas->GetState() != KOOPAS_STATE_DIE)
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

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	AddCoin();
}

void CMario::OnCollisionWithPlant(LPCOLLISIONEVENT e)
{
	CPlant* plant = dynamic_cast<CPlant*>(e->obj);
	if (untouchable == 0 || attacking == 0)
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
		if (untouchable == 0 || attacking == 0)
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
					vy = -MARIO_JUMP_SPEED_Y / 1.5f;
				}
			}
			break;

		case MARIO_STATE_RELEASE_JUMP:
			if (vy < 0) vy = vy /2;
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
			StartPBA(1, 1);
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
