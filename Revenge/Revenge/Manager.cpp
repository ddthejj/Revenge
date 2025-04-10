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
#include "ComponentManager.h"
#include "PhysicsManager.h"
#include "CharacterManager.h"

GAME_STATE Manager::gameState = GAME_STATE::STATE_TITLE;
SpriteBatch* Manager::spriteBatch = nullptr;
std::vector<Object*> Manager::UpdateList;
std::vector<Object*> Manager::DrawList;

bool Manager::fadingIn = false;
bool Manager::fadingOut = false;

Sprite* Manager::fadeRectangle = nullptr;

bool Manager::quit = false;

void Manager::ChangeGameState(GAME_STATE _gameState)
{

}

void Manager::Quit()
{
	Clean();
	quit = true;
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

void Manager::Init(HWND hwnd)
{
	// necessary managers
	InputManager::Init();
	SoundManager::Init(hwnd);
	MenuManager::Init();
	ComponentManager::Init();
	CharacterManager::Init();
	// rendering
	spriteBatch = new SpriteBatch(hwnd);

	InitGameState();
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
	TextureManager::LoadTextures(L"../Assets/TestTextures/TestTexture_Title_List.txt", spriteBatch);
	SoundManager::LoadSounds("../Assets/Sounds/MenuSounds.txt");
	TitleManager::Init();

	gameState = GAME_STATE::STATE_TITLE;

	SafeDelete(fadeRectangle);
	fadeRectangle = new Sprite("fadeRectangle", 0, 0, WIDTH, HEIGHT, TextureManager::GetTexture("BLACK"), 1.f, 0.f);
}

void Manager::InitOverworld()
{
	TextureManager::LoadTextures(L"../Assets/TestTextures/TestTexture_List.txt", spriteBatch);
	OverworldManager::Init();

	gameState = GAME_STATE::STATE_OVERWORLD;

	SafeDelete(fadeRectangle);
	fadeRectangle = new Sprite("fadeRectangle", 0, 0, WIDTH, HEIGHT, TextureManager::GetTexture("BLACK"), 1.f, 0.f);
}

void Manager::UnloadGameState()
{
	switch (gameState)
	{
	case GAME_STATE::STATE_OVERWORLD:
	{
		MenuManager::Clean();
		OverworldManager::Clean();
		break;
	}
	case GAME_STATE::STATE_TITLE:
	{
		MenuManager::Clean();
		TitleManager::Clean();
		break;
	}
	}

	TextureManager::UnloadTextures();
	SoundManager::UnloadSounds();

	DrawList.clear();
	UpdateList.clear();
}

void Manager::Clean()
{
	UnloadGameState();

	TextureManager::Clean();
	InputManager::Clean();
	SoundManager::Clean();
	PhysicsManager::Clean();
	CharacterManager::Clean();
	SafeDelete(fadeRectangle);
	SafeDelete(spriteBatch);

	DrawList.clear();
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

bool Manager::CanChangeRoom()
{
	if (fadingOut)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Manager::StartRoomChange()
{
	fadingOut = true;
}

FADE_STATUS Manager::FadeScene()
{
	if (fadingOut && fadeRectangle->GetOpacity() <= 0.f) // begin fade
	{
		fadeRectangle->Activate();
		fadeRectangle->IncreaseOpacity(ROOM_FADE_SPEED);
		return FADE_STATUS::FADE_START;
	}
	else if (fadingOut && fadeRectangle->GetOpacity() >= 1.f) // when fade out is done
	{
		fadingOut = false;
		fadingIn = true;
		fadeRectangle->DecreaseOpacity(ROOM_FADE_SPEED);
		return FADE_STATUS::FADE_SWITCH;
	}
	else if (fadingIn && fadeRectangle->GetOpacity() <= 0.f) // fading done
	{
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