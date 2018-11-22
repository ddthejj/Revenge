#pragma once
#include "Sprite.h"
#include <string>
#include <vector>

class MenuBox : public Sprite
{
	friend class MenuManager;

	struct MenuOption
	{
		std::string text = "DEFAULT";
		int option = 0;
		float x = 0.f, y = 0.f;

		MenuOption() = default;
		MenuOption(char* _text, int _option, float _x, float _y) { text = std::string(_text); option = _option; x = _x; y = _y; }
	};

	std::vector<MenuOption> options;
	int optionAt;

protected:

	MenuBox* previousMenu;

public:

	MenuBox(float _x, float _y, float _width, float _height, Texture* _texture, const char* filename);
	~MenuBox();

	void Open(MenuBox* _previousMenu);
	void ChangeOptions(char** texts, int* options, float** positions, int count);
	void Resize(float x, float y, float height, float width);
	void Move(float x, float y);

	int ChooseOption() { return optionAt; }
	//int

	virtual void Draw(SpriteBatch* spriteBatch);
	virtual void Update();
};

