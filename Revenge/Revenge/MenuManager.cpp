#include "defines.h"
#include "MenuManager.h"
#include "TextureManager.h"
#include "PauseMenu.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Sprite.h"
#include "Character.h"
#include "InputManager.h"
#include "DialogueBox.h"
#include "SoundManager.h"

std::vector<MenuBox*> MenuManager::menuList = std::vector<MenuBox*>();
MenuBox* MenuManager::activeMenu = nullptr;
DialogueBox* MenuManager::activeDialogueBox = nullptr;

void MenuManager::Init()
{

}

void MenuManager::Clean()
{
	UnloadMenus();
}

void MenuManager::UnloadMenus()
{
	for (int i = 0; i < menuList.size(); i++)
	{
		if (menuList[i])
		{
			SafeDelete(menuList[i]);
		}
	}
	menuList.clear();

	activeMenu = nullptr;
	SafeDelete(activeDialogueBox);
}

void MenuManager::CloseAllMenus()
{
	for (auto it = menuList.begin(); it != menuList.end(); it++)
	{
		(*it)->Deactivate();
		SafeDelete(*it);
	}
	menuList.clear();

	Manager::UnfreezeScene();
}

void MenuManager::SetMenuOpacity(float _opacity)
{
	for (int i = 0; i < menuList.size(); i++)
	{
		if (menuList[i])
		{
			menuList[i]->SetOpacity(_opacity);
		}
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

void MenuManager::OpenTitleMenu()
{
	activeMenu = new TitleMenu();
	activeMenu->Open(nullptr);
}

void MenuManager::OpenOverworldMenu()
{
	activeMenu = new OverworldMenu();
	activeMenu->Open(nullptr);
}

void MenuManager::MenuCreated(MenuBox* menu)
{
	menuList.push_back(menu);
}

void MenuManager::ActiveMenuCreated(MenuBox* menu)
{
	MenuCreated(menu);
	activeMenu->Freeze();
	activeMenu = menu;
}

void MenuManager::MenuClosed(MenuBox* menu)
{
	for (auto it = menuList.begin(); it != menuList.end(); it++)
	{
		if (*it == menu)
		{
			if (menu == activeMenu)
			{
				activeMenu = menu->previousMenu;
			}

			SafeDelete(menu);
			menuList.erase(it);

			if (activeMenu)
			{
				activeMenu->Unfreeze();
			}
			else
			{
				Manager::UnfreezeScene();
			}

			return;
		}
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

	activeDialogueBox = new DialogueBox("DialogBox", 0, 50, WIDTH - 100, (HEIGHT / 8.f) + 25, TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_MENU), Manager::GetTexture((int)TEXTURES_TEST::TEX_T_ARROW));
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


