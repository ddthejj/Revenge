#pragma once
#include "defines.h"
#include "Manager.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Room.h"
#include "Tile.h"
#include "Character.h"
#include "Map.h"
#include "MenuBox.h"
#include "MenuManager.h"
#include "InputManager.h"
#include "OverworldManager.h"
#include "TextureManager.h"

SpriteBatch* Manager::spriteBatch = nullptr;
//Texture* Manager::textures[TEX_MAX];
//ProtoTile* Manager::protoTiles[TILE_MAX];

std::vector<Sprite*> Manager::UpdateList;
std::vector<Sprite*> Manager::DrawList;
std::vector<Character*> Manager::party;

//std::vector<Map*> Manager::maps;
//Map* Manager::currentMap;
//Player* Manager::currentPlayer = nullptr;

bool Manager::fadingIn = false;
bool Manager::fadingOut = false;

Sprite* Manager::fadeRectangle = nullptr;

//ProtoTile* Manager::GetProtoTile(int index)
//{
//	return OverworldManager::GetProtoTile(index);
//}

//Room* Manager::GetRoom(int index)
//{
//	return OverworldManager::GetRoom(index);
//}

//Room* Manager::GetCurrentRoom()
//{
//	return OverworldManager::GetCurrentRoom();
//}

///Player* Manager::GetCurrentPlayer()
///{
///	return OverworldManager::GetCurrentPlayer();
///}

Texture* Manager::GetTexture(int index)
{
	return TextureManager::GetTexture(index);
}

Texture* Manager::GetTexture(const char* name)
{
	return TextureManager::GetTexture(name);
}

bool Manager::IsKeyDown(KEYS index)
{
	return InputManager::IsKeyDown(index);
}

bool Manager::IsPreviousKeyDown(KEYS index)
{
	return InputManager::IsPreviousKeyDown(index);
}

bool Manager::IsKeyPressed(KEYS index)
{
	return InputManager::IsKeyPressed(index);
}

bool Manager::IsMouseKeyDown(MOUSE_KEYS index)
{
	return InputManager::IsMouseKeyDown(index);
}

bool Manager::IsPreviousMouseKeyDown(MOUSE_KEYS index)
{
	return InputManager::IsPreviousMouseKeyDown(index);
}

bool Manager::IsMouseKeyPressed(MOUSE_KEYS index)
{
	return InputManager::IsMouseKeyPressed(index);
}

char Manager::CharPressed()
{
	return InputManager::CharPressed();
}

float Manager::GetMouseX()
{
	return InputManager::GetMouseX();
}

float Manager::GetMouseY()
{
	return InputManager::GetMouseY();
}

Point<float> Manager::GetMousePosition()
{
	return InputManager::GetMousePos();
}


void Manager::MoveMouse(HWND hwnd, LPARAM lParam)
{

}

void Manager::PressKey(WPARAM wParam)
{
	InputManager::PressKey(wParam);
}

void Manager::ReleaseKey(WPARAM wParam)
{
	InputManager::ReleaseKey(wParam);
}

void Manager::RepeatKey()
{
	InputManager::RepeatKey();
}

void Manager::PressChar(WPARAM wParam)
{
	InputManager::PressChar(wParam);
}

void Manager::PressMouseKey(MOUSE_KEYS key)
{
	InputManager::PressMouseKey(key);
}

void Manager::ReleaseMouseKey(MOUSE_KEYS key)
{
	InputManager::ReleaseMouseKey(key);
}

void Manager::ResizeWindow(HWND hWnd)
{
	if (spriteBatch)
		spriteBatch->Resize(hWnd);

	RECT rc;
	GetClientRect(hWnd, &rc);
}


void Manager::Init(HWND hwnd)
{
	InputManager::Init();
	// spritebatch
	spriteBatch = new SpriteBatch(hwnd);
	// textures
	TextureManager::LoadTextures(L"../Assets/TestTextures/TestTexture_List.txt", spriteBatch);

	/*
	textures[TEX_BLACK] = spriteBatch->Load(L"../Assets/TestTextures/Black.png", 32, 32);
	textures[TEX_MENU] = spriteBatch->Load(L"../Assets/TestTextures/TextBox.png", 10 * 4, 10 * 3);
	textures[TEX_TESTROOM] = spriteBatch->Load(L"../Assets/TestTextures/TestRoom_Spritesheet.png", 128, 128);

	textures[TEX_BROWNFLOOR] = new Texture(textures[TEX_TESTROOM], MyRectangle(0, 0, 32, 32));
	textures[TEX_REDWALL] = new Texture(textures[TEX_TESTROOM], MyRectangle(32, 0, 32, 32));
	textures[TEX_GREENDOOR] = new Texture(textures[TEX_TESTROOM], MyRectangle(64, 0, 32, 32));
	textures[TEX_BLUETEXT] = new Texture(textures[TEX_TESTROOM], MyRectangle(96, 0, 32, 32));

	textures[TEX_PLAYER] = spriteBatch->Load(L"../Assets/TestTextures/Player_Spritesheet.png", 32 * 4, 32 * 4);
	textures[TEX_ARROW] = spriteBatch->Load(L"../Assets/TestTextures/Arrow.png", 15, 30);
	*/

	InitOverworld();
}

