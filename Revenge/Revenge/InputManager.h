#pragma once
#include "Manager.h"

class InputManager
{
public:

	static char keyOptions[Manager::KEY_MAX];
	static bool keys[Manager::KEY_MAX];
	static bool previousKeys[Manager::KEY_MAX];

	static void PressKey(WPARAM wParam);
	static void ReleaseKey(WPARAM wParam);

	static bool IsKeyDown(Manager::KEYS index) { return keys[index]; }
	static bool IsPreviousKeyDown(Manager::KEYS index) { return previousKeys[index]; }
	static bool IsKeyPressed(Manager::KEYS index);

	static void Init();
	static void Update(float delta_time);
};

