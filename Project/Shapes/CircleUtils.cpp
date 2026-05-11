#include "Circle.h"

// This exists only once in the entire project!
void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc - x, yc - y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc - y, yc - x, color);
    SetPixel(hdc, xc + y, yc - x, color);
}