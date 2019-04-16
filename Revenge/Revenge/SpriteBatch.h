#pragma once
#include "defines.h"

using native_handle = void*;

class Texture;
class MyRectangle;
class Renderer;

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
	Texture* Load(const wchar_t* filepath, float height, float width);
	// begin drawing
	void Begin();
	// draw a texture with a source rectangle
	void Draw(Texture* texture, MyRectangle* rectangle, MyRectangle* source, float opacity, float layer, int rot = 0);
	// draw a texture without a source rectangle
	void Draw(Texture* texture, MyRectangle* rectangle, float opacity, float layer, int rot = 0) { Draw(texture, rectangle, nullptr, opacity, layer, rot); }
	// draw a ui object with a source rectangle
	void DrawUI(Texture* texture, MyRectangle* rectangle, MyRectangle* source, float opacity, float layer, int rot = 0);
	// draw a ui object without a source rectangle
	void DrawUI(Texture* texture, MyRectangle* rectangle, float opacity, float layer, int rot = 0) { DrawUI(texture, rectangle, nullptr, opacity, layer, rot); }
	// write text
	void WriteText(const char* text, MyRectangle* rectangle, float layer);
	// end drawing
	void End();
	// move the camera
	void MoveCamera(float x, float y);
	// set the camera's position
	void SetCamera(float x, float y);
	// resize the renderer
	void Resize(native_handle hwnd);

#pragma endregion

};

