#pragma once

#include <Windows.h>
#include <gdiplus.h>

class BitmapBuffer
{
public:
	struct PixelBuffer
	{
		unsigned* pixels;
		int stride;
	};

	void UpdateSize(WORD width, WORD height)
	{
		this->width = width;
		this->height = height;

		bitmapRect = Gdiplus::Rect(0, 0, width, height);

		delete buffer;
		buffer = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
	}

	PixelBuffer GetPixelBuffer()
	{
		bitmapData = new Gdiplus::BitmapData;
		Gdiplus::Rect bitmapRect(0, 0, width, height);
		buffer->LockBits(&bitmapRect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, bitmapData);

		return { static_cast<unsigned*>(bitmapData->Scan0), abs(bitmapData->Stride) };
	}

	void WriteBuffer()
	{
		buffer->UnlockBits(bitmapData);
	}

	Gdiplus::Bitmap* GetNextFrame() { return buffer; };

private:
	Gdiplus::Bitmap* buffer;
	int width = 0;
	int height = 0;
	Gdiplus::BitmapData* bitmapData;
	Gdiplus::Rect bitmapRect;
};