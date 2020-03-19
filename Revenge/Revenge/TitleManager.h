#pragma once

#define SPLASH_TIME 2.f

class Sprite;
class UISprite;

class TitleManager
{
	enum TITLE_STATE
	{
		TITLE_COMPANY_LOGO,
		TITLE_TITLE_FADEIN,
		TITLE_BACKGROUND_FADEIN,
		TITLE_DONE,
		TITLE_MAX
	};

	static UISprite* logo;
	static UISprite* title;

	static TITLE_STATE titleState;
	static float timer;

public:
	static void Init();
	static void Clean();

	static void EndSplash();

	static void Update(float delta_time);
};

