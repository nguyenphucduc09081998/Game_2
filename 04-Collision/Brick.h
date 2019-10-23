#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  15//16
#define BRICK_BBOX_HEIGHT 15//16 chi bao ngoai cua vao cham

class CBrick : public CGameObject
{
public:
	virtual void Render(float &xcamera, float &ycamera);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};