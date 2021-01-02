#pragma once
#include "Sprite.h"
#include <string>
#include <vector>

template <typename T>
struct Point;

// class for menu boxes
class MenuBox : public UISprite
{
	friend class MenuManager;

protected:

#pragma region Structs

	struct MenuOption
	{
		std::string text = "DEFAULT";
		int option = 0;
		float x = 0.f, y = 0.f;

		MenuOption() = default;
		MenuOption(char* _text, int _option, float _x, float _y) { text = std::string(_text); option = _option; x = _x; y = _y; }
	};

#pragma endregion

#pragma region Properties
		
	MenuOption*** options = nullptr;				// list of options the menu has (options can be nullptr)
	int optionsWidth = 0, optionsHeight = 0;		// dimensions of the options array
	Point<int>* optionAt = nullptr;					// which option is currently selected
	UISprite* arrow = nullptr;						// the sprite for the arrow 
	int arrowTimer = 0;								// the timer to blink the arrow
	MenuBox* previousMenu = nullptr;				// pointer to the previous menu (if nullptr, no previous menu)
	Point<float> textOffset;

#pragma endregion
	
#pragma region Methods

	// override the set opacity function so that we can set the opacity of the arrow as well
	virtual void SetOpacity(float _opacity);
	// update the location of the arrow to the currently selected option
	void UpdateArrowLocation();
	// reset the arrow position to the default location
	void ResetArrow();

#pragma endregion

public:

#pragma region Methods

	// load a menu without a file
	MenuBox(float _x, float _y, float _width, float _height, Texture* _texture, Texture* _arrowTexture, float _layer = .8f, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_TOP_LEFT);
	// load a menu from a file
	MenuBox(float _x, float _y, float _width, float _height, Texture* _texture, Texture* _arrowTexture, const char* filename);
	~MenuBox();

	// create a menu with _previousMenu as its previousMenu
	void Open(MenuBox* _previousMenu);
	// set the options of a menu to passed in values
	void SetOptions(char** texts, int* option, Point<float>* positions, Point<int>* layout, int sizeX, int sizeY);
	// resize the menu box
	void Resize(float x, float y, float width, float height);
	// move the menu box
	void Move(float x, float y);

	// returns the return value of the selected option
	int ChooseOption();

	// draw the menu box
	virtual void Draw(SpriteBatch* spriteBatch);
	// update the menu box
	virtual void Update(float delta_time);

	// freeze the menu box
	virtual void Freeze();


private:

#pragma endregion

#pragma region Key Pressed Callbacks

	void BindCallbacks();
	void UnbindCallbacks();

	void UpPressed();
	void DownPressed();
	void LeftPressed();
	void RightPressed();

	void SelectPressed();

#pragma endregion

};