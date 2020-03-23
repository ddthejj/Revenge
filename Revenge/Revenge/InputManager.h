#pragma once 
#include "Manager.h"
#include <vector>

// handles all input for the game
class InputManager
{
	
#pragma region Structs

	template <class T>
	struct Callback
	{
		void* object;
		T func;
	};

#pragma endregion

#pragma region Properties

	// Holds values for whether keys are up or down
	static char keyOptions[(int)KEYS::KEY_MAX];						// list of keybinds
	static bool keys[(int)KEYS::KEY_MAX];							// whether a key is down or not this frame
	static bool previousKeys[(int)KEYS::KEY_MAX];					// whether a key was down or not last frame
	static bool mouseKeys[(int)MOUSE_KEYS::MOUSE_KEY_MAX];			// whether a mouse key is down or not this frame
	static bool previousMouseKeys[(int)MOUSE_KEYS::MOUSE_KEY_MAX];	// whether a mouse key was down or not last frame
	static float mouseX, mouseY;									// the position of the mouse
	static char charPressed;										// which character was pressed this frame
	static char previousCharPressed;								// which character was pressed last frame

	// Callback functions that will trigger once a key is pressed or released
	static std::vector<Callback<void (*)(char, void*)>> anyKeyPressedCallback;									// standard callback for any key being pressed
	static std::vector<Callback<void (*)(char, void*)>> anyKeyReleasedCallback;									// standard callback for any key being released
	static std::vector<Callback<void (*)(void*)>> keyPressedCallbacks[(int)KEYS::KEY_MAX];						// keyboard key pressed callbacks
	static std::vector<Callback<void (*)(void*)>> keyReleasedCallbacks[(int)KEYS::KEY_MAX];						// keyboard key released callbacks
	static std::vector<Callback<void (*)(void*)>> mouseKeyPressedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];		// mouse button pressed callbacks
	static std::vector<Callback<void (*)(void*)>> mouseKeyReleasedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];	// mouse button released callbacks

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

	// 
	// Whether or not a key is down this frame
	static bool IsKeyDown(KEYS index)
	{
		return keys[(int)index];
	}
	// Whether or not a key was down last frame
	static bool IsPreviousKeyDown(KEYS index)
	{
		return previousKeys[(int)index];
	}
	// Whether or not a key was first pressed this frame
	static bool IsKeyPressed(KEYS index);
	// Whether or not a mouse button is down this frame
	static bool IsMouseKeyDown(MOUSE_KEYS index)
	{
		return mouseKeys[(int)index];
	}
	// Whether or not a mouse button was down last frame
	static bool IsPreviousMouseKeyDown(MOUSE_KEYS index)
	{
		return previousMouseKeys[(int)index];
	}
	// Whether or not a mouse button was first pressed this frame
	static bool IsMouseKeyPressed(MOUSE_KEYS index);
	// Which character was last pressed
	static char CharPressed()
	{
		return charPressed;
	}
	// Which character was pressed this frame
	static char GetCharPressed()
	{
		return (charPressed != '\0' && charPressed != previousCharPressed) ? charPressed : '\0';
	}
	// The X position of the mouse
	static float GetMouseX()
	{
		return mouseX;
	}
	// The Y position of the mouse
	static float GetMouseY()
	{
		return mouseY;
	}
	// The position of the mouse
	static Point<float> GetMousePos()
	{
		return Point<float>(mouseX, mouseY);
	}

#pragma endregion

#pragma region Access Callbacks

	// Attatch to the "any key pressed" callback
	static bool AnyKeyPressedCallback_Attatch(void(*func)(char, void*), void* this_ptr);
	// Removed from the "any key pressed" callback
	static bool AnyKeyPressedCallback_Remove(void(*func)(char, void*), void* this_ptr);
	// Attatch to the "key pressed" callback
	static bool KeyPressedCallbacks_Attatch(KEYS whichKey, void(*func)(void*), void* this_ptr);
	// Remove from the "key pressed" callback
	static bool KeyPressedCallbacks_Remove(KEYS whichKey, void(*func)(void*), void* this_ptr);
	// Attatch to the "mouse key pressed" callback
	static bool MouseKeyPressedCallbacks_Attatch(MOUSE_KEYS whichKey, void(*func)(void*), void* this_ptr);
	// Remove from the "mouse key pressed" callback
	static bool MouseKeyPressedCallbacks_Remove(MOUSE_KEYS whichKey, void(*func)(void*), void* this_ptr);
	// Attatch to the "any key released" callback
	static bool AnyKeyReleasedCallback_Attatch(void(*func)(char, void*), void* this_ptr);
	// Removed from the "any key released" callback
	static bool AnyKeyReleasedCallback_Remove(void(*func)(char, void*), void* this_ptr);
	// Attatch to the "key released" callback
	static bool KeyReleasedCallbacks_Attatch(KEYS whichKey, void(*func)(void*), void* this_ptr);
	// Remove from the "key released" callback
	static bool KeyReleasedCallbacks_Remove(KEYS whichKey, void(*func)(void*), void* this_ptr);
	// Attatch to the "mouse key released" callback
	static bool MouseKeyReleasedCallbacks_Attatch(MOUSE_KEYS whichKey, void(*func)(void*), void* this_ptr);
	// Remove from the "mouse key released" callback
	static bool MouseKeyReleasedCallbacks_Remove(MOUSE_KEYS whichKey, void(*func)(void*), void* this_ptr);

#pragma endregion

#pragma region Methods

	// Initialize input and keybinds
	static void Init();
	// Clean
	static void Clean();
	// Update the state of the keys
	static void Update(float delta_time);

#pragma endregion

protected:

	template <class T>
	static bool AddFunctionToCallbackList(std::vector<Callback<T>>* callbackList, T func, void* this_ptr);

	template <class T>
	static bool RemoveFunctionFromCallbackList(std::vector<Callback<T>>* callbackList, T func, void* this_ptr);
};
