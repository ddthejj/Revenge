#pragma once
#include "defines.h"
#include <string>

using native_handle = void*;

class Texture;
class MyRectangle;
class Renderer;
class Text;

// the access for the renderer
class SpriteBatch
{
#pragma region Properties

	Renderer* renderer;	// the real renderer for the game
	float camera[2];	// the position of the camera
	int windowWidth = (int)WIDTH, windowHeight = (int)HEIGHT;	// the window dimensions

#pragma endregion

public:

#pragma region Methods

	SpriteBatch(native_handle hwnd);
	~SpriteBatch();

	// load a texture
	Texture* Load(const wchar_t* filepath, float width, float height);
	// unload all textures
	void UnloadTextures();
	// begin drawing
	void Begin();
	// draw a texture with a source rectangle
	void Draw(Texture* texture, MyRectangle* rectangle, MyRectangle* source, float opacity, float layer, int rot = 0);
	// draw a texture without a source rectangle
	void Draw(Texture* texture, MyRectangle* rectangle, float opacity, float layer, int rot = 0) { Draw(texture, rectangle, nullptr, opacity, layer, rot); }
	// draw a ui object with a source rectangle
	void DrawUI(Texture* texture, MyRectangle* rectangle, MyRectangle* source, float opacity, float layer, int rot = 0, ANCHOR_POINT anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	// draw a ui object without a source rectangle
	void DrawUI(Texture* texture, MyRectangle* rectangle, float opacity, float layer, int rot = 0, ANCHOR_POINT anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT) { DrawUI(texture, rectangle, nullptr, opacity, layer, rot, anchor); }
	// write text
	void WriteText(const char* text, MyRectangle* rectangle, float layer, float opacity = 1.f, ANCHOR_POINT anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	// write text with a text object
	void WriteText(Text* text);
	// end drawing
	void End();
	// move the camera
	void MoveCamera(float x, float y);
	// set the camera's position
	void SetCamera(float x, float y);
	// resize the renderer
	void Resize(native_handle hwnd);


	// Measure a string
	Point<float> MeasureString(std::string text);

#pragma endregion

#pragma region Get Values
	
	// Return the height of the window
	int ScreenWidth() { return windowWidth; }
	// Return the width of the window
	int ScreenHeight() { return windowHeight; }

#pragma endregion

};

