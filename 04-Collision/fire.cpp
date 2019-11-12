#include "fire.h"
#include "item.h"
#include "debug.h"
CFire::CFire()
{
	id_vacham = 2;
	SetState(FIRE_STATE);
	type_col_morningtar = 2;
}

void CFire::Render(float &xcamera, float&ycamera)
{

	animations[0]->Render(x - xcamera, y - ycamera);
	//RenderBoundingBox(x, y );
}

void CFire::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {
	
	
}
void CFire::delete_object(vector<LPGAMEOBJECT> &objects, int i) {
	DebugOut(L"CFire:delete_object: \n");
	CItem *item = new CItem();
	//item->SetSpeed(0,-0.1);
	item->SetPosition(objects[i]->x, objects[i]->y);
	objects.push_back(item);
	objects.erase(objects.begin() + i);

}
void CFire::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects,vector<LPGAMEOBJECT> &colidingObjects) {

	
}

void CFire::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + FIRE_BBOX_WIDTH;
	b = y + FIRE_BBOX_HEIGHT;
}
void CFire::SetState(int state)
{
	//DebugOut(L"state1: %d\n", this->state);
	CGameObject::SetState(state);
	//DebugOut(L"state2: %d\n", this->state);
	switch (state)
	{
	case FIRE_STATE_DIE:
		timeremove_fire = GetTickCount();

		//DebugOut(L"CFire::SetState: ");
		//x = 1000;
		//y = 1000;
	/*	vx = 0;
		vy = 0;*/
		break;
		/*case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED + 0.05f;*/
	}
	//DebugOut(L"state3: %d\n", this->state);
}
