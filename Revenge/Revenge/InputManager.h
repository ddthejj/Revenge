#pragma once
#include "Manager.h"

// handles all input for the game
class InputManager
{
public:

	static char keyOptions[Manager::KEY_MAX];	// list of keybinds
	static bool keys[Manager::KEY_MAX];			// whether a key is down or not this frame
	static bool previousKeys[Manager::KEY_MAX];	// whether a key was down or not last frame

#pragma region Access From Main

	// key pressed reaction
	static void PressKey(WPARAM wParam);		
	// key released reaction
	static void ReleaseKey(WPARAM wParam);	

#pragma endregion

#pragma region Key Access

	static bool IsKeyDown(Manager::KEYS index) { return keys[index]; }					// Whether or not a key is down this frame
	static bool IsPreviousKeyDown(Manager::KEYS index) { return previousKeys[index]; }	// Whether or not a key was down last frame
	static bool IsKeyPressed(Manager::KEYS index);										// Whether or not the key was first pressed this frame

#pragma endregion

#pragma region Methods

	// Initialize input and keybinds
	static void Init();
	// Update the state of the keys
	static void Update(float delta_time);

#pragma endregion
};

