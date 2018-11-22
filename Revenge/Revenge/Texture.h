#pragma once
#include "defines.h"

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

class Texture
{
	unsigned int textureID;
	float width, height;

public:

	Texture() {}
	Texture(int id, float _height, float _width) { textureID = id; height = _height; width = _width; }
	~Texture() {}

	float Width() { return width; }
	float Height() { return height; }
	unsigned int ID() { return textureID; }

};

