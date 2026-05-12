#include "Clipping.h"

// Reuse existing Bresenham line algorithm from Shapes folder
extern void bresenhamLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);

void DrawClippedLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    bresenhamLine(hdc, x1, y1, x2, y2, color);
}

void DrawClippedPoint(HDC hdc, int x, int y, COLORREF color)
{
    SetPixel(hdc, x, y, color);
}

void DrawClippedPolygon(HDC hdc, const std::vector<Vertex>& polygon, COLORREF color)
{
    if (polygon.empty()) return;

    // Draw edges of the polygon
    for (size_t i = 0; i < polygon.size(); i++)
    {
        const Vertex& v1 = polygon[i];
        const Vertex& v2 = polygon[(i + 1) % polygon.size()];

        DrawClippedLine(hdc, (int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, color);
    }
}

void DrawWindow(HDC hdc, const ClippingWindow& window, COLORREF color)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, color);
    HPEN hOld = (HPEN)SelectObject(hdc, hPen);

    // Use a hollow brush so the inside stays white
    HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Rectangle(hdc, window.left, window.top, window.right, window.bottom);

    SelectObject(hdc, hOld);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}