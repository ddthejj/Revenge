#pragma once

#define SPLASH_TIME 2.f

class Sprite;
class UISprite;

class TitleManager
{
	enum class TITLE_STATE
	{
		TITLE_COMPANY_LOGO,
		TITLE_TITLE_FADEIN,
		TITLE_PRESS_TO_CONTINUE,
		TITLE_MENU_FADEIN,
		TITLE_DONE,
		TITLE_MAX
	};

	static UISprite* logo;
	static UISprite* title;
	static UISprite* titleBackground;

	static TITLE_STATE titleState;
	static float timer;

public:
	static void Init();
	static void Clean();

	static void EndSplash();
	static void GoToTitleMenu();

	static void Update(float delta_time);
};

