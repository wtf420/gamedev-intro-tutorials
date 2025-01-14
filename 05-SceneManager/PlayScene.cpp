#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "CBackground.h"
#include "Myth.h"
#include "Rewards.h"
#include "Koopas.h"
#include "Plant.h"
#include "hud.h"
#include "worldmap.h"
#include "titlescreen.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		if (CSprites::GetInstance()->Get(sprite_id) == NULL)
			DebugOut(L"ANIMATION %i: SPRITE NOT FOUND: %i\n", ani_id, sprite_id);
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_BACKGROUND:
	{
		float k = (float)atof(tokens[3].c_str());
		obj = new CBackground(x, y, k);
		DebugOut(L"[INFO] Background has been created: %f\n", k);
		break;
	}
	case OBJECT_TYPE_TITLESCREEN_BACKGROUND:
	{
		float k = (float)atof(tokens[3].c_str());
		obj = new CTSBackground(x, y, k);
		DebugOut(L"[INFO] Background has been created: %f\n", k);
		break;
	}
	case OBJECT_TYPE_ANIMATED_BACKGROUND:
	{
		float k = (float)atof(tokens[3].c_str());
		obj = new CAnimatedBackground(x, y, k);
		DebugOut(L"[INFO] Background has been created: %f\n", k);
		break;
	}
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}
	case OBJECT_TYPE_WORLD_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CWMario(x, y);
		player = (CWMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}
	case OBJECT_TYPE_TITLESCREEN_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CTSMario(x, y);
		player = (CTSMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}
	case OBJECT_TYPE_TITLESCREEN_LUIGI:
	{
		obj = new CTSLuigi(x, y);
		break;
	}
	case OBJECT_TYPE_WORLD_ENEMY: obj = new CWEnemy(x, y); break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
	case OBJECT_TYPE_SUPERGOOMBA: obj = new CSuperGoomba(x, y); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(x, y); break;
	case OBJECT_TYPE_SUPER_KOOPAS: obj = new CSuperKoopas(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_BRICK_WITH_COIN: obj = new CBrickWithCoin(x, y); break;
	case OBJECT_TYPE_BRICK_WITH_POWER: obj = new CBrickWithP(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_PLANT:	obj = new CPlant(x, y + 24); break;
	case OBJECT_TYPE_PLANT_2:	obj = new CPlant2(x, y + 24); break;
	case OBJECT_TYPE_TITLESCREEN_MUSHROOM:	obj = new CTSMushroom(x, y); break;
	case OBJECT_TYPE_TITLESCREEN_LEAF:	obj = new CTSLeaf(x, y); break;
	case OBJECT_TYPE_TITLESCREEN_GOOMBA:	obj = new CTSGoomba(x, y); break;
	case OBJECT_TYPE_TITLESCREEN_TURTLE_SHELL:	obj = new CTSTurtleShell(x, y); break;
	case OBJECT_TYPE_MYTH_COIN:
	{
		obj = new CMyth(x, y, 1);
		break;
	}
	case OBJECT_TYPE_MYTH_MUSHROOM:
	{
		obj = new CMyth(x, y, 2);
		break;
	}
	case OBJECT_TYPE_MYTH_1UP:
	{
		obj = new CMyth(x, y, 3);
		break;
	}

	case OBJECT_TYPE_PLATFORM2:
	{

		float a = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());

		obj = new CPlatform2(
			x, y, a, b
		);
		

		break;
	}

	case OBJECT_TYPE_PLATFORM_ONEWAY:
	{

		float a = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());

		obj = new CPlatformOneWay(
			x, y, a, b
		);


		break;
	}

	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_PLATFORM_3:
	{

		float a = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		float f = (float)atof(tokens[5].c_str());

		obj = new CPlatform3(
			x, y, a, b, f
		);


		break;
	}

	case OBJECT_TYPE_WARP_PIPE:
	{

		float a = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		float f = (float)atof(tokens[5].c_str());
		float ex = (float)atof(tokens[6].c_str());
		float ey = (float)atof(tokens[7].c_str());

		obj = new CWarpPipe(
			x, y, a, b, f, ex, ey
		);


		break;
	}


	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}
	CHud* hud = new CHud(100.0f, 100.0f);
	hud->SetCoinCount(CGame::GetInstance()->GetCoinCount());
	hud->SetLives(CGame::GetInstance()->GetLivesCount());
	hud->SetScore(CGame::GetInstance()->GetScore());
	this->hud = hud;
	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	this->player->Update(dt, &coObjects);
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (IsGameObjectInCamera(objects[i]) && (this->player != objects[i]))
			objects[i]->Update(dt, &coObjects);
	}
	if (dynamic_cast<CHud*>(this->hud))
	{
		CHud* h = dynamic_cast<CHud*>(this->hud);
		h->Update(dt);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	if (dynamic_cast<CWMario*>(player))
	{
		float cx, cy;
		cx = -CGame::GetInstance()->GetBackBufferWidth() / 2;
		cy = -CGame::GetInstance()->GetBackBufferHeight() / 2;
		CGame::GetInstance()->SetCamPos(cx, cy);
	}
	else if (dynamic_cast<CTSMario*>(player))
	{
		float cx = 200;
		float cy = 132;
		cx -= CGame::GetInstance()->GetBackBufferWidth() / 2;
		cy -= CGame::GetInstance()->GetBackBufferHeight() / 2;
		CGame::GetInstance()->SetCamPos(cx, cy);
	}
	else if (player->GetState() != MARIO_STATE_DIE)
	{
		// Update camera to follow mario
		float cx, cy;
		player->GetPosition(cx, cy);
		float cxx, cyy;
		CGame::GetInstance()->GetCamPos(cxx, cyy);

		//get mario position and clamp it
		//mario is under ground set cam to underground
		if (cy > 220)	cy = 340; else
			//snap to ground utill you reach highest point possible without flying, except when falling down
			if (cy > -15 && cy < cyy + CGame::GetInstance()->GetBackBufferHeight() / 2) cy = 132; else
				//highest point that you can fly to.
				if (cy < -150) cy = -150;
		if (cx > 2700) cx = 2700; else
			if (cx < 170) cx = 170;

		//translate it to camera coordinates
		cx -= CGame::GetInstance()->GetBackBufferWidth() / 2;
		cy -= CGame::GetInstance()->GetBackBufferHeight() / 2;

		//moving cam to new location
		//snap camera to underground
		int a = 340 - CGame::GetInstance()->GetBackBufferHeight() / 2;
		int b = 132 - CGame::GetInstance()->GetBackBufferHeight() / 2;
		if (cy >= a) cyy = a; else
			if (cy < a && cy >= b) cyy = b; else
			//slowing pan camera to new location
		{
			if (cy < cyy)
				cyy -= 10;
			if (cy > cyy)
				cyy += 10;
			if (abs(cyy - cy) < 10) cyy = cy;
		}
		cxx = cx;

		/*
		* CMario* mario = dynamic_cast<CMario*>(player);
		if (mario && (mario->GetLevel() == MARIO_LEVEL_RACCOON && mario->IsMaxPower()))
		{
			if (cy < cyy)
				cyy -= 10;
		}
		if (cy > cyy)
			cyy += 10;
		if (abs(cyy - cy) < 10) cyy = cy;
		*/

		CGame::GetInstance()->SetCamPos(cxx, cyy);
	}

	float mx, my;
	CGame::GetInstance()->GetCamPos(mx, my);
	this->hud->SetPosition(mx + 76.0f + 10.0f, my + (CGame::GetInstance()->GetBackBufferHeight() - 28.0f / 2) - 20.0f);
	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	if (CGame::GetInstance()->DrawBoundingBox == true)
	{
		LPSPRITE sprite = CSprites::GetInstance()->Get(100000);
		sprite->Draw(1400-16, 88);
	}
	for (int i = 0; i < objects.size(); i++)
		if (IsGameObjectInCamera(objects[i]))
			objects[i]->Render();
	if (CGame::GetInstance()->GetCurrentScene()->id != 1)
	this->hud->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

void CPlayScene::Add(LPGAMEOBJECT object)
{
	objects.push_back(object);
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

vector<LPGAMEOBJECT>* CPlayScene::GetObjectsInCamera()
{
	vector<LPGAMEOBJECT>* result = new vector<LPGAMEOBJECT>();

	for (int i = 0; i < objects.size(); i++)
		if (IsGameObjectInCamera(objects[i]))
			result->push_back(objects[i]);

	return result;
}

bool CPlayScene::IsGameObjectInCamera(LPGAMEOBJECT obj) { 
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float w = CGame::GetInstance()->GetBackBufferWidth() / 2.0f;
	float h = CGame::GetInstance()->GetBackBufferHeight() / 2.0f;
	cx += w;
	cy += h;
	if ((l > cx + w) || (t > cy + w) || (r < cx - w) || (b < cy - w))
		return false;
	return true;
}

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}