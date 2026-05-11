#pragma once
#pragma once
#include <windows.h>



// Helper function used by all circle algorithms to draw the 8 symmetric pixels
void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color);

// circle bresenham
void DrawCircleBresenham(HDC hdc, int xc, int yc, int R, COLORREF color);
//circle midpoint
void DrawCircleMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color);
//circle polar
void DrawCirclePolar(HDC hdc, int xc, int yc, int R, COLORREF color);
