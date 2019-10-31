#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  15//16
#define BRICK_BBOX_HEIGHT 15//16 chi bao ngoai cua vao cham

class CBrick : public CGameObject
{
	//int untouchable; // biến true false khi simon va chạm trong thời gian sẽ không bị ảnh hưởng bởi va trạm khác
	//DWORD untouchable_start;
public:
	CBrick();
	//CBrick() : CGameObject() {
	//	for (int i = 0; i < 48; i++)//add nền 
	//	{
	//		//CBrick *brick = new CBrick();
	//		AddAnimation(900);
	//		SetPosition(0 + i * 16.0f, 176);//150 là khoảng cách của nền vs top
	//		//untouchable = 0;
	//		//objects.push_back(brick);
	//	}
	//	//AddAnimation(900);
	//}
	void SetPosition(float x, float y) { this->x = x, this->y = y; };
	virtual void Render(float &xcamera, float &ycamera);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};