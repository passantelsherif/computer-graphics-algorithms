#include "Fill.h"
#include "../Shapes/Lines.h" 
#include<algorithm>
#include "../Shapes/Curve.h" 

void FillSquareHermiteVertical(HDC hdc, int left, int top, int sideLen, COLORREF c) {
    int right = left + sideLen;
    int bottom = top + sideLen;

    // Draw borders
    bresenhamLine(hdc, left, top, right, top, c);
    bresenhamLine(hdc, left, top, left, bottom, c);
    bresenhamLine(hdc, right, top, right, bottom, c);
    bresenhamLine(hdc, left, bottom, right, bottom, c);

    while (left < right) {
        double nwLeft = left - sideLen / 16.0;
        double T1x = left - nwLeft, T1y = top - (top + sideLen / 16.0);
        double T2x = left - nwLeft, T2y = bottom - (bottom - sideLen / 16.0);
        DrawHermiteCurve(hdc, (double)left, (double)top, T1x, T1y, (double)left, (double)bottom, T2x, T2y, 100, c);
        left += max(1, sideLen / 128);
    }
}