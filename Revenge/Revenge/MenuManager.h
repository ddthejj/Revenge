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

class MenuManager
{
	static std::vector<MenuBox*> menuList;						// the list of opened menus
	static MenuBox* activeMenu;									// the currently active menu
	static DialogueBox* activeDialogueBox;						// the dialogue box to use during dialogue
public:
	static void Init();											// initialize the menu manager and load all menus
	static void Clean();										// clean all menus

	static void UnloadMenus();									// unload the current set of menus

	static void CloseAllMenus();								// close all menus and unpause the game
	static void SetMenuOpacity(float _opacity);					// set the opacity of all active menus

	static void Update(float delta_time);						// update the menum anager and active menus

	static void OpenTitleMenu();
	static void OpenOverworldMenu();
	static void MenuCreated(MenuBox* menu);
	static void ActiveMenuCreated(MenuBox* menu);
	static void MenuClosed(MenuBox* menu);

	static void StartDialogue(Character* speaker, std::vector<std::string> text);
	static void EndDialogue();
};

