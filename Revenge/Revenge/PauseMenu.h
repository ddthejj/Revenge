#pragma once

#include "MenuBox.h"

class Character;

class TitleMenu : public MenuBox
{
public:

	TitleMenu();

protected:

	virtual void OnClose() override;
	void NewGameSelected(MenuOption* option);
	void ContinueSelected(MenuOption* option);
	void OptionsSelected(MenuOption* option);
	void QuitSelected(MenuOption* option);
};

class OverworldMenu : public MenuBox
{
protected:

public:

	OverworldMenu();

protected:

	virtual void OnClose() override;
	void CharacterSelected(MenuOption* option);
	void InventorySelected(MenuOption* option);
	void MagicSelected(MenuOption* option);
	void OptionsSelected(MenuOption* option);
	void BackSelected(MenuOption* option);
};

class PartyMenu : public MenuBox
{
	struct CharacterMenuOption : MenuOption
	{
		Character* character;

		CharacterMenuOption(std::string _text, float _x, float _y, ANCHOR_POINT _anchor, std::function<void(MenuOption*)> _onPressedFunc, Character* _character) : MenuOption(_text, _x, _y, _anchor, _onPressedFunc)
		{
			character = _character;
		}
	};
public:

	PartyMenu(Point<float> location);

protected:

	void PlayerSelected(MenuOption* option);
	void BackSelected(MenuOption* option);
	virtual void OnClose() override;
};

class InventoryMenu : public MenuBox
{
public:

	InventoryMenu();

protected:

	virtual void OnClose() override;
};

class OptionsMenu : public MenuBox
{
public:

	OptionsMenu();

protected:

	virtual void OnClose() override;
};

class StatsMenu : public MenuBox
{
public:

	Character* character;

	StatsMenu(Point<float> location, Character* _character);

protected:

	void BackSelected(MenuOption* option);
	virtual void OnClose() override;
};