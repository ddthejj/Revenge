#pragma once

enum ROTATIONS
{
	NONE = 0,
	HORIZONTAL,
	VERTICAL,
	ROT_90,
	ROT_90_VERTICAL,
	ROT_180,
	ROT_270,
	ROT_270_VERTICAL
};

class MyRectangle;

class Texture
{
	unsigned int textureID = 0;
	float width = 0.f, height = 0.f;
	MyRectangle* source = nullptr;

public:

	Texture(int id, float _height, float _width) { textureID = id; width = _width; height = _height; source = nullptr; }
	Texture(int id, float _height, float _width, float sourceX, float sourceY, float sourceWidth, float sourceHeight);
	~Texture();

	float Width() { return width; }
	float Height() { return height; }
	unsigned int ID() { return textureID; }
	MyRectangle* SourceRectangle() { return source; }
};

