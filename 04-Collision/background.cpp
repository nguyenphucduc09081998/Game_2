#include "background.h"

void RoundNumber(float &number) {
	int temp = number * 10000;
	number = temp / 10000;
}
void CBackground::Render(float &xcamera, float &ycamera)
{
	//animations[0]->Render(x - xcamera, y - ycamera);
	RenderBoundingBox(xcamera, ycamera);
	int fromIndex = (xcamera - 5) / 16;
	int toIndex = (xcamera + 5 + 260) / 16;
	fromIndex = fromIndex < 0 ? 0 : fromIndex;
	toIndex = toIndex > position[0].size() - 1 ? position[0].size() - 1 : toIndex;
	for (int i = 0; i < position.size(); i++) {
		for (int j = fromIndex; j <= toIndex; j++) {
			float drawX = j * 16 - xcamera;
			float drawY = i * 16 - ycamera;
			RoundNumber(drawX);
			RoundNumber(drawY);
			sprites[position[i][j]]->Draw(drawX, drawY + 32, 255);

		}

	}
}

void CBackground::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + Background_BBOX_WIDTH;
	b = y + Background_BBOX_HEIGHT;

}