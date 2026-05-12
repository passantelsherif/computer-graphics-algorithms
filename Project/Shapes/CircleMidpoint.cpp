#include "Circle.h"

void DrawCircleMidpoint(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    int x = 0, y = R;
    Draw8Points(hdc, xc, yc, x, y, c);

    while (x < y)
    {
        double d = (x + 1) * (x + 1) + (y - 0.5) * (y - 0.5) - R * R;
        if (d >= 0) y--;
        x++;
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}