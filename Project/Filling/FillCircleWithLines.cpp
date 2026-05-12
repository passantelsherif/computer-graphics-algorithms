#include "Fill.h"

void FillCircleWithLines(HDC hdc, int xc, int yc, int rOuter, COLORREF c, int fillQuarter) {
    for (int r = 1; r <= rOuter; r++) {
        int x = 0, y = r, d = 1 - r, d1 = 5 - 2 * r, d2 = 3;
        while (x <= y) {
            switch (fillQuarter) {
            case 1: MoveToEx(hdc, xc, yc, NULL); LineTo(hdc, xc + x, yc - y); MoveToEx(hdc, xc, yc, NULL); LineTo(hdc, xc + y, yc - x); break;
            case 2: MoveToEx(hdc, xc, yc, NULL); LineTo(hdc, xc - x, yc - y); MoveToEx(hdc, xc, yc, NULL); LineTo(hdc, xc - y, yc - x); break;
            case 3: MoveToEx(hdc, xc, yc, NULL); LineTo(hdc, xc - x, yc + y); MoveToEx(hdc, xc, yc, NULL); LineTo(hdc, xc - y, yc + x); break;
            case 4: MoveToEx(hdc, xc, yc, NULL); LineTo(hdc, xc + x, yc + y); MoveToEx(hdc, xc, yc, NULL); LineTo(hdc, xc + y, yc + x); break;
            }
            if (d < 0) { d += d2; d2 += 2; d1 += 2; }
            else { d += d1; d2 += 2; d1 += 4; y--; }
            x++;
        }
    }
}