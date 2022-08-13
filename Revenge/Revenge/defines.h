#pragma once

// MEMORY LEAK DETECTION
#if defined(DEBUG) || defined (_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include "Math.h"
#include "EnumParser.h"

#if defined(_DEBUG) || defined(DEBUG)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define WIDTH 800.0f
#define HEIGHT 800.0f
#define TILE_WIDTH 32.0f
#define TILE_HEIGHT 32.0f
#define ROOM_FADE_SPEED	1.f / 30.f

template <typename T>
struct Point
{
	T x = 0, y = 0;

	Point() = default;
	Point(T _x, T _y) { x = _x; y = _y; }
};

template <class T> void SafeDelete(T* &p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

// enums have been moved to the enum parser class