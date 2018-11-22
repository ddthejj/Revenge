#pragma once

// MEMORY LEAK
#if defined(DEBUG) || defined (_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#if defined(_DEBUG) || defined(DEBUG)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define WIDTH 800.0f
#define HEIGHT 800.0f
#define TILE_WIDTH 32.0f
#define TILE_HEIGHT 32.0f
#define ROOM_FADE_SPEED	1.f / 30.f

struct Point
{
	float x = 0.f, y = 0.f;

	Point() = default;
	Point(float _x, float _y) { x = _x; y = _y; }
};

