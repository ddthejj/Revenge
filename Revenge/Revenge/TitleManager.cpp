#include "defines.h"
#include "TitleManager.h"
#include "Manager.h"
#include "Sprite.h"
#include "Manager.h"
#include "Texture.h"
#include "Math.h"

UISprite* TitleManager::logo = nullptr;
UISprite* TitleManager::title = nullptr;
UISprite* TitleManager::titleBackground = nullptr;

TitleManager::TITLE_STATE TitleManager::titleState = TitleManager::TITLE_STATE::TITLE_COMPANY_LOGO;
float TitleManager::timer = 0.0f;

void TitleManager::Init()
{
	Texture* logoTexture = Manager::GetTexture("LOGO");
	float desiredheight = logoTexture->Height() * ((float)(Manager::GetScreenWidth() * .75f) / (float)logoTexture->Width());

	logo = new UISprite(0, 0, Manager::GetScreenWidth() * .75f, desiredheight, logoTexture, 1.f, 0.f, ANCHOR_POINT::ANCHOR_CENTER);
	logo->Activate();
}

void TitleManager::Clean()
{
	SafeDelete(logo);
	SafeDelete(title);
	SafeDelete(titleBackground);
}

void TitleManager::EndSplash()
{
	// kill the logo sprite
	logo->Deactivate();
	SafeDelete(logo);

	// make the actual title sprite
	Texture* titleTexture = Manager::GetTexture("TITLE");
	float desiredHeight = titleTexture->Height() * ((float)(Manager::GetScreenWidth() * .75f) / (float)titleTexture->Width());
	title = new UISprite(0, 0, Manager::GetScreenWidth() * .75f, desiredHeight, titleTexture, 1.f, 0.f, ANCHOR_POINT::ANCHOR_CENTER);
	title->Activate();
	// make the title background sprite
	Texture* titleBackgroundTexture = Manager::GetTexture("TITLE_BACKGROUND");
	float widthRatio = (float)Manager::GetScreenWidth() / (float)titleBackgroundTexture->Width(),
		  heightRatio = (float) Manager::GetScreenHeight() / (float)titleBackgroundTexture->Height();
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

	titleBackground = new UISprite(0, 0, titleBackgroundWidth, titleBackgroundHeight, titleBackgroundTexture, .9f, 0.f, ANCHOR_POINT::ANCHOR_CENTER);
	titleBackground->Activate();

	timer = 0.f;
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
			float startpos = 0, endpos = (Manager::GetScreenHeight() / 2.f) - (title->GetRectangle()->Height() / 2.f) - (Manager::GetScreenHeight() * .05f);\
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
			titleState = TITLE_STATE::TITLE_PRESS_TO_CONTINUE;
		}

		break;
	}
	case TITLE_STATE::TITLE_PRESS_TO_CONTINUE:
	{

	}
	case TITLE_STATE::TITLE_MENU_FADEIN:
	{

		break;
	}
	case TITLE_STATE::TITLE_DONE:
	{

	}
	}

	timer += (delta_time / 1000.f);
}
