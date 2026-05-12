#include "Curve.h"

void DrawHermiteCurve(HDC hdc, double x1, double y1, double u1, double v1, double x2, double y2, double u2, double v2, int numPoints, COLORREF c)
{
    double H[4][4] = {
        {1,   0,   0,   0},
        {0,   1,   0,   0},
        {-3, -2,   3,  -1},
        {2,   1,  -2,   1}
    };

    double G[4][2] = {
        {x1, y1},
        {u1, v1},
        {x2, y2},
        {u2, v2}
    };

    double C[4][2] = { 0 };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 4; k++) {
                C[i][j] += H[i][k] * G[k][j];
            }
        }
    }

    double step = 1.0 / (numPoints - 1);

    for (double t = 0; t <= 1.0; t += step) {
        double v[4] = { 1, t, t * t, t * t * t };
        double x = 0, y = 0;

        for (int j = 0; j < 4; j++) {
            x += v[j] * C[j][0];
            y += v[j] * C[j][1];
        }

        SetPixel(hdc, (int)round(x), (int)round(y), c);
    }
}

void DrawBezier(HDC hdc, double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4, COLORREF c)
{
    for (double t = 0.0; t <= 1.0; t += 0.001)
    {
        double drawX = pow(1 - t, 3) * x1 + 3 * t * pow(1 - t, 2) * x2
            + 3 * t * t * (1 - t) * x3 + t * t * t * x4;
        double drawY = pow(1 - t, 3) * y1 + 3 * t * pow(1 - t, 2) * y2
            + 3 * t * t * (1 - t) * y3 + t * t * t * y4;
        SetPixel(hdc, (int)round(drawX), (int)round(drawY), c);
    }
}

void DrawCardinalSpline(HDC hdc, vector<POINT> points, double c, COLORREF col)
{
    int n = (int)points.size();
    if (n < 4) return;

    vector<POINT> T(n);
    for (int i = 1; i < n - 1; i++)
    {
        T[i].x = (int)(c * (points[i + 1].x - points[i - 1].x));
        T[i].y = (int)(c * (points[i + 1].y - points[i - 1].y));
    }

    T[0] = T[1];
    T[n-1] = T[n - 2];

    for (int i = 0; i < n - 1; i++)
    {
        DrawHermiteCurve(hdc, points[i].x, points[i].y, T[i].x, T[i].y,
            points[i + 1].x, points[i + 1].y,
            T[i + 1].x, T[i + 1].y,
            500, col);
    }
}