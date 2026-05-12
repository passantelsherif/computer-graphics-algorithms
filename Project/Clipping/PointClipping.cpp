#include "Clipping.h"

// Point Clipping with Rectangle Window
void PointClippingRect(int x, int y, const ClippingWindow& window,
    HDC hdc, COLORREF color)
{
    if (x >= window.left && x <= window.right &&
        y >= window.top && y <= window.bottom)
    {
        // Point is inside the clipping window, draw it
        SetPixel(hdc, x, y, color);
    }
    // If outside, do nothing (clip it)
}

// Point Clipping with Square Window
void PointClippingSquare(int x, int y, const ClippingWindow& window,
    HDC hdc, COLORREF color)
{
    // Square window uses same logic as rectangle
    PointClippingRect(x, y, window, hdc, color);
}