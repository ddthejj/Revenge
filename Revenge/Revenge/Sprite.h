#pragma once
#include "Object.h"

class MyRectangle;
class Texture;
template<typename T>
struct Point;

// base class for all drawable objects
class Sprite : public Object
{
protected:

#pragma region Properties

	MyRectangle* rectangle;			// the rectangle to draw the spirte on
	MyRectangle* sourceRectangle;	// the source rectangle for drawing the sprite (can be nullptr)
	Texture* texture;				// the sprite's texture
	float layer = 0.f;				// the sprite's layer
	float opacity = 1.f;			// the sprite's opacity

#pragma endregion

public:

#pragma region Methods

	// create a sprite without a source rectangle
	Sprite(float x, float y, float width, float height, Texture* _texture, float _layer, float _opacity = 1.f);
	// create a sprite with a source rectangle
	Sprite(float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, Texture* _texture, float _layer, float _opacity = 1.f);
	~Sprite();

	// set the main rectangle
	void SetRectangle(const MyRectangle& _rectangle);
	// set the main rectangle's position
	void SetPos(const Point<float>& location);
	// set the source rectangle
	void SetSource(const MyRectangle& _source);
	// set the source rectangle's position
	void SetSourcePos(const Point<float>& location);
	// decrease the sprite's opacity
	void DecreaseOpacity(float amnt) { opacity -= amnt; if (opacity < 0) opacity = 0.f; }
	// increase the sprite's opacity
	void IncreaseOpacity(float amnt) { opacity += amnt; if (opacity > 1) opacity = 1.f; }

	// return the main rectangle
	MyRectangle* GetRectangle() const;
	// return the opacity
	float Opacity() { return opacity; }

	// draw the sprite
	virtual void Draw(SpriteBatch* spriteBatch);
	// draw the sprite as UI
	virtual void DrawUI(SpriteBatch* spriteBatch);
	// update the sprite
	virtual void Update() {}
	// activate the sprite
	virtual void Activate();
	// deactivate the sprite
	virtual void Deactivate();
	// freeze the sprite
	virtual void Freeze();
	// unfreeze the sprite
	virtual void Unfreeze();

#pragma endregion

};

