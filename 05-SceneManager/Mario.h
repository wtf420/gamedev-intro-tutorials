#pragma once
#include "GameObject.h"
#include "Platform.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f
#define MARIO_MAX_GRAVITY		0.1f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.35f
#define MARIO_JUMP_RUN_SPEED_Y	0.4f

#define MARIO_GRAVITY			0.001f

#define MARIO_JUMP_DEFLECT_SPEED  0.25f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_WALKING_UP		101
#define MARIO_STATE_WALKING_DOWN	202

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_ATTACK			700

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 100
#define ID_ANI_MARIO_IDLE_LEFT 101

#define ID_ANI_MARIO_IDLE_HOLD_RIGHT 110
#define ID_ANI_MARIO_IDLE_HOLD_LEFT 111

#define ID_ANI_MARIO_WALKING_RIGHT 200
#define ID_ANI_MARIO_WALKING_LEFT 201

#define ID_ANI_MARIO_RUNNING_RIGHT 210
#define ID_ANI_MARIO_RUNNING_LEFT 211

#define ID_ANI_MARIO_RUNNING_MP_RIGHT 410
#define ID_ANI_MARIO_RUNNING_MP_LEFT 411

#define ID_ANI_MARIO_WALKRUN_HOLD_RIGHT 420
#define ID_ANI_MARIO_WALKRUN_HOLD_LEFT 421

#define ID_ANI_MARIO_JUMP_RIGHT 500
#define ID_ANI_MARIO_JUMP_LEFT 501

#define ID_ANI_MARIO_JUMP_MP_RIGHT 510
#define ID_ANI_MARIO_JUMP_MP_LEFT 511

#define ID_ANI_MARIO_FALL_RIGHT 600
#define ID_ANI_MARIO_FALL_LEFT 601

#define ID_ANI_MARIO_SIT_RIGHT 700
#define ID_ANI_MARIO_SIT_LEFT 701

#define ID_ANI_MARIO_BRACE_RIGHT 800
#define ID_ANI_MARIO_BRACE_LEFT 801

#define ID_ANI_MARIO_KICK_RIGHT 900
#define ID_ANI_MARIO_KICK_LEFT 901

#define ID_ANI_MARIO_DIE 999
#define ID_ANI_MARIO_PIPE 690
#define ID_ANI_MARIO_LEVEL_UPDOWN_BIG 990
#define ID_ANI_MARIO_LEVEL_UP_SMALL_RIGHT 991
#define ID_ANI_MARIO_LEVEL_UP_SMALL_LEFT 992
#define ID_ANI_MARIO_LEVEL_DOWN_SMALL_RIGHT 993
#define ID_ANI_MARIO_LEVEL_DOWN_SMALL_LEFT 994

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_RUNNING_MP_RIGHT 1310
#define ID_ANI_MARIO_SMALL_RUNNING_MP_LEFT 1311

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_MP_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_MP_LEFT 1601

#define ID_ANI_MARIO_SMALL_KICK_RIGHT 1700
#define ID_ANI_MARIO_SMALL_KICK_LEFT 1701

#define ID_ANI_MARIO_SMALL_PIPE 1800

#define ID_ANI_MARIO_SMALL_IDLE_HOLD_RIGHT 1900
#define ID_ANI_MARIO_SMALL_IDLE_HOLD_LEFT 1901

#define ID_ANI_MARIO_SMALL_WALKRUN_HOLD_RIGHT 1910
#define ID_ANI_MARIO_SMALL_WALKRUN_HOLD_LEFT 1911

// RACCOON MARIO
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 2100
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 2101

#define ID_ANI_MARIO_RACCOON_IDLE_HOLD_RIGHT 2110
#define ID_ANI_MARIO_RACCOON_IDLE_HOLD_LEFT 2111

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 2200
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 2201

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 2210
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 2211

#define ID_ANI_MARIO_RACCOON_RUNNING_MP_RIGHT 2410
#define ID_ANI_MARIO_RACCOON_RUNNING_MP_LEFT 2411

#define ID_ANI_MARIO_RACCOON_WALKRUN_HOLD_RIGHT 2420
#define ID_ANI_MARIO_RACCOON_WALKRUN_HOLD_LEFT 2421

