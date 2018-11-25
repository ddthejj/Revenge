#pragma once
#include "defines.h"
#include "vld.h"
#include "Manager.h"
#include <time.h>

// MEMORY LEAK
#ifdef _DEBUG
struct MemLeak
{
	MemLeak() {}
	~MemLeak()
	{
		_CrtDumpMemoryLeaks();
	}
};
#endif


int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);


int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	const float FPS = 60.f;

#ifdef _DEBUG
	MemLeak leaks;
	_CrtSetBreakAlloc(-1);
#endif
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass1";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, (LONG)WIDTH, (LONG)HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(
		NULL,
		L"WindowClass1",
		L"Revenge",
		WS_OVERLAPPEDWINDOW,
		300, 150, wr.left + wr.right, wr.top + wr.bottom,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	MSG msg;

	Manager::Init(hWnd);

	clock_t lastUpdate = clock();

	//Game Loop
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			clock_t ts = clock() - lastUpdate;
			if (((float)ts / CLOCKS_PER_SEC) >= (1.f / FPS))
			{
				lastUpdate = clock();

				Manager::Update((float)ts);
				Manager::Draw();
			}
		}
	}
	Manager::Clean();
	/*
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	*/
	return (int)msg.wParam;
}
LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{

	switch (message)
	{
		//Window is activated or deactivated
	case WM_ACTIVATE:
		switch (wParam)
		{
			//Window is activated
		case WA_ACTIVE:
			//Start capturing the mouse
			break;
			//Window is deactivated
		case WA_INACTIVE:
			//Stop capturing the mouse
			break;
		}
		break;
		//Window is destroyed
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		//Mouse movement is detected
	case WM_MOUSEMOVE:
		if (hWnd == GetForegroundWindow())
			Manager::MoveMouse(hWnd, lParam);
		break;
		//Key is pressed down
	case WM_KEYDOWN:
		if (hWnd != GetForegroundWindow())
			break;
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		default:
			Manager::PressKey(wParam);
			break;
		}
		break;
		//Key is released
	case WM_KEYUP:
		Manager::ReleaseKey(wParam);
		break;
	case WM_SIZE:
		Manager::ResizeWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