void Manager::InitOverworld()
{
	// load the overworld
	OverworldManager::Init();

	fadeRectangle = new Sprite(0, 0, WIDTH, HEIGHT, GetTexture("BLACK"), 1.f, 0.f);
	party.push_back(OverworldManager::GetCurrentPlayer());

	MenuManager::Init();
}

void Manager::Clean()
{
	// menu
	MenuManager::Clean();
	// overworld 
	OverworldManager::Clean();
	// textures
	TextureManager::Clean();
	// fade rectangle
	delete fadeRectangle;
	// spritebatch 
	delete spriteBatch;
	// textures
	//for (int i = 0; i < TEX_MAX; i++)
	//{
	//	delete textures[i];
	//}

	// draw list (probably unesseccary)
	DrawList.clear();
	// update list (probably unesseccary)
	UpdateList.clear();
}

bool Manager::AddUpdate(Sprite* that)
{
	UpdateList.push_back(that);
	return true;
}

bool Manager::RemoveUpdate(Sprite* that)
{
	for (std::vector<Sprite*>::iterator it = UpdateList.begin(); it != UpdateList.end(); ++it)
	{
		if (*it == that)
		{
			UpdateList.erase(it);
			return true;
		}
	}
	return false;
}

bool Manager::AddDraw(Sprite* that)
{
	DrawList.push_back(that);
	return true;
}

bool Manager::RemoveDraw(Sprite* that)
{
	for (std::vector< Sprite*>::iterator it = DrawList.begin(); it != DrawList.end(); ++it)
	{
		if (*it == that)
		{
			DrawList.erase(it);
			return true;
		}
	}
	return false;
}


void Manager::CenterCamera(float x, float y)
{
	spriteBatch->SetCamera(x, y);
}

void Manager::HitDoor(Door* hit)
{
	if (fadingOut)
		return;
	//doorHit = hit;
	OverworldManager::HitDoor(hit);
	fadingOut = true;
}

FADE_STATUS Manager::FadeScene()
{
	if (fadingOut && fadeRectangle->Opacity() <= 0.f)
	{
		// begin fade
		fadeRectangle->Activate();
		//currentMap->Freeze();
		//currentPlayer->Freeze();
		fadeRectangle->IncreaseOpacity(ROOM_FADE_SPEED);
		return FADE_START;
	}
	else if (fadingOut && fadeRectangle->Opacity() >= 1.f)
	{
		// when fade out is done
		//currentMap->Deactivate();
		//currentMap->SetRoom(doorHit->Destination());
		//currentPlayer->GetRectangle()->SetX(doorHit->DestinationX());
		//currentPlayer->GetRectangle()->SetY(doorHit->DestinationY());
		//currentMap->Activate();
		//currentMap->Freeze();
		fadingOut = false;
		fadingIn = true;
		fadeRectangle->DecreaseOpacity(ROOM_FADE_SPEED);
		return FADE_SWITCH;
	}
	else if (fadingIn && fadeRectangle->Opacity() <= 0.f)
	{
		// fading done
		fadingIn = false;
		//currentMap->Unfreeze();
		//currentPlayer->Unfreeze();
		fadeRectangle->Deactivate();
		return FADE_DONE;
	}
	else if (fadingOut)
	{
		// fade out slowly
		fadeRectangle->IncreaseOpacity(ROOM_FADE_SPEED);
		return FADE_OUT;
	}
	else if (fadingIn)
	{
		// fade in slowly
		fadeRectangle->DecreaseOpacity(ROOM_FADE_SPEED);
		return FADE_IN;
	}
	else
		return FADE_DONE;
}

Point<float> Manager::MeasureString(std::string text)
{
	return spriteBatch->MeasureString(text);
}


void Manager::FreezeScene()
{
	OverworldManager::FreezeScene();
}

void Manager::UnfreezeScene()
{
	OverworldManager::UnfreezeScene();
}

void Manager::Update(float delta_time)
{
	MenuManager::Update(delta_time);

	for (std::vector<Sprite*>::iterator it = UpdateList.begin(); it != UpdateList.end(); ++it)
	{
		Sprite* sprite = (*it);
		sprite->Update();
	}
	OverworldManager::Update(delta_time);
	InputManager::Update(delta_time);
}

void Manager::Draw()
{
	spriteBatch->Begin();
	// test
	//MyRectangle* test = new MyRectangle(100, 100, 32, 32);
	//spriteBatch->Draw(textures[0], test);

	for (std::vector< Sprite*>::iterator it = DrawList.begin(); it != DrawList.end(); ++it)
	{
		(*it)->Draw(spriteBatch);
	}
	spriteBatch->End();

	//delete test;
}
