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
	static bool LoadSounds(const char* filepath);
	// plays a sound
	static void Play(int index);
};

