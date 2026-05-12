#pragma once
#include <Windows.h>
#include <math.h>

void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c);
void DrawEllipseDirect(HDC hdc, int xc, int yc, int a, int b, COLORREF c);
void DrawEllipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF c);
void DrawEllipseMidpoint(HDC hdc, int xc, int yc, int a, int b, COLORREF c);