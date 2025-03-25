#pragma once



class Map;                                                                                                                                                                                                                                               
struct ProtoTile;
class Sprite;
class Room;
class Door;
class Player;
class Character;

class OverworldManager
{

#pragma region Properties

	static std::vector<Map*> maps;				// List of all maps possible
	static ProtoTile* protoTiles[(int)TILES::TILE_MAX];		// List of all prototype tiles 

	static Map* currentMap;						// Currently loaded map
	static Door* doorHit;						// The door that the player hit
	static Player* currentPlayer;				// Current active player
	static std::vector<Character*> party;		// List of party members

	static bool isRoomTransitioning;			// Whether or not the game is switching between rooms

#pragma endregion

public:

#pragma region Methods

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
	static const ProtoTile* GetProtoTile(int index);
	// Get a room
	static const Room* GetRoom(int index);
	// Get the current room
	static const Room* GetCurrentRoom();
	// Get the current map
	static const Map* GetCurrentMap();
	// Get the current player
	static const Player* GetCurrentPlayer();
	// Get the current party
	static const std::vector<Character*> GetCurrentParty();

	// Handle transitioning rooms when the player enters a door
	static void HitDoor(Door* hit);
	// Fade the screen out, load the new room, fade the screen in
	static void TransitionRoom();
	// Update the overworld manager
	static void Update(float delta_time);

	static void OnInteract(Point<float> interactPoint);

#pragma endregion

};