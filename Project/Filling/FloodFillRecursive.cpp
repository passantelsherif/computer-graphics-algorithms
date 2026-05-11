#include "Fill.h"
#include<Windows.h>
#include <stack>


void FloodFillRecursive(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor)
{
    
    COLORREF currentColor = GetPixel(hdc, x, y);

    
    // CLR_INVALID means we clicked outside the window window bounds
    if (currentColor == CLR_INVALID ||
        currentColor == borderColor ||
        currentColor == fillColor)
    {
        return;
    }

    SetPixel(hdc, x, y, fillColor);

   
    FloodFillRecursive(hdc, x + 1, y, borderColor, fillColor);
    FloodFillRecursive(hdc, x - 1, y, borderColor, fillColor);
    FloodFillRecursive(hdc, x, y + 1, borderColor, fillColor);
    FloodFillRecursive(hdc, x, y - 1, borderColor, fillColor);
}