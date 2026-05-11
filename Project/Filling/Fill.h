#pragma once
#include <windows.h>

// Non-Recursive  Flood Fill
void FloodFillNonRecursive(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor);

//Recursive flood fill
void FloodFillRecursive(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor);