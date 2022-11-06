#include "defines.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Manager.h"
#include "Texture.h"
#include "MenuManager.h"
#include "FileReader.h"
#include "InputManager.h"

MenuBox::MenuBox(float _x, float _y, float _width, float _height, const Texture* _texture, const Texture* _arrowTexture, int _menuValue, float _layer, float _opacity, ANCHOR_POINT _anchor)
	: BorderedBox(_x, _y, _width, _height, _texture, _layer, _opacity, _anchor)
{
	optionAt = new Point<int>();
	arrow = new UISprite(0, 0, 7, 7, 0, 0, 15, 15, _arrowTexture, _layer + .1f, _opacity, _anchor);
	menuValue = _menuValue;
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

MenuBox::MenuBox(float _x, float _y, float _width, float _height, const Texture* _texture, const Texture* _arrowTexture, int _menuValue, const char* filename)
	: BorderedBox(_x, _y, _width, _height, _texture, .8f, 1.f, ANCHOR_POINT::ANCHOR_TOP_LEFT)
{
	menuValue = _menuValue;

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

	optionAt = new Point<int>(); \
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

	Activate();

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
				goto FOUNDOPTION; // pog goto
			}
		}
	}
FOUNDOPTION:
	UpdateArrowLocation();
}

void MenuBox::SetOptions(std::string* texts, int* option, Point<float>* positions, Point<int>* layout, ANCHOR_POINT* anchors, int sizeX, int sizeY)
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
		for (int j = 0; j < optionsHeight; j++)
			options[i][j] = nullptr;
	}

	for (int i = 0; i < optionsWidth * optionsHeight; i++)
	{
		options[layout[i].x][layout[i].y] = new MenuOption(texts[i], option[i], positions[i].x, positions[i].y, anchors[i]);
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
	DrawBox(spriteBatch);

	// write text
	for (int i = 0; i < optionsWidth; i++)
	{
		for (int j = 0; j < optionsHeight; j++)
		{
			MenuOption* option = options[i][j];
			if (option)
			{
				MyRectangle textRectangle = MyRectangle(rectangle->X() + option->x + textOffset.x, rectangle->Y() + option->y + textOffset.y, 100, 10);//Manager::MeasureString(option->text).x, 10);
				spriteBatch->WriteText(option->text.c_str(), &textRectangle, layer + .1f, opacity, anchor);
			}
		}
	}

	// draw the arrow
	arrow->Draw(spriteBatch);
	return;
}


void MenuBox::Activate()
{
	Sprite::Activate();
}

void MenuBox::Deactivate()
{
	Sprite::Deactivate();
}

void MenuBox::Freeze()
{
	Sprite::Freeze();
	//UnbindCallbacks();
	ResetArrow();
}

void MenuBox::Unfreeze()
{
	Sprite::Unfreeze();
	//BindCallbacks();
}


void MenuBox::BindCallbacks()
{
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_UP, std::bind(&MenuBox::UpPressed, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_DOWN, std::bind(&MenuBox::DownPressed, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_LEFT, std::bind(&MenuBox::LeftPressed, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_RIGHT, std::bind(&MenuBox::RightPressed, this), this);

	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_INTERACT, std::bind(&MenuBox::InteractPressed, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_MENU, std::bind(&MenuBox::MenuPressed, this), this);
}
void MenuBox::UnbindCallbacks()
{
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_UP, std::bind(&MenuBox::UpPressed, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_DOWN, std::bind(&MenuBox::DownPressed, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_LEFT, std::bind(&MenuBox::LeftPressed, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_RIGHT, std::bind(&MenuBox::RightPressed, this), this);

	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_INTERACT, std::bind(&MenuBox::InteractPressed, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_MENU, std::bind(&MenuBox::MenuPressed, this), this);
}

void MenuBox::UpPressed()
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
void MenuBox::DownPressed()
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
void MenuBox::LeftPressed()
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
void MenuBox::RightPressed()
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

void MenuBox::InteractPressed()
{
	MenuManager::OptionSelected(ChooseOption());
}

void MenuBox::MenuPressed()
{
	MenuManager::CloseAllMenus();
	Manager::UnfreezeScene();
}
