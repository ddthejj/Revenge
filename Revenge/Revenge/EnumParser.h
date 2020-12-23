#pragma once

#include <map>
#include <string>
#include <stdexcept>


template <typename T>
class EnumParser
{
	std::map<std::string, T> map;

public:

	EnumParser() {}
	T Parse(const std::string& name)
	{
		auto it = map.find(name);
		if (it == map.end())
		{
			throw std::runtime_error("Parsing Enum Failed");
		}
		return (*it).second;
	}
};

#pragma region Enums

#pragma region Tiles

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

#pragma endregion

#pragma region Textures

#pragma region Test

enum class TEXTURES_TEST		// Textures for test map
{
	TEX_T_BLACK,				// 1 black pixel
	TEX_T_MENU,					// Menus borders, corners, and backgrounds
	TEX_T_ARROW,				// Menu arrow
	TEX_T_TESTROOM,				// Test room spritesheet
	TEX_T_BROWNFLOOR,			// Test floor 
	TEX_T_REDWALL,				// Test wall
	TEX_T_GREENDOOR,			// Test door
	TEX_T_BLUETEXT,				// Test interaction tile
	TEX_T_PLAYER,				// Test Player spritesheet
	TEX_T_MAX
};


EnumParser<TEXTURES_TEST>::EnumParser()
{
	map["TEX_T_BLACK"] =		TEXTURES_TEST::TEX_T_BLACK;
	map["TEX_T_MENU"] =			TEXTURES_TEST::TEX_T_MENU;
	map["TEX_T_ARROW"] =		TEXTURES_TEST::TEX_T_ARROW;
	map["TEX_T_TESTROOM"] =		TEXTURES_TEST::TEX_T_TESTROOM;
	map["TEX_T_BROWNFLOOR"] =	TEXTURES_TEST::TEX_T_BROWNFLOOR;
	map["TEX_T_REDWALL"] =		TEXTURES_TEST::TEX_T_REDWALL;
	map["TEX_T_GREENDOOR"] =	TEXTURES_TEST::TEX_T_GREENDOOR;
	map["TEX_T_BLUETEXT"] =		TEXTURES_TEST::TEX_T_BLUETEXT;
	map["TEX_T_PLAYER"] =		TEXTURES_TEST::TEX_T_PLAYER;
	map["TEX_T_MAX"] =			TEXTURES_TEST::TEX_T_MAX;
}

#pragma endregion

#pragma region Title

enum class TEXTURES_TITLE		// Textures for title screen
{
	TEX_TITLE_BLACK,			// 1 black pixel
	TEX_TITLE_LOGO,				// Studio logo
	TEX_TITLE_TITLE,			// Game Title / Game Logo
	TEX_TITLE_BACKGROUND,		// Background for title menu
	TEX_TITLE_MENU,				// Menu texture
	TEX_TITLE_ARROW,			// Menu arrow texture
	TEX_TITLE_MAX
};

EnumParser<TEXTURES_TITLE>::EnumParser()
{
	map["TEX_TITLE_BLACK"] =		TEXTURES_TITLE::TEX_TITLE_BLACK;
	map["TEX_TITLE_LOGO"] =			TEXTURES_TITLE::TEX_TITLE_LOGO;
	map["TEX_TITLE_TITLE"] =		TEXTURES_TITLE::TEX_TITLE_TITLE;
	map["TEX_TITLE_BACKGROUND"] =	TEXTURES_TITLE::TEX_TITLE_BACKGROUND;
	map["TEX_TITLE_MENU"] =			TEXTURES_TITLE::TEX_TITLE_MENU;
	map["TEX_TITLE_ARROW"] =		TEXTURES_TITLE::TEX_TITLE_ARROW;
}

#pragma endregion

#pragma endregion

#pragma region Keys

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

#pragma endregion

#pragma region Mouse Keys

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

#pragma endregion

#pragma region Fade Status

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

#pragma endregion

#pragma region Game State

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

#pragma endregion

#pragma region Anchor Point

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

#pragma endregion

#pragma region Sound Channel

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

#pragma endregion
