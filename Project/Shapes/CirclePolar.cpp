#include "Circle.h"
#include <math.h>

void DrawCirclePolar(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    double dtheta = 1.0 / R;
    double theta = 0;
    int x = R, y = 0;
    Draw8Points(hdc, xc, yc, x, y, c);

    while (x > y)
    {
        theta += dtheta;
        x = (int)round(R * cos(theta));
        y = (int)round(R * sin(theta));
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}

