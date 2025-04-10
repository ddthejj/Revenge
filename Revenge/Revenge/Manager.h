#pragma once
#include "defines.h"

#include <Windows.h>

#include <string>

class SpriteBatch;
class Texture;
class Object;
class Sprite;
struct ProtoTile;
class Room;
class Player;
class Door;

enum class FADE_STATUS	// Fade in and out status
{
	FADE_START,			// first frame of the fade out
	FADE_OUT,			// transitioning to black
	FADE_SWITCH,		// first frame of the fade in
	FADE_IN,			// transitioning the screen black in
	FADE_DONE,			// transition over
	FADE_MAX
};

enum class GAME_STATE	// game state
{
	STATE_TITLE,		// title screen
	STATE_LOADING,		// loading
	STATE_OVERWORLD,	// overworld map
	STATE_BATTLE,		// fighting 
	STATE_CUTSCENE,		// in a cutscene
	STATE_DIALOGUE,		// talking 
	STATE_MAX
};

// handles the entire game and all other managers
class Manager
{
private:

	static GAME_STATE gameState;				// Current mode the game is in
	static SpriteBatch* spriteBatch;			// Renderer class
	static std::vector<Object*> UpdateList;		// Items that are being updated
	static std::vector<Object*> DrawList;		// Items that are being drawn

	static bool fadingIn, fadingOut;			// Whether or not the screen is being faded in or out
	static float fadeOpacity;					// What the current opacity of the fade is
	static Sprite* fadeRectangle;				// The rectangle that covers the screen when fading

	static bool quit;							// updated for main to end the loop gracefully

public:

	// Get the game state
	static GAME_STATE GetGameState() { return gameState; }
	// Set the game state
	static void ChangeGameState(GAME_STATE _gameState);
	// Quit the game
	static void Quit();

	// Update the window size and position when the window is moved / resized
	static void ResizeWindow(HWND hwnd);
	// Focus is lost
	static void FocusLost();
	// Check if we have quit the game
	static bool ShouldEnd() { return quit; }

	// Returns the height of the game window
	static int GetScreenWidth();
	//Returns the width of the game window
	static int GetScreenHeight();


	// Initialize the whole game
	static void Init(HWND hwnd);
	// Call initialize function based on game state
	static void InitGameState();
	// Initialize the title screen
	static void InitTitle();
	// Initialize the overworld manager
	static void InitOverworld();
	// Unload data specific to the current game state
	static void UnloadGameState();
	// Destroy the whole game
	static void Clean();
	// Add an object to the list of objects being updated
	static bool AddUpdate(Object*that);
	// Remove an object from the list of objects being updated
	static bool RemoveUpdate(Object*that);
	// Add an object to the list of objects being drawn
	static bool AddDraw(Object*that);
	// Remove an object from the list of objects being drawn
	static bool RemoveDraw(Object*that);

	// Center the camera on an object
	static void CenterCamera(float x, float y);
	// Indicate that the player has hit a door
	//static void HitDoor(Door* hit);
	// Check whether or not the player can do a room transition at the moment
	static bool CanChangeRoom();
	// Start the fade process for a room transition
	static void StartRoomChange();
	// Fade the scene
	static FADE_STATUS FadeScene();
	// Measure the size of a string with a given font
	static Point<float> MeasureString(std::string text);

	// Update every object in the update list
	static void Update(float delta_time);
	// Draw every ovject in the draw list
	static void Draw();
};

