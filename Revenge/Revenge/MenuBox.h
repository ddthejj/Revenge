#pragma once
#include "Sprite.h"
#include <string>
#include <vector>

template <typename T>
struct Point;

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

	//std::vector<MenuOption> options;
	MenuOption*** options = nullptr;
	int optionsWidth = 0, optionsHeight = 0;
	Point<int>* optionAt = nullptr;
	Sprite* arrow = nullptr;
	int arrowTimer = 0;

protected:

	MenuBox* previousMenu;
	
	void UpdateArrowLocation();
	void ResetArrow();

public:

	MenuBox(float _x, float _y, float _width, float _height, Texture* _texture);
	MenuBox(float _x, float _y, float _width, float _height, Texture* _texture, const char* filename);
	~MenuBox();

	void Open(MenuBox* _previousMenu);
	void SetOptions(char** texts, int* option, Point<float>* positions, Point<int>* layout, int sizeX, int sizeY);
	void Resize(float x, float y, float height, float width);
	void Move(float x, float y);

	int ChooseOption();
	//int

	virtual void Draw(SpriteBatch* spriteBatch);
	virtual void Update();

	virtual void Freeze();
};

