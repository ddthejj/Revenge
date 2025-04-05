#include "defines.h"
#include "DialogueBox.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "Rectangle.h"
#include "Text.h"

DialogueBox::DialogueBox(std::string _debugName, float _x, float _y, float _width, float _height, const Texture* _texture, const Texture* _arrowTexture, float _layer, float _opacity, ANCHOR_POINT _anchor)
	: BorderedBox(_debugName, _x, _y, _width, _height, _texture, _layer, _opacity, _anchor)
{
	arrow = new UISprite("dialogArrow", 15, 15, 7, 7, 0, 0, 15, 15, _arrowTexture, _layer + .01f, _opacity, ROTATIONS::NONE, ANCHOR_POINT::ANCHOR_BOTTOM_RIGHT);
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
	DrawBox(spriteBatch);

	if (speaker)
	{

	}

	// draw text
	Text text = Text(debugName + "Text", 25, 25, rectangle->Width() - 50, 10, currentText, layer + .01f, opacity, ANCHOR_POINT::ANCHOR_TOP_LEFT);
	spriteBatch->WriteTextInSprite(&text, this);

	// draw the arrow
	arrow->DrawInSprite(spriteBatch, this);
}

void DialogueBox::Update(float delta_time)
{
	ScrollText(scrollSpeed);
}

void DialogueBox::SetText(Character* _speaker, std::vector<std::string> _text)
{
	speaker = _speaker;
	text = _text;
}

void DialogueBox::ScrollText(int speed)
{
	// scroll text
	if (charAt < text[textAt].length())
	{
		// text is currently scrolling

		// ensure next word won't be cut off by the end of the box

		for (int i = 0; i < speed; i++)
		{
			if (text[textAt][charAt] == ' ')
			{
				std::string futureLine = currentLine;
				int futureAt = charAt + 1;

				// fill out futureLine with the next word
				while (text[textAt][futureAt] != ' ' && futureAt < text[textAt].length())
				{
					futureLine += (text[textAt][futureAt]);
					futureAt++;
				}

				Point<float> lineSize = Manager::MeasureString(futureLine);

				if (lineSize.x > (rectangle->Width() - 50.f))
				{
					// next word it too long, add a new line and skip the space character
					currentText += '\n';
					numLines++;
					currentLine = "";
					charAt++;

					// make sure text doesn't overrun the box
					if (numLines == 5)
					{
						std::string nextText(&text[textAt].c_str()[charAt]);
						auto it = text.begin() + textAt + 1;
						text.insert(it, nextText);
						charAt = (int)(text[textAt].length());
						break;
					}
				}
			}

			// scroll text
			currentText.push_back(text[textAt][charAt]);
			currentLine.push_back(text[textAt][charAt]);

			charAt++;

			if (charAt >= text[textAt].length())
			{
				arrow->SetOpacity(opacity);
				break;
			}
		}
	}
	else
	{
		// text is done scrolling

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

void DialogueBox::BindCallbacks()
{
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_INTERACT, std::bind(&DialogueBox::InteractPressed, this), this);
}

void DialogueBox::UnbindCallbacks()
{
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_INTERACT, std::bind(&DialogueBox::InteractPressed, this), this);
}

bool DialogueBox::InteractPressed()
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
  		ScrollText(999);
	}

	return true;
}
