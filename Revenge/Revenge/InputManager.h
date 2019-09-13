#pragma once
#include "Manager.h"

// handles all input for the game
class InputManager
{
#pragma region Properties

	static char keyOptions[KEY_MAX];				// list of keybinds
	static bool keys[KEY_MAX];						// whether a key is down or not this frame
	static bool previousKeys[KEY_MAX];				// whether a key was down or not last frame
	static bool mouseKeys[MOUSE_KEY_MAX];			// whether a mouse key is down or not this frame
	static bool previousMouseKeys[MOUSE_KEY_MAX];	// whether a mouse key was down or not last frame
	static float mouseX, mouseY;					// the position of the mouse
	static char charPressed;						// which character was pressed this frame
	static char previousCharPressed;				// which character was pressed last frame

#pragma endregion

public:

#pragma region Access From Main

	// key pressed reaction
	static void PressKey(WPARAM wParam);		
	// key released reaction
	static void ReleaseKey(WPARAM wParam);	
	// mouse moved reaction
	static void MoveMouse(HWND hwnd, LPARAM lParam);
	// mouse key pressed reaction
	static void PressMouseKey(MOUSE_KEYS key);
	// mouse key released reaction
	static void ReleaseMouseKey(MOUSE_KEYS key);
	// check what letter has been pressed
	static void PressChar(WPARAM wParam) { charPressed = (char)wParam; }
	// repeat a key when it's held down
	static void RepeatKey() { previousCharPressed = '\0'; }

#pragma endregion

#pragma region Key Access

	// Whether or not a key is down this frame
	static bool IsKeyDown(KEYS index) 
	{ return keys[index]; }					
	// Whether or not a key was down last frame
	static bool IsPreviousKeyDown(KEYS index) 
	{ return previousKeys[index]; }	
	// Whether or not a key was first pressed this frame
	static bool IsKeyPressed(KEYS index);										
	// Whether or not a mouse button is down this frame
	static bool IsMouseKeyDown(MOUSE_KEYS index) 
	{ return mouseKeys[index]; }
	// Whether or not a mouse button was down last frame
	static bool IsPreviousMouseKeyDown(MOUSE_KEYS index) 
	{ return previousMouseKeys[index]; }
	// Whether or not a mouse button was first pressed this frame
	static bool IsMouseKeyPressed(MOUSE_KEYS index);
	// Which character was last pressed
	static char CharPressed() 
	{ return charPressed; }
	// Which character was pressed this frame
	static char GetCharPressed() 
	{ return (charPressed != '\0' && charPressed != previousCharPressed) ? charPressed : '\0'; }
	// The X position of the mouse
	static float GetMouseX()
	{ return mouseX; }
	// The Y position of the mouse
	static float GetMouseY() 
	{ return mouseY; }
	// The position of the mouse
	static Point<float> GetMousePos() 
	{ return Point<float>(mouseX, mouseY); }

#pragma endregion

#pragma region Methods

	// Initialize input and keybinds
	static void Init();
	// Update the state of the keys
	static void Update(float delta_time);

#pragma endregion
};

