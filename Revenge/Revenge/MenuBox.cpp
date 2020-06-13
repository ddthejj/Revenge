#include "defines.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Manager.h"
#include "Texture.h"
#include "MenuManager.h"
#include "FileReader.h"

MenuBox::MenuBox(float _x, float _y, float _width, float _height, Texture* _texture) : Sprite(_x, _y, _width, _height, _texture, 0.f)
{
	optionAt = new Point<int>();
	arrow = new UISprite(0, 0, 7, 7, 0, 0, 15, 15, Manager::GetTexture((int)TEXTURES::TEX_ARROW), .81f);
	// options need to be set later
}

MenuBox::MenuBox(float _x, float _y, float _width, float _height, Texture* _texture, const char* filename) : MenuBox(_x, _y, _width, _height, _texture)
{
	// use the MenuReader class to read the menu data from a file
	MenuReader reader;
	reader.Open(filename);

	Point<int> optionsDims = reader.GetDimensions();
	optionsWidth = optionsDims.x;
	optionsHeight = optionsDims.y;

	options = new MenuOption * *[optionsDims.x];
	for (int i = 0; i < optionsWidth; i++)
	{
		options[i] = new MenuOption * [optionsDims.y];
		for (int j = 0; j < optionsHeight; j++)
			options[i][j] = nullptr;
	}

	MenuReader::OptionData* optionData = nullptr;
	int optionCount = reader.GetOptions(&optionData);

	for (int i = 0; i < optionCount; i++)
	{
		MenuOption* option = new MenuOption;
		option->text = optionData[i].text;
		option->option = optionData[i].returnValue;
		option->x = optionData[i].position.x;
		option->y = optionData[i].position.y;

		options[optionData[i].matrixLocation.x][optionData[i].matrixLocation.y] = option;
	}

	delete[] optionData;
}

MenuBox::~MenuBox()
{
	if (active) Deactivate();

	for (int i = 0; i < optionsWidth; i++)
	{
		for (int j = 0; j < optionsHeight; j++)
		{
			if (options[i][j])
				delete options[i][j];
		}
		delete[] options[i];
	}
	delete[] options;

	delete optionAt;
	delete arrow;
}


void MenuBox::Open(MenuBox* _previousMenu)
{
	previousMenu = _previousMenu;
	Sprite::Activate();

	optionAt->x = 0; optionAt->y = 0;
	if (options[optionAt->x][optionAt->y])
	{
		UpdateArrowLocation();
		return;
	}

	for (int y = 0; y < optionsHeight; y++)
	{
		for (int x = 0; x < optionsWidth; x++)
		{
			if (options[x][y])
			{
				optionAt->x = x; optionAt->y = y;
				goto FOUNDOPTION;
			}
		}
	}
FOUNDOPTION:
	UpdateArrowLocation();
}

void MenuBox::SetOptions(char** texts, int* option, Point<float>* positions, Point<int>* layout, int sizeX, int sizeY)
{
	for (int i = 0; i < optionsWidth; i++)
	{
		for (int j = 0; j < optionsHeight; j++)
		{
			if (options[i][j])
				delete options[i][j];
		}
		delete[] options[i];
	}
	delete[] options;

	optionsWidth = sizeX;
	optionsHeight = sizeY;

	options = new MenuOption * *[optionsWidth];
	for (int i = 0; i < optionsWidth; i++)
	{
		options[i] = new MenuOption * [optionsHeight];
		for (int j = 0; j < optionsWidth; j++)
			options[i][j] = nullptr;
	}

	for (int i = 0; i < optionsWidth; i++)
	{
		MenuOption* newOption = new MenuOption(texts[i], option[i], positions[i].x, positions[i].y);
		options[layout[i].x][layout[i].y] = newOption;
	}
}

void MenuBox::Resize(float x, float y, float width, float height)
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

void MenuBox::UpdateArrowLocation()
{
	int offsetX = -12, offsetY = 0;
	float fontSize = 20.f; // this should probably be like retrieved from the renderer
	offsetX += (int)rectangle->X();
	offsetY += (int)rectangle->Y();
	offsetY += (int)((fontSize / 2.f) - (arrow->GetRectangle()->Height() / 2.f) - 2);

	arrow->SetPos(Point<float>(options[optionAt->x][optionAt->y]->x + offsetX, options[optionAt->x][optionAt->y]->y + offsetY));
}

void MenuBox::ResetArrow()
{
	arrow->SetSourcePos(Point<float>(0, 0));
	arrowTimer = 0;
}


int MenuBox::ChooseOption()
{
	return options[optionAt->x][optionAt->y]->option;
}


