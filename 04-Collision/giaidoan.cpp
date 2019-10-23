#include "giadoan.h"
#include <ctime>

giadoan::giadoan(int giaidoan, CBackground *Background)
{
	this->giaidoan = giaidoan;
	this->Background = Background;
}

void giadoan::Update(DWORD dt, float & cameraPosX, float & cameraPosY)
{

}

void giadoan::Render(float cameraPosX, float cameraPosY)
{
	if (cameraPosX < 0) {
		cameraPosX = 0;
	}
	Background->Render(cameraPosX, cameraPosY);
}




