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
	if (audioPlayer)
	{
		audioPlayer->Clean();
		SafeDelete(audioPlayer);
	}
}

bool SoundManager::LoadSounds(const char* filepath)
{
	return audioPlayer->LoadWAVs(filepath);
}

bool SoundManager::UnloadSounds()
{
	if (audioPlayer)
	{
		return audioPlayer->UnloadWAVs();
	}
	return false;
}

void SoundManager::Play(int index)
{
	audioPlayer->PlayWAV(index);
}
