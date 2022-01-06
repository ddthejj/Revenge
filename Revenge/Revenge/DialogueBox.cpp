#include "defines.h"
#include "DialogueBox.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "InputManager.h"
#include "MenuManager.h"

DialogueBox::DialogueBox(float _x, float _y, float _width, float _height, Texture* _texture, Texture* _arrowTexture, float _layer, float _opacity, ANCHOR_POINT _anchor) : UISprite(_x, _y, _width, _height, _texture, _layer, _opacity, _anchor)
{
	arrow = new UISprite(15, 15, 7, 7, 0, 0, 15, 15, _arrowTexture, _layer + .01f, _opacity, ANCHOR_POINT::ANCHOR_BOTTOM_RIGHT);
	arrow->SetOpacity(0.f);
}

DialogueBox::~DialogueBox()
{
	if (active) Deactivate();
	
	SafeDelete(arrow);
}

void DialogueBox::Open()
{
	Activate();
}

void DialogueBox::Draw(SpriteBatch* spriteBatch)
{
	if (texture)
	{
		// source rectangles for box parts
		MyRectangle cornerSource(0, 0, 10, 10);
		MyRectangle borderSource(10, 0, 10, 10);
		MyRectangle backgroundSource(20, 0, 10, 10);

		MyRectangle screenspaceRec = GetScreenLocation(*rectangle, anchor);

		// background
		spriteBatch->DrawUI(texture, rectangle, &backgroundSource, opacity, layer, 0, anchor);
		// edges
		MyRectangle edgeRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), borderSource.Width(), rectangle->Height());
		spriteBatch->DrawUI(texture, &edgeRectangle, &borderSource, opacity, layer + .2f, 0); // left
		borderSource.SetY(10);
		edgeRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), rectangle->Width(), borderSource.Height());
		spriteBatch->DrawUI(texture, &edgeRectangle, &borderSource, opacity, layer + .2f, (int)ROTATIONS::ROT_90); // top
		borderSource.SetY(20);
		edgeRectangle = MyRectangle(screenspaceRec.Right() - borderSource.Width(), screenspaceRec.Y(), borderSource.Width(), rectangle->Height());
		spriteBatch->DrawUI(texture, &edgeRectangle, &borderSource, opacity, layer + .2f, (int)ROTATIONS::HORIZONTAL); // right
		borderSource.SetY(30);
		edgeRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Bottom() - borderSource.Height(), rectangle->Width(), borderSource.Height());
		spriteBatch->DrawUI(texture, &edgeRectangle, &borderSource, opacity, layer + .2f, (int)ROTATIONS::ROT_270); // bottom
		// corners
		MyRectangle cornerRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Y(), cornerSource.Width(), cornerSource.Height());
		spriteBatch->DrawUI(texture, &cornerRectangle, &cornerSource, opacity, layer + .3f, 0); // top left
		cornerSource.SetY(10);
		cornerRectangle = MyRectangle(screenspaceRec.Right() - cornerSource.Width(), screenspaceRec.Y(), cornerSource.Width(), cornerSource.Height());
		spriteBatch->DrawUI(texture, &cornerRectangle, &cornerSource, opacity, layer + .3f, (int)ROTATIONS::ROT_90); // top right
		cornerSource.SetY(20);
		cornerRectangle = MyRectangle(screenspaceRec.Right() - cornerSource.Width(), screenspaceRec.Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height());
		spriteBatch->DrawUI(texture, &cornerRectangle, &cornerSource, opacity, layer + .3f, (int)ROTATIONS::ROT_180); // bottom right
		cornerSource.SetY(30);
		cornerRectangle = MyRectangle(screenspaceRec.X(), screenspaceRec.Bottom() - cornerSource.Height(), cornerSource.Width(), cornerSource.Height());
		spriteBatch->DrawUI(texture, &cornerRectangle, &cornerSource, opacity, layer + .3f, (int)ROTATIONS::ROT_270); // bottom left
	}

	MyRectangle textRectangle = MyRectangle(25, 25, rectangle->Width() - 50, 10);
	spriteBatch->WriteTextInSprite(currentText.c_str(), &textRectangle, this, layer + .01f, opacity, ANCHOR_POINT::ANCHOR_TOP_LEFT);

	// draw the arrow
	arrow->DrawInSprite(spriteBatch, this);
}

void DialogueBox::Update(float delta_time)
{
	if (charAt < text[textAt].length())
	{
		// scroll text
		currentText.push_back(text[textAt][charAt]);

		charAt++;

		if (charAt >= text[textAt].length())
			arrow->SetOpacity(opacity);
	}
	else
	{
		// blink arrow
		arrowTimer++;
		if (arrowTimer == 40)
		{
			arrow->SetSourcePos(Point<float>(15, 0));
		}
		else if (arrowTimer == 80)
		{
			arrow->SetSourcePos(Point<float>(0, 0));
			arrowTimer = 0;
		}
	}
}

void DialogueBox::SetText(Character* _speaker, std::vector<std::string> _text)
{
	speaker = _speaker;
	text = _text;
}

void DialogueBox::BindCallbacks()
{
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_INTERACT, std::bind(&DialogueBox::InteractPressed, this), this);
}

void DialogueBox::UnbindCallbacks()
{
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_INTERACT, std::bind(&DialogueBox::InteractPressed, this), this);
}

void DialogueBox::InteractPressed()
{
	// reset arrow blinking
	arrow->SetSourcePos(Point<float>(0, 0));
	arrowTimer = 0;
	arrow->SetOpacity(0.f);

	if (charAt >= text[textAt].length())
	{
		// go to the next string, if there is one - else close the dialogue box
		if (textAt < text.size() - 1)
		{
			charAt = 0;
			textAt++;
			currentText = "";
		}
		else
		{
			MenuManager::EndDialogue();
		}
	}
	else
	{
		// skip the text scrolling
		currentText = text[textAt];
		charAt = (int)text[textAt].length();
	}
}
