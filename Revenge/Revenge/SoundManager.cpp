#include "defines.h"
#include "SoundManager.h"
#include "AudioPlayer.h"

AudioPlayer* SoundManager::audioPlayer = nullptr;

void SoundManager::Init(HWND hWnd)
{
	audioPlayer = new AudioPlayer();
	audioPlayer->Init(hWnd);
}

void SoundManager::Clean()
{
	audioPlayer->Clean();
	delete audioPlayer;
}

int SoundManager::LoadSounds(const wchar_t* filepath)
{
	return 0;
}

int SoundManager::LoadSound(const char* filepath)
{
	return audioPlayer->LoadWAV(filepath);
}

void SoundManager::Play(int index)
{
	audioPlayer->PlayWAV(index);
}
