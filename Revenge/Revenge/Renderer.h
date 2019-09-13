#pragma once
#include "defines.h"

#include <Windows.h>

#include <string>


class Renderer
{
#pragma region Structs

	struct ToDraw;
	struct ObjectDraw;
	struct TextDraw;
	
	struct Impl_Elements;

#pragma endregion

#pragma region Properties

	Impl_Elements* elements;	// all the properties required by the renderer implementation

#pragma endregion

public:

#pragma region Methods

	Renderer(HWND hwnd);
	~Renderer();

	// begin rendering
	bool Begin();
	// draw an object
	bool Draw(unsigned int textureID, float x, float y, int width, int height, float x_S, float y_S, int width_S, int height_S, float opacity, float layer, int rot);
	// write text
	bool Write(const char* text, float x, float y, float width, float height, float layer);
	// end rendering and draw the finalized frame
	bool End();

	// load a texture into permanent texture memory
	int LoadContent(const wchar_t* filePath, float height, float width);
	// resize the renderer
	bool Resize(HWND hwnd);
	// return the dimensions of a string
	Point<float> MeasureString(std::string text, float screenWidth, float screenHeight);

#pragma endregion
};