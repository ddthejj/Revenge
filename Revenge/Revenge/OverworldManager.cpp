#include "defines.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "OverworldManager.h"
#include "SaveManager.h"
#include "ComponentManager.h"

#include "Map.h"
#include "TestMaps.h"
#include "Room.h"
#include "Tile.h"
#include "PlayerCharacters.h"
#include "InteractionComponent.h"
#include "Rectangle.h"

#include <cmath>

std::vector<ProtoTile*> OverworldManager::protoTiles;
std::vector<Map*> OverworldManager::maps;

Map* OverworldManager::currentMap = nullptr;
OverworldManager::DoorData OverworldManager::doorHit(0,0,0);
Player* OverworldManager::currentPlayer = nullptr;
std::vector<Character*> OverworldManager::party;

bool OverworldManager::isRoomTransitioning = false;


void OverworldManager::Init()
{
	// test player
	currentPlayer = new PC_Archer("Player", 240, 240, 32, 32, TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_PLAYER), .6f);
	currentPlayer->Activate();
	// prototype tiles
	protoTiles.push_back(new ProtoTile(TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_BROWNFLOOR), 32, 32));
	protoTiles.push_back(new ProtoTile(TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_REDWALL), 32, 32, true));
	protoTiles.push_back(new ProtoTile(TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_GREENDOOR), 32, 32, false, true));
	protoTiles.push_back(new ProtoTile(TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_BLUETEXT), 32, 32, true, false, true));
	// add maps
	maps.push_back(new TestMap0());
	currentMap = maps[0];
	currentMap->Activate();
	currentMap->SetRoom(0);

	party.push_back(currentPlayer);

	SaveManager::OpenSave(0);
	SaveManager::Save();
}

void OverworldManager::Clean()
{
	// prototype tiles
	for (int i = 0; i < (int)TILES::TILE_MAX; i++)
	{
		delete protoTiles[i];
	}
	// test rooms
	for (int i = 0; i < maps.size(); i++)
		delete maps.at(i);
	maps.clear();
	// test player
	delete currentPlayer;
}

void OverworldManager::FreezeScene()
{
	currentMap->Freeze();
	currentPlayer->Freeze();
}

void OverworldManager::UnfreezeScene()
{
	currentPlayer->Unfreeze();
	currentMap->Unfreeze();
}

void OverworldManager::ActivateScene()
{
	currentPlayer->Activate();
	currentMap->Activate();
}

void OverworldManager::DeactivateScene()
{
	currentPlayer->Deactivate();
	currentMap->Deactivate();
}


const ProtoTile* OverworldManager::GetProtoTile(int index)
{
	if (index < (int)TILES::TILE_MAX)
		return protoTiles[index];
	else
		return nullptr;
}

const Room* OverworldManager::GetRoom(int index)
{
	return currentMap->GetRoom(index);
}

const Room* OverworldManager::GetCurrentRoom()
{
	return currentMap->CurrentRoom();
}

const Map* OverworldManager::GetCurrentMap()
{
	return currentMap;
}

const Player* OverworldManager::GetCurrentPlayer()
{
	return currentPlayer;
}

const std::vector<Character*> OverworldManager::GetCurrentParty()
{
	return party;
}


void OverworldManager::HitDoor(int destination, int x, int y)
{
	if (Manager::CanChangeRoom())
	{
		doorHit = DoorData(destination, x, y);
		isRoomTransitioning = true;
		Manager::StartRoomChange();
	}
}

void OverworldManager::TransitionRoom()
{
	FADE_STATUS fade = Manager::FadeScene();


	switch (fade)
	{
	case FADE_STATUS::FADE_START:
		// begin fade
		currentMap->Freeze();
		currentPlayer->Freeze();
		break;
	case FADE_STATUS::FADE_SWITCH:
		// when fade out is done
		currentMap->Deactivate();
		currentMap->SetRoom(doorHit.destination);
		currentPlayer->GetRectangle()->SetX((float)doorHit.x);
		currentPlayer->GetRectangle()->SetY((float)doorHit.y);
		currentMap->Activate();
		currentMap->Freeze();
		break;
	case FADE_STATUS::FADE_DONE:
		// fading done
		currentMap->Unfreeze();
		currentPlayer->Unfreeze();
		isRoomTransitioning = false;
		break;
	case FADE_STATUS::FADE_OUT:
		// fade out slowly
		break;
	case FADE_STATUS::FADE_IN:
		// fade in slowly
		break;
	}
}

void OverworldManager::Update(float delta_time)
{
	if (isRoomTransitioning)
		TransitionRoom();

	Manager::CenterCamera(currentPlayer->GetRectangle()->CenterX(), currentPlayer->GetRectangle()->CenterY());
}


bool OverworldManager::OnInteract(Point<float> interactPoint)
{
	std::vector<Component*> components = ComponentManager::GetComponentsByType(COMPONENT_TYPE::INTERACTION);

	for (auto it = components.begin(); it != components.end(); it++)
	{
		if (((InteractionComponent*)(*it))->TryInteract(interactPoint))
		{
			return true;
		}
	}

	return false;
}