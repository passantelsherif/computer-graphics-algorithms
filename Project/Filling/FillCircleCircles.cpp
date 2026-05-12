#include "Fill.h"

void FillCircleWithCircles(HDC hdc, int xc, int yc, int rOuter, COLORREF c, int fillQuarter) {
    for (int r = 1; r <= rOuter; r++) {
        int x = 0, y = r, d = 1 - r, d1 = 5 - 2 * r, d2 = 3;
        while (x <= y) {
            switch (fillQuarter) {
            case 1: SetPixel(hdc, xc + x, yc - y, c); SetPixel(hdc, xc + y, yc - x, c); break; // Top-Right
            case 2: SetPixel(hdc, xc - x, yc - y, c); SetPixel(hdc, xc - y, yc - x, c); break; // Top-Left
            case 3: SetPixel(hdc, xc - x, yc + y, c); SetPixel(hdc, xc - y, yc + x, c); break; // Bottom-Left
            case 4: SetPixel(hdc, xc + x, yc + y, c); SetPixel(hdc, xc + y, yc + x, c); break; // Bottom-Right
            }
            if (d < 0) { d += d2; d2 += 2; d1 += 2; }
            else { d += d1; d2 += 2; d1 += 4; y--; }
            x++;
        }
    }
}