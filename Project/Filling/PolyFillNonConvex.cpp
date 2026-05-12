#include "Fill.h"
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

class nonTable {
public:
    vector<double> intersections[1000];
    void clear() { for (int i = 0; i < 1000; i++) intersections[i].clear(); }
};

void Edge2TableNon(POINT p1, POINT p2, nonTable& T) {
    if (p1.y == p2.y) return;
    if (p1.y > p2.y) { POINT tmp = p1; p1 = p2; p2 = tmp; }
    int y = p1.y, y2 = p2.y;
    double x = p1.x, mi = (double)(p2.x - p1.x) / (p2.y - p1.y);
    while (y < y2) { T.intersections[y].push_back(x); x += mi; y++; }
}

void Polygon2Table(POINT P[], int n, nonTable& T)
{
    POINT v1 = P[n - 1];
    for (int i = 0; i < n; i++)
    {
        POINT v2 = P[i];
        Edge2TableNon(v1, v2, T);
        v1 = v2;
    }
}

void Table2Screen(HDC hdc, nonTable& T, COLORREF c)
{
    for (int y = 0; y < 1000; y++)
    {
        if (!T.intersections[y].empty())
        {
            // Sort intersections from left to right
            sort(T.intersections[y].begin(), T.intersections[y].end());

            // Pair them up 0-1, 2-3, 4-5,
            for (size_t i = 0; i + 1 < T.intersections[y].size(); i += 2)
            {
                int left = (int)ceil(T.intersections[y][i]);
                int right = (int)floor(T.intersections[y][i + 1]);

                // Fill between pairs
                for (int x = left; x <= right; x++)
                {
                    COLORREF pixelColor = GetPixel(hdc, x, y);
                    if (pixelColor != RGB(0, 0, 0))
                    {
                        SetPixel(hdc, x, y, c);
                    }
                }
            }
        }
    }
}

void nonPolyFill(HDC hdc, POINT P[], int n, COLORREF c)
{
    nonTable t;
    t.clear();
    Polygon2Table(P, n, t);
    Table2Screen(hdc, t, c);
}