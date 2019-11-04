

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include <string>
#include "Simon.h"
#include "Brick.h"
#include "Goomba.h"
#include "Background.h"
#include "fire.h"
#include "background.h"
#include "giadoan.h"
#include "MorningStar.h"
using namespace std;


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
CBrick *brick; 
CMorningStar * morningstar;
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
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_Z:
		//if(simon->y <= 121)
			
			simon->setcheckjump(true);
			simon->SetState(SIMON_STATE_JUMP);
		
		//simon->SetLevel(SIMON_LEVEL_BIG);
		break;
	//gsdg
	case DIK_X://attact
		simon->StartAttact();
		morningstar->StartMorningStar();
		simon->SetState(SIMON_STATE_ATTACT);
		//DebugOut(L"attatc: %d\n", simon->state);
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
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
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
vector<string> split(const string str)
{
	vector<string> tokens;
//	cout << "str: " << str << "\n";
	size_t prev = 0, pos = 0;
	do
	{
	//	cout << "do: \n";
		pos = str.find(" ", prev);
	//	cout << "pos: " << pos << "\n";
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + 1;
	} while (pos < str.length() && prev < str.length());
	return tokens;
}
void loadprites(CTextures * textures, CSprites * sprites, CAnimations * animations) {
	ifstream inFile;
	inFile.open(L"resource\\sprite.txt");//status{textures = 1, sprites = 2, ani = 3}
	if (inFile.is_open()) {
		string value; 
		int id_image = -1;
		while (getline(inFile,value)) {
			if (value == "") {
				continue;
			}
			//cout<< "value: " << value << "\n";
			vector<string> chuoi = split(value);
			int status = stoi(chuoi[0]);
			if (status == ID_textures) { //textures
				//format: action texId file_path
				int texId = stoi(chuoi[1]);
				string fileName = chuoi[2];
				//đổi lại định data type cho file name
				wstring stemp = wstring(fileName.begin(), fileName.end());
				LPCWSTR formatedFileName = stemp.c_str();
				//add texture
				textures->Add(texId, formatedFileName, D3DCOLOR_XRGB(255, 0, 255));
				id_image = texId;
			}
			if (status == ID_sprite) {// sprite
				//format: action sprite_id left top right bottom ....
				int spriteId = stoi(chuoi[1]);
				int left = stoi(chuoi[2]);
				int top = stoi(chuoi[3]);
				int right = stoi(chuoi[4]);
				int bottom = stoi(chuoi[5]);
				LPDIRECT3DTEXTURE9 textureHandle = textures->Get(id_image);
				sprites->Add(spriteId, left, top, right, bottom, textureHandle);
			}
			if (status == ID_ani) {// ani
				//format: action animation_id anmation_time sprite_id_1 sprite_id_2 sprite_id_3 ....
				int animationID = stoi(chuoi[1]);
				int animationTime = stoi(chuoi[2]);
				LPANIMATION ani;
				ani = new CAnimation(animationTime);
				for (int i = 3; i < chuoi.size(); i++) {
					ani->Add(stoi(chuoi[i]));
				}
				animations->Add(animationID, ani);
				//this->Add
			}
		}
	}
}
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	LPANIMATION ani;//khai bao
	loadprites(textures, sprites, animations);

	textures->Add(ID_TEX_FIRE, L"textures\\fire.png", D3DCOLOR_XRGB(0, 0, 0));//add fire

	LPDIRECT3DTEXTURE9 texfire = textures->Get(ID_TEX_FIRE);//add fire
	sprites->Add(15000, 1, 1, 17, 31, texfire);
	sprites->Add(15001, 28, 1, 43, 31, texfire);

	//
	ani = new CAnimation(100);	// fire
	ani->Add(15000);
	ani->Add(15001);
	animations->Add(150, ani);
	
	for (int i = 0; i < 5; i++) {//adđ fire
		CFire  *fire = new CFire();
		fire->AddAnimation(150);
		fire->SetPosition(80 + i * 130.0f, 145);
		objects.push_back(fire);
	}
	morningstar = new CMorningStar();

	simon = new CSimon(morningstar);
	objects.push_back(simon);

	
//	objects.push_back(morningstar);

	for (int i = 1; i < 10; i++)//add nền 
	{
		brick = new CBrick();
		brick->AddAnimation(900);
		brick->SetPosition(-height_width_brick, top_to_land - (i*height_width_brick));//150 là khoảng cách của nền vs top
	///	//objects.push_back(brick);
		//brick->AddAnimation(900);
		//brick->SetPosition(16, 176);//150 là khoảng cách của nền vs top
		objects.push_back(brick);
	}
	for (int i = 1; i < 10; i++)//add nền 
	{
		brick = new CBrick();
		brick->AddAnimation(900);
		brick->SetPosition(width_last_screen, top_to_land - (i*height_width_brick));//150 là khoảng cách của nền vs top
	///	//objects.push_back(brick);
		//brick->AddAnimation(900);
		//brick->SetPosition(16, 176);//150 là khoảng cách của nền vs top
		objects.push_back(brick);
	}

	for (int i = 0; i < 48; i++)//add nền 
	{
		brick = new CBrick();
		brick->AddAnimation(900);
		brick->SetPosition(0 + i * height_width_brick, top_to_land);//150 là khoảng cách của nền vs top
	///	//objects.push_back(brick);
		//brick->AddAnimation(900);
		//brick->SetPosition(16, 176);//150 là khoảng cách của nền vs top
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
	textures->Add(ID_TEX_BACKGROUND_1, L"textures\\tilemap\\map1.png", BACKGROUND_COLOR);
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
		float temp = (simon->x - SCREEN_WIDTH / 2 + 20);
		float x = temp;
		if (temp < width_last_screen_render) {
			 x = temp;
		}
		else {
			x = width_last_screen_render;
		}
			
		float y = 0;
		
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
			//background->Render(x,y);


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