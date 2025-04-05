#pragma once
#include "defines.h"
#include <string>

using native_handle = void*;

class Texture;
class MyRectangle;
class Renderer;
class Sprite;
class UISprite;
class Text;

// the access for the renderer
class SpriteBatch
{
#pragma region Properties

	Renderer* renderer;							// the real renderer for the game
	float camera[3] = { 0, 0, 1.f };			// the position of the camera
	static int windowWidth, windowHeight;		// the window dimensions

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
	// draw a sprite
	void Draw(const Sprite* sprite);
	// draw a UI sprite
	void DrawUI(const UISprite* sprite);
	// draw a UI sprite within another UI sprite
	void DrawUIInSprite(const UISprite* drawSprite, const UISprite* sourceSprite);
	// write text with a text object
	void WriteText(const Text* text);
	// write text within a bounding UIsprite
	void WriteTextInSprite(const Text* text, const UISprite* sprite);
	// end drawing
	void End();
	// move the camera
	void MoveCamera(float x, float y);
	// set the camera's position
	void SetCamera(float x, float y);
	// set the camera's scale
	void SetCameraScale(float z);

	void IncreaseCameraScale(float z) { camera[2] += z; }
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

