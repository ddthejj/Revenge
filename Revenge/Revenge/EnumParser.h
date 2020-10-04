#pragma once

#include <map>
#include <string>

template <typename T>
class EnumParser
{
	EnumParser() {}
	std::map<std::string, T> map;

public:

	T Parse(const std::string &name);
};

#pragma region Enums

enum class TILES : int	// Tiles
{
	TILE_BROWNFLOOR = 0,	// Test floor
	TILE_REDWALL,		// Test wall
	TILE_GREENDOOR,		// Test door
	TILE_BLUETEXT,		// Test interaction tile
	TILE_MAX
};

EnumParser<TILES>::EnumParser()
{
	map["TILE_BROWNFLOOR"] = TILES::TILE_BROWNFLOOR;
	map["TILE_REDWALL"] = TILES::TILE_REDWALL;
	map["TILE_GREENDOOR"] = TILES::TILE_GREENDOOR;
	map["TILE_BLUETEXT"] = TILES::TILE_BLUETEXT;
	map["TILE_MAX"] = TILES::TILE_MAX;
}

enum class TEXTURES		// Textures
{
	TEX_BLACK,			// 1 black pixel
	TEX_MENU,			// Menus borders, corners, and backgrounds
	TEX_ARROW,			// Menu arrow
	TEX_TESTROOM,		// Test room spritesheet
	TEX_BROWNFLOOR,		// Test floor 
	TEX_REDWALL,		// Test wall
	TEX_GREENDOOR,		// Test door
	TEX_BLUETEXT,		// Test interaction tile
	TEX_PLAYER,			// Test Player spritesheet
	TEX_MAX
};

EnumParser<TEXTURES>::EnumParser()
{
	map["TEX_BLACK"] = TEXTURES::TEX_BLACK;
	map["TEX_MENU"] = TEXTURES::TEX_MENU;
	map["TEX_ARROW"] = TEXTURES::TEX_ARROW;
	map["TEX_TESTROOM"] = TEXTURES::TEX_TESTROOM;
	map["TEX_BROWNFLOOR"] = TEXTURES::TEX_BROWNFLOOR;
	map["TEX_REDWALL"] = TEXTURES::TEX_REDWALL;
	map["TEX_GREENDOOR"] = TEXTURES::TEX_GREENDOOR;
	map["TEX_BLUETEXT"] = TEXTURES::TEX_BLUETEXT;
	map["TEX_PLAYER"] = TEXTURES::TEX_PLAYER;
	map["TEX_MAX"] = TEXTURES::TEX_MAX;
}

enum class KEYS			// Keybinds
{
	KEY_UP,				// Up key
	KEY_DOWN,			// Down key
	KEY_LEFT,			// Left key
	KEY_RIGHT,			// Right key
	KEY_MENU,			// Open menu
	KEY_INTERACT,		// Select / interact with tiles
	KEY_MAX
};

EnumParser<KEYS>::EnumParser()
{
	map["KEY_UP"] = KEYS::KEY_UP;
	map["KEY_DOWN"] = KEYS::KEY_DOWN;
	map["KEY_LEFT"] = KEYS::KEY_LEFT;
	map["KEY_RIGHT"] = KEYS::KEY_RIGHT;
	map["KEY_MENU"] = KEYS::KEY_MENU;
	map["KEY_INTERACT"] = KEYS::KEY_INTERACT;
	map["KEY_MAX"] = KEYS::KEY_MAX;
};

enum class MOUSE_KEYS	// Mouse buttons
{
	MOUSE_KEY_LEFT,		// Left mouse button
	MOUSE_KEY_RIGHT,	// Right mouse button
	MOUSE_KEY_MIDDLE,	// Middle mouse button
	MOUSE_KEY_MAX
};

EnumParser<MOUSE_KEYS>::EnumParser()
{
	map["MOUSE_KEY_LEFT"] = MOUSE_KEYS::MOUSE_KEY_LEFT;
	map["MOUSE_KEY_RIGHT"] = MOUSE_KEYS::MOUSE_KEY_RIGHT;
	map["MOUSE_KEY_MIDDLE"] = MOUSE_KEYS::MOUSE_KEY_MIDDLE;
	map["MOUSE_KEY_MAX"] = MOUSE_KEYS::MOUSE_KEY_MAX;
}

