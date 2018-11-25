#include "defines.h"
#include "MenuManager.h"
#include "Manager.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Sprite.h"

MenuBox** MenuManager::menuList = new MenuBox*[MENU_MAX];
MenuBox* MenuManager::activeMenu = nullptr;
Texture* MenuManager::menuTex = nullptr;

void MenuManager::Init()
{
	menuTex = Manager::GetTexture(Manager::TEX_MENU);

	for (int i = 0; i < MENU_MAX; i++)
		menuList[i] = nullptr;

	menuList[MENU_BASE] = new MenuBox(50, 50, 200, 200, menuTex, "../Assets/Menus/Menu_Base.txt");
	//menuList[MENU_CHARACTER] = new MenuBox(250, 50, 100, 100, menuTex, "../Assets/Menus/Menu_Character.txt");
	//menuList[MENU_INVENTORY] = new MenuBox(250, 50, 100, 100, menuTex, "../Assets/Menus/Menu_Inventory.txt");
	//menuList[MENU_OPTIONS] = new MenuBox(250, 50, 100, 100, menuTex, "../Assets/Menus/Menu_Options.txt");
}

void MenuManager::Clean()
{
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (menuList[i])
			delete menuList[i];
	}
	delete menuList;
}

void MenuManager::OpenMenu(MENUS index)
{
	if (index == MENU_PREVIOUS)
	{
		CloseMenu();
		return;
	}

	if (activeMenu)
		activeMenu->Freeze();

	switch (index)
	{
	case MENU_BASE:
		break;
	case MENU_CHARACTER:

		break;
	case MENU_INVENTORY:

		break;
	case MENU_OPTIONS:

		break;
	}

	menuList[index]->Open(activeMenu);
	activeMenu = menuList[index];
}

void MenuManager::CloseMenu()
{
	activeMenu->Deactivate();
	activeMenu = activeMenu->previousMenu;

	if (activeMenu)
		activeMenu->Unfreeze();
	else
	{
		Manager::UnfreezeScene();
	}
}

void MenuManager::CloseAllMenus()
{
	while (activeMenu)
	{
		activeMenu->Deactivate();
		activeMenu = activeMenu->previousMenu;
	}

	Manager::UnfreezeScene();
}


void MenuManager::Update(float delta_time)
{
	if (!activeMenu)
	{
		if (Manager::IsKeyPressed(Manager::KEY_MENU))
		{
			Manager::FreezeScene();
			OpenMenu(MENU_BASE);
		}
	}
	else
	{
		if (Manager::IsKeyPressed(Manager::KEY_MENU))
		{
			Manager::UnfreezeScene();
			CloseAllMenus();
		}
		else if (Manager::IsKeyPressed(Manager::KEY_INTERACT))
		{
			int option = activeMenu->ChooseOption();
			if (option == MENUS::MENU_PREVIOUS)
				OpenMenu((MENUS)option);
			
			if (activeMenu == menuList[MENU_BASE])
			{
				OpenMenu((MENUS)option);
			}
			else if (activeMenu == menuList[MENU_CHARACTER])
			{

			}
			else if (activeMenu == menuList[MENU_INVENTORY])
			{

			}
			else if (activeMenu == menuList[MENU_OPTIONS])
			{

			}
		}
	}
}