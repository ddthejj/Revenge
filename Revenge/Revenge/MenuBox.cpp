#include "defines.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Manager.h"
#include "Texture.h"
#include <iostream>
#include <fstream>

MenuBox::MenuBox(float _x, float _y, float _width, float _height, Texture* _texture, const char* filename) : Sprite(_x, _y, _width, _height, _texture, 0.f)
{
	optionAt = new Point<int>();


	std::ifstream menu(filename, std::ios::in);

	if (menu.is_open())
	{
		char word[255];
		int whichChar = 0;

		whichChar = 0;
		do
		{
			menu.get(word[whichChar]);
		} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !menu.eof());
		word[whichChar] = '\0';

		optionsWidth = atoi(word);

		whichChar = 0;
		do
		{
			menu.get(word[whichChar]);
		} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !menu.eof());
		word[whichChar] = '\0';

		optionsHeight = atoi(word);

		options = new MenuOption*[optionsWidth];
		for (int i = 0; i < optionsWidth; i++)
			options[i] = new MenuOption[optionsHeight];


		while (!menu.eof())
		{
			MenuOption option;

			whichChar = 0;
			do
			{
				menu.get(word[whichChar]);
			} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !menu.eof());

			word[whichChar] = '\0';
			option.text = std::string(word);

			whichChar = 0;
			do
			{
				menu.get(word[whichChar]);
			} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !menu.eof());

			word[whichChar] = '\0';
			option.option = atoi(word);

			whichChar = 0;
			do
			{
				menu.get(word[whichChar]);
			} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !menu.eof());

			word[whichChar] = '\0';
			option.x = (float)atof(word);

			whichChar = 0;
			do
			{
				menu.get(word[whichChar]);
			} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !menu.eof());

			word[whichChar] = '\0';
			option.y = (float)atof(word);

			whichChar = 0;
			do
			{
				menu.get(word[whichChar]);
			} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !menu.eof());

			word[whichChar] = '\0';
			int optionX = atoi(word);

			whichChar = 0;
			do
			{
				menu.get(word[whichChar]);
			} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !menu.eof());

			word[whichChar] = '\0';
			int optionY = atoi(word);

			options[optionX][optionY] = option;
		}
	}
}

MenuBox::~MenuBox()
{
	for (int i = 0; i < optionsWidth; i++)
		if (options[i])
			delete[] options[i];

	delete[] options;

	delete optionAt;
}


void MenuBox::Open(MenuBox* _previousMenu)
{
	previousMenu = _previousMenu;
	Sprite::Activate();
}

void MenuBox::ChangeOptions(char** texts, int* option, Point<float>* positions, Point<int>* layout, int sizeX, int sizeY)
{
	for (int i = 0; i < optionsWidth; i++)
	{
		delete options[i];
	}
	delete options;

	optionsWidth = sizeX;
	optionsHeight = sizeY;

	options = new MenuOption*[optionsWidth];
	for (int i = 0; i < optionsWidth; i++)
		options[i] = new MenuOption[optionsHeight];

	for (int i = 0; i < optionsWidth; i++)
	{
		MenuOption newOption;

		newOption.text = texts[i];
		newOption.option = option[i];
		newOption.x = positions[i].x;
		newOption.y = positions[i].y;

		//options.push_back(MenuOption(texts[i], option[i], positions[i].x, positions[i].y));
		options[layout[i].x][layout[i].y] = MenuOption(newOption);
	}
}

void MenuBox::Resize(float x, float y, float height, float width)
{
	rectangle->SetX(x);
	rectangle->SetY(y);
	rectangle->SetWidth(width);
	rectangle->SetHeight(height);
}

void MenuBox::Move(float x, float y)
{
	rectangle->SetX(x);
	rectangle->SetY(y);
}


int MenuBox::ChooseOption()
{
	return options[optionAt->x][optionAt->y].option;
}

