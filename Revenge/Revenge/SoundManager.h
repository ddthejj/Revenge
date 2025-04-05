#pragma once
#include <Windows.h>

class AudioPlayer;

enum class SOUND_CHANNEL
{
	MASTER,
	MUSIC,
	SFX,
	SOUND_CHANNEL_MAX
};

class SoundManager
{
private:

	static AudioPlayer* audioPlayer;

public: 
	static void Init(HWND hWnd);

	static void Clean();

	// returns number of successfully loaded sounds
	static bool LoadSounds(const char* filepath);
	// unloads currently loaded sound bank
	static bool UnloadSounds();
	// plays a sound
	static void Play(int index);
};

