#pragma once
#include "BorderedBox.h"


#include <string>

class Character;

class DialogueBox : public BorderedBox
{
    friend class MenuManager;


#pragma region Properties
private:

    Character* speaker = nullptr;
    std::vector<std::string> text;
    std::string currentText, currentLine;
    int charAt = 0, textAt = 0, numLines = 0;
    
    UISprite* arrow = nullptr;
    int arrowTimer = 0;

    // probably should move this to a manager
    int scrollSpeed = 3;
 

#pragma endregion

public:

#pragma region Methods

    // load a dialogue box without a file
    DialogueBox(std::string _debugName, float _x, float _y, float _width, float _height, const Texture* _texture, const Texture* _arrowTexture, float _layer = .8f, float _opacity = 1.f, ANCHOR_POINT _anchor = ANCHOR_POINT::ANCHOR_BOTTOM_CENTER);
    ~DialogueBox();

    // open the dialogue box
    void Open();
    // draw the dialogue box
    virtual void Draw(SpriteBatch* spriteBatch);
    // update the dialogue box
    virtual void Update(float delta_time);

    void SetText(Character* _speaker, std::vector<std::string> _text);

#pragma endregion

private:

#pragma region Methods

    void ScrollText(int speed);

#pragma endregion

#pragma region Callbacks

    virtual void BindCallbacks();
    virtual void UnbindCallbacks();

    void InteractPressed();

#pragma endregion
};

