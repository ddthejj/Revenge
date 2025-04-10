#pragma once
#include "Object.h"
#include <map>
#include <functional>

class MyRectangle;
class Texture;
template<typename T> struct Point;

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

enum class ANCHOR_POINT : unsigned char
{
	TOP = (0) << 2,
	VCENTER = (1) << 2,
	BOTTOM = (1 << 1) << 2,
	LEFT = 0,
	HCENTER = 1,
	RIGHT = 1 << 1,

	ANCHOR_TOP_LEFT = TOP | LEFT,
	ANCHOR_TOP_CENTER = TOP | HCENTER,
	ANCHOR_TOP_RIGHT = TOP | RIGHT,
	ANCHOR_CENTER_LEFT = VCENTER | LEFT,
	ANCHOR_CENTER = VCENTER | HCENTER,
	ANCHOR_CENTER_RIGHT = VCENTER | RIGHT,
	ANCHOR_BOTTOM_LEFT = BOTTOM | LEFT,
	ANCHOR_BOTTOM_CENTER = BOTTOM | HCENTER,
	ANCHOR_BOTTOM_RIGHT = BOTTOM | RIGHT
};

// base class for all drawable objects
class Sprite : public Object
{
protected:

#pragma region Properties

	MyRectangle* rectangle;						// the rectangle to draw the spirte on
	MyRectangle* sourceRectangle;				// the source rectangle for drawing the sprite (can be nullptr)
	const Texture* texture;						// the sprite's texture
	float layer = 0.f;							// the sprite's layer
	float opacity = 1.f;						// the sprite's opacity
	ROTATIONS rotation = ROTATIONS::NONE;		// the sprite's rotation

#pragma endregion

public:

#pragma region Methods

	// create a sprite without a source rectangle
	Sprite(std::string _debugName, float x, float y, float width, float height, const Texture* _texture, float _layer, float _opacity = 1.f, ROTATIONS _rotation = ROTATIONS::NONE);
	// create a sprite with a source rectangle
	Sprite(std::string _debugName, float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, const Texture* _texture, float _layer, float _opacity = 1.f, ROTATIONS _rotation = ROTATIONS::NONE);
	Sprite(const Sprite& _that);
	~Sprite();

	// set the main rectangle
	void SetRectangle(const MyRectangle& _rectangle);
	// set the main rectangle's position
	void SetPos(const Point<float>& location);
	// set the source rectangle
	void SetSource(const MyRectangle& _source);
	// set the source rectangle's position
	void SetSourcePos(const Point<float>& location);
	// set the sprite's opacity
	virtual void SetOpacity(float _opacity);
	// decrease the sprite's opacity
	void DecreaseOpacity(float amnt);
	// increase the sprite's opacity
	void IncreaseOpacity(float amnt);
	// set the layer
	void SetLayer(float _layer) { layer = _layer; }
	// set the rotation
	void SetRotation(ROTATIONS _rotation) { rotation = _rotation; }

	// return the main rectangle
	MyRectangle* GetRectangle() const { return rectangle; };
	// return the main rectangle's position
	Point<float> GetPos() const;
	// return the main rectangle
	MyRectangle* GetSourceRectangle() const { return sourceRectangle; }
	// return the main rectangle's position
	Point<float> GetSourcePos() const;
	// return the texture
	const Texture* GetTexture() const { return texture; }
	// return the opacity
	float GetOpacity() const { return opacity; }
	// return the layer
	float GetLayer() const { return layer; }
	// return the rotation
	ROTATIONS GetRotation() const { return rotation; }

	// draw the sprite in the world
	virtual void Draw(SpriteBatch* spriteBatch);
	// update the sprite
	virtual void Update(float delta_time) {}
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

class UISprite : public Sprite
{
protected:

	ANCHOR_POINT anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT;

public:

	// create a sprite without a source rectangle
	UISprite(std::string _debugName, float x, float y, float width, float height, const Texture* _texture, float _layer, float _opacity = 1.f, ROTATIONS _rotation = ROTATIONS::NONE, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	// create a sprite with a source rectangle
	UISprite(std::string _debugName, float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, const Texture* _texture, float _layer, float _opacity = 1.f, ROTATIONS _rotation = ROTATIONS::NONE, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	UISprite(const UISprite& _that);

	virtual void Deactivate();
	// draw the sprite as UI
	virtual void Draw(SpriteBatch* spriteBatch);

	virtual void DrawInSprite(SpriteBatch* spriteBatch, UISprite* parent);

	ANCHOR_POINT GetAnchorPoint() const { return anchor; }

	void SetAnchorPoint(ANCHOR_POINT _anchor) { anchor = _anchor; }
};