void MenuBox::Update(float delta_time)
{

#pragma region Arrow key movement

	if (Manager::IsKeyPressed(KEYS::KEY_UP))
	{
		MenuOption* whichOption = options[optionAt->x][optionAt->y];

		do
		{
			optionAt->y--;
			if (optionAt->y < 0)
				optionAt->y = optionsHeight - 1;

			whichOption = options[optionAt->x][optionAt->y];
		} while (whichOption == nullptr);

		UpdateArrowLocation();
		ResetArrow();
		MenuManager::PlayHoverSound();
	}
	else if (Manager::IsKeyPressed(KEYS::KEY_DOWN))
	{
		MenuOption* whichOption = options[optionAt->x][optionAt->y];

		do
		{
			optionAt->y++;
			if (optionAt->y >= optionsHeight)
				optionAt->y = 0;

			whichOption = options[optionAt->x][optionAt->y];
		} while (whichOption == nullptr);

		UpdateArrowLocation();
		ResetArrow();
		MenuManager::PlayHoverSound();
	}
	else if (Manager::IsKeyPressed(KEYS::KEY_RIGHT))
	{
		MenuOption* whichOption = options[optionAt->x][optionAt->y];

		do
		{
			optionAt->x++;
			if (optionAt->x >= optionsWidth)
				optionAt->x = 0;

			whichOption = options[optionAt->x][optionAt->y];
		} while (whichOption == nullptr);

		UpdateArrowLocation();
		ResetArrow();
		MenuManager::PlayHoverSound();
	}
	else if (Manager::IsKeyPressed(KEYS::KEY_LEFT))
	{
		MenuOption* whichOption = options[optionAt->x][optionAt->y];

		do
		{
			optionAt->x--;
			if (optionAt->x < 0)
				optionAt->x = optionsWidth - 1;

			whichOption = options[optionAt->x][optionAt->y];
		} while (whichOption == nullptr);

		UpdateArrowLocation();
		ResetArrow();
		MenuManager::PlayHoverSound();
	}

#pragma endregion

#pragma region Mouse Key Selection

	for (int i = 0; i < optionsWidth; i++)
	{
		for (int j = 0; j < optionsHeight; j++)
		{
			if (options[i][j])
			{
				//options[i][j]->text;
			}
		}

	}


#pragma endregion

	arrowTimer++;
	if (arrowTimer == 40)
	{
		arrow->SetSourcePos(Point<float>(15, 0));
	}
	else if (arrowTimer == 80)
	{
		ResetArrow();
	}
}

void MenuBox::Draw(SpriteBatch* spriteBatch)
{
	MyRectangle cornerSource(0, 0, 10, 10);
	MyRectangle borderSource(10, 0, 10, 10);
	MyRectangle backgroundSource(20, 0, 10, 10);

	if (texture)
	{
		// background
		spriteBatch->DrawUI(texture, rectangle, &backgroundSource, 1.0f, .8f, 0);
		// edges
		spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Y(), borderSource.Width(), rectangle->Height()), &borderSource, 1.f, .82f); // left
		borderSource.SetY(10);
		spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Y(), rectangle->Width(), borderSource.Height()), &borderSource, 1.0f, .82f, (int)ROTATIONS::ROT_90); // top
		borderSource.SetY(20);
		spriteBatch->DrawUI(texture, &MyRectangle(rectangle->Right() - borderSource.Width(), rectangle->Y(), borderSource.Width(), rectangle->Height()), &borderSource, 1.f, .82f, (int)ROTATIONS::HORIZONTAL); // right
		borderSource.SetY(30);
		spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Bottom() - borderSource.Height(), rectangle->Width(), borderSource.Height()), &borderSource, 1.0f, .82f, (int)ROTATIONS::ROT_270); // bottom
		// corners
		spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Y(), cornerSource.Width(), cornerSource.Height()), &cornerSource, 1.0f, .83f); // top left
		cornerSource.SetY(10);
		spriteBatch->DrawUI(texture, &MyRectangle(rectangle->Right() - cornerSource.Width(), rectangle->Y(), cornerSource.Width(), cornerSource.Height()), &cornerSource, 1.0f, .83f, (int)ROTATIONS::ROT_90); // top right
		cornerSource.SetY(20);
		spriteBatch->DrawUI(texture, &MyRectangle(rectangle->Right() - cornerSource.Width(), rectangle->Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height()), &cornerSource, 1.0f, .83f, (int)ROTATIONS::ROT_180); // bottom right
		cornerSource.SetY(30);
		spriteBatch->DrawUI(texture, &MyRectangle(rectangle->X(), rectangle->Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height()), &cornerSource, 1.0f, .83f, (int)ROTATIONS::ROT_270); // bottom left
	}
	// text
	//spriteBatch->WriteText(L"Menu", &MyRectangle(rectangle->X() + 20, rectangle->Y() + 20, 30, 10), .81f);
	for (int i = 0; i < optionsWidth; i++)
	{
		for (int j = 0; j < optionsHeight; j++)
		{
			MenuOption* option = options[i][j];
			if (option)
			{
				char* text = new char[option->text.length() + 1];
				strcpy_s(text, option->text.length() + 1, option->text.c_str());
				MyRectangle* textRectangle = new MyRectangle(rectangle->X() + option->x, rectangle->Y() + option->y, (rectangle->Width() - 20), 10);
				spriteBatch->WriteText(text, textRectangle, .81f);
				delete textRectangle;
				delete[] text;
			}
		}
	}
	arrow->Draw(spriteBatch);
}


void MenuBox::Freeze()
{
	Sprite::Freeze();
	ResetArrow();
}
