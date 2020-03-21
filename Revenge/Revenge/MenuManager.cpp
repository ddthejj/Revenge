#include "defines.h"
#include "MenuManager.h"
#include "Manager.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Sprite.h"
#include "Character.h"

MenuBox** MenuManager::menuList = nullptr; 
MenuBox* MenuManager::activeMenu = nullptr;
Texture* MenuManager::menuTex = nullptr;

void MenuManager::Init()
{
	menuList = new MenuBox* [(int)MENUS::MENU_MAX];
	menuTex = Manager::GetTexture((int)TEXTURES::TEX_MENU);

	for (int i = 0; i < (int)MENUS::MENU_MAX; i++)
		menuList[i] = nullptr;

	menuList[(int)MENUS::MENU_BASE] = new MenuBox(50, 50, 150, 200, menuTex, "../Assets/Menus/Menu_Base.txt");
	menuList[(int)MENUS::MENU_CHARACTER] = new MenuBox(50, 50, 10, 10, menuTex);
	//menuList[MENU_INVENTORY] = new MenuBox(250, 50, 100, 100, menuTex, "../Assets/Menus/Menu_Inventory.txt");
	//menuList[MENU_OPTIONS] = new MenuBox(250, 50, 100, 100, menuTex, "../Assets/Menus/Menu_Options.txt");
}

void MenuManager::Clean()
{
	if (!menuList) return;

	for (int i = 0; i < (int)MENUS::MENU_MAX; i++)
	{
		if (menuList[i])
			delete menuList[i];
	}
	delete menuList;
}

void MenuManager::OpenMenu(MENUS index)
{
	if (index == MENUS::MENU_PREVIOUS)
	{
		CloseMenu();
		return;
	}

	if (!menuList[(int)index])
		return;

	if (activeMenu)
		activeMenu->Freeze();

	switch (index)
	{
	case MENUS::MENU_BASE:
	{

		break;
	}
	case MENUS::MENU_CHARACTER:
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
			positions[i] = Point<float>(40.f, 30.f * (float)(i + 1));
			layouts[i] = Point<int>(0, i);
		}

		menuList[(int)MENUS::MENU_CHARACTER]->SetOptions(texts, options, positions, layouts, 1, (int)party.size());
		menuList[(int)MENUS::MENU_CHARACTER]->SetRectangle(MyRectangle(Point<float>(activeMenu->GetRectangle()->Right(), activeMenu->GetRectangle()->Top()), 150, 150));

		delete[] texts;
		delete[] options;
		delete[] positions;
		delete[] layouts;
		break;
	}
	case MENUS::MENU_INVENTORY:
	{

		break;
	}
	case MENUS::MENU_OPTIONS:
	{
		break;

	}
	}

	menuList[(int)index]->Open(activeMenu);
	activeMenu = menuList[(int)index];
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
		if (Manager::IsKeyPressed(KEYS::KEY_MENU))
		{
			Manager::FreezeScene();
			OpenMenu(MENUS::MENU_BASE);
		}
	}
	else
	{
		if (Manager::IsKeyPressed(KEYS::KEY_MENU))
		{
			Manager::UnfreezeScene();
			CloseAllMenus();
		}
		else if (Manager::IsKeyPressed(KEYS::KEY_INTERACT))
		{
			int option = activeMenu->ChooseOption();
			if (option == (int)MENUS::MENU_PREVIOUS)
				OpenMenu((MENUS)option);

			if (activeMenu == menuList[(int)MENUS::MENU_BASE])
			{
				OpenMenu((MENUS)option);
			}
			else if (activeMenu == menuList[(int)MENUS::MENU_CHARACTER])
			{

			}
			else if (activeMenu == menuList[(int)MENUS::MENU_INVENTORY])
			{

			}
			else if (activeMenu == menuList[(int)MENUS::MENU_OPTIONS])
			{

			}
		}
	}
}