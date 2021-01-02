#include "defines.h"
#include "InputManager.h"
#include <Windowsx.h>


bool InputManager::keys[(int)KEYS::KEY_MAX];
bool InputManager::previousKeys[(int)KEYS::KEY_MAX];
char InputManager::keyOptions[(int)KEYS::KEY_MAX];
bool InputManager::mouseKeys[(int)MOUSE_KEYS::MOUSE_KEY_MAX];
bool InputManager::previousMouseKeys[(int)MOUSE_KEYS::MOUSE_KEY_MAX];
float InputManager::mouseX = 0.f, InputManager::mouseY = 0.f;
char InputManager::charPressed = '\0', InputManager::previousCharPressed = '\0';

std::vector<std::function<void(char)>> InputManager::anyKeyPressedCallback  = std::vector<std::function<void(char)>>();
std::vector<std::function<void(char)>> InputManager::anyKeyReleasedCallback = std::vector<std::function<void(char)>>();
std::vector<std::function<void()>> InputManager::keyPressedCallbacks[(int)KEYS::KEY_MAX];
std::vector<std::function<void()>> InputManager::keyReleasedCallbacks[(int)KEYS::KEY_MAX];
std::vector<std::function<void()>> InputManager::mouseKeyPressedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];
std::vector<std::function<void()>> InputManager::mouseKeyReleasedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];



template<typename T, typename... U>
size_t getAddress(std::function<T(U...)> f) {
	typedef T(fnType)(U...);
	fnType** fnPointer = f.template target<fnType*>();
	if (!fnPointer) return 0;
	return (size_t)*fnPointer;
}




bool InputManager::IsKeyPressed(KEYS index)
{
	return (keys[(int)index] == true && previousKeys[(int)index] == false);
}

bool InputManager::IsMouseKeyPressed(MOUSE_KEYS index)
{
	return mouseKeys[(int)index] == true && previousMouseKeys[(int)index] == false;
}


void InputManager::PressKey(WPARAM wParam)
{
	for (int key = 0; key < (int)KEYS::KEY_MAX; key++)
	{
		if (wParam == keyOptions[key])
		{
			keys[key] = true;
			for (int j = 0; j < keyPressedCallbacks[key].size(); j++)
			{
				keyPressedCallbacks[key][j]();
			}
			break;
		}
	}
}

void InputManager::ReleaseKey(WPARAM wParam)
{
	for (int key = 0; key < (int)KEYS::KEY_MAX; key++)
	{
		if (wParam == keyOptions[key])
		{
			for (int j = 0; j < keyReleasedCallbacks[key].size(); j++)
			{
				keyReleasedCallbacks[key][j]();
			}
			keys[key] = false;
			break;
		}
	}
}

void InputManager::MoveMouse(HWND hwnd, LPARAM lParam)
{
	mouseX = (float)(GET_X_LPARAM(lParam));
	mouseY = (float)(GET_Y_LPARAM(lParam));
}

void InputManager::PressMouseKey(MOUSE_KEYS key)
{
	mouseKeys[(int)key] = true;

	for (int j = 0; j < mouseKeyPressedCallbacks[(int)key].size(); j++)
	{
		mouseKeyPressedCallbacks[(int)key][j]();
	}
}

void InputManager::ReleaseMouseKey(MOUSE_KEYS key)
{
	mouseKeys[(int)key] = false;

	for (int j = 0; j < mouseKeyReleasedCallbacks[(int)key].size(); j++)
	{
		mouseKeyReleasedCallbacks[(int)key][j]();
	}
}

void InputManager::PressChar(WPARAM wParam)
{
	charPressed = (char)wParam;

	for (int j = 0; j < anyKeyPressedCallback.size(); j++)
	{
		//anyKeyPressedCallback[j].func((char)wParam, anyKeyPressedCallback[j].object);
		anyKeyPressedCallback[j](charPressed);
	}

}


bool InputManager::AnyKeyPressedCallback_Attatch(std::function<void(char)> func)
{
	return AddFunctionToCallbackList(&anyKeyPressedCallback, func);
}

bool InputManager::AnyKeyPressedCallback_Remove(std::function<void(char)> func)
{
	return RemoveFunctionFromCallbackList(&anyKeyPressedCallback, func);
}

bool InputManager::KeyPressedCallbacks_Attatch(KEYS whichKey, std::function<void()> func)
{
	return AddFunctionToCallbackList(&keyPressedCallbacks[(int)whichKey], func);
}

bool InputManager::KeyPressedCallbacks_Remove(KEYS whichKey, std::function<void()> func)
{
	return RemoveFunctionFromCallbackList(&keyPressedCallbacks[(int)whichKey], func);
}

