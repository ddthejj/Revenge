#pragma once
#include <vector>


class Map;
struct ProtoTile;
class Sprite;
class Room;
class Door;
class Player;

class OverworldManager
{
#pragma region Properties

	static std::vector<Map*> maps;				// List of all maps possible
	static ProtoTile* protoTiles[TILE_MAX];		// List of all prototype tiles 

	static Map* currentMap;						// Currently loaded map
	static Door* doorHit;						// The door that the player hit
	static Player* currentPlayer;				// Current active player

	static bool isRoomTransitioning;			// Whether or not the game is switching between rooms


#pragma endregion


public:
	// initialize the overworld manager
	static void Init();
	// clean the overworld manager
	static void Clean();
	// freeze the map and the player
	static void FreezeScene();
	// unfreeze the map and the player
	static void UnfreezeScene();
	// activate the map and the player
	static void ActivateScene();
	// deactivate the map and the player
	static void DeactivateScene();

	// Get a prototyle
	static ProtoTile* GetProtoTile(int index);
	// Get a room
	static Room* GetRoom(int index);
	// Get the current room
	static Room* GetCurrentRoom();
	// Get the current map
	static Map* GetCurrentMap();
	// Get the current player
	static Player* GetCurrentPlayer();

	static void HitDoor(Door* hit);
	static void TransitionRoom();				// Fade the screen out, load the new room, fade the screen in
	static void Update(float delta_time);
};