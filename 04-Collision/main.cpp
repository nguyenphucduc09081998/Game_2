

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Simon.h"
#include "Brick.h"
#include "Goomba.h"
#include "Background.h"
#include "fire.h"
#include "background.h"
#include "giadoan.h"


#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"



#define MAX_FRAME_RATE 120 // vẽ tối đa fram trong 1 giây, 

#define ID_TEX_SIMON 0
#define ID_TEX_FIRE 5//add fire
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_BACKGROUND 40

CGame *game;
CSimon *simon;
CGoomba *goomba;
CBackground *background;
giadoan *giadoan1;
vector<LPGAMEOBJECT> objects;
vector<giaidoan> giadoans;

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	
	case DIK_Z:
		//if(simon->y <= 121)
			simon->SetState(SIMON_STATE_JUMP);
		
		//simon->SetLevel(SIMON_LEVEL_BIG);
		break;
	//gsdg
	case DIK_X://attact
		simon->SetState(SIMON_STATE_ATTACT);
		//simon->SetLevel(SIMON_LEVEL_ATTACT);
		break;
	//dgsdfg
	case DIK_A: // reset
		simon->SetState(SIMON_STATE_IDLE);
		simon->SetLevel(SIMON_LEVEL_BIG);
		simon->SetPosition(50.0f, 0.0f);
		simon->SetSpeed(0, 0);
		break;
	
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// disable control key when Mario die 
	//if (simon->GetState() == SIMON_STATE_DIE) return;
	
	if (game->IsKeyDown(DIK_RIGHT)) {
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
		//simon->SetLevel(SIMON_LEVEL_BIG);
	}
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
		//simon->SetLevel(SIMON_LEVEL_ATTACT);
	//
	//else if (game->IsKeyDown(DIK_X))
	//	simon->SetState(SIMON_STATE_ATTACT);
	//
	else if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(SIMON_STATE_DOWN);
		//simon->SetLevel(SIMON_LEVEL_ATTACT);
	else 
		simon->SetState(SIMON_STATE_IDLE);
		//simon->SetLevel(SIMON_LEVEL_ATTACT);
	

}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	
	//textures->Add(ID_TEX_BACKGROUND, L"textures\\background.png", D3DCOLOR_XRGB(0, 0, 0));//add background
	textures->Add(ID_TEX_SIMON, L"textures\\simon.png", D3DCOLOR_XRGB(128, 128, 128));//add simon
	textures->Add(ID_TEX_MISC, L"textures\\2.png", D3DCOLOR_XRGB(176, 224, 248));//
	textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_FIRE, L"textures\\fire.png", D3DCOLOR_XRGB(0, 0, 0));//add fire
	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	

	//LPDIRECT3DTEXTURE9 texbackground = textures->Get(ID_TEX_BACKGROUND);//add background
	//sprites->Add(11000, 1, 1, 767, 183, texbackground);

	LPDIRECT3DTEXTURE9 texfire = textures->Get(ID_TEX_FIRE);//add fire
	sprites->Add(15000, 1, 1, 17, 31, texfire);
	sprites->Add(15001, 28, 1, 43, 31, texfire);

	//
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	sprites->Add(10003, 682, 9, 696, 40, texSimon);
	sprites->Add(10002, 708, 8, 721, 39, texSimon);		// walk
	sprites->Add(10001, 734, 9, 750, 39, texSimon);	// idle right
	sprites->Add(10004, 652, 9, 668, 40, texSimon);	//down right

	sprites->Add(12000, 735, 10, 751, 40, texSimon);	//jump right
	sprites->Add(12001, 652, 17, 668, 40, texSimon);
	
	

	sprites->Add(10011, 80, 10, 97, 40, texSimon);		// idle left
	sprites->Add(10012, 109, 9, 121, 39, texSimon);		// walk
	sprites->Add(10013, 134, 9, 150, 40, texSimon);
	sprites->Add(10014, 163, 9, 180, 40, texSimon);		//down left

	sprites->Add(13000, 80, 9, 97, 40, texSimon); //jump left
	sprites->Add(13001, 164, 17, 180, 40, texSimon);
	//sdfg
	sprites->Add(20001, 60, 50, 85, 85, texSimon);		//attact left
	sprites->Add(20002, 106, 50, 127, 85, texSimon);		
	sprites->Add(20003, 150, 50, 177, 85, texSimon);

	sprites->Add(20011, 743, 50, 773, 85, texSimon);		//attact right
	sprites->Add(20012, 700, 50, 725, 85, texSimon);
	sprites->Add(20013, 652, 50, 680, 85, texSimon);
	//sdfg
	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(40001, 0, 0, 31, 31, texMisc);

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
	sprites->Add(30001, 5, 14, 21, 29, texEnemy);
	sprites->Add(30002, 25, 14, 41, 29, texEnemy);
	sprites->Add(30003, 45, 21, 61, 29, texEnemy); // die sprite

	LPANIMATION ani;//khai bao

	ani = new CAnimation(100);	// fire
	ani->Add(15000);
	ani->Add(15001);
	animations->Add(150, ani);

	//ani = new CAnimation(100);	// background
	//ani->Add(11000);
	//animations->Add(110, ani);

	//fsd
	ani = new CAnimation(100);	// jump right
	ani->Add(12000);
	ani->Add(12001);
	ani->Add(12001);
	ani->Add(12000);
	animations->Add(120, ani);
	ani = new CAnimation(100);	// jump left
	ani->Add(13000);
	ani->Add(13001);
	ani->Add(13000);
	ani->Add(13001);
	animations->Add(130, ani);

	ani = new CAnimation(100);	// attact left
	ani->Add(20001);
	ani->Add(20002);
	ani->Add(20003);
	animations->Add(300, ani);
	ani = new CAnimation(100);	// attact right
	ani->Add(20011);
	ani->Add(20012);
	ani->Add(20013);
	animations->Add(310, ani);
	//fdsdgf

	ani = new CAnimation(100);	// idle big right
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	// walk right big
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new CAnimation(100);	// ngoi phai
	ani->Add(10004);
	animations->Add(402, ani);

	ani = new CAnimation(100);	// idle big left
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(100);	// // walk left big
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	ani = new CAnimation(100);	// ngoi trai
	ani->Add(10014);
	animations->Add(403, ani);

	ani = new CAnimation(100);		// brick
	ani->Add(40001);
	animations->Add(601, ani);

	ani = new CAnimation(300);		// Goomba walk
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(701, ani);

	ani = new CAnimation(1000);		// Goomba dead
	ani->Add(30003);
	animations->Add(702, ani);
	//background
	//CBackground *background = new CBackground();
	//background->AddAnimation(110);
	//objects.push_back(background);

	simon = new CSimon();
	simon->AddAnimation(400);		// dung tai cho phai            0
	simon->AddAnimation(401);		// dung tai cho trai               1 
 	simon->AddAnimation(402);		// ngoi phai                      2
 	simon->AddAnimation(403);		// ngoi trai                         3
	simon->AddAnimation(500);		// di phai               4
	simon->AddAnimation(501);		// di trai         5
	simon->AddAnimation(300);		//attact left	6
	simon->AddAnimation(310);     //attact right	7
	simon->AddAnimation(120);		//	jump right 8
	simon->AddAnimation(130);     //jump left 9
	//simon->AddAnimation(599);		// died                 6
	simon->SetPosition(50.0f, 0);
	objects.push_back(simon);


	//for (int i = 0; i < 5; i++) {//adđ fire
	//	CFire  *fire = new CFire();
	//	fire->AddAnimation(150);
	//	fire->SetPosition(80 + i*  130.0f, 152);
	//	objects.push_back(fire);
	//}
	


	//for (int i = 0; i < 5; i++)//tạo brick
	//{
	//	CBrick *brick = new CBrick();
	//	brick->AddAnimation(601);
	//	brick->SetPosition(100 + i * 48.0f, 74);
	//	objects.push_back(brick);

	//	brick = new CBrick();
	//	brick->AddAnimation(601);
	//	brick->SetPosition(100 + i * 48.0f, 90);
	//	objects.push_back(brick);

	//	brick = new CBrick();
	//	brick->AddAnimation(601);
	//	brick->SetPosition(84 + i * 48.0f, 90);
	//	objects.push_back(brick);
	//}


	for (int i = 0; i < 48; i++)//add nền 
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(0 + i * 16.0f, 176);//150 là khoảng cách của nền vs top
		objects.push_back(brick);
	}

	// and Goombas 
	//for (int i = 0; i < 5; i++)//số lượng goomba xuất hiện
	//{
	//	goomba = new CGoomba();
	//	goomba->AddAnimation(701);
	//	goomba->AddAnimation(702);
	//	goomba->SetPosition(200 + i * 60, 135);
	//	goomba->SetState(GOOMBA_STATE_WALKING);//zzz
	//	objects.push_back(goomba);
	//}
	ifstream inFile;
	inFile.open(L"textures\\tilemap\\map1.txt");
	int value;
	vector<int> temp1;
	vector<vector<int>> Stage1Pos;
	while (inFile >> value) {
		temp1.push_back(value);
		if (temp1.size() == so_Ovuong) {
			Stage1Pos.push_back(temp1);
			temp1.clear();
		}
	}
	textures->Add(ID_TEX_BACKGROUND_1, L"textures\\tilemap\\map1.png", D3DCOLOR_XRGB(255, 255, 255));
	LPDIRECT3DTEXTURE9 texBackGround = textures->Get(ID_TEX_BACKGROUND_1);
	vector<LPSPRITE> Stage1Sprite;
	for (int i = 0; i < 47; i++) {
		sprites->Add(30000 + i, i * 16, 0, (i + 1) * 16, 16, texBackGround);
		Stage1Sprite.push_back(sprites->Get(30000 + i));
	}
	background = new CBackground(Stage1Sprite, Stage1Pos);
	giadoan1 = new giadoan(1, background);
	giadoans.push_back(giadoan1);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);

	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR); // tô màu màn hình.

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);//màu nào mà trong xuốt thì đừng vẽ
		float x = simon->x - SCREEN_WIDTH / 2;
		float y = 0;
		DebugOut(L"[INFO] xxxx = %f\n", x);
		giadoans[0]->Render(x, y);

		for (int i = 0; i < objects.size(); i++) {
			//background->
		   //	void width_map = background->GetBoundingBox();
			   //DebugOut(L"[INFO] Main.cpp->Render()->for: %f\n", x);
			float left, top, right, bottom;
			if (x < 0) {
				x = 0;
			}

			objects[i]->Render(x, y);
			//objects[1]->Render(x, y);
		//	background->Render(x,y);


		}
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;// dt là thời gian bắt đầu vẽ đến lúc hết vẽ.
		
		if (dt >= tickPerFrame)// thời gian vẽ một frame kể cả lúc sleep
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);// thời gian đã trôi qua kể từ lúc bất đầu 
			Render();
		}
		else
			Sleep(tickPerFrame - dt);//vẽ xong một fram thì cho đi ngủ để chờ vẽ tiếp nhằm tiết kiệm CPU, 

	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance(); //
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}