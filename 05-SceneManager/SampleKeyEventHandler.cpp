#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"
#include "worldmap.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 
	if (!mario->acceptKeyboardInput)
		return; else
	switch (KeyCode)
	{
	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
		{
			mario->SetState(MARIO_STATE_ATTACK);
		}
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		mario->isWarping = 1;
		break;
	case DIK_UP:
		mario->isWarping = 1;
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_4:
		mario->ToggleNoclip();
		break;
	case DIK_9:
		CGame::GetInstance()->DrawBoundingBox = !(CGame::GetInstance()->DrawBoundingBox);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!mario->acceptKeyboardInput)
		return;
	mario->isWarping = 0;
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = dynamic_cast<CMario*>(((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
	CWMario* wmario = dynamic_cast<CWMario*>(((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer());

	DebugOut(L"********************************%i, %i\n", mario, wmario);

	if (wmario)
	{
		if (!wmario->acceptKeyboardInput) return;
		if (game->IsKeyDown(DIK_X))
		{
			wmario->Enter();
		}
		else
			if (game->IsKeyDown(DIK_RIGHT))
			{
				wmario->MoveToNode(wmario->FindPossibleNode(DIRECTION_RIGHT));
			}
			else if (game->IsKeyDown(DIK_LEFT))
			{
				wmario->MoveToNode(wmario->FindPossibleNode(DIRECTION_LEFT));
			}
			else if (game->IsKeyDown(DIK_UP))
			{
				wmario->MoveToNode(wmario->FindPossibleNode(DIRECTION_UP));
			}
			else if (game->IsKeyDown(DIK_DOWN))
			{
				wmario->MoveToNode(wmario->FindPossibleNode(DIRECTION_DOWN));
			}
	} else if (mario)
	{
		if (!mario->acceptKeyboardInput) return;
		if (game->IsKeyDown(DIK_A))
			mario->SetHold(1); else
			mario->SetHold(0);

		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (game->IsKeyDown(DIK_A))
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			else
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			if (game->IsKeyDown(DIK_A))
				mario->SetState(MARIO_STATE_RUNNING_LEFT);
			else
				mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
		else if (game->IsKeyDown(DIK_UP))
		{
			mario->SetState(MARIO_STATE_WALKING_UP);
		}
		else if (game->IsKeyDown(DIK_DOWN))
		{
			mario->SetState(MARIO_STATE_WALKING_DOWN);
		}
		else
			mario->SetState(MARIO_STATE_IDLE);
	}
}