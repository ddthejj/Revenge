#include "defines.h"
#include "TitleManager.h"
#include "Manager.h"
#include "Sprite.h"
#include "Manager.h"
#include "Texture.h"
#include "InputManager.h"
#include "Text.h"
#include "SoundManager.h"
#include "MenuManager.h"

UISprite* TitleManager::logo = nullptr;
UISprite* TitleManager::title = nullptr;
UISprite* TitleManager::titleBackground = nullptr;

TitleManager::TITLE_STATE TitleManager::titleState = TitleManager::TITLE_STATE::TITLE_COMPANY_LOGO;
float TitleManager::timer = 0.0f;

std::vector<Text*> TitleManager::textList = std::vector<Text*>();

void TitleManager::AnyKeyPressed(char key)
{
	switch (titleState)
	{
	case TITLE_STATE::TITLE_COMPANY_LOGO:
		// skip logo
		EndSplash();
		titleState = TITLE_STATE::TITLE_TITLE_FADEIN;
		break;
	case TITLE_STATE::TITLE_TITLE_FADEIN:
		// skip title fade in
		GoToTitleMenu();
		titleState = TITLE_STATE::TITLE_DONE;
		break;
	case TITLE_STATE::TITLE_PRESS_TO_CONTINUE:
	{
		// go to the next screen
		FadeInMenu();
		titleState = TITLE_STATE::TITLE_MENU_FADEIN;
		break;
	}
	case TITLE_STATE::TITLE_MENU_FADEIN:
	{

		break;
	}
	case TITLE_STATE::TITLE_DONE:
	{

		break;
	}
	}
}

void TitleManager::Init()
{
	Texture* logoTexture = Manager::GetTexture("LOGO");
	float desiredheight = logoTexture->Height() * ((float)(Manager::GetScreenWidth() * .75f) / (float)logoTexture->Width());

	logo = new UISprite(0, 0, Manager::GetScreenWidth() * .75f, desiredheight, logoTexture, 1.f, 0.f, ANCHOR_POINT::ANCHOR_CENTER);
	logo->Activate();

	auto delHandle = InputManager::AnyKeyPressedCallback_Attatch(std::bind(&TitleManager::AnyKeyPressed, std::placeholders::_1), nullptr);
	delete delHandle;
}

void TitleManager::Clean()
{
	InputManager::AnyKeyPressedCallback_Remove(std::bind(&TitleManager::AnyKeyPressed, std::placeholders::_1), nullptr);

	SafeDelete(logo);
	SafeDelete(title);
	SafeDelete(titleBackground);

	for (int i = 0; i < textList.size(); i++)
	{
		SafeDelete(textList[i]);
		textList[i] = nullptr;
	}

	textList.clear();
}

void TitleManager::EndSplash()
{
	// kill the logo sprite
	logo->Deactivate();
	SafeDelete(logo);

	// make the actual title sprite
	Texture* titleTexture = Manager::GetTexture("TITLE");
	float desiredHeight = titleTexture->Height() * ((float)(Manager::GetScreenWidth() * .75f) / (float)titleTexture->Width());
	title = new UISprite(0, 0, Manager::GetScreenWidth() * .75f, desiredHeight, titleTexture, .1f, 0.f, ANCHOR_POINT::ANCHOR_CENTER);
	title->Activate();
	// make the title background sprite
	Texture* titleBackgroundTexture = Manager::GetTexture("TITLE_BACKGROUND");
	float widthRatio = (float)Manager::GetScreenWidth() / (float)titleBackgroundTexture->Width(),
		heightRatio = (float)Manager::GetScreenHeight() / (float)titleBackgroundTexture->Height();
	float titleBackgroundHeight, titleBackgroundWidth;
	if (std::abs(widthRatio - 1.f) > std::abs(heightRatio - 1.f)) // resize the image so it fits the whole screen without stretching
	{
		titleBackgroundWidth = titleBackgroundTexture->Width() * heightRatio;
		titleBackgroundHeight = (float)Manager::GetScreenHeight();
	}
	else
	{
		titleBackgroundWidth = (float)Manager::GetScreenWidth();
		titleBackgroundHeight = titleBackgroundTexture->Height() * widthRatio;
	}

	titleBackground = new UISprite(0, 0, titleBackgroundWidth, titleBackgroundHeight, titleBackgroundTexture, 0.f, 0.f, ANCHOR_POINT::ANCHOR_CENTER);
	titleBackground->Activate();

	timer = 0.f;
}

