#pragma once
#include "Sprite.h"

#include <vector>
#include <string>

class Character;

class DialogueBox : public UISprite
{
    friend class MenuManager;


#pragma region Properties
private:

    Character* speaker = nullptr;
    std::vector<std::string> text;
    
    UISprite* arrow = nullptr;
    int arrowTimer = 0;

#pragma endregion

public:

#pragma region Methods

    // load a dialogue box without a file
    DialogueBox(float _x, float _y, float _width, float _height, Texture* _texture, Texture* _arrowTexture, float _layer = .8f, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_BOTTOM_CENTER);
    ~DialogueBox();

    // open the dialogue box
    void Open();
    // draw the dialogue box
    virtual void Draw(SpriteBatch* spriteBatch);
    // update the dialogue box
    virtual void Update();

    void SetText(Character* _speaker, std::vector<std::string> _text);

#pragma endregion

private:

#pragma region Callbacks

    virtual void BindCallbacks();
    virtual void UnbindCallbacks();

    void InteractPressed();

#pragma endregion
};

