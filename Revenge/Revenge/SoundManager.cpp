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

bool SoundManager::LoadSounds(const char* filepath)
{
	return audioPlayer->LoadWAVs(filepath);
}

void SoundManager::Play(int index)
{
	audioPlayer->PlayWAV(index);
}
