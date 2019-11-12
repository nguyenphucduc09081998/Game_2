﻿#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};



 class CGameObject
{
public:

	float x;			//vị trí trong map
	float y;			// vị trí trong map

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;		// vận tốc
	float vy;		// vận tốc

	int nx;	 

	int state;
	//check
	int id_vacham;
	int type_col_morningtar;
	vector<int> listvacham;
	vector<int> listvacham_morningtar;
	DWORD dt; 

	vector<LPANIMATION> animations;
	bool checkdestroy = false;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; } // để đặt tọa đọ của vật lúc run game
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

	virtual void RenderBoundingBox(float &xcamera, float &ycamera);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny);

	void AddAnimation(int aniId);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL); //
	virtual void Render(float &xcamera, float &ycamera) = 0;
	virtual void SetState(int state) { this->state = state; }

	virtual void delete_object(vector<LPGAMEOBJECT> &coObjects, int i) {};

	~CGameObject();
};

