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


class Manager
{
public:
	enum TILES
	{
		TILE_BROWNFLOOR,
		TILE_REDWALL,
		TILE_GREENDOOR,
		TILE_MAX
	};

	enum TEXTURES
	{
		TEX_BLACK,
		TEX_MENUCORNER,
		TEX_MENUBORDER,
		TEX_BROWNFLOOR,
		TEX_REDWALL,
		TEX_GREENDOOR,
		TEX_PLAYER_DOWN_1,
		TEX_MAX
	};

	enum KEYS
	{
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_MENU,
		KEY_MAX
	};

	static char keyOptions[KEY_MAX];
	static bool keys[KEY_MAX];

private:
	static SpriteBatch* spriteBatch;
	static Texture* textures[TEX_MAX];
	static ProtoTile* protoTiles[TILE_MAX];
	static std::vector<Sprite*> UpdateList;
	static std::vector<Sprite*> DrawList;
	static std::vector<Map*> maps;

	static Map* currentMap;
	static Player* currentPlayer;

	static bool fadingIn, fadingOut;
	static float fadeOpacity;
	static Door* doorHit;
	static Sprite* fadeRectangle;
	static MenuBox* menu;

	static void TransitionRoom();

public:

	static ProtoTile* GetProtoTile(int index);
	static Room* GetRoom(int index);
	static Player* GetCurrentPlayer();
	static Room* GetCurrentRoom();

	static void MoveMouse(HWND hwnd, LPARAM lParam);
	static void PressKey(WPARAM wParam);
	static void ReleaseKey(WPARAM wParam);
	static void ResizeWindow(HWND hwnd);

	static void Init(HWND hwnd);
	static void Clean();
	static bool AddUpdate(Sprite*that);
	static bool RemoveUpdate(Sprite*that);
	static bool AddDraw(Sprite*that);
	static bool RemoveDraw(Sprite*that);

	static void CenterCamera(float x, float y);
	static void HitDoor(Door* hit);

	static void Update();
	static void Draw();
};

