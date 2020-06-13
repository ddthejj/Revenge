#pragma once

#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

// linker 
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

struct Impl_Elements;

class AudioPlayer
{
public: 
	AudioPlayer();
	~AudioPlayer();

	bool Init(HWND hWnd);
	void Clean();

	int LoadWAV(const char* filepath);
	bool UnloadWAV(int index);
	bool PlayWAV(int index);

private:

	Impl_Elements* elements;

};
#endif