#define ID_ANI_MARIO_RACCOON_JUMP_RIGHT 2500
#define ID_ANI_MARIO_RACCOON_JUMP_LEFT 2501

#define ID_ANI_MARIO_RACCOON_JUMP_MP_RIGHT 2510
#define ID_ANI_MARIO_RACCOON_JUMP_MP_LEFT 2511

#define ID_ANI_MARIO_RACCOON_FLYING_RIGHT 2520
#define ID_ANI_MARIO_RACCOON_FLYING_LEFT 2521

#define ID_ANI_MARIO_RACCOON_FALL_RIGHT 2600
#define ID_ANI_MARIO_RACCOON_FALL_LEFT 2601

#define ID_ANI_MARIO_RACCOON_FALL_JS_RIGHT 2610
#define ID_ANI_MARIO_RACCOON_FALL_JS_LEFT 2611

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 2700
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 2701

#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 2800
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 2801

#define ID_ANI_MARIO_RACCOON_KICK_RIGHT 2900
#define ID_ANI_MARIO_RACCOON_KICK_LEFT 2901

#define ID_ANI_MARIO_RACCOON_ATTACK_RIGHT 2090
#define ID_ANI_MARIO_RACCOON_ATTACK_LEFT 2091

#define ID_ANI_MARIO_RACCOON_PIPE 2990	
#define ID_ANI_MARIO_TITLESCREEN_1 2998
#define ID_ANI_MARIO_TITLESCREEN_2 2999
#pragma endregion

#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_RACCOON		3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 28
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 16

#define MARIO_UNTOUCHABLE_TIME 300
#define MARIO_RACCOONSPAMJUMP_TIME 500
#define MARIO_ATTACK_TIME 100
#define MARIO_ATTACK_ANIMATION_TIME 300
#define MARIO_POWER_LIMIT 50

class CMario : public CGameObject
{
	BOOLEAN isSitting, isNoclipping;
	float maxVx;
	float maxVy;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	float absx() { return x + (MARIO_BIG_BBOX_WIDTH / 2); };
	float absy() { return y + (MARIO_BIG_BBOX_HEIGHT / 2); };

	int level; 
	int untouchable; 

	int currentPba = -1;
	int PbaStep;
	ULONGLONG PbaTimer, timetillnextPba;

	int colliable = 1;
	ULONGLONG untouchable_start, raccoonSpamJump_start, attack_start, kick_start;
	BOOLEAN isOnPlatform;
	int coin, score; 

	LPGAMEOBJECT currentPlatform;
	LPGAMEOBJECT holdingObject = NULL;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithSuperGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithSuperKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant2(LPCOLLISIONEVENT e);
	void OnCollisionWithBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithPowerBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWith1Up(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

	void StartPBA(int pba, int step);
	void GetDiePBA(int step);
	void GetPipeDownPBA(int step);
	void GetPipeUpPBA(int step);
	void GetFinishPBA(int step);
	void GetLevelUpToRaccoonPBA(int step);
	void GetLevelUpToBigPBA(int step);
	void GetLevelDownFromRaccoonPBA(int step);
	void GetLevelDownFromBigPBA(int step);

public:
	int acceptKeyboardInput = 1;
	int attacking;
	int isHolding;
	int isWarping;
	float warptoX, warptoY;
	float power;

	CMario(float x, float y) : CGameObject(x, y)
	{
		isWarping = 0;
		isSitting = false;
		maxVx = 0.0f;
		maxVy = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		attacking = 0;
		attack_start = -1;
		isOnPlatform = false;
		coin = 0;
		score = 0;
		isNoclipping = false;
		isHolding = true;
		power = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetHold(int h);
	vector<LPGAMEOBJECT>* Attack(float rangeX, float rangeY);

	int IsCollidable()
	{ 
		return (colliable);
	}

	int IsMaxPower()
	{
		return (power >= MARIO_POWER_LIMIT);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void AddCoin();
	void AddScore(float x, float y, int amount);

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	int GetLevel() { return this->level; };
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void ToggleNoclip();
};