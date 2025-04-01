#pragma once
#include "Object.h"
#include "EnumParser.h"
#include <map>
#include <functional>

class MyRectangle;
class Texture;
template<typename T> struct Point;

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

	std::vector<std::function<void()>> onPlayerCollided;		// callback for when the player collides with this sprite

#pragma endregion

public:

#pragma region Methods

	// create a sprite without a source rectangle
	Sprite(std::string _debugName, float x, float y, float width, float height, const Texture* _texture, float _layer, float _opacity = 1.f);
	// create a sprite with a source rectangle
	Sprite(std::string _debugName, float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, const Texture* _texture, float _layer, float _opacity = 1.f);
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

	// return the main rectangle
	MyRectangle* GetRectangle() const;
	// return the main rectangle's position
	Point<float> GetPos() const;
	// return the opacity
	float Opacity() const { return opacity; }

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
	// called when the player collides with this sprite
	virtual void PlayerCollided();

	void BindToPlayerCollided(std::function<void()> func);

	void UnbindFromPlayerCollided(std::function<void()> func);

#pragma endregion

};

class UISprite : public Sprite
{
protected:

	ANCHOR_POINT anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT;

public: 

	// create a sprite without a source rectangle
	UISprite(std::string _debugName, float x, float y, float width, float height, const Texture* _texture, float _layer, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	// create a sprite with a source rectangle
	UISprite(std::string _debugName, float x, float y, float width, float height, float sX, float sY, float sWidth, float sHeight, const Texture* _texture, float _layer, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	
	virtual void Deactivate();
	// draw the sprite as UI
	virtual void Draw(SpriteBatch* spriteBatch);

	virtual void DrawInSprite(SpriteBatch* spriteBatch, UISprite* parent);

	ANCHOR_POINT GetAnchorPoint() const { return anchor; }

	void SetAnchorPoint(ANCHOR_POINT _anchor) { anchor = _anchor; }
};