#pragma once

class Sprite;

class TitleManager
{
	static Sprite* Logo;
	static Sprite* Title;

public:
	static void Init();
	static void Clean();

	static void ShowSplash();

	static void Update();
};

