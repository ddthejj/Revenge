#pragma once

#include <map>
#include <string>
#include <stdexcept>

#pragma region Enums

#pragma region Tiles

enum class TILES : int	// Tiles
{
	TILE_BROWNFLOOR = 0,	// Test floor
	TILE_REDWALL,			// Test wall
	TILE_GREENDOOR,			// Test door
	TILE_BLUETEXT,			// Test interaction tile
	TILE_MAX
};
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
#pragma endregion

#pragma region Mouse Keys

enum class MOUSE_KEYS	// Mouse buttons
{
	MOUSE_KEY_LEFT,		// Left mouse button
	MOUSE_KEY_RIGHT,	// Right mouse button
	MOUSE_KEY_MIDDLE,	// Middle mouse button
	MOUSE_KEY_MAX
};

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

#pragma endregion

#pragma region Sound Channel

enum class SOUND_CHANNEL
{
	MASTER,
	MUSIC,
	SFX,
	SOUND_CHANNEL_MAX
};

#pragma endregion

#pragma region Characters

enum class MOVE_MODE
{
	NONE,
	RANDOM_RADIUS,
	FOLLOW
};

#pragma endregion

#pragma region Magic Types

enum class MAGIC_TYPE
{
	NONE = 0,

	ELEMENTAL = 1 << 5,
	HUMANITY = 1 << 6,

	FIRE = ELEMENTAL | (1 << 0),
	WATER = ELEMENTAL | (1 << 1),
	LIFE = ELEMENTAL | (1 << 2),
	EARTH = ELEMENTAL | (1 << 3),
	VENOM = ELEMENTAL | (1 << 4),

	MAGNET = HUMANITY | (1 << 0),
	WARD = HUMANITY | (1 << 1),
	LIGHT = HUMANITY | (1 << 2),
	MIND = HUMANITY | (1 << 3),
	NECRO = HUMANITY | (1 << 4),

	UNDECIDED = 1 << 7,
};
#pragma endregion

#pragma endregion


class EnumParser
{
	static std::map<std::string, TILES>				tilesMap;
	static std::map<std::string, TEXTURES_TEST>		testTexturesMap;
	static std::map<std::string, TEXTURES_TITLE>	titleTexturesMap;
	static std::map<std::string, KEYS>				keysMap;
	static std::map<std::string, MOUSE_KEYS>		mouseKeysMap;
	static std::map<std::string, FADE_STATUS>		fadeStatusMap;
	static std::map<std::string, GAME_STATE>		gameStateMap;
	static std::map<std::string, ANCHOR_POINT>		anchorPointMap;
	static std::map<std::string, SOUND_CHANNEL>		soundChannelMap;
	
	static std::map<std::string, MAGIC_TYPE>		magicTypeMap;

public:

	static TILES ParseTiles(const std::string& name);
	static TEXTURES_TEST ParseTestTexture(const std::string& name);
	static TEXTURES_TITLE ParseTitleTexture(const std::string& name);
	static KEYS ParseKeys(const std::string& name);
	static MOUSE_KEYS ParseMouseKeys(const std::string& name);
	static FADE_STATUS ParseFadeStatus(const std::string& name);
	static GAME_STATE ParseGameState(const std::string& name);
	static ANCHOR_POINT ParseAnchorPoint(const std::string& name);
	static SOUND_CHANNEL ParseSoundChannel(const std::string& name);
	static MAGIC_TYPE ParseMagicType(const std::string& name);
};