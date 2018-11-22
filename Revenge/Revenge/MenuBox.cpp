#include "defines.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include <iostream>
#include <fstream>

MenuBox::MenuBox(float _x, float _y, float _width, float _height, Texture* _texture, const char* filename) : Sprite(_x, _y, _width, _height, _texture, 0.f)
{
	std::ifstream menu(filename, std::ios::in);

	if (menu.is_open())
	{
		while (!menu.eof())
		{
			MenuOption option;

			char word[255];
			int whichChar = 0;

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

			options.push_back(option);
		}
	}
}

MenuBox::~MenuBox()
{
}


void MenuBox::Open(MenuBox* _previousMenu)
{
	previousMenu = _previousMenu;
	Sprite::Activate();
}

void MenuBox::ChangeOptions(char** texts, int* option, float** positions, int count)
{
	options.clear();
	for (int i = 0; i < count; i++)
	{
		options.push_back(MenuOption(texts[i], option[i], positions[i][0], positions[i][1]));
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
	for (int i = 0; i < options.size(); i++)
	{
		MenuOption option = options[i];
		spriteBatch->WriteText(option.text.c_str(), &MyRectangle(rectangle->X() + option.x, rectangle->Y() + option.y, (rectangle->Width() - 20) , 10), .81f);
	}

}