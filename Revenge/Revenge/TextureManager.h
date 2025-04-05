#pragma once
#include <map>
#include <string>

class Texture;
class SpriteBatch;

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

class TextureManager
{
	static Texture** textures;
	static std::map<std::string, int> texNames;

public:
	static bool LoadTextures(const wchar_t* filepath, SpriteBatch* spriteBatch);
	static bool UnloadTextures();
	static const Texture* GetTexture(unsigned int index);
	static const Texture* GetTexture(const char* name);
	static void Clean();
};

