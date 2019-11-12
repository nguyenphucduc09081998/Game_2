#pragma once
#include "GameObject.h"
#include "MorningStar.h"
#include "DN.h"
#define FIRE_BBOX_WIDTH  15//16
#define FIRE_BBOX_HEIGHT 35//16 chi bao ngoai cua vao cham

class CFire : public CGameObject
{
	//bool checkdis = false;
	DWORD timeremove_fire;
	bool destroy = false;
public:

	CFire();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects, vector<LPGAMEOBJECT> &colidingObjects);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render(float &xcamera, float &ycamera);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
	/*void setdestroy(bool checkattach_f) { this->checkdis = checkattach_f; }
	bool getdestroy() { return checkdis; }*/
	virtual void delete_object(vector<LPGAMEOBJECT> &coObjects,int i);
};