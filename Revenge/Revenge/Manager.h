#pragma once
#include "defines.h"

#include <Windows.h>
#include <vector>



class MyRectangle;
class SpriteBatch;
class Texture;
class Sprite;
struct ProtoTile;
class Room;
class Map;
class Player;
class Door;
class MenuBox;
class Character;

// handles the entire game and all other managers
class Manager
{
public:


private:

#pragma region Properties

	static SpriteBatch* spriteBatch;			// Renderer class
	static Texture* textures[TEX_MAX];			// List of all loaded textures
	//static ProtoTile* protoTiles[TILE_MAX];		// List of all prototype tiles 
	static std::vector<Sprite*> UpdateList;		// Items that are being updated
	static std::vector<Sprite*> DrawList;		// Items that are being drawn
	//static std::vector<Map*> maps;				// List of all maps possible
	static std::vector<Character*> party;		// List of party members

	//static Map* currentMap;						// Currently loaded map
	//static Player* currentPlayer;				// Current active player

	static bool fadingIn, fadingOut;			// Whether or not the screen is being faded in or out
	static float fadeOpacity;					// What the current opacity of the fade is
	static Sprite* fadeRectangle;				// The rectangle that covers the screen when fading

#pragma endregion

public:

#pragma region GameManager Access

	// Get a prototyle
	static ProtoTile* GetProtoTile(int index);
	// Get a room
	static Room* GetRoom(int index);
	// Get the current room
	static Room* GetCurrentRoom();
	// Get the current player
	static Player* GetCurrentPlayer();
	// Get the party list
	static std::vector<Character*> GetParty() { return party; }
	// Get a texture
	static Texture* GetTexture(int index) { if (index < TEX_MAX) return textures[index]; else return nullptr; }

#pragma endregion

#pragma region InputManager Access
	
	// Check if a key is down this frame
	static bool IsKeyDown(KEYS index);// { return keys[index]; }
	// Check if a key was down last frame
	static bool IsPreviousKeyDown(KEYS index);// { return previousKeys[index]; }
	// Check if a key was just pressed this frame
	static bool IsKeyPressed(KEYS index);

#pragma endregion

#pragma region Access from Main

	// Update the mouse position
	static void MoveMouse(HWND hwnd, LPARAM lParam);
	// Update the keystates when a key is pressed
	static void PressKey(WPARAM wParam);
	// Update the keystates when a key is released
	static void ReleaseKey(WPARAM wParam);
	// Update the window size and position when the window is moved / resized
	static void ResizeWindow(HWND hwnd);

#pragma endregion

#pragma region Methods

	// Initialize the whole game
	static void Init(HWND hwnd);
	// Destroy the whole game
	static void Clean();
	// Add an object to the list of objects being updated
	static bool AddUpdate(Sprite*that);
	// Remove an object from the list of objects being updated
	static bool RemoveUpdate(Sprite*that);
	// Add an object to the list of objects being drawn
	static bool AddDraw(Sprite*that);
	// Remove an object from the list of objects being drawn
	static bool RemoveDraw(Sprite*that);

	// Center the camera on an object
	static void CenterCamera(float x, float y);
	// Indicate that the player has hit a door
	static void HitDoor(Door* hit);
	// Freeze the entire scene
	static void FreezeScene();
	// Unfreeze the entire scene
	static void UnfreezeScene();
	// Fade the scene
	static FADE_STATUS FadeScene();

	// Update every object in the update list
	static void Update(float delta_time);
	// Draw every ovject in the draw list
	static void Draw();

#pragma endregion
};

