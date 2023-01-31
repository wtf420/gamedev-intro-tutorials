#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected:		// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;

public:
	int length;
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
};

typedef CPlatform* LPPLATFORM;

class CPlatform2 : public CGameObject
{
protected:
	float height;
public:
	float length;
	CPlatform2(float x, float y,
		float a, float b ) :CGameObject(x, y)
	{
		length = a;
		height = b;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
};

class CPlatform3 : public CGameObject
{
public:
	float length, height, spriteId;
	CPlatform3(float x, float y,
		float a, float b) :CGameObject(x, y)
	{
		length = a;
		height = b;
		spriteId = -1;
	}

	CPlatform3(float x, float y,
		float a, float b, float f) :CGameObject(x, y)
	{
		length = a;
		height = b;
		spriteId = f;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class CWarpPipe : public CGameObject
{
public:
	float length, height, spriteId;
	float exitX, exitY;
	CWarpPipe(float x, float y,
		float a, float b, float f, float ex, float ey) :CGameObject(x, y)
	{
		length = a;
		height = b;
		exitX = ex;
		exitY = ey;
		spriteId = f;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class CPlatformOneWay : public CGameObject
{
protected:
	float height;
public:
	float length;
	CPlatformOneWay(float x, float y,
		float a, float b) :CGameObject(x, y)
	{
		length = a;
		height = b;
	}

	float GetTopPosition()
	{
		return y - height / 2;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};