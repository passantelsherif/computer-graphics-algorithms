#include "Lines.h"
#include <math.h>
#include <algorithm> // For swap

using namespace std;

void DrawLineParametric(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
{
    int dx = xe - xs;
    int dy = ye - ys;
    if (abs(dy) <= abs(dx))
    {
        double slope = (double)dy / dx;
        if (xs > xe) { swap(xs, xe); swap(ys, ye); }
        for (int x = xs; x <= xe; x++)
        {
            int y = (int)round(ys + (x - xs) * slope);
            SetPixel(hdc, x, y, color);
        }
    }
    else
    {
        double islope = (double)dx / dy;
        if (ys > ye) { swap(xs, xe); swap(ys, ye); }
        for (int y = ys; y <= ye; y++)
        {
            int x = (int)round(xs + (y - ys) * islope);
            SetPixel(hdc, x, y, color);
        }
    }
}