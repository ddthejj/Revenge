#pragma once

#include <string>

class MenuBox;
class SpriteBatch;
class Texture;
class Sprite;
template <typename T>
struct Point;
class DialogueBox;
class Character;

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
	MENU_NEWGAME,
	MENU_CONTINUE,
	MENU_OPTIONS,
	MENU_MAX
};

class MenuManager
{
	static std::vector<MenuBox*> menuList;									// the list of loaded menus
	static MenuBox* activeMenu;									// the currently active menu
	static const Texture* menuTex;								// the texture of all menus
	static DialogueBox* activeDialogueBox;						// the dialogue box to use during dialogue
public:
	static void Init();											// initialize the menu manager and load all menus
	static void Clean();										// clean all menus

	static void LoadMenusFromFile(const wchar_t* filepath);		// load a batch of menus from a single file
	static void LoadTitleMenus();								// load the title screen menus
	static void LoadOverworldMenus();							// load the overworld menus
	static void UnloadMenus();									// unload the current set of menus

	static void OpenMenu(int index);							// open a menu
	static void CloseMenu();									// close the current menu
	static void CloseAllMenus();								// close all menus and unpause the game
	static void SetMenuOpacity(float _opacity);					// set the opacity of all active menus

	static void Update(float delta_time);						// update the menum anager and active menus

	static void PlayHoverSound();								// play the menu hover sound

	static void OptionSelected(int option);

	static void StartDialogue(Character* speaker, std::vector<std::string> text);
	static void EndDialogue();

private:
	
	static int GetMenuCount();
};

