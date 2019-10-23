#include "Brick.h"

void CBrick::Render(float &xcamera, float &ycamera)
{
	animations[0]->Render(x - xcamera, y - ycamera);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}




