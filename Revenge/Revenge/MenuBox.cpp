#include "defines.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Manager.h"
#include "Texture.h"
#include "MenuManager.h"
#include "FileReader.h"
#include "InputManager.h"

MenuBox::MenuBox(float _x, float _y, float _width, float _height, Texture* _texture, Texture* _arrowTexture, float _layer, float _opacity, ANCHOR_POINT _anchor) : UISprite(_x, _y, _width, _height, _texture, _layer, _opacity, _anchor)
{
	optionAt = new Point<int>();
	arrow = new UISprite(0, 0, 7, 7, 0, 0, 15, 15, _arrowTexture, _layer + .1f, _opacity, _anchor);
	// options need to be set later
	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::HCENTER)
	{
		textOffset.x = -rectangle->Width() / 2.f;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::RIGHT)
	{
		//textOffset.x = rectangle->Width() / 2.f;
	}

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::VCENTER)
	{
		textOffset.y = -rectangle->Height() / 2.f;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::BOTTOM)
	{
		//textOffset.y = rectangle->Height() / 2.f;
	}
}

MenuBox::MenuBox(float _x, float _y, float _width, float _height, Texture* _texture, Texture* _arrowTexture, const char* filename) : UISprite(_x, _y, _width, _height, _texture, .8f, 1.f, ANCHOR_POINT::ANCHOR_TOP_LEFT)
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
		option->x = (float)optionData[i].position.x;
		option->y = (float)optionData[i].position.y;

		options[optionData[i].matrixLocation.x][optionData[i].matrixLocation.y] = option;
	}
	
	anchor = reader.GetAnchor();

	reader.Close();

	delete[] optionData;

	optionAt = new Point<int>();\
	arrow = new UISprite(0, 0, 7, 7, 0, 0, 15, 15, Manager::GetTexture((int)TEXTURES_TEST::TEX_T_ARROW), layer + .1f, opacity, anchor);
	// options need to be set later
	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::HCENTER)
	{
		//textOffset.x = -rectangle->Width() / 2.f;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::RIGHT)
	{
		//textOffset.x = rectangle->Width() / 2.f;
	}

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::VCENTER)
	{
		//textOffset.y = -rectangle->Height() / 2.f;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::BOTTOM)
	{
		//textOffset.y = rectangle->Height() / 2.f;
	}
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


void MenuBox::SetOpacity(float _opacity)
{
	Sprite::SetOpacity(_opacity);
	arrow->SetOpacity(_opacity);
}

void MenuBox::UpdateArrowLocation()
{
	int offsetX = -12, offsetY = 0;
	float fontSize = 20.f; // this should probably be like retrieved from the renderer
	offsetX += (int)rectangle->X();
	offsetY += (int)rectangle->Y();
	offsetY += (int)((fontSize / 2.f) - (arrow->GetRectangle()->Height() / 2.f) - 2);

	Point<float> drawLocation = Point<float>(options[optionAt->x][optionAt->y]->x + offsetX, options[optionAt->x][optionAt->y]->y + offsetY);

	// adjust the horizontal location of the arrow based on the orientation of the text
	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::HCENTER)
	{
		drawLocation.x -= Manager::MeasureString(options[optionAt->x][optionAt->y]->text).x / 2.f;
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::RIGHT)
	{
		drawLocation.x -= Manager::MeasureString(options[optionAt->x][optionAt->y]->text).x;
	}

	arrow->SetPos(drawLocation);
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
	if (texture)
	{
		// source rectangles for menu parts
		MyRectangle cornerSource(0, 0, 10, 10);
		MyRectangle borderSource(10, 0, 10, 10);
		MyRectangle backgroundSource(20, 0, 10, 10);

		MyRectangle screenspaceRec = GetScreenLocation(*rectangle, anchor);

		// background
		spriteBatch->DrawUI(texture, rectangle, &backgroundSource, opacity, layer, 0, anchor);
		// edges
		spriteBatch->DrawUI(texture, &MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), borderSource.Width(), rectangle->Height()), &borderSource, opacity, layer + .2f, 0); // left
		borderSource.SetY(10);
		spriteBatch->DrawUI(texture, &MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), rectangle->Width(), borderSource.Height()), &borderSource, opacity, layer + .2f, (int)ROTATIONS::ROT_90); // top
		borderSource.SetY(20);
		spriteBatch->DrawUI(texture, &MyRectangle(screenspaceRec.Right() - borderSource.Width(), screenspaceRec.Y(), borderSource.Width(), rectangle->Height()), &borderSource, opacity, layer + .2f, (int)ROTATIONS::HORIZONTAL); // right
		borderSource.SetY(30);
		spriteBatch->DrawUI(texture, &MyRectangle(screenspaceRec.X(), screenspaceRec.Bottom() - borderSource.Height(), rectangle->Width(), borderSource.Height()), &borderSource, opacity, layer + .2f, (int)ROTATIONS::ROT_270); // bottom
		// corners
		spriteBatch->DrawUI(texture, &MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), cornerSource.Width(), cornerSource.Height()), &cornerSource, opacity, layer + .3f, 0); // top left
		cornerSource.SetY(10);
		spriteBatch->DrawUI(texture, &MyRectangle(screenspaceRec.Right() - cornerSource.Width(), screenspaceRec.Y(), cornerSource.Width(), cornerSource.Height()), &cornerSource, opacity, layer + .3f, (int)ROTATIONS::ROT_90); // top right
		cornerSource.SetY(20);
		spriteBatch->DrawUI(texture, &MyRectangle(screenspaceRec.Right() - cornerSource.Width(), screenspaceRec.Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height()), &cornerSource, opacity, layer + .3f, (int)ROTATIONS::ROT_180); // bottom right
		cornerSource.SetY(30);
		spriteBatch->DrawUI(texture, &MyRectangle(screenspaceRec.X(), screenspaceRec.Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height()), &cornerSource, opacity, layer + .3f, (int)ROTATIONS::ROT_270); // bottom left
	}

	// write text
	for (int i = 0; i < optionsWidth; i++)
	{
		for (int j = 0; j < optionsHeight; j++)
		{
			MenuOption* option = options[i][j];
			if (option)
			{
				char* text = new char[option->text.length() + 1];
				strcpy_s(text, option->text.length() + 1, option->text.c_str());
				MyRectangle textRectangle = MyRectangle(rectangle->X() + option->x + textOffset.x, rectangle->Y() + option->y + textOffset.y, Manager::MeasureString(text).x, 10);
				spriteBatch->WriteText(text, &textRectangle, layer + .1f, opacity, anchor);
				delete[] text;
			}
		}
	}

	// draw the arrow
	arrow->Draw(spriteBatch);
}

void MenuBox::Freeze()
{
	Sprite::Freeze();
	ResetArrow();
}


void MenuBox::BindCallbacks()
{
	InputManager::KeyPressedCallbacks_Attatch(KEYS::KEY_UP, std::bind(&MenuBox::UpPressed, this));
}
void MenuBox::UnbindCallbacks()
{

}

void MenuBox::UpPressed()
{

}
void MenuBox::DownPressed()
{

}
void MenuBox::LeftPressed()
{

}
void MenuBox::RightPressed()
{

}

void MenuBox::SelectPressed()
{

}