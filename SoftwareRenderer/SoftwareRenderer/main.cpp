#include <iostream>
#include <Windows.h>
#include "Window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Window window;
	window.Init(hInstance, nCmdShow);
	window.Run();
}