enum class FADE_STATUS	// Fade in and out status
{
	FADE_START,			// first frame of the fade out
	FADE_OUT,			// transitioning to black
	FADE_SWITCH,		// first frame of the fade in
	FADE_IN,			// transitioning the screen black in
	FADE_DONE,			// transition over
	FADE_MAX
};

EnumParser<FADE_STATUS>::EnumParser()
{
	map["FADE_START"] = FADE_STATUS::FADE_START;
	map["FADE_OUT"] = FADE_STATUS::FADE_OUT;
	map["FADE_SWITCH"] = FADE_STATUS::FADE_SWITCH;
	map["FADE_IN"] = FADE_STATUS::FADE_IN;
	map["FADE_DONE"] = FADE_STATUS::FADE_DONE;
	map["FADE_MAX"] = FADE_STATUS::FADE_MAX;
}

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

EnumParser<GAME_STATE>::EnumParser()
{
	map["STATE_TITLE"] = GAME_STATE::STATE_TITLE;
	map["STATE_LOADING"] = GAME_STATE::STATE_LOADING;
	map["STATE_OVERWORLD"] = GAME_STATE::STATE_OVERWORLD;
	map["STATE_BATTLE"] = GAME_STATE::STATE_BATTLE;
	map["STATE_CUTSCENE"] = GAME_STATE::STATE_CUTSCENE;
	map["STATE_DIALOGUE"] = GAME_STATE::STATE_DIALOGUE;
	map["STATE_MAX"] = GAME_STATE::STATE_MAX;
}

enum class ANCHOR_POINT : unsigned char
{
	TOP = (0) << 2,
	VCENTER = (1) << 2,
	BOTTOM = (1 << 1) << 2,
	LEFT = 0,
	HCENTER = 1,
	RIGHT = 1 << 1,

	ANCHOR_TOP_LEFT = TOP | LEFT,
	ANCHOR_TOP_CENTER = TOP | HCENTER,
	ANCHOR_TOP_RIGHT = TOP | RIGHT,
	ANCHOR_CENTER_LEFT = VCENTER | LEFT,
	ANCHOR_CENTER = VCENTER | HCENTER,
	ANCHOR_CENTER_RIGHT = VCENTER | RIGHT,
	ANCHOR_BOTTOM_LEFT = BOTTOM | LEFT,
	ANCHOR_BOTTOM_CENTER = BOTTOM | HCENTER,
	ANCHOR_BOTTOM_RIGHT = BOTTOM | RIGHT
};

EnumParser<ANCHOR_POINT>::EnumParser()
{
	map["ANCHOR_TOP_LEFT"] = ANCHOR_POINT::ANCHOR_TOP_LEFT;
	map["ANCHOR_TOP_CENTER"] = ANCHOR_POINT::ANCHOR_TOP_CENTER;
	map["ANCHOR_TOP_RIGHT"] = ANCHOR_POINT::ANCHOR_TOP_RIGHT;
	map["ANCHOR_CENTER_LEFT"] = ANCHOR_POINT::ANCHOR_CENTER_LEFT;
	map["ANCHOR_CENTER"] = ANCHOR_POINT::ANCHOR_CENTER;
	map["ANCHOR_CENTER_RIGHT"] = ANCHOR_POINT::ANCHOR_CENTER_RIGHT;
	map["ANCHOR_BOTTOM_LEFT"] = ANCHOR_POINT::ANCHOR_BOTTOM_LEFT;
	map["ANCHOR_BOTTOM_CENTER"] = ANCHOR_POINT::ANCHOR_BOTTOM_CENTER;
	map["ANCHOR_BOTTOM_RIGHT"] = ANCHOR_POINT::ANCHOR_BOTTOM_RIGHT;
}

enum class SOUND_CHANNEL
{
	MASTER,
	MUSIC,
	SFX,
	SOUND_CHANNEL_MAX
};

EnumParser<SOUND_CHANNEL>::EnumParser()
{
	map["MASTER"] = SOUND_CHANNEL::MASTER;
	map["MUSIC"] = SOUND_CHANNEL::MUSIC;
	map["SFX"] = SOUND_CHANNEL::SFX;
	map["SOUND_CHANNEL_MAX"] = SOUND_CHANNEL::SOUND_CHANNEL_MAX;
}

#pragma endregion
