#pragma once

class MyRectangle;

class Texture
{
	unsigned int textureID = 0;
	float width = 0.f, height = 0.f;
	MyRectangle* source = nullptr;

public:

	Texture(int id, float _width, float _height);
	Texture(int id, float _width, float _height, float sourceX, float sourceY, float sourceWidth, float sourceHeight);
	Texture(Texture* that);
	Texture(Texture* that, MyRectangle source);
	~Texture();

	float Width() const { return width; }
	float Height() const { return height; }
	unsigned int ID() const { return textureID; }
	const MyRectangle* SourceRectangle() const { return source; }
};

