#include "defines.h"
#include "OverworldManager.h"
#include "Tile.h"
#include "Map.h"
#include "Character.h"
#include "Manager.h"
#include "InputManager.h"
#include "Room.h"

ProtoTile* OverworldManager::protoTiles[(int)TILES::TILE_MAX];
std::vector<Map*> OverworldManager::maps;

Map* OverworldManager::currentMap = nullptr;
Door* OverworldManager::doorHit = nullptr;
Player* OverworldManager::currentPlayer = nullptr;

bool OverworldManager::isRoomTransitioning = false;


void OverworldManager::Init()
{
	// test player
	currentPlayer = new Player(240, 240, 32, 32, Manager::GetTexture((int)TEXTURES_TEST::TEX_T_PLAYER), .6f);
	currentPlayer->Activate();
	// prototype tiles
	protoTiles[(int)TILES::TILE_BROWNFLOOR] = new ProtoTile(Manager::GetTexture((int)TEXTURES_TEST::TEX_T_BROWNFLOOR), 32, 32);
	protoTiles[(int)TILES::TILE_REDWALL] = new ProtoTile(Manager::GetTexture((int)TEXTURES_TEST::TEX_T_REDWALL), 32, 32, true);
	protoTiles[(int)TILES::TILE_GREENDOOR] = new ProtoTile(Manager::GetTexture((int)TEXTURES_TEST::TEX_T_GREENDOOR), 32, 32, false, true);
	protoTiles[(int)TILES::TILE_BLUETEXT] = new ProtoTile(Manager::GetTexture((int)TEXTURES_TEST::TEX_T_BLUETEXT), 32, 32, true, false, true);
	// add maps
	maps.push_back(new Map("../Assets/RoomData/TestRoom/TestRoom", 2));
	currentMap = maps[0];
	currentMap->Activate();
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


ProtoTile * OverworldManager::GetProtoTile(int index)
{
	if (index < (int)TILES::TILE_MAX)
		return protoTiles[index];
	else
		return nullptr;
}

Room * OverworldManager::GetRoom(int index)
{
	return currentMap->GetRoom(index);
}

Room * OverworldManager::GetCurrentRoom()
{
	return currentMap->CurrentRoom();
}

Map* OverworldManager::GetCurrentMap()
{
	return currentMap;
}

Player* OverworldManager::GetCurrentPlayer()
{
	return currentPlayer;
}


void OverworldManager::HitDoor(Door* hit)
{
	doorHit = hit;
	isRoomTransitioning = true;
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
		currentMap->SetRoom(doorHit->Destination());
		currentPlayer->GetRectangle()->SetX(doorHit->DestinationX());
		currentPlayer->GetRectangle()->SetY(doorHit->DestinationY());
		currentMap->Activate();
		currentMap->Freeze();
		break;
	case FADE_STATUS::FADE_DONE:
		// fading done
		currentMap->Unfreeze();
		currentPlayer->Unfreeze();
		doorHit = nullptr;
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


void OverworldManager::OnInteract(Point<float> interactPoint)
{
	Interactable** tiles = currentMap->CurrentRoom()->GetInteractables();
	
	for (int i = 0; i < currentMap->CurrentRoom()->GetInteractableCount(); i++)
	{
		Interactable* tile = tiles[i];

		if (tile->GetRectangle()->Contains(interactPoint))
		{
			tile->Interact();
			return;
		}
	}
}