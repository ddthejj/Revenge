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

SpriteBatch* Manager::spriteBatch = nullptr;
Texture* Manager::textures[TEX_MAX];
ProtoTile* Manager::protoTiles[TILE_MAX];

std::vector<Sprite*> Manager::UpdateList;
std::vector<Sprite*> Manager::DrawList;

std::vector<Map*> Manager::maps;
Map* Manager::currentMap;
Player* Manager::currentPlayer;

bool Manager::keys[KEY_MAX];
bool Manager::previousKeys[KEY_MAX];
char Manager::keyOptions[KEY_MAX];

bool Manager::fadingIn = false;
bool Manager::fadingOut = false;
Door* Manager::doorHit = nullptr;

Sprite* Manager::fadeRectangle = nullptr;

ProtoTile* Manager::GetProtoTile(int index)
{
	if (index < TILES::TILE_MAX)
		return protoTiles[index];
	else
		return nullptr;
}

Room * Manager::GetCurrentRoom()
{
	return currentMap->CurrentRoom();
}

Room* Manager::GetRoom(int index)
{
	return currentMap->GetRoom(index);
}

bool Manager::IsKeyPressed(KEYS index)
{
	if (keys[index] == true && previousKeys[index] == false)
		return true;

	return false;
}


void Manager::MoveMouse(HWND hwnd, LPARAM lParam)
{
}

void Manager::PressKey(WPARAM wParam)
{
	for (int key = 0; key < KEY_MAX; key++)
	{
		if (wParam == keyOptions[key])
		{
			keys[key] = true;
			break;
		}
	}
}

void Manager::ReleaseKey(WPARAM wParam)
{
	for (int key = 0; key < KEY_MAX; key++)
	{
		if (wParam == keyOptions[key])
		{
			keys[key] = false;
			break;
		}
	}
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
	// key options
	keyOptions[KEY_UP] = 'W';
	keyOptions[KEY_DOWN] = 'S';
	keyOptions[KEY_LEFT] = 'A';
	keyOptions[KEY_RIGHT] = 'D';
	keyOptions[KEY_MENU] = 'E';
	keyOptions[KEY_INTERACT] = VK_SPACE;
	// spritebatch
	spriteBatch = new SpriteBatch(hwnd);
	// textures
	textures[TEX_BLACK] = spriteBatch->Load(L"../Assets/TestTextures/Black.png", 32, 32);
	textures[TEX_MENU] = spriteBatch->Load(L"../Assets/TestTextures/TextBox.png", 10, 10 * 3);
	textures[TEX_BROWNFLOOR] = spriteBatch->Load(L"../Assets/TestTextures/BrownTile.png", 32, 32);
	textures[TEX_REDWALL] = spriteBatch->Load(L"../Assets/TestTextures/RedTile.png", 32, 32);
	textures[TEX_GREENDOOR] = spriteBatch->Load(L"../Assets/TestTextures/GreenTile.png", 32, 32);
	textures[TEX_PLAYER] = spriteBatch->Load(L"../Assets/TestTextures/Player_Spritesheet.png", 32 * 4, 32 * 4);
	textures[TEX_ARROW] = spriteBatch->Load(L"../Assets/TestTextures/Arrow.png", 15, 30);
	// prototype tiles
	protoTiles[TILE_BROWNFLOOR] = new ProtoTile(textures[TEX_BROWNFLOOR], 32, 32);
	protoTiles[TILE_REDWALL] = new ProtoTile(textures[TEX_REDWALL], 32, 32, true);
	protoTiles[TILE_GREENDOOR] = new ProtoTile(textures[TEX_GREENDOOR], 32, 32, false, true);

	// test room 1 
	//rooms.push_back(new Room("../RoomData/TestRoom/TestRoom0.txt"));
	//rooms.push_back(new Room("../RoomData/TestRoom/TestRoom1.txt"));
	//currentMap = rooms[0];
	//currentMap->Activate();
	maps.push_back(new Map("../Assets/RoomData/TestRoom/TestRoom", 2));
	currentMap = maps[0];
	currentMap->Activate();
	// test player
	currentPlayer = new Player(240, 240, 32, 32, textures[TEX_PLAYER], .6f);
	currentPlayer->Activate();
	// fade rectangle
	fadeRectangle = new Sprite(0, 0, WIDTH, HEIGHT, textures[TEX_BLACK], 1.f, 0.f);

	MenuManager::Init();
}

void Manager::Clean()
{
	// menu
	MenuManager::Clean();
	// fade rectangle
	delete fadeRectangle;
	// test player
	delete currentPlayer;
	// test rooms
	for (int i = 0; i < maps.size(); i++)
		delete maps.at(i);
	maps.clear();
	// spritebatch 
	delete spriteBatch;
	// prototype tiles
	for (int i = 0; i < TILE_MAX; i++)
	{
		delete protoTiles[i];
	}
	// textures
	for (int i = 0; i < TEX_MAX; i++)
	{
		delete textures[i];
	}

	// draw list (probably unesseccary)
	DrawList.clear();
	// update list (probably unesseccary)
	UpdateList.clear();
}

bool Manager::AddUpdate(Sprite*that)
{
	UpdateList.push_back(that);
	return true;
}

bool Manager::RemoveUpdate(Sprite*that)
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

bool Manager::AddDraw(Sprite*that)
{
	DrawList.push_back(that);
	return true;
}

bool Manager::RemoveDraw(Sprite*that)
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
	doorHit = hit;
	fadingOut = true;
}

void Manager::TransitionRoom()
{
	if (fadingOut && fadeRectangle->opacity <= 0.f)
	{
		// begin fade
		fadeRectangle->Activate();
		currentMap->Freeze();
		currentPlayer->Freeze();
		fadeRectangle->opacity += ROOM_FADE_SPEED;
	}
	else if (fadingOut && fadeRectangle->opacity >= 1.f)
	{
		// when fade out is done
		currentMap->Deactivate();
		currentMap->SetRoom(doorHit->Destination());
		currentPlayer->GetRectangle()->SetX(doorHit->DestinationX());
		currentPlayer->GetRectangle()->SetY(doorHit->DestinationY());
		currentMap->Activate();
		currentMap->Freeze();
		fadingOut = false;
		fadingIn = true;
		fadeRectangle->opacity -= ROOM_FADE_SPEED;
	}
	else if (fadingIn && fadeRectangle->opacity <= 0.f)
	{
		// fading done
		fadingIn = false;
		currentMap->Unfreeze();
		currentPlayer->Unfreeze();
		fadeRectangle->Deactivate();
	}
	else if (fadingOut)
	{
		// fade out slowly
		fadeRectangle->opacity += ROOM_FADE_SPEED;
	}
	else if (fadingIn)
	{
		fadeRectangle->opacity -= ROOM_FADE_SPEED;
		// fade in slowly
	}

}

void Manager::FreezeScene()
{
	currentMap->Freeze();
	currentPlayer->Freeze();
}

void Manager::UnfreezeScene()
{
	currentPlayer->Unfreeze();
	currentMap->Unfreeze();
}

void Manager::Update(float delta_time)
{
	MenuManager::Update(delta_time);

	if (fadingIn || fadingOut)
		TransitionRoom();

	for (std::vector<Sprite*>::iterator it = UpdateList.begin(); it != UpdateList.end(); ++it)
	{
		Sprite* sprite = (*it);
		sprite->Update();
	}
	CenterCamera(currentPlayer->GetRectangle()->CenterX(), currentPlayer->GetRectangle()->CenterY());

	for (int i = 0; i < KEY_MAX; i++)
		previousKeys[i] = keys[i];
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