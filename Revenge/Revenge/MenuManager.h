#pragma once
#include <vector>

class MenuBox;
class SpriteBatch;
class Texture;
class Sprite;
template <typename T>
struct Point;

enum class OVERWORLD_MENUS			// the list of menus
{
	MENU_PREVIOUS = -1,		// Return to the previous menu
	MENU_BASE,				// The menu that opens when you hit pause
	MENU_CHARACTER,			// The character list menu
	MENU_INVENTORY,			// The inventory menu
	MENU_OPTIONS,			// The options menu
	MENU_CHARACTER_STATS,	// The character submenu
	MENU_MAX
};

enum class TITLE_MENUS
{
	MENU_PREVIOUS = -1,
	MENU_TITLE,
	MENU_MAX
};

class MenuManager
{
	static MenuBox** menuList;				// the list of loaded menus
	static MenuBox* activeMenu;				// the currently active menu
	static Texture* menuTex;				// the texture of all menus
public:
	static void Init();						// initialize the menu manager and load all menus
	static void Clean();					// clean all menus

	static void LoadMenusFromFile(const wchar_t* filepath);
	static void LoadTitleMenus();			// load the title screen menus
	static void LoadOverworldMenus();		// load the overworld menus
	static void UnloadMenus();				// unload the current set of menus

	static void OpenMenu(int index);		// open a menu
	static void CloseMenu();				// close the current menu
	static void CloseAllMenus();			// close all menus and unpause the game
	
	static void Update(float delta_time);	// update the menum anager and active menus

	static void PlayHoverSound();			// play the menu hover sound
};