void MenuBox::MoveSelected(int key)
{
	if (key == (int)Manager::KEYS::KEY_UP)
	{
		MenuOption* whichOption = &options[optionAt->x][optionAt->y];

		do
		{
			optionAt->y++;
			if (optionAt->y >= optionsHeight)
				optionAt->y = 0;

			whichOption = &options[optionAt->x][optionAt->y];
		} while (whichOption == nullptr);
	}
	else if (key == (int)Manager::KEYS::KEY_DOWN)
	{
		MenuOption* whichOption = &options[optionAt->x][optionAt->y];

		do
		{
			optionAt->y--;
			if (optionAt->y < 0)
				optionAt->y = optionsHeight - 1;

			whichOption = &options[optionAt->x][optionAt->y];
		} while (whichOption == nullptr);
	}
	else if (key == (int)Manager::KEYS::KEY_RIGHT)
	{
		MenuOption* whichOption = &options[optionAt->x][optionAt->y];

		do
		{
			optionAt->x++;
			if (optionAt->x >= optionsWidth)
				optionAt->x = 0;

			whichOption = &options[optionAt->x][optionAt->y];
		} while (whichOption == nullptr);
	}
	else if (key == (int)Manager::KEYS::KEY_LEFT)
	{
		MenuOption* whichOption = &options[optionAt->x][optionAt->y];

		do
		{
			optionAt->x--;
			if (optionAt->x < 0)
				optionAt->x = optionsWidth - 1;

			whichOption = &options[optionAt->x][optionAt->y];
		} while (whichOption == nullptr);
	}
}


void MenuBox::Update()
{

}

void MenuBox::Draw(SpriteBatch* spriteBatch)
{

	MyRectangle cornerSource(0, 0, 10, 10);
	MyRectangle borderSource(10, 0, 10, 10);
	MyRectangle backgroundSource(20, 0, 10, 10);

	// background
	spriteBatch->DrawUI(texture, rectangle, &backgroundSource, 1.0f, .8f, 0);
	// edges
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Y(), rectangle->Height(), borderSource.Width()), &borderSource, 1.f, .82f); // left
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->Right() - borderSource.Width(), rectangle->Y(), rectangle->Height(), borderSource.Width()), &borderSource, 1.f, .82f, (int)ROTATIONS::HORIZONTAL); // right
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Y(), borderSource.Height(), rectangle->Width()), &borderSource, 1.0f, .82f, (int)ROTATIONS::ROT_90); // top
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Bottom() - borderSource.Height(), borderSource.Height(), rectangle->Width()), &borderSource, 1.0f, .82f, (int)ROTATIONS::ROT_270); // bottom
	// corners
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Y(), cornerSource.Height(), cornerSource.Width()), &cornerSource, 1.0f, .83f); // top left
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->Right() - cornerSource.Width(), rectangle->Y(), cornerSource.Height(), cornerSource.Width()), &cornerSource, 1.0f, .83f, (int)ROTATIONS::ROT_90); // top right
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->Right() - cornerSource.Width(), rectangle->Bottom() - cornerSource.Height(), cornerSource.Height(), cornerSource.Width()), &cornerSource, 1.0f, .83f, (int)ROTATIONS::ROT_180); // bottom right
	spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Bottom() - cornerSource.Height(), cornerSource.Height(), cornerSource.Width()), &cornerSource, 1.0f, .83f, (int)ROTATIONS::ROT_270); // bottom left
	// text
	//spriteBatch->WriteText(L"Menu", &MyRectangle(rectangle->X() + 20, rectangle->Y() + 20, 30, 10), .81f);
	for (int i = 0; i < optionsWidth; i++)
	{
		for (int j = 0; j < optionsHeight; j++)
		{
			MenuOption option = options[i][j];
			spriteBatch->WriteText(option.text.c_str(), &MyRectangle(rectangle->X() + option.x, rectangle->Y() + option.y, (rectangle->Width() - 20), 10), .81f);
		}
	}

}