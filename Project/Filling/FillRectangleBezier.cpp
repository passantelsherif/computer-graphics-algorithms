#include "Fill.h"
#include "../Shapes/Lines.h" 
#include<algorithm>
#include "../Shapes/Curve.h" 

void FillRectangleBezierHorizontal(HDC hdc, int left, int top, int width, int height, COLORREF c) {
    int right = left + width;
    int bottom = top + height;

    // Draw borders
    bresenhamLine(hdc, left, top, right, top, c);
    bresenhamLine(hdc, left, top, left, bottom, c);
    bresenhamLine(hdc, right, top, right, bottom, c);
    bresenhamLine(hdc, left, bottom, right, bottom, c);

    while (top < bottom) {
        double T2x = left + width / 32.0, T2y = top + height / 32.0;
        double T1x = right - width / 32.0, T1y = top + height / 32.0;
        DrawBezier(hdc, (double)left, (double)top, T1x, T1y, T2x, T2y, (double)right, (double)top, c);
        top += max(1, height / 128);
    }
}