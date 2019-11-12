#include <algorithm>
#include "debug.h"
#include "Simon.h"
#include "Game.h"
#include "Sprites.h"
#include "Goomba.h"
#include "fire.h"
#include "MorningStar.h"
#include <windows.h>
#include <iostream>
int xx = 0;
void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - timeAttact > 450)
	{
		
		timeAttact = 0;
		checkattach = false;
		//frame = 0;
		//state = SIMON_STATE_IDLE;
	}
	if (GetTickCount() - timeAttact <= 450 && checkattach) {
	//	xx++;
		CAnimation *ani = morningstar->getcurrentanimation();
	//DebugOut(L"frame: %d\n", frame);
		//ani->setcurrentFrame(0);
		 frame = ani->getcurrentFrame();
		//int frame =0;
		if (nx > 0) {
			//DebugOut(L"nx > 0: %d\n");
			if (frame == 0) {
				//DebugOut(L"0000:\n");
				morningstar->SetPosition(x - 10, y + 5);
			}
			else if (frame == 1) {
				//DebugOut(L"111:\n");
				morningstar->SetPosition(x - 10, y + 5);
			}
			else if (frame == 2) {
				//DebugOut(L"222:\n");
				morningstar->SetPosition(x + 20, y + 5);
				//CFire *fire = new CFire();
				vector<LPGAMEOBJECT> colidingObjects;		
				 morningstar->isColidingObjects(coObjects, colidingObjects);
				 DebugOut(L"colidingObjects111111 %d \n", colidingObjects.size());
				 for (UINT i = 0; i < colidingObjects.size(); i++)
				 {

					 LPGAMEOBJECT c = colidingObjects[i];
					 //c->RenderBoundingBox();
					 c->checkdestroy = true;
					// CFire->SetState(FIRE_STATE_DIE);
					// DebugOut(L"%d \n", colidingObjects.size());
					 //c->SetState(FIRE_STATE_DIE);
						
				 }
				 //ani->setcurrentFrame(-1);
			}
			//
			
		}
		else {
			//DebugOut(L"nx < 0: %d\n");
		//	DebugOut(L"vao: %d\n", frame);
			if (frame == 0) {
				morningstar->SetPosition(x +10, y + 5);
			}
			else if (frame == 1) {
				morningstar->SetPosition(x +10, y + 5);
			}
			else if (frame == 2) {
				morningstar->SetPosition(x - 20, y + 5);
				vector<LPGAMEOBJECT> colidingObjects;
				
				morningstar->isColidingObjects(coObjects, colidingObjects);
				DebugOut(L"colidingObjects222222 %d \n", colidingObjects.size());
				for (UINT i = 0; i < colidingObjects.size(); i++)
				{

					LPGAMEOBJECT c = colidingObjects[i];
					c->checkdestroy = true;
					// CFire->SetState(FIRE_STATE_DIE);
					
					 //c->SetState(FIRE_STATE_DIE);

				}
				colidingObjects.clear();
			}

		}


			
		
		//ani->setcurrentFrame(-1);
	}



	if (untouchable == 1) {
		level = SIMON_STATE_ATTACT;
		untouchable = 0;
	}
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//if (dynamic_cast<CGoomba *>(e->obj))
			//{
			//	CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
			//	if (level < SIMON_LEVEL_BIG) {
			//		level == SIMON_LEVEL_ATTACT;
			//		StartUntouchable();
			//		//level = SIMON_LEVEL_BIG;	
			//	}
			//
			//	// jump on top >> kill Goomba and deflect a bit 
			//	//if (e->ny < 0)
			//	//{
			//	//	if (goomba->GetState() != GOOMBA_STATE_DIE)
			//	//	{
			//	//		goomba->SetState(GOOMBA_STATE_DIE);
			//	//		vy = -SIMON_JUMP_DEFLECT_SPEED;
			//	//	}
			//	//}
			//	//else if (e->nx = 0)
			//	//{
			//	//	if (untouchable == 0)
			//	//	{					
			//	//			if (level == SIMON_LEVEL_ATTACT)
			//	//			{
			//	//				StartUntouchable();
			//	//				//level = SIMON_LEVEL_BIG;								
			//	//			}						
			//	//	}
			//	//}
			//}

		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	/*
	DebugOut(L"frame111: %d\n", frame);
	if(frame ==2) frame = 0;
	DebugOut(L"frame222: %d\n", frame);*/
}

void CSimon::Render(float &xcamera, float &ycamera)
{
	int ani;
	//if (state == SIMON_STATE_DIE)
		//ani = SIMON_ANI_DIE;
	//if (level == SIMON_LEVEL_BIG) {
	
		if (state == SIMON_STATE_DOWN) {
			if (nx > 0) ani = SIMON_ANI_Ngoi_Phai;
			else ani = SIMON_ANI_Ngoi_Trai;
		}
		else
		{
			if (vy < 0) {
			/*	if (!checkjump) {
					if (nx > 0) ani = SIMON_ANI_JUMP_LEFT;
					else ani = SIMON_ANI_JUMP_RIGHT;
				}
				else {*/
					if (nx > 0) ani = SIMON_ANI_JUMP_LEFT;
					else ani = SIMON_ANI_JUMP_RIGHT;
				
			}
			else {
				if (vx == 0)
				{
					if (nx > 0) ani = SIMON_ANI_BIG_IDLE_RIGHT;
					else ani = SIMON_ANI_BIG_IDLE_LEFT;
				}
				else if (vx > 0)
					ani = SIMON_ANI_BIG_WALKING_RIGHT;
				else if (vx < 0) ani = SIMON_ANI_BIG_WALKING_LEFT;
			}
		}
		if (checkattach == true) {
			
			morningstar->Render(xcamera, ycamera, nx);
			if (nx > 0) ani = SIMON_ANI_ATTACT_RIGHT;
			else ani = SIMON_ANI_ATTACT_LEFT;
		}


	int alpha = 255;
	if (untouchable) alpha = 128;//128
	float xtemp = x - xcamera;
	float yTemp = y - ycamera;
	animations[ani]->Render(x - xcamera, y - ycamera, alpha);

	//RenderBoundingBox(xcamera, ycamera);
	//if(x > )
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	
	//fghjsdfsd
	//case SIMON_STATE_ATTACT:
	//	//DebugOut(L"[INFO] atattactattactattacttact: %d\n");
	//	//DebugOut(L"[INFO] atattactattactattacttact: %d\n");
	//	//DebugOut(L"[INFO] atattactattactattacttact: %d\n");
	//	break;
	//dsfdsfsfd
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;//vx laf van toc di ngang
		nx = 1; //khi va cham bi doi lai
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		//DebugOut(L"[INFO] vy: %d\n", vy);
		//DebugOut(L"[INFO] vx: %d\n", vx);
		//if (vy == 0) {
			vy = -SIMON_JUMP_SPEED_Y;
		//}
		break;
	case SIMON_STATE_DOWN:
		vy = 0;
		vx = 0;
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		//vy = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}
}




void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BIG_BBOX_WIDTH;
	bottom = y + SIMON_BIG_BBOX_HEIGHT;
	
	//}
	//if(level == SIMON_LEVEL_ATTACT)
	//{
		//right = x + SIMON_SMALL_BBOX_WIDTH;
		//bottom = y + SIMON_SMALL_BBOX_HEIGHT;
	//}
}

