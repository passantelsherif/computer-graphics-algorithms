#include "Circle.h"

void DrawCircleBresenham(HDC hdc, int xc, int yc, int R, COLORREF color)
{
    if (R <= 0) return;
    int x = 0, y = R, d = 1 - R;
    int ch1 = 3, ch2 = 5 - 2 * R;


    while (x < y) {
        if (d < 0) { d += ch1; ch2 += 2; }
        else { d += ch2; ch2 += 4; y--; }
        ch1 += 2; x++;
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}