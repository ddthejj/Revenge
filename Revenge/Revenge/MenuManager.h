#pragma once
#include <vector>

class MenuBox;
class SpriteBatch;
class Texture;
class Sprite;

enum MENUS
{
	MENU_PREVIOUS = -1,
	MENU_BASE,
	MENU_CHARACTER,
	MENU_INVENTORY,
	MENU_OPTIONS,
	MENU_MAX
};

class MenuManager
{
	static MenuBox** menuList;
	static MenuBox* activeMenu;
	static Texture* menuTex;
	static Sprite* arrow;
public:
	static void Init();
	static void Clean();

	static void OpenMenu(MENUS index);
	static void CloseMenu();
	static void CloseAllMenus();
	
	static void Update(float delta_time);
};

