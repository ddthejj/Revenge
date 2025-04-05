#pragma once

// MEMORY LEAK DETECTION
#if defined(DEBUG) || defined (_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include "Math.h"
#include <vector>

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
	bool operator ==(const Point a) const { return a.x == x && a.y == y; }
	bool operator !=(const Point a) const { return !(a.x == x && a.y == y); }
	Point<T> operator -(const Point<T> a) const { return Point<T>(x - a.x, y - a.y); }
	Point<T> operator +(const Point<T> a) const { return Point<T>(x + a.x, y + a.y); }
	Point<T> operator *(const float a) const { return Point<T>(x * a, y * a); }
	Point<T>& operator +=(const Point<T>& a) { x += a.x; y += a.y; return *this; }
	Point<T>& operator -=(const Point<T>& a) { x -= a.x; y -= a.y; return *this; }
	Point<T>& operator *=(const float a) { x *= a; y *= a; return *this; }
	bool Equals(const Point a, const T threshold) { return ((*this - a).Magnitude() <= threshold); }
	T Magnitude() { return std::abs(x) + std::abs(y); }
	void Normalize() { T _magnitude = Magnitude();  x = x / _magnitude; y = y / _magnitude; }
};

template <class T> void SafeDelete(T*& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}