bool InputManager::MouseKeyPressedCallbacks_Attatch(MOUSE_KEYS whichKey, std::function<void()> func)
{
	return AddFunctionToCallbackList(&mouseKeyPressedCallbacks[(int)whichKey], func);
}

bool InputManager::MouseKeyPressedCallbacks_Remove(MOUSE_KEYS whichKey, std::function<void()> func)
{
	return RemoveFunctionFromCallbackList(&mouseKeyPressedCallbacks[(int)whichKey], func);
}

bool InputManager::AnyKeyReleasedCallback_Attatch(std::function<void(char)> func)
{
	return AddFunctionToCallbackList(&anyKeyReleasedCallback, func);
}

bool InputManager::AnyKeyReleasedCallback_Remove(std::function<void(char)> func)
{
	return RemoveFunctionFromCallbackList(&anyKeyReleasedCallback, func);
}

bool InputManager::KeyReleasedCallbacks_Attatch(KEYS whichKey, std::function<void()> func)
{
	return AddFunctionToCallbackList(&keyReleasedCallbacks[(int)whichKey], func);
}

bool InputManager::KeyReleasedCallbacks_Remove(KEYS whichKey, std::function<void()> func)
{
	return RemoveFunctionFromCallbackList(&keyReleasedCallbacks[(int)whichKey], func);
}

bool InputManager::MouseKeyReleasedCallbacks_Attatch(MOUSE_KEYS whichKey, std::function<void()> func)
{
	return AddFunctionToCallbackList(&mouseKeyReleasedCallbacks[(int)whichKey], func);
}

bool InputManager::MouseKeyReleasedCallbacks_Remove(MOUSE_KEYS whichKey, std::function<void()> func)
{
	return RemoveFunctionFromCallbackList(&mouseKeyReleasedCallbacks[(int)whichKey], func);
}


void InputManager::Init()
{
	// initial keybinds (replace with reading from settings file later)
	keyOptions[(int)KEYS::KEY_UP] = 'W';
	keyOptions[(int)KEYS::KEY_DOWN] = 'S';
	keyOptions[(int)KEYS::KEY_LEFT] = 'A';
	keyOptions[(int)KEYS::KEY_RIGHT] = 'D';
	keyOptions[(int)KEYS::KEY_MENU] = 'E';
	keyOptions[(int)KEYS::KEY_INTERACT] = VK_SPACE;


	// initialize callback arrays
	for (int i = 0; i < (int)KEYS::KEY_MAX; i++)
	{
		keyPressedCallbacks[i] = std::vector<std::function<void()>>();
		keyReleasedCallbacks[i] = std::vector<std::function<void()>>();
	}
	for (int i = 0; i < (int)MOUSE_KEYS::MOUSE_KEY_MAX; i++)
	{
		mouseKeyPressedCallbacks[i] = std::vector<std::function<void()>>();
		mouseKeyReleasedCallbacks[i] = std::vector<std::function<void()>>();
	}
}

void InputManager::Clean()
{
	// clear callback arrays
	for (int i = 0; i < (int)KEYS::KEY_MAX; i++)
	{
		keyPressedCallbacks[i].clear();
		keyReleasedCallbacks[i].clear();
	}
	for (int i = 0; i < (int)MOUSE_KEYS::MOUSE_KEY_MAX; i++)
	{
		mouseKeyPressedCallbacks[i].clear();
		mouseKeyReleasedCallbacks[i].clear();
	}
}

void InputManager::Update(float delta_time)
{
	for (int i = 0; i < (int)KEYS::KEY_MAX; i++)
		previousKeys[i] = keys[i];

	for (int i = 0; i < (int)MOUSE_KEYS::MOUSE_KEY_MAX; i++)
		previousMouseKeys[i] = mouseKeys[i];

	previousCharPressed = charPressed;
}


template <class T>
bool InputManager::AddFunctionToCallbackList(std::vector<T>* callbackList, T func)
{

	// check if we already contain the function
	for (auto it = callbackList->begin(); it != callbackList->end(); it++)
	{
		if (getAddress(*it) == getAddress(func))
		{
			// this list already contains the callback
			return false;
		}
	}
	// match wasn't found

	// add the new callback to the list
	callbackList->push_back(func);
	return true;
}

template<class T>
bool InputManager::RemoveFunctionFromCallbackList(std::vector<T>* callbackList, T func)
{
	// make sure we already contain the function
	for (auto it = callbackList->begin(); it != callbackList->end(); it++)
	{
		if (getAddress(*it) == getAddress(func))
		{
			callbackList->erase(it);
			return true;
		}
	}
	return false;
}