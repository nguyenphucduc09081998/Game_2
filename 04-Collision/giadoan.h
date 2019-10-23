#pragma once
#pragma once
#pragma once
#pragma once
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Simon.h"
#include "Brick.h"
#include "BackGround.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Simon.h"
#include "Brick.h"
#include "BackGround.h"

class giadoan
{

	int giaidoan;
	CBackground *Background;
public:

	giadoan(
		int stage,
		CBackground *Background
		//int stageWidth

	);
	void Update(DWORD dt, float &cameraPosX, float &cameraPosY);
	void Render(float cameraPosX, float cameraPosY);
	//void Insert_gameObject(LPGAMEOBJECT go, vector<LPGAMEOBJECT> &allGameObject);
	~giadoan();

};

typedef giadoan * giaidoan;