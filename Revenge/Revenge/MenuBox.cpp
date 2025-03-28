#include "defines.h"
#include "MenuBox.h"
#include "SpriteBatch.h"
#include "Manager.h"
#include "Texture.h"
#include "MenuManager.h"
#include "FileReader.h"
#include "InputManager.h"
#include "SoundManager.h"

MenuBox::MenuBox(std::string _debugName, float _x, float _y, float _width, float _height, const Texture* _texture, const Texture* _arrowTexture, float _layer, float _opacity, ANCHOR_POINT _anchor)
	: BorderedBox(_debugName, _x, _y, _width, _height, _texture, _layer, _opacity, _anchor)
{
	optionAt = new Point<int>();
	arrow = new UISprite("menuArrow", 0, 0, 7, 7, 0, 0, 15, 15, _arrowTexture, _layer + .1f, _opacity, _anchor);
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

MenuBox::~MenuBox()
{
	if (active) Deactivate();

	for (int i = 0; i < optionsWidth; i++)
	{
		for (int j = 0; j < optionsHeight; j++)
		{
			if (options[i][j])
				SafeDelete(options[i][j]);
		}
	}

	SafeDelete(optionAt);
	SafeDelete(arrow);
}


void MenuBox::Open(MenuBox* _previousMenu)
{
	previousMenu = _previousMenu;
	
	MenuManager::ActiveMenuCreated(this);
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
				UpdateArrowLocation();
				return;
			}
		}
	}

	UpdateArrowLocation();
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
	ANCHOR_POINT optionAnchor = options[optionAt->x][optionAt->y]->anchor;
	Point<float> optionDimensions = options[optionAt->x][optionAt->y]->GetDimenstions();

	int offsetX = (unsigned char)optionAnchor & (unsigned char)ANCHOR_POINT::RIGHT ? 12 : -12, offsetY = 0;
	float fontSize = optionDimensions.y;
	offsetY += (int)(fontSize * ((unsigned char)optionAnchor & (unsigned char)ANCHOR_POINT::RIGHT ? .6f : .2f));
	Point<float> drawLocation = Point<float>(options[optionAt->x][optionAt->y]->x + offsetX, options[optionAt->x][optionAt->y]->y + offsetY);

	// adjust the horizontal location of the arrow based on the anchor point of the option
	if ((unsigned char)optionAnchor & (unsigned char)ANCHOR_POINT::HCENTER)
	{
		drawLocation.x -= optionDimensions.x / 2.f;
		drawLocation.y -= optionDimensions.y / 2.5f;
	}
	else if ((unsigned char)optionAnchor & (unsigned char)ANCHOR_POINT::RIGHT)
	{
		drawLocation.x += optionDimensions.x;
	}

	arrow->SetPos(drawLocation);
	arrow->SetAnchorPoint(optionAnchor);
}

void MenuBox::ResetArrow()
{
	arrow->SetSourcePos(Point<float>(0, 0));
	arrowTimer = 0;
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
				MyRectangle textRectangle = MyRectangle(option->x + textOffset.x, option->y + textOffset.y, option->GetDimenstions().x, option->GetDimenstions().y);
				spriteBatch->WriteTextInSprite(option->GetText().c_str(), &textRectangle, this, layer + .1f, opacity, option->anchor);
			}
		}
	}

	// draw the arrow
	arrow->DrawInSprite(spriteBatch, this);
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
	UnbindCallbacks();
	ResetArrow();
}

void MenuBox::Unfreeze()
{
	Sprite::Unfreeze();
	BindCallbacks();
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
	SoundManager::Play(0);
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
	SoundManager::Play(0);
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
	SoundManager::Play(0);
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
	SoundManager::Play(0);
}

void MenuBox::InteractPressed()
{
	//MenuManager::OptionSelected(ChooseOption());
	options[optionAt->x][optionAt->y]->OnPressed();
}

void MenuBox::MenuPressed()
{
	MenuManager::CloseAllMenus();
	Manager::UnfreezeScene();
}

MenuBox::MenuOption::MenuOption(std::string _text, float _x, float _y, ANCHOR_POINT _anchor, std::function<void(MenuOption*)> _onPressedFunc)
{
	text = std::string(_text);
	x = _x; y = _y;
	anchor = _anchor;
	dimensions = Manager::MeasureString(_text);
	onPressedFunc = _onPressedFunc;
}

void MenuBox::MenuOption::SetText(std::string _text)
{
	text = _text;
	dimensions = Manager::MeasureString(_text);
}
