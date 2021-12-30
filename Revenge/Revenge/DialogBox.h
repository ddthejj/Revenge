#pragma once
#include "Sprite.h"

#include <vector>
#include <string>

class Character;

class DialogBox : public UISprite
{
    friend class MenuManager;


#pragma region Properties
private:

    Character* speaker = nullptr;
    std::vector<std::vector<std::string>> text;
    
    UISprite* arrow = nullptr;
    int arrowTimer = 0;

#pragma endregion

public:

#pragma region Methods

    // load a dialog box without a file
    DialogBox(float _x, float _y, float _width, float _height, Texture* _texture, Texture* _arrowTexture, float _layer = .8f, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_BOTTOM_CENTER);
    ~DialogBox();

    // open the dialog box
    void Open();
    // draw the dialog box
    virtual void Draw(SpriteBatch* spriteBatch);
    // update the dialog box
    virtual void Update();

    void SetText(Character* _speaker, std::vector<std::vector<std::string>> _text);

#pragma endregion

private:

#pragma region Callbacks

    void SelectPressed();

#pragma endregion
};

