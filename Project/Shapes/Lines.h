#pragma once
#include <windows.h> 


void bresenhamLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);
void DrawLineDDA(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color);
void DrawLineParametric(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color);

