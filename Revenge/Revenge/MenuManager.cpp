#include "MenuManager.h"
#include "Manager.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Texture.h"

MenuBox** MenuManager::menuList = new MenuBox*[MENU_MAX];
MenuBox* MenuManager::activeMenu = nullptr;
Texture* MenuManager::menuTex = nullptr;


void MenuManager::Init()
{
	menuTex = Manager::GetTexture(Manager::TEX_MENU);

	menuList[MENU_BASE] = new MenuBox(50, 50, 200, 200, menuTex);
	menuList[MENU_CHARACTER] = new MenuBox(250, 50, 100, 100, menuTex);
	menuList[MENU_INVENTORY] = new MenuBox(250, 50, 100, 100, menuTex);
	menuList[MENU_MAGIC] = new MenuBox(250, 50, 100, 100, menuTex);
}

void MenuManager::Clean()
{
	for (int i = 0; i < MENU_MAX; i++)
	{
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
	menuList[index]->Open(activeMenu);
	activeMenu = menuList[index];
	activeMenu->Activate();
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

void MenuManager::Update()
{
	if (Manager::IsKeyPressed(Manager::KEY_MENU))
	{
		if (!activeMenu)
		{
			Manager::FreezeScene();
			OpenMenu(MENU_BASE);
		}
		else
		{
			Manager::UnfreezeScene();
			CloseAllMenus();
		}
	}
}
