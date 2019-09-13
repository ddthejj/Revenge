#pragma once

// MEMORY LEAK
#if defined(DEBUG) || defined (_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#if defined(_DEBUG) || defined(DEBUG)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define WIDTH 800.0f
#define HEIGHT 800.0f
#define TILE_WIDTH 32.0f
#define TILE_HEIGHT 32.0f
#define ROOM_FADE_SPEED	1.f / 30.f

template <typename T>
struct Point
{
	T x = 0, y = 0;

	Point() = default;
	Point(T _x, T _y) { x = _x; y = _y; }
};

template <class T> void SafeDelete(T *p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}


#pragma region ENUMS 

enum TILES				// Tiles
{
	TILE_BROWNFLOOR,	// Test floor
	TILE_REDWALL,		// Test wall
	TILE_GREENDOOR,		// Test door
	TILE_BLUETEXT,		// Test interaction tile
	TILE_MAX
};

enum TEXTURES			// Textures
{
	TEX_BLACK,			// 1 black pixel
	TEX_MENU,			// Menus borders, corners, and backgrounds
	TEX_ARROW,			// Menu arrow
	TEX_BROWNFLOOR,		// Test floor 
	TEX_REDWALL,		// Test wall
	TEX_GREENDOOR,		// Test door
	TEX_BLUETEXT,		// Test interaction tile
	TEX_PLAYER,			// Test Player spritesheet
	TEX_MAX
};

enum KEYS				// Keybinds
{
	KEY_UP,				// Up key
	KEY_DOWN,			// Down key
	KEY_LEFT,			// Left key
	KEY_RIGHT,			// Right key
	KEY_MENU,			// Open menu
	KEY_INTERACT,		// Select / interact with tiles
	KEY_MAX
};
	
enum MOUSE_KEYS			// Mouse buttons
{
	MOUSE_KEY_LEFT,		// Left mouse button
	MOUSE_KEY_RIGHT,	// Right mouse button
	MOUSE_KEY_MIDDLE,	// Middle mouse button
	MOUSE_KEY_MAX
};

enum FADE_STATUS		// Fade in and out status
{
	FADE_START,			// first frame of the fade out
	FADE_OUT,			// transitioning to black
	FADE_SWITCH,		// first frame of the fade in
	FADE_IN,			// transitioning the screen black in
	FADE_DONE,			// transition over
	FADE_MAX
};

enum GAME_STATE			// game state
{
	STATE_TITLE,		// title screen
	STATE_LOADING,		// loading
	STATE_OVERWORLD,	// overworld map
	STATE_BATTLE,		// fighting 
	STATE_CUTSCENE,		// in a cutscene
	STATE_DIALOGUE,		// talking 
	STATE_MAX
};

#pragma endregion