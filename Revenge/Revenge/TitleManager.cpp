#include "defines.h"
#include "TitleManager.h"
#include "Manager.h"
#include "Sprite.h"
#include "Manager.h"
#include "Texture.h"
#include "Math.h"

UISprite* TitleManager::logo = nullptr;
UISprite* TitleManager::title = nullptr;

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
}

void TitleManager::EndSplash()
{
	logo->Deactivate();
	SafeDelete(logo);

	Texture* titleTexture = Manager::GetTexture("TITLE");
	float desiredHeight = titleTexture->Height() * ((float)(Manager::GetScreenWidth() * .75f) / (float)titleTexture->Width());

	title = new UISprite(0, 0, Manager::GetScreenWidth() * .75f, desiredHeight, titleTexture, 1.f, 0.f, ANCHOR_POINT::ANCHOR_CENTER);
	title->Activate();

	timer = 0.f;
}

void TitleManager::Update(float delta_time)
{
	switch (titleState)
	{
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
	case TITLE_STATE::TITLE_TITLE_FADEIN:
	{
		// the amount to change the fade of the title by
		float fadeAmount = (delta_time / 1000.f) / (SPLASH_TIME / 2.f);
		if (timer < SPLASH_TIME / 2.f)
		{
			title->IncreaseOpacity(fadeAmount);
		}
		else if (timer < SPLASH_TIME)
		{

		}
		else if (timer < SPLASH_TIME * 2.5f)
		{
			float startpos = 0, endpos = (Manager::GetScreenHeight() / 2.f) - (title->GetRectangle()->Height() / 2.f) - (Manager::GetScreenHeight() * .05f);

			title->SetPos(Point<float>(title->GetPos().x, LerpToRangeClamped(SPLASH_TIME, SPLASH_TIME * 2.5f, startpos, endpos, timer)));
		}

		break;
	}
	case TITLE_STATE::TITLE_BACKGROUND_FADEIN:
	{

		break;
	}
	case TITLE_STATE::TITLE_DONE:
	{

	}
	}

	timer += (delta_time / 1000.f);
}
