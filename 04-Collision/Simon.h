#pragma once
#include "GameObject.h"
#include "fire.h"
#include "Brick.h"
#include "Sprites.h"
#include "DN.h"
#include "MorningStar.h"
#define SIMON_WALKING_SPEED		0.1f 
//fgdf
#define SIMON_
//sdfgs
//0.1f
//#define SIMON_DOWN_SPEED   0f
#define SIMON_JUMP_SPEED_Y		0.2f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.0005f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DOWN			400
#define SIMON_STATE_DIE				500
//dsfdgfbng
#define SIMON_STATE_ATTACT			600 
//#define SIMON_STATE_ATTACT_RIGHT			700 
//sfdgddf

#define SIMON_ANI_BIG_IDLE_RIGHT		0
#define SIMON_ANI_BIG_IDLE_LEFT			1
#define SIMON_ANI_Ngoi_Phai		2
#define SIMON_ANI_Ngoi_Trai			3


#define SIMON_ANI_BIG_WALKING_RIGHT		4
#define SIMON_ANI_BIG_WALKING_LEFT			5
//#define SIMON_ANI_DOWN 6


#define SIMON_ANI_ATTACT_LEFT		6
#define SIMON_ANI_ATTACT_RIGHT		7

#define SIMON_ANI_JUMP_LEFT		8
#define SIMON_ANI_JUMP_RIGHT		9

#define SIMON_ANI_DIE				10





#define	SIMON_LEVEL_ATTACT	1
#define	SIMON_LEVEL_BIG		2

#define SIMON_BIG_BBOX_WIDTH  15//15
#define SIMON_BIG_BBOX_HEIGHT 29//27

#define SIMON_SMALL_BBOX_WIDTH  18
#define SIMON_SMALL_BBOX_HEIGHT 29

#define SIMON_UNTOUCHABLE_TIME 5000


class CSimon : public CGameObject
{
	int level;
	int untouchable; // biến true false khi simon va chạm trong thời gian sẽ không bị ảnh hưởng bởi va trạm khác
	DWORD timeAttact;
	DWORD timeJump;
	bool checkattach = false;
	bool checkjump = false;
	int frame;
public:
	CMorningStar *morningstar;
	//CAnimation *ani;
	CSimon(CMorningStar *ms) : CGameObject()
	{
		morningstar = ms;
		ms->nx = this->nx;
		listvacham.push_back(3);
		AddAnimation(105);		// dung tai cho phai            0
		AddAnimation(108);		// dung tai cho trai               1 
		AddAnimation(107);		// ngoi phai                      2
		AddAnimation(110);		// ngoi trai                         3
		AddAnimation(106);		// di phai               4
		AddAnimation(109);		// di trai         5
		AddAnimation(103);		//attact left	6
		AddAnimation(104);     //attact right	7
		AddAnimation(101);		//	jump right 8
		AddAnimation(102);     //jump left 9
		SetPosition(50.0f, 0);
		
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render(float &xcamera, float &ycamera);//them x y trong render 
	void setcheckjump(bool checkattach_f) { this->checkjump = checkattach_f; }
	bool getcheckjump() { return checkattach; }
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartAttact() { checkattach = true; timeAttact = GetTickCount(); }
	//void Startjump() { checkjump = true; timeAttact = GetTickCount(); }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};