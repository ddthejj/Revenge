#pragma once

// Link to "windowscodecs.lib"

class SpriteBatch;
struct ID2D1Bitmap;

class Texture
{
	ID2D1Bitmap* bmp;
	static SpriteBatch* spriteBatch;

public:

	Texture(const wchar_t* filename);
	~Texture();

	ID2D1Bitmap* Bitmap() { return bmp; }

	static void SetSpriteBatch(SpriteBatch* _spriteBatch) { spriteBatch = _spriteBatch; }
};

