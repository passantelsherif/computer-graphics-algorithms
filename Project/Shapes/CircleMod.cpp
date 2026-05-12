#include "Circle.h"

void DrawCircleMod(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    int x = 0, y = R, d = 1 - R, d2 = 3, d1 = 5 - 2 * R;
    Draw8Points(hdc, xc, yc, x, y, c);
    while (x < y)
    {
        if (d < 0)
        {
            d += d2;
            d2 += 2;
            d1 += 2;
        }
        else
        {
            d += d1;
            d2 += 2;
            d1 += 4;
            y--;
        }
        x++;
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}