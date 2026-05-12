#include "Fill.h"
#include <math.h>

struct Rec {
    double xleft, xright;
    Rec(double l = INT_MAX, double r = INT_MIN) : xleft(l), xright(r) {}
};
typedef Rec Table[1000];

void InitTable(Table& T) {
    for (int i = 0; i < 1000; i++) { T[i].xleft = INT_MAX; T[i].xright = INT_MIN; }
}

void Edge2Table(POINT p1, POINT p2, Table& T) {
    if (p1.y == p2.y) return;
    if (p1.y > p2.y) { POINT tmp = p1; p1 = p2; p2 = tmp; }
    int y = p1.y;
    double x = p1.x;
    double mi = (double)(p2.x - p1.x) / (p2.y - p1.y);
    while (y < p2.y) {
        if (x < T[y].xleft) T[y].xleft = x;
        if (x > T[y].xright) T[y].xright = x;
        x += mi; y++;
    }
}

void Polygon2Table(POINT P[], int n, Table& t)
{
    POINT v1 = P[n - 1];
    for (int i = 0; i < n; i++)
    {
        POINT v2 = P[i];
        Edge2Table(v1, v2, t);
        v1 = v2;
    }
}

void Table2Screen(HDC hdc, Table& t, COLORREF c)
{
    for (int i = 0; i < 1000; i++)
    {
        if (t[i].xleft <= t[i].xright)
        {
            int left = (int)ceil(t[i].xleft);
            int right = (int)floor(t[i].xright);

            // Fill ALL pixels from left to right
            for (int x = left; x <= right; x++)
            {
                COLORREF pixelColor = GetPixel(hdc, x, i);
                if (pixelColor != RGB(0, 0, 0))
                {
                    SetPixel(hdc, x, i, c);
                }
            }
        }
    }
}

void PolyFill(HDC hdc, POINT P[], int n, COLORREF c)
{
    Table t;
    InitTable(t);
    Polygon2Table(P, n, t);
    Table2Screen(hdc, t, c);
}
