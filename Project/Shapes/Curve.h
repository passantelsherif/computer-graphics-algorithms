#pragma once
#include <Windows.h>
#include <math.h>
#include <vector>

using namespace std;

void DrawHermiteCurve(HDC hdc, double x1, double y1, double u1, double v1, double x2, double y2, double u2, double v2, int numPoints, COLORREF c);
void DrawBezier(HDC hdc, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, COLORREF c);
void DrawCardinalSpline(HDC hdc, vector<POINT> points, double c, COLORREF col);