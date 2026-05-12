#include "Clipping.h"
#include <cmath>
#include <iostream>

using namespace std;

// Helper: Distance from point to center
double DistanceFromPoint(int x, int y, int cx, int cy)
{
    int dx = x - cx;
    int dy = y - cy;
    return sqrt((double)(dx * dx + dy * dy));
}

// ===== POINT CLIPPING WITH CIRCLE =====
void PointClippingCircle(int x, int y, const CircleClippingWindow& circle,
    HDC hdc, COLORREF color)
{
    double dist = DistanceFromPoint(x, y, circle.centerX, circle.centerY);

    if (dist <= circle.radius)
    {
        SetPixel(hdc, x, y, color);
        cout << "[CIRCLE POINT] (" << x << ", " << y << ") INSIDE (dist=" << dist << ")" << endl;
    }
    else
    {
        cout << "[CIRCLE POINT] (" << x << ", " << y << ") OUTSIDE (dist=" << dist << ")" << endl;
    }
}

// ===== LINE CLIPPING WITH CIRCLE =====
bool FindLineCircleIntersections(int x1, int y1, int x2, int y2,
    const CircleClippingWindow& circle,
    double& t1, double& t2)
{
    // Parametric line: P(t) = P1 + t*(P2-P1), t in [0,1]
    double dx = x2 - x1;
    double dy = y2 - y1;

    // Vector from circle center to start point
    double fx = x1 - circle.centerX;
    double fy = y1 - circle.centerY;

    // Quadratic: at² + bt + c = 0
    double a = dx * dx + dy * dy;
    double b = 2.0 * (fx * dx + fy * dy);
    double c = fx * fx + fy * fy - circle.radius * circle.radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false; // No intersection

    double sqrtDisc = sqrt(discriminant);
    t1 = (-b - sqrtDisc) / (2.0 * a);
    t2 = (-b + sqrtDisc) / (2.0 * a);

    return true;
}

void CircleLineClipping(HDC hdc, int x1, int y1, int x2, int y2,
    const CircleClippingWindow& circle, COLORREF color)
{
    double dist1 = DistanceFromPoint(x1, y1, circle.centerX, circle.centerY);
    double dist2 = DistanceFromPoint(x2, y2, circle.centerX, circle.centerY);

    bool p1_inside = (dist1 <= circle.radius);
    bool p2_inside = (dist2 <= circle.radius);

    cout << "[CIRCLE LINE] P1(" << x1 << "," << y1 << ") dist=" << dist1
        << " [" << (p1_inside ? "IN" : "OUT") << "]" << endl;
    cout << "[CIRCLE LINE] P2(" << x2 << "," << y2 << ") dist=" << dist2
        << " [" << (p2_inside ? "IN" : "OUT") << "]" << endl;

    // Case 1: Both inside
    if (p1_inside && p2_inside)
    {
        cout << "[CIRCLE LINE] Both inside - draw entire line" << endl;
        DrawClippedLine(hdc, x1, y1, x2, y2, color);
        return;
    }

    // Case 2: Both outside
    if (!p1_inside && !p2_inside)
    {
        double t1, t2;
        if (!FindLineCircleIntersections(x1, y1, x2, y2, circle, t1, t2))
        {
            cout << "[CIRCLE LINE] Both outside - no intersection" << endl;
            return;
        }

        t1 = max(0.0, min(1.0, t1));
        t2 = max(0.0, min(1.0, t2));

        if (t1 > t2) swap(t1, t2);

        cout << "[CIRCLE LINE] Both outside - draw segment [" << t1 << ", " << t2 << "]" << endl;

        double x1_clip = x1 + t1 * (x2 - x1);
        double y1_clip = y1 + t1 * (y2 - y1);
        double x2_clip = x1 + t2 * (x2 - x1);
        double y2_clip = y1 + t2 * (y2 - y1);

        DrawClippedLine(hdc, (int)x1_clip, (int)y1_clip, (int)x2_clip, (int)y2_clip, color);
        return;
    }

    // Case 3: One inside, one outside
    double t1, t2;
    if (!FindLineCircleIntersections(x1, y1, x2, y2, circle, t1, t2))
    {
        cout << "[CIRCLE LINE] Error: no intersection found" << endl;
        return;
    }

    t1 = max(0.0, min(1.0, t1));
    t2 = max(0.0, min(1.0, t2));

    cout << "[CIRCLE LINE] One in/one out - clipping" << endl;

    if (p1_inside)
    {
        double x_exit = x1 + t2 * (x2 - x1);
        double y_exit = y1 + t2 * (y2 - y1);
        DrawClippedLine(hdc, x1, y1, (int)x_exit, (int)y_exit, color);
    }
    else
    {
        double x_entry = x1 + t1 * (x2 - x1);
        double y_entry = y1 + t1 * (y2 - y1);
        DrawClippedLine(hdc, (int)x_entry, (int)y_entry, x2, y2, color);
    }
}