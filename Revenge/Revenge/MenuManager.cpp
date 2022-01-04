#include "defines.h"
#include "MenuManager.h"
#include "Manager.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Sprite.h"
#include "Character.h"
#include "InputManager.h"
#include "DialogueBox.h"

MenuBox** MenuManager::menuList = nullptr;
MenuBox* MenuManager::activeMenu = nullptr;
Texture* MenuManager::menuTex = nullptr;
DialogueBox* MenuManager::activeDialogueBox = nullptr;

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

	SafeDelete(activeDialogueBox);
}


void MenuManager::LoadTitleMenus()
{
	UnloadMenus();

	menuList = new MenuBox * [(int)TITLE_MENUS::MENU_MAX];

	for (int i = 0; i < (int)TITLE_MENUS::MENU_MAX; i++)
		menuList[i] = nullptr;

	menuList[(int)TITLE_MENUS::MENU_TITLE] = new MenuBox(0, 0, 300, 300, nullptr, Manager::GetTexture((int)TEXTURES_TITLE::TEX_TITLE_ARROW), "../Assets/Menus/Menu_Title_Base.txt");
}

void MenuManager::LoadOverworldMenus()
{
	UnloadMenus();

	DelegateHandle* menuPressedHandle = InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_MENU, std::bind(&MenuManager::MenuPressed), nullptr);
	delete menuPressedHandle;
	//InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_INTERACT, std::bind(&MenuManager::InteractPressed));

	menuList = new MenuBox * [(int)OVERWORLD_MENUS::MENU_MAX];
	menuTex = Manager::GetTexture((int)TEXTURES_TEST::TEX_T_MENU);

	for (int i = 0; i < (int)OVERWORLD_MENUS::MENU_MAX; i++)
		menuList[i] = nullptr;

	menuList[(int)OVERWORLD_MENUS::MENU_BASE] = new MenuBox(50, 50, 150, 200, menuTex, Manager::GetTexture((int)TEXTURES_TEST::TEX_T_ARROW), "../Assets/Menus/Menu_Base.txt");
	menuList[(int)OVERWORLD_MENUS::MENU_CHARACTER] = new MenuBox(50, 50, 10, 10, menuTex, Manager::GetTexture((int)TEXTURES_TEST::TEX_T_ARROW));


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
		//InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_INTERACT, std::bind(&MenuManager::InteractPressed), menuList[(int)OVERWORLD_MENUS::MENU_BASE]);
	}
}

void MenuManager::CloseAllMenus()
{
	while (activeMenu)
	{
		activeMenu->Deactivate();
		activeMenu = activeMenu->previousMenu;
	}

	//InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_INTERACT, std::bind(&MenuManager::InteractPressed), menuList[(int)OVERWORLD_MENUS::MENU_BASE]);

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


void MenuManager::MenuPressed()
{
	if (!activeMenu)
	{
		Manager::FreezeScene();
		OpenMenu((int)OVERWORLD_MENUS::MENU_BASE);
	}
	else
	{
		Manager::UnfreezeScene();
		CloseAllMenus();
	}
}

void MenuManager::OptionSelected(int option)
{
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

void MenuManager::StartDialogue(Character* speaker, std::vector<std::string> text)
{
	if (activeDialogueBox)
	{
		// this function should not be called if dialogue is already open
		return;
	}

	Manager::FreezeScene();

	activeDialogueBox = new DialogueBox(0, 50, WIDTH - 100, (HEIGHT / 8.f) + 25, menuTex, Manager::GetTexture((int)TEXTURES_TITLE::TEX_TITLE_ARROW));
	activeDialogueBox->SetText(speaker, text);
	activeDialogueBox->Open();
}

void MenuManager::EndDialogue()
{
	if (!activeDialogueBox) return;

	activeDialogueBox->Deactivate();
	Manager::UnfreezeScene();
	SafeDelete(activeDialogueBox);
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


