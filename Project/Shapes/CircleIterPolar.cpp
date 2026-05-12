#include "Circle.h"
#include <math.h>

void DrawCircleIterPolar(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    double dtheta = 1.0 / R;
    double cos_dtheta = cos(dtheta);
    double sin_dtheta = sin(dtheta);
    double x = R, y = 0;
    Draw8Points(hdc, xc, yc, (int)x, (int)y, c);

    while (x > y)
    {
        double x_new = x * cos_dtheta - y * sin_dtheta;
        y = x * sin_dtheta + y * cos_dtheta;
        x = x_new;
        Draw8Points(hdc, xc, yc, (int)round(x), (int)round(y), c);
    }
}