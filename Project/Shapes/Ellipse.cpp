#include "Ellipse.h"
#include <Windows.h>


void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + x, yc - y, c);
}

void DrawEllipseDirect(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
    int x = 0;
    double y = b;
    Draw4Points(hdc, xc, yc, x, y, c);
    while (b * b * x < a * a * y) { //slope<1
        x++;
        y = b * sqrt(1.0 - (double)x * x / (a * a));
        Draw4Points(hdc, xc, yc, x, round(y), c);
    }
    double x1 = a;
    int y1 = 0;
    Draw4Points(hdc, xc, yc, x1, y1, c);
    while (b * b * x1 > a * a * y1) { //slope>1
        y1++;
        x1 = a * sqrt(1.0 - (double)y1 * y1 / (b * b));
        Draw4Points(hdc, xc, yc, round(x1), y1, c);
    }
}

void DrawEllipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF c)
{
    double dtheta = 1.0 / (a > b ? a : b);
    double theta = 0;

    while (theta <= 1.57)
    {
        int x = (int)round(a * cos(theta));
        int y = (int)round(b * sin(theta));
        Draw4Points(hdc, xc, yc, x, y, c);
        theta += dtheta;
    }
}

void DrawEllipseMidpoint(HDC hdc, int xc, int yc, int a, int b, COLORREF c)
{
    int x = 0, y = b;
    int a2 = a * a, b2 = b * b;

    // Region 1
    int d = (int)(b2 - a2 * b + 0.25 * a2);
    int dx = 2 * b2 * x;
    int dy = 2 * a2 * y;

    while (dx < dy)
    {
        Draw4Points(hdc, xc, yc, x, y, c);
        x++;
        dx += 2 * b2;
        if (d < 0)
            d += dx + b2;
        else
        {
            y--;
            dy -= 2 * a2;
            d += dx - dy + b2;
        }
    }

    // Region 2
    d = (int)(b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2);

    while (y >= 0)
    {
        Draw4Points(hdc, xc, yc, x, y, c);
        y--;
        dy -= 2 * a2;
        if (d > 0)
            d += a2 - dy;
        else
        {
            x++;
            dx += 2 * b2;
            d += dx - dy + a2;
        }
    }
}