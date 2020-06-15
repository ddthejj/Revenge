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

}

void MenuManager::Clean()
{
	int menuCount = 0; 
	switch (Manager::GetGameState())
	{
	case GAME_STATE::STATE_TITLE:
		menuCount = (int)TITLE_MENUS::MENU_MAX;
		break;
	case GAME_STATE::STATE_OVERWORLD:
		menuCount = (int)OVERWORLD_MENUS::MENU_MAX;
		break;
	}

	if (menuList)
	{
		for (int i = 0; i < menuCount; i++)
		{
			if (menuList[i])
				SafeDelete(menuList[i]);
		}
		SafeDelete(menuList);
	}
}


void MenuManager::LoadTitleMenus()
{
	UnloadMenus();

	menuList = new MenuBox * [(int)TITLE_MENUS::MENU_MAX];

	for (int i = 0; i < (int)TITLE_MENUS::MENU_MAX; i++)
		menuList[i] = nullptr;

  	menuList[(int)TITLE_MENUS::MENU_TITLE] = new MenuBox(0, 0, 300, 300, nullptr, "../Assets/Menus/Menu_Title_Base.txt");
}

void MenuManager::LoadOverworldMenus()
{
	UnloadMenus();

	menuList = new MenuBox * [(int)OVERWORLD_MENUS::MENU_MAX];
	menuTex = Manager::GetTexture((int)TEXTURES::TEX_MENU);

	for (int i = 0; i < (int)OVERWORLD_MENUS::MENU_MAX; i++)
		menuList[i] = nullptr;

	menuList[(int)OVERWORLD_MENUS::MENU_BASE] = new MenuBox(50, 50, 150, 200, menuTex, "../Assets/Menus/Menu_Base.txt");
	menuList[(int)OVERWORLD_MENUS::MENU_CHARACTER] = new MenuBox(50, 50, 10, 10, menuTex);
	//menuList[MENU_INVENTORY] = new MenuBox(250, 50, 100, 100, menuTex, "../Assets/Menus/Menu_Inventory.txt");
	//menuList[MENU_OPTIONS] = new MenuBox(250, 50, 100, 100, menuTex, "../Assets/Menus/Menu_Options.txt");
}

void MenuManager::UnloadMenus()
{
	if (menuList)
	{
		for (int i = 0; i < (int)OVERWORLD_MENUS::MENU_MAX; i++)
		{
			if (menuList[i])
				SafeDelete(menuList[i]);
		}
		SafeDelete(menuList);
	}
}


void MenuManager::OpenMenu(int index)
{
	if (index == -1)
	{
		CloseMenu();
		return;
	}

	if (!menuList || !menuList[index])
		return;

	if (activeMenu)
		activeMenu->Freeze();

	switch (Manager::GetGameState())
	{
	case GAME_STATE::STATE_OVERWORLD:
	{
#pragma region Overworld Menu Handling
		switch ((OVERWORLD_MENUS)index)
		{
		case OVERWORLD_MENUS::MENU_BASE:
		{

			break;
		}
		case OVERWORLD_MENUS::MENU_CHARACTER:
		{
			std::vector<Character*> party = Manager::GetParty();

			if (party.size() <= 0)
			{
				activeMenu->Unfreeze();
				return;
			}

			char** texts = new char* [party.size()];
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

			menuList[(int)OVERWORLD_MENUS::MENU_CHARACTER]->SetOptions(texts, options, positions, layouts, 1, (int)party.size());
			menuList[(int)OVERWORLD_MENUS::MENU_CHARACTER]->SetRectangle(MyRectangle(Point<float>(activeMenu->GetRectangle()->Right(), activeMenu->GetRectangle()->Top()), 150, 150));

			delete[] texts;
			delete[] options;
			delete[] positions;
			delete[] layouts;
			break;
		}
		case OVERWORLD_MENUS::MENU_INVENTORY:
		{

			break;
		}
		case OVERWORLD_MENUS::MENU_OPTIONS:
		{

			break;
		}
		}
#pragma endregion
		break;
	}
	case GAME_STATE::STATE_TITLE:
	{
#pragma region Title Menu Handling

		switch ((TITLE_MENUS)index)
		{
		case TITLE_MENUS::MENU_TITLE:
		{
			
			break;
		}
		}

#pragma endregion
	}
	default:
	{

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

void MenuManager::SetMenuOpacity(float _opacity)
{
	for (int i = 0; i < GetMenuCount(); i++)
	{
		menuList[i]->SetOpacity(_opacity);
	}
}


void MenuManager::Update(float delta_time)
{
	switch (Manager::GetGameState())
	{
	case GAME_STATE::STATE_TITLE:
	{

		break;
	}
	case GAME_STATE::STATE_OVERWORLD:
	{
		if (!activeMenu)
		{
			if (Manager::IsKeyPressed(KEYS::KEY_MENU))
			{
				Manager::FreezeScene();
				OpenMenu((int)OVERWORLD_MENUS::MENU_BASE);
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
				if (option == (int)OVERWORLD_MENUS::MENU_PREVIOUS)
					OpenMenu(option);

				if (activeMenu == menuList[(int)OVERWORLD_MENUS::MENU_BASE])
				{
					OpenMenu(option);
				}
				else if (activeMenu == menuList[(int)OVERWORLD_MENUS::MENU_CHARACTER])
				{

				}
				else if (activeMenu == menuList[(int)OVERWORLD_MENUS::MENU_INVENTORY])
				{

				}
				else if (activeMenu == menuList[(int)OVERWORLD_MENUS::MENU_OPTIONS])
				{

				}
			}
		}
		break;
	}
	default:
	{
		break;
	}
	}
}


void MenuManager::PlayHoverSound()
{
	Manager::PlayWAV(0);
}


int MenuManager::GetMenuCount()
{
	switch (Manager::GetGameState())
	{
	case GAME_STATE::STATE_TITLE:
		return (int)TITLE_MENUS::MENU_MAX;
		break;
	case GAME_STATE::STATE_OVERWORLD:
		return (int)OVERWORLD_MENUS::MENU_MAX;
		break;
	default:
		return 0;
	}
}