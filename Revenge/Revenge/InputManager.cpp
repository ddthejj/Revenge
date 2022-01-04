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

std::map<void*, std::function<void(char)>> InputManager::anyKeyPressedCallback = std::map<void*, std::function<void(char)>>();
std::map<void*, std::function<void(char)>> InputManager::anyKeyReleasedCallback = std::map<void*, std::function<void(char)>>();
std::map<void*, std::function<void()>> InputManager::keyPressedCallbacks[(int)KEYS::KEY_MAX];
std::map<void*, std::function<void()>> InputManager::keyReleasedCallbacks[(int)KEYS::KEY_MAX];
std::map<void*, std::function<void()>> InputManager::mouseKeyPressedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];
std::map<void*, std::function<void()>> InputManager::mouseKeyReleasedCallbacks[(int)MOUSE_KEYS::MOUSE_KEY_MAX];


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
			std::map<void*, std::function<void()>> callback = keyPressedCallbacks[key];

			keys[key] = true;
			for (auto it = callback.begin(); it != callback.end(); it++)
			{
				it->second();
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
			std::map<void*, std::function<void()>> callback = keyReleasedCallbacks[key];

			keys[key] = false;
			for (auto it = callback.begin(); it != callback.end(); it++)
			{
				it->second();
			}
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
	std::map<void*, std::function<void()>> callback = mouseKeyPressedCallbacks[(int)key];

	mouseKeys[(int)key] = true;
	for (auto it = callback.begin(); it != callback.end(); it++)
	{
		it->second();
	}
}

void InputManager::ReleaseMouseKey(MOUSE_KEYS key)
{
	std::map<void*, std::function<void()>> callback = mouseKeyReleasedCallbacks[(int)key];

	mouseKeys[(int)key] = false;
	for (auto it = callback.begin(); it != callback.end(); it++)
	{
		it->second();
	}
}

void InputManager::PressChar(WPARAM wParam)
{
	charPressed = (char)wParam;
	for (auto it = anyKeyPressedCallback.begin(); it != anyKeyPressedCallback.end(); it++)
	{
		it->second(charPressed);
	}

}


DelegateHandle* InputManager::AnyKeyPressedCallback_Attach(std::function<void(char)> func, void* userObj)
{
	return AddFunctionToCallbackList(&anyKeyPressedCallback, func, userObj);
}

bool InputManager::AnyKeyPressedCallback_Remove(std::function<void(char)> func, void* userObj)
{
	return RemoveFunctionFromCallbackList(&anyKeyPressedCallback, func, userObj);
}

DelegateHandle* InputManager::KeyPressedCallbacks_Attach(KEYS whichKey, std::function<void()> func, void* userObj)
{
	return AddFunctionToCallbackList(&keyPressedCallbacks[(int)whichKey], func, userObj);
}

bool InputManager::KeyPressedCallbacks_Remove(KEYS whichKey, std::function<void()> func, void* userObj)
{
	return RemoveFunctionFromCallbackList(&keyPressedCallbacks[(int)whichKey], func, userObj);
}

DelegateHandle* InputManager::MouseKeyPressedCallbacks_Attach(MOUSE_KEYS whichKey, std::function<void()> func, void* userObj)
{
	return AddFunctionToCallbackList(&mouseKeyPressedCallbacks[(int)whichKey], func, userObj);
}

bool InputManager::MouseKeyPressedCallbacks_Remove(MOUSE_KEYS whichKey, std::function<void()> func, void* userObj)
{
	return RemoveFunctionFromCallbackList(&mouseKeyPressedCallbacks[(int)whichKey], func, userObj);
}

DelegateHandle* InputManager::AnyKeyReleasedCallback_Attach(std::function<void(char)> func, void* userObj)
{
	return AddFunctionToCallbackList(&anyKeyReleasedCallback, func, userObj);
}

bool InputManager::AnyKeyReleasedCallback_Remove(std::function<void(char)> func, void* userObj)
{
	return RemoveFunctionFromCallbackList(&anyKeyReleasedCallback, func, userObj);
}

DelegateHandle* InputManager::KeyReleasedCallbacks_Attach(KEYS whichKey, std::function<void()> func, void* userObj)
{
	return AddFunctionToCallbackList(&keyReleasedCallbacks[(int)whichKey], func, userObj);
}

bool InputManager::KeyReleasedCallbacks_Remove(KEYS whichKey, std::function<void()> func, void* userObj)
{
	return RemoveFunctionFromCallbackList(&keyReleasedCallbacks[(int)whichKey], func, userObj);
}

DelegateHandle* InputManager::MouseKeyReleasedCallbacks_Attach(MOUSE_KEYS whichKey, std::function<void()> func, void* userObj)
{
	return AddFunctionToCallbackList(&mouseKeyReleasedCallbacks[(int)whichKey], func, userObj);
}

bool InputManager::MouseKeyReleasedCallbacks_Remove(MOUSE_KEYS whichKey, std::function<void()> func, void* userObj)
{
	return RemoveFunctionFromCallbackList(&mouseKeyReleasedCallbacks[(int)whichKey], func, userObj);
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
		keyPressedCallbacks[i] = std::map<void*, std::function<void()>>();
		keyReleasedCallbacks[i] = std::map<void*, std::function<void()>>();
	}
	for (int i = 0; i < (int)MOUSE_KEYS::MOUSE_KEY_MAX; i++)
	{
		mouseKeyPressedCallbacks[i] = std::map<void*, std::function<void()>>();
		mouseKeyReleasedCallbacks[i] = std::map<void*, std::function<void()>>();
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
DelegateHandle* InputManager::AddFunctionToCallbackList(std::map<void*, std::function<T>>* callbackList, std::function<T> func, void* userObj)
{
	// check if we already contain a function from the user object
	for (auto it = callbackList->begin(); it != callbackList->end(); it++)
	{
		if ((*it).first == userObj)
		{
			// this list already contains a function from this user object
			return nullptr;
		}
	}
	// match wasn't found

	if (userObj)
	{
		// user object is valid, add the new callback to the list using it as reference
		callbackList->insert(std::pair<void*, std::function<T>>(userObj, func));
		return nullptr;
	}
	else
	{
		// user object is not valid, bind this callback to a handle the user is responsible for 
		DelegateHandle* handle = new DelegateHandle();
		callbackList->insert(std::pair<void*, std::function<T>>(handle, func));
		return handle;
	}
}

template<class T>
bool InputManager::RemoveFunctionFromCallbackList(std::map<void*, std::function<T>>* callbackList, std::function < T >func, void* userObj)
{
	// make sure we already contain the function
	for (auto it = callbackList->begin(); it != callbackList->end(); it++)
	{
		if ((*it).first == userObj)
		{
			callbackList->erase(it);
			return true;
		}
	}
	return false;
}