void TitleManager::GoToTitleMenu()
{
	// instantly finish fading in the title background and the title 
	titleBackground->SetOpacity(1.f);
	title->SetOpacity(1.f);
	// move the title to its final position
	title->SetPos(Point<float>(0, -((Manager::GetScreenHeight() / 2.f) - (title->GetRectangle()->Height() / 2.f) - (Manager::GetScreenHeight() * .05f))));
	// reset the timer
	timer = 0.f;
	// create the title screen menu
	CreateTitleMenu();
	// set the state to done
	titleState = TITLE_STATE::TITLE_DONE;
}

void TitleManager::FadeInMenu()
{
	// erase "Press any key to continue" text
	SafeDelete(textList[0]);
	textList.clear();
	timer = 0.f;
	// create the title screen menu
	CreateTitleMenu();
	MenuManager::SetMenuOpacity(0.f);
}

void TitleManager::FinishMenuFade()
{
	MenuManager::SetMenuOpacity(1.f);
}


void TitleManager::CreateTitleMenu()
{
	MenuManager::LoadTitleMenus();
	MenuManager::OpenMenu((int)TITLE_MENUS::MENU_TITLE);
}

void TitleManager::Update(float delta_time)
{
	switch (titleState)
	{
		// showing the initial logo splash screen
	case TITLE_STATE::TITLE_COMPANY_LOGO:
	{
		// the amount to change the fade of the logo by
		float fadeAmount = (delta_time / 1000.f) / SPLASH_TIME;

		// if the splash screen is fading in
		if (timer < SPLASH_TIME)
		{
			logo->IncreaseOpacity(fadeAmount);
		}
		// if the splash screen is fading out
		else if (timer > SPLASH_TIME * 2.f && timer < SPLASH_TIME * 3.f)
		{
			logo->DecreaseOpacity(fadeAmount);
		}
		// if the splash screen is done fading out
		else if (timer > SPLASH_TIME * 3.f)
		{
			EndSplash();
			titleState = TITLE_STATE::TITLE_TITLE_FADEIN;
		}

		break;
	}
	// 
	case TITLE_STATE::TITLE_TITLE_FADEIN:
	{
		// fade in the title 
		if (timer < SPLASH_TIME / 2.f)
		{
			// the amount to change the fade of the title by
			float fadeAmount = (delta_time / 1000.f) / (SPLASH_TIME / 2.f);
			title->IncreaseOpacity(fadeAmount);
		}
		// pause for effect
		else if (timer < SPLASH_TIME)
		{
		}
		// slide the title up and fade in the background 
		else if (timer < SPLASH_TIME * 2.5f)
		{
			// move title sprite
			float startpos = 0, endpos = -((Manager::GetScreenHeight() / 2.f) - (title->GetRectangle()->Height() / 2.f) - (Manager::GetScreenHeight() * .05f));
			title->SetPos(Point<float>(title->GetPos().x, LerpToRangeClamped(SPLASH_TIME, SPLASH_TIME * 2.5f, startpos, endpos, timer)));
			// fade in background sprite
			if (titleBackground->Opacity() != 1.f)
			{
				// the amount to change the fade of the title by
				float fadeAmount = (delta_time / 1000.f) / (SPLASH_TIME);
				titleBackground->IncreaseOpacity(fadeAmount);
			}
		}
		else
		{
			// everything is done fading / moving
			// init the "press any key to continue" text block and reset the timer
			titleState = TITLE_STATE::TITLE_PRESS_TO_CONTINUE;
			std::string text = "PRESS ANY KEY TO BEGIN";
			Point<float> textDim = Manager::MeasureString(text);
			Text* presstobegin = new Text(0, 0, textDim.x, textDim.y, text, 1.f, 0.f, ANCHOR_POINT::ANCHOR_CENTER);
			presstobegin->Activate();
			textList.push_back(presstobegin);
			timer = 0.f;
		}

		break;
	}
	case TITLE_STATE::TITLE_PRESS_TO_CONTINUE:
	{
		// fade the "press any key" in and out
		if (timer < 1.5f)
		{
			// fade in
			textList[0]->SetOpacity(timer / 1.5f);
		}
		else if (timer < 2.f)
		{
			// pause
		}
		else if (timer < 3.5f)
		{
			// fade out
			textList[0]->SetOpacity((3.5f - timer) / 1.5f);
		}
		else
		{
			// reset
			timer = 0.f;
		}

		break;
	}
	case TITLE_STATE::TITLE_MENU_FADEIN:
	{
		if (timer < 1.f)
		{
			// fade in
			MenuManager::SetMenuOpacity(timer);
		}
		else
		{
			// reset
			titleState = TITLE_STATE::TITLE_DONE;
			timer = 0.f;
		}
			break;
	}
	case TITLE_STATE::TITLE_DONE:
	{

		break;
	}
	}

	timer += (delta_time / 1000.f);
}
