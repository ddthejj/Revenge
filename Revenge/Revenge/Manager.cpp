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
#include "TitleManager.h"
#include "TextureManager.h"
#include "SoundManager.h"

GAME_STATE Manager::gameState = GAME_STATE::STATE_OVERWORLD;
SpriteBatch* Manager::spriteBatch = nullptr;
std::vector<Object*> Manager::UpdateList;
std::vector<Object*> Manager::DrawList;

bool Manager::fadingIn = false;
bool Manager::fadingOut = false;

Sprite* Manager::fadeRectangle = nullptr;


const std::vector<Character*> Manager::GetParty() 
{ 
	return OverworldManager::GetCurrentParty();
}

const Texture* Manager::GetTexture(int index)
{
	return TextureManager::GetTexture(index);
}

const Texture* Manager::GetTexture(const char* name)
{
	return TextureManager::GetTexture(name);
}

void Manager::ChangeGameState(GAME_STATE _gameState)
{

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

void Manager::FocusLost()
{
	InputManager::FocusLost();
}

int Manager::GetScreenWidth()
{
	if (spriteBatch)
		return spriteBatch->ScreenWidth();
	else
		return 0;
}

int Manager::GetScreenHeight()
{
	if (spriteBatch)
		return spriteBatch->ScreenHeight();
	else
		return 0;
}


void Manager::PlayWAV(int index)
{
	SoundManager::Play(index);
}


void Manager::Init(HWND hwnd)
{
	// init managers that will be used no matter what
	InputManager::Init();
	SoundManager::Init(hwnd);
	MenuManager::Init();
	// spritebatch
	spriteBatch = new SpriteBatch(hwnd);

	InitGameState();

	// create the rectangle used for fading the screen in and out
	//fadeRectangle = new Sprite("fadeRectangle", 0, 0, WIDTH, HEIGHT, GetTexture("BLACK"), 1.f, 0.f);

}

void Manager::InitGameState()
{
	switch (gameState)
	{
	case GAME_STATE::STATE_TITLE:
		InitTitle();
		break;
	case GAME_STATE::STATE_OVERWORLD:
		InitOverworld();
		break;
	}
}

void Manager::InitTitle()
{
	// load title textures
	TextureManager::LoadTextures(L"../Assets/TestTextures/TestTexture_Title_List.txt", spriteBatch);

	// load title sounds
	SoundManager::LoadSounds("../Assets/Sounds/MenuSounds.txt");

	// init managers for title
	TitleManager::Init();

	// ensure game state is correct
	gameState = GAME_STATE::STATE_TITLE;

	SafeDelete(fadeRectangle);
	fadeRectangle = new Sprite("fadeRectangle", 0, 0, WIDTH, HEIGHT, GetTexture("BLACK"), 1.f, 0.f);
}

void Manager::InitOverworld()
{
	// load overworld textures
	TextureManager::LoadTextures(L"../Assets/TestTextures/TestTexture_List.txt", spriteBatch);
	// load overworld menus
	MenuManager::LoadOverworldMenus();
	// load the overworld
	OverworldManager::Init();

	gameState = GAME_STATE::STATE_OVERWORLD;

	SafeDelete(fadeRectangle);
	fadeRectangle = new Sprite("fadeRectangle", 0, 0, WIDTH, HEIGHT, GetTexture("BLACK"), 1.f, 0.f);
}

void Manager::UnloadGameState()
{
	switch (gameState)
	{
	case GAME_STATE::STATE_OVERWORLD:
	{
		// menu
		MenuManager::Clean();
		// overworld 
		OverworldManager::Clean();
		break;
	}
	case GAME_STATE::STATE_TITLE:
	{
		// menu
		MenuManager::Clean();
		// title
		TitleManager::Clean();
		break;
	}
	}

	TextureManager::UnloadTextures();
	SoundManager::UnloadSounds();

	// draw list (probably unesseccary)
	DrawList.clear();
	// update list (probably unesseccary)
	UpdateList.clear();
}

void Manager::Clean()
{
	UnloadGameState();

	// textures
	TextureManager::Clean();
	// input
	InputManager::Clean();
	// sound
	SoundManager::Clean();
	// fade rectangle
	SafeDelete(fadeRectangle);
	// spritebatch 
	SafeDelete(spriteBatch);

	// draw list (probably unesseccary)
	DrawList.clear();
	// update list (probably unesseccary)
	UpdateList.clear();
}

bool Manager::AddUpdate(Object* that)
{
	UpdateList.push_back(that);
	return true;
}

bool Manager::RemoveUpdate(Object* that)
{
	for (std::vector<Object*>::iterator it = UpdateList.begin(); it != UpdateList.end(); ++it)
	{
		if (*it == that)
		{
			UpdateList.erase(it);
			return true;
		}
	}
	return false;
}

bool Manager::AddDraw(Object* that)
{
	DrawList.push_back(that);
	return true;
}

bool Manager::RemoveDraw(Object* that)
{
	for (std::vector< Object*>::iterator it = DrawList.begin(); it != DrawList.end(); ++it)
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
		fadeRectangle->IncreaseOpacity(ROOM_FADE_SPEED);
		return FADE_STATUS::FADE_START;
	}
	else if (fadingOut && fadeRectangle->Opacity() >= 1.f)
	{
		// when fade out is done
		fadingOut = false;
		fadingIn = true;
		fadeRectangle->DecreaseOpacity(ROOM_FADE_SPEED);
		return FADE_STATUS::FADE_SWITCH;
	}
	else if (fadingIn && fadeRectangle->Opacity() <= 0.f)
	{
		// fading done
		fadingIn = false;
		fadeRectangle->Deactivate();
		return FADE_STATUS::FADE_DONE;
	}
	else if (fadingOut)
	{
		// fade out slowly
		fadeRectangle->IncreaseOpacity(ROOM_FADE_SPEED);
		return FADE_STATUS::FADE_OUT;
	}
	else if (fadingIn)
	{
		// fade in slowly
		fadeRectangle->DecreaseOpacity(ROOM_FADE_SPEED);
		return FADE_STATUS::FADE_IN;
	}
	else
		return FADE_STATUS::FADE_DONE;
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

	for (std::vector<Object*>::iterator it = UpdateList.begin(); it != UpdateList.end(); ++it)
	{
		Object* object = (*it);
		object->Update(delta_time);
	}
	if (gameState == GAME_STATE::STATE_OVERWORLD)
	{
	OverworldManager::Update(delta_time);
	}
	else if (gameState == GAME_STATE::STATE_TITLE)
	{
		TitleManager::Update(delta_time);
	}
	InputManager::Update(delta_time);
}

void Manager::Draw()
{
	spriteBatch->Begin();

	for (std::vector< Object*>::iterator it = DrawList.begin(); it != DrawList.end(); ++it)
	{
		(*it)->Draw(spriteBatch);
	}
	spriteBatch->End();
}