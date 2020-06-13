#pragma once
#include <Windows.h>

class AudioPlayer;

class SoundManager
{
private:

	static AudioPlayer* audioPlayer;

public: 
	static void Init(HWND hWnd);

	static void Clean();

	// returns number of successfully loaded sounds
	static int LoadSounds(const wchar_t* filepath);
	// returns index of loaded sound (-1 if failed)
	static int LoadSound(const char* filepath);
	// plays a sound
	static void Play(int index);
};

