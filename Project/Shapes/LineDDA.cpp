#include "Lines.h"
#include <math.h>

void DrawLineDDA(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
{
    int dx = xe - xs;
    int dy = ye - ys;
    SetPixel(hdc, xs, ys, color);
    if (abs(dx) >= abs(dy))
    {
        int x = xs;
        int xinc = (dx > 0) ? 1 : -1;
        double y = ys;
        double yinc = (double)dy / dx * xinc;
        while (x != xe)
        {
            x += xinc;
            y += yinc;
            SetPixel(hdc, x, (int)round(y), color);
        }
    }
    else
    {
        int y = ys;
        int yinc = (dy > 0) ? 1 : -1;
        double x = xs;
        double xinc = (double)dx / dy * yinc;
        while (y != ye)
        {
            y += yinc;
            x += xinc;
            SetPixel(hdc, (int)round(x), y, color);
        }
    }
}