#pragma once
#include "GameObject.h"
#include "DN.h"


class CBackground : public CGameObject
{
	vector<LPSPRITE> sprites;
	vector<vector<int>> position;
	//int level;
public:
	//void load()
	CBackground(vector<LPSPRITE> sprites, vector<vector<int>> position)
	{
		this->sprites = sprites;
		this->position = position;
		//	this->x = x;
		//	this->y = y;
	}
	virtual void Render(float &xcamera, float &ycamera); //them x y trong render 
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};