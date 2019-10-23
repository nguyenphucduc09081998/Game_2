#include "fire.h"

void CFire::Render(float &xcamera, float&ycamera)
{
	animations[0]->Render(x - xcamera, y - ycamera);
	//RenderBoundingBox();
}

void CFire::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + FIRE_BBOX_WIDTH;
	b = y + FIRE_BBOX_HEIGHT;
}