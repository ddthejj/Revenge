#include "defines.h"
#include "PauseMenu.h"
#include "TextureManager.h"
#include "MenuManager.h"
#include "Manager.h"
#include "Character.h"
#include "OverworldManager.h"
#include "Rectangle.h"

TitleMenu::TitleMenu() : MenuBox(
	std::string("TitleMenuMENU_TITLE"), 
	0, 0, 300, 300, 
	nullptr, 
	TextureManager::GetTexture((int)TEXTURES_TITLE::TEX_TITLE_ARROW)
)
{
	optionsWidth = 1; optionsHeight = 4;
	anchor = ANCHOR_POINT::ANCHOR_CENTER;
	options.push_back(std::vector<MenuOption*>());
	options[0].push_back(new MenuOption("New Game", 0, 30, ANCHOR_POINT::ANCHOR_CENTER, std::bind(&TitleMenu::NewGameSelected, this, std::placeholders::_1)));
	options[0].push_back(new MenuOption("Continue", 0, 60, ANCHOR_POINT::ANCHOR_CENTER, std::bind(&TitleMenu::ContinueSelected, this, std::placeholders::_1)));
	options[0].push_back(new MenuOption("Options", 0, 90, ANCHOR_POINT::ANCHOR_CENTER, std::bind(&TitleMenu::OptionsSelected, this, std::placeholders::_1)));
	options[0].push_back(new MenuOption("Quit", 0, 120, ANCHOR_POINT::ANCHOR_CENTER, std::bind(&TitleMenu::QuitSelected, this, std::placeholders::_1)));
}

void TitleMenu::OnClose()
{
	Manager::Quit();
}

void TitleMenu::NewGameSelected(MenuOption* option)
{
	Manager::UnloadGameState();
	Manager::InitOverworld();
}

void TitleMenu::ContinueSelected(MenuOption* option)
{
}

void TitleMenu::OptionsSelected(MenuOption* option)
{
}

void TitleMenu::QuitSelected(MenuOption* option)
{
	OnClose();
}

OverworldMenu::OverworldMenu() : MenuBox(
	std::string("OverworldMenuMENU_BASE"), 
	50, 50, 150, 200, 
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_MENU),
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_ARROW)
)
{
	optionsWidth = 1; optionsHeight = 5;
	anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT;
	options.push_back(std::vector<MenuOption*>());
	options[0].push_back(new MenuOption("Character", 40, 30, ANCHOR_POINT::ANCHOR_TOP_LEFT, std::bind(&OverworldMenu::CharacterSelected, this, std::placeholders::_1)));
	options[0].push_back(new MenuOption("Inventory", 40, 60, ANCHOR_POINT::ANCHOR_TOP_LEFT, std::bind(&OverworldMenu::InventorySelected, this, std::placeholders::_1)));
	options[0].push_back(new MenuOption("Magic",	 40, 90, ANCHOR_POINT::ANCHOR_TOP_LEFT, std::bind(&OverworldMenu::MagicSelected, this, std::placeholders::_1)));
	options[0].push_back(new MenuOption("Options",	40, 120, ANCHOR_POINT::ANCHOR_TOP_LEFT, std::bind(&OverworldMenu::OptionsSelected, this, std::placeholders::_1)));
	options[0].push_back(new MenuOption("Back",		40, 150, ANCHOR_POINT::ANCHOR_TOP_LEFT, std::bind(&OverworldMenu::BackSelected, this, std::placeholders::_1)));

}

void OverworldMenu::OnClose()
{
	MenuManager::MenuClosed(this);
}

void OverworldMenu::CharacterSelected(MenuOption* option)
{
	PartyMenu* partyMenu = new PartyMenu(Point<float>(this->GetRectangle()->Right(), this->GetRectangle()->Top()));
	partyMenu->Open(this);
}

void OverworldMenu::InventorySelected(MenuOption* option)
{
}

void OverworldMenu::MagicSelected(MenuOption* option)
{
}

void OverworldMenu::OptionsSelected(MenuOption* option)
{
}

void OverworldMenu::BackSelected(MenuOption* option)
{
	OnClose();
}

PartyMenu::PartyMenu(Point<float> location) : MenuBox(
	std::string("OverworldMenuMENU_CHARACTER"),
	location.x, location.y, 150, 150,
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_MENU),
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_ARROW)
)
{
	// menu for selecting party members
	std::vector<Character*> party = OverworldManager::GetCurrentParty();

	options.push_back(std::vector<MenuOption*>());

	// party members
	for (int i = 0; i < (int)party.size(); i++)
	{
		options[0].push_back(new CharacterMenuOption(
			party[i]->GetCharacterData()->GivenName(),
			40.f,
			30.f * (float)(i + 1),
			ANCHOR_POINT::ANCHOR_TOP_LEFT,
			std::bind(&PartyMenu::PlayerSelected, this, std::placeholders::_1),
			party[i]
		));

	}
	// back button
	options[0].push_back(new MenuOption(
		std::string("Back"),
		40.f,
		30.f * (float)(party.size() + 1),
		ANCHOR_POINT::ANCHOR_TOP_LEFT,
		std::bind(&PartyMenu::BackSelected, this, std::placeholders::_1)
	));

	optionsWidth = 1; optionsHeight = (int)party.size() + 1;
}

void PartyMenu::PlayerSelected(MenuOption* option)
{
	StatsMenu* statsMenu = new StatsMenu(Point<float>(GetRectangle()->Right(), GetRectangle()->Top()), ((CharacterMenuOption*)option)->character);
	statsMenu->Open(this);
}

void PartyMenu::BackSelected(MenuOption* option)
{
	OnClose();
}

void PartyMenu::OnClose()
{
	MenuManager::MenuClosed(this);
}

InventoryMenu::InventoryMenu() : MenuBox(
	std::string("OverworldMenuMENU_INVENTORY"),
	50, 50, 10, 10,
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_MENU),
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_ARROW)
)
{
}

void InventoryMenu::OnClose()
{
}

OptionsMenu::OptionsMenu() : MenuBox(
	std::string("OverworldMenuMENU_OPTIONS"), 
	50, 50, 10, 10,
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_MENU),
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_ARROW)
)
{
}

void OptionsMenu::OnClose()
{
}

StatsMenu::StatsMenu(Point<float> location, Character* _character) : MenuBox(
	std::string("OverworldMenuMENU_CHARACTER_STATS"), 
	location.x, location.y, 400, 400,
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_MENU),
	TextureManager::GetTexture((int)TEXTURES_TEST::TEX_T_ARROW)
)
{
	character = _character;

	options.push_back(std::vector<MenuOption*>());
	options[0].push_back(new MenuOption("Back", 40.f, 40.f, ANCHOR_POINT::ANCHOR_BOTTOM_RIGHT, std::bind(&StatsMenu::BackSelected, this, std::placeholders::_1)));

	optionsHeight = 1; optionsWidth = 1;
}

void StatsMenu::BackSelected(MenuOption* option)
{
	OnClose();
}

void StatsMenu::OnClose()
{
	MenuManager::MenuClosed(this);
}
