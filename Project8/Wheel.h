#pragma once
#include<iostream>
using namespace std;
class Wheel
{
public:
	HBITMAP hBitMap;
	int frame_col;
	int frame_line;
	int interval;
	int value;
	int frame;
	int size_wheel_x;
	int size_wheel_y;
	Wheel()
	{
		frame = frame_col = frame_line = 0;
		interval = 20;
		hBitMap = (HBITMAP)LoadImage(NULL, L"sprite2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		size_wheel_x = 284;
		size_wheel_y = 284;
	}
	void GetVal()
	{
		frame--;
		switch (frame)
		{
		case 1:
		{
			value = -1;
			break;
		}
		case 2:
		{
			value = 200;
			break;
		}
		case 3:
		{
			value = 300;
			break;
		}
		case 4:
		{
			value = 250;
			break;
		}
		case 5:
		{
			value = 200;
			break;
		}
		case 6:
		{
			value = 800;
			break;
		}
		case 7:
		{
			value = 0;
			break;
		}
		case 8:
		{
			value = 200;
			break;
		}
		case 9:
		{
			value = 300;
			break;
		}
		case 10:
		{
			value = 350;
			break;
		}
		case 11:
		{
			value = 250;
			break;
		}
		case 12:
		{
			value = 400;
			break;
		}
		case 13:
		{
			value = 300;
			break;
		}
		case 14:
		{
			value = 200;
			break;
		}
		case 15:
		{
			value = 250;
			break;
		}
		case 16:
		{
			value = 500;
			break;
		}
		case 17:
		{
			value = 350;
			break;
		}
		case 18:
		{
			value = 250;
			break;
		}
		case 19:
		{
			value = 0;
			break;
		}
		case 20:
		{
			value = 200;
			break;
		}
		case 21:
		{
			value = 300;
			break;
		}
		case 22:
		{
			value = 400;
			break;
		}
		case 23:
		{
			value = 250;
			break;
		}
		default:
			value = 600;
		}
		
	}
	void Display(HDC hdc)
	{
		int m = rand() % 21;
		int m10 = rand() % 3 + 1;
		interval = 10;
		for (int i = 0; i < 20 * m10 + m; i++)
		{
			HDC hdcMem1 = CreateCompatibleDC(hdc);
			SelectObject(hdcMem1, hBitMap);
			StretchBlt(hdc, 100, 150, size_wheel_x, size_wheel_y, hdcMem1, frame_col * size_wheel_x, frame_line * size_wheel_y, size_wheel_x, size_wheel_y, SRCCOPY);
			DeleteDC(hdcMem1);
			Sleep(interval);
			interval++;
			if (interval > 100)
			{
				break;
			}
			frame_col++;

			if (frame_col >= 3 and frame_line >= 4)
			{
				frame_col = frame_line = frame = 0;
			}
			if (frame_col > 4)
			{
				frame_col = 0;
				frame_line++;
			}
		}
		frame = frame_col + 1 + frame_line * 5;
		Sleep(1500);
		GetVal();

	}
};


