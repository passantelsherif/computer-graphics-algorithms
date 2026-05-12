#pragma once
#include <windows.h>
#include <vector>

using namespace std;

// Requirement 7(f): Flood Fill
void FloodFillNonRecursive(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor);
void FloodFillRecursive(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor);

// Requirement 7(a) & 7(b): Circle Filling
void FillCircleWithCircles(HDC hdc, int xc, int yc, int rOuter, COLORREF c, int fillQuarter);
void FillCircleWithLines(HDC hdc, int xc, int yc, int rOuter, COLORREF c, int fillQuarter);

// Requirement 7(c) & 7(d): Square and Rectangle Filling
void FillSquareHermiteVertical(HDC hdc, int left, int top, int sideLen, COLORREF c);
void FillRectangleBezierHorizontal(HDC hdc, int left, int top, int width, int height, COLORREF c);

// Requirement 7(e): Convex and Non-Convex Polygon Filling
void PolyFill(HDC hdc, POINT P[], int n, COLORREF c);       // Convex
void nonPolyFill(HDC hdc, POINT P[], int n, COLORREF c);    // Non-Convex