#pragma once
#include "defines.h"

#include <Windows.h>


class Renderer
{
public:
	Renderer(HWND hwnd);
	~Renderer();

	bool Begin();
	bool Draw(unsigned int textureID, float x, float y, int width, int height, float opacity, float layer, int rot) { return Draw(textureID, x, y, width, height, 0, 0, width, height, opacity, layer, rot); }
	bool Draw(unsigned int textureID, float x, float y, int width, int height, float x_S, float y_S, int width_S, int height_S, float opacity, float layer, int rot);
	bool End();

	int LoadContent(const wchar_t* filePath, float height, float width);
	bool Resize(HWND hwnd);
};