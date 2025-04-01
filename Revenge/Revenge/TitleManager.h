#pragma once

#define SPLASH_TIME 2.f



class Sprite;
class UISprite;
class Text;

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

	static std::vector<Text*> textList;

public:

	static bool AnyKeyPressed(char key);

	static void Init();
	static void Clean();

	static void EndSplash();
	static void GoToTitleMenu();
	static void FadeInMenu();
	static void FinishMenuFade();

	static void CreateTitleMenu();

	static void Update(float delta_time);
};

