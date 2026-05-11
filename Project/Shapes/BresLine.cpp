#include "Lines.h"  
#include <Windows.h>


using namespace std;

// Bresenham's Midpoint Line Algorithm — Case 1: 0 <= slope <= 1, x1 < x2
void bresenhamLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Initial decision parameter and increments
    int d = dx - 2 * dy;         // d = Δx - 2Δy
    int dd1 = 2 * dx - 2 * dy;   // Δd1: diagonal step (d < 0)
    int dd2 = -2 * dy;           // Δd2: horizontal step (d >= 0)

    int x = x1;
    int y = y1;

    // Plot the first pixel
    SetPixel(hdc, x, y, color);
  

    while (x < x2) {
        if (d < 0) {
            // Line is above midpoint -> go diagonal (NE)
            y--;        // y-- because in Windows, y increases DOWNWARD
            d += dd1;
        }
        else {
            // Line is below midpoint -> go horizontal (E)
            d += dd2;
        }
        x++;
        SetPixel(hdc, x, y, color);
      
    }
}