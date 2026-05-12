#include "Circle.h"
#include <math.h>

void DrawCircleDirect(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    int x = 0;
    int y = R;
    Draw8Points(hdc, xc, yc, x, y, c);

    while (x < y)
    {
        x++;
        y = (int)round(sqrt(R * R - x * x));
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}