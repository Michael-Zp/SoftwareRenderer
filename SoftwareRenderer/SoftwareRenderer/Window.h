#pragma once

#include <Windows.h>
#include "Timer.h"
#include <thread>
#include "DoubleBuffer.h"

class Window
{
public:
	int Init(const HINSTANCE hInstance, const int nCmdShow);
	void SetSize(const WORD width, const WORD height) { this->width = width; this->height = height; doubleBuffer.UpdateSize(width, height); };
	void Run();
	WORD GetWidth() const { return width; };
	WORD GetHeight() const { return height; };
	const Timer& GetTimer() const { return timer; };
	void InitializeTimer() { timer.Initialize(); };
	void UpdateTimer() { timer.Update(); };
	BitmapBuffer& GetDoubleBuffer() { return doubleBuffer; };
	void BeginFrame() { frameRendered = false; }
	bool IsLastFrameRendered() const { return frameRendered; };
	void EndFrame() { frameRendered = true; }

private:
	bool frameRendered = false;
	HWND hWnd;
	ULONG_PTR gdiplusToken;
	WORD width = 800;
	WORD height = 600;
	Timer timer;
	BitmapBuffer doubleBuffer;
};

