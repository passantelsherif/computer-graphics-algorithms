#include "Lines.h"
#include <Windows.h>
#include <cmath>

// Generalized Bresenham's Line Algorithm
void bresenhamLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    // Determine the step direction for both axes
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        // Draw the current pixel
        SetPixel(hdc, x1, y1, color);

        // If we reached the end point, stop
        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;

        // Adjust X
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        // Adjust Y
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}