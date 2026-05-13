#include "Fill.h"
#include "../Shapes/Lines.h"


void FillCircleWithLines(HDC hdc, int xc, int yc, int rOuter, COLORREF c, int fillQuarter)
{
    for (int r = 1; r <= rOuter; r++)
    {
        int x = 0, y = r;
        int d = 1 - r;
        int d1 = 5 - 2 * r;
        int d2 = 3;

        while (x <= y)
        {
            switch (fillQuarter)
            {
            case 1: // Top‑Right
                DrawLineDDA(hdc, xc, yc, xc + x, yc - y, c);
                DrawLineDDA(hdc, xc, yc, xc + y, yc - x, c);
                break;
            case 2: // Top‑Left
                DrawLineDDA(hdc, xc, yc, xc - x, yc - y, c);
                DrawLineDDA(hdc, xc, yc, xc - y, yc - x, c);
                break;
            case 3: // Bottom‑Left
                DrawLineDDA(hdc, xc, yc, xc - x, yc + y, c);
                DrawLineDDA(hdc, xc, yc, xc - y, yc + x, c);
                break;
            case 4: // Bottom‑Right
                DrawLineDDA(hdc, xc, yc, xc + x, yc + y, c);
                DrawLineDDA(hdc, xc, yc, xc + y, yc + x, c);
                break;
            default:
                return; // invalid quarter
            }

            if (d < 0)
            {
                d += d2;
                d1 += 2;
                d2 += 2;
            }
            else
            {
                d += d1;
                d2 += 2;
                d1 += 4;
                y--;
            }
            x++;
        }
    }
}