#include"Fill.h"
#include <Windows.h>

#include <stack>

void FloodFillNonRecursive(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor)
{
    std::stack<POINT> s;
    s.push({ x, y });

    while (!s.empty())
    {
        POINT p = s.top();
        s.pop();

        COLORREF c = GetPixel(hdc, p.x, p.y);
        // Stop if we hit the border color, or if the pixel is already filled
        if (c == borderColor || c == fillColor) continue;

        SetPixel(hdc, p.x, p.y, fillColor);

        s.push({ p.x,     p.y + 1 });
        s.push({ p.x,     p.y - 1 });
        s.push({ p.x + 1, p.y });
        s.push({ p.x - 1, p.y });
    }
}