#pragma once 
#include "Manager.h"
#include <functional>
#include <map>

struct DelegateHandle
{
};

enum class KEYS			// Keybinds
{
	KEY_UP,				// Up key
	KEY_DOWN,			// Down key
	KEY_LEFT,			// Left key
	KEY_RIGHT,			// Right key
	KEY_MENU,			// Open menu
	KEY_INTERACT,		// Select / interact with tiles
	KEY_MAX
};

enum class MOUSE_KEYS	// Mouse buttons
{
	MOUSE_KEY_LEFT,		// Left mouse button
	MOUSE_KEY_RIGHT,	// Right mouse button
	MOUSE_KEY_MIDDLE,	// Middle mouse button
	MOUSE_KEY_MAX
};

// handles all input for the game
class InputManager
{

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
	static std::map<void*, std::function<bool (char)>> anyKeyPressedCallback;									// standard callback for any key being pressed
	static std::map<void*, std::function<bool(char)>> anyKeyReleasedCallback;									// standard callback for any key being released
	static std::map<void*, std::function<bool()>> keyPressedCallbacks[(int)KEYS::KEY_MAX];						// keyboard key pressed callbacks
	static std::map<void*, std::function<bool()>> keyReleasedCallbacks[(int)KEYS::KEY_MAX];						// keyboard key released callbacks
	static std::map<void*, std::function<bool()>> mouseKeyPressedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];		// mouse button pressed callbacks
	static std::map<void*, std::function<bool()>> mouseKeyReleasedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];	// mouse button released callbacks

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
	static void PressChar(WPARAM wParam);
	// repeat a key when it's held down
	static void RepeatKey() { previousCharPressed = '\0'; }
	// on focus lost to our window
	static void FocusLost();

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

	// Attach to the "any key pressed" callback
	static DelegateHandle* AnyKeyPressedCallback_Attach(std::function<bool(char)> func, void* userObj);
	// Removed from the "any key pressed" callback
	static bool AnyKeyPressedCallback_Remove(std::function<bool(char)> func, void* userObj);
	// Attach to the "key pressed" callback
	static DelegateHandle* KeyPressedCallbacks_Attach(KEYS whichKey, std::function<bool()> func, void* userObj);
	// Remove from the "key pressed" callback
	static bool KeyPressedCallbacks_Remove(KEYS whichKey, std::function<bool()> func, void* userObj);
	// Attach to the "mouse key pressed" callback
	static DelegateHandle* MouseKeyPressedCallbacks_Attach(MOUSE_KEYS whichKey, std::function<bool()> func, void* userObj);
	// Remove from the "mouse key pressed" callback
	static bool MouseKeyPressedCallbacks_Remove(MOUSE_KEYS whichKey, std::function<bool()> func, void* userObj);
	// Attach to the "any key released" callback
	static DelegateHandle* AnyKeyReleasedCallback_Attach(std::function<bool(char)> func, void* userObj);
	// Removed from the "any key released" callback
	static bool AnyKeyReleasedCallback_Remove(std::function<bool(char)> func, void* userObj);
	// Attach to the "key released" callback
	static DelegateHandle* KeyReleasedCallbacks_Attach(KEYS whichKey, std::function<bool()> func, void* userObj);
	// Remove from the "key released" callback
	static bool KeyReleasedCallbacks_Remove(KEYS whichKey, std::function<bool()> func, void* userObj);
	// Attach to the "mouse key released" callback
	static DelegateHandle* MouseKeyReleasedCallbacks_Attach(MOUSE_KEYS whichKey, std::function<bool()> func, void* userObj);
	// Remove from the "mouse key released" callback
	static bool MouseKeyReleasedCallbacks_Remove(MOUSE_KEYS whichKey, std::function<bool()> func, void* userObj);

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
	static DelegateHandle* AddFunctionToCallbackList(std::map<void*, std::function<T>>* callbackList, std::function<T> func, void* userObj);

	template <class T>
	static bool RemoveFunctionFromCallbackList(std::map<void*, std::function<T>>* callbackList, std::function<T> func, void* userObj);
};
