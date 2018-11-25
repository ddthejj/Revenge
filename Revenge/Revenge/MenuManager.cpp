#include "defines.h"
#include "MenuManager.h"
#include "Manager.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Sprite.h"
#include "Character.h"

MenuBox** MenuManager::menuList = new MenuBox*[MENU_MAX];
MenuBox* MenuManager::activeMenu = nullptr;
Texture* MenuManager::menuTex = nullptr;

void MenuManager::Init()
{
	menuTex = Manager::GetTexture(Manager::TEX_MENU);

	for (int i = 0; i < MENU_MAX; i++)
		menuList[i] = nullptr;

	menuList[MENU_BASE] = new MenuBox(50, 50, 200, 200, menuTex, "../Assets/Menus/Menu_Base.txt");
	menuList[MENU_CHARACTER] = new MenuBox(50, 50, 100, 100, menuTex);
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
	{

		break;
	}
	case MENU_CHARACTER:
	{
		std::vector<Character*> party = Manager::GetParty();

		if (party.size() <= 0)
		{
			activeMenu->Unfreeze();
			return;
		}

		char** texts = new char*[party.size()];
		int* options = new int[party.size()];
		Point<float>* positions = new Point<float>[party.size()];
		Point<int>* layouts = new Point<int>[party.size()];

		for (int i = 0; i < (int)party.size(); i++)
		{
			texts[i] = party[i]->Name();
			options[i] = i;
			positions[i] = Point<float>(30, 30 * (i + 1));
			layouts[i] = Point<int>(0, i);
		}

		menuList[MENU_CHARACTER]->SetOptions(texts, options, positions, layouts, 1, (int)party.size());
		menuList[MENU_CHARACTER]->SetRectangle(MyRectangle(Point<float>(activeMenu->GetRectangle()->Right(), activeMenu->GetRectangle()->Top()), 200, 150));

		delete[] texts;
		delete[] options;
		delete[] positions;
		delete[] layouts;
		break;
	}
	case MENU_INVENTORY:
	{

		break;
	}
	case MENU_OPTIONS:
	{
		break;

	}
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