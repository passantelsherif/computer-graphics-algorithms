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

// ===== point clipping with circle
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


void CircleLineClipping(HDC hdc, int x1, int y1, int x2, int y2,
    const CircleClippingWindow& circle, COLORREF color)
{
    int cx = circle.centerX; 

    int cy = circle.centerY;
    int r = circle.radius;

    // Calculate distances
    double dist1 = DistanceFromPoint(x1, y1, cx, cy);
    double dist2 = DistanceFromPoint(x2, y2, cx, cy);

    bool p1_inside = (dist1 <= r);
    bool p2_inside = (dist2 <= r);

    cout << "[CIRCLE LINE] P1(" << x1 << "," << y1 << ") [" << (p1_inside ? "IN" : "OUT") << "]" << endl;
    cout << "[CIRCLE LINE] P2(" << x2 << "," << y2 << ") [" << (p2_inside ? "IN" : "OUT") << "]" << endl;

    //  Check every step on the line 

    // Calculate total steps (length of line)
    int dx_total = x2 - x1;
    int dy_total = y2 - y1;
    int max_steps = max(abs(dx_total), abs(dy_total));

    if (max_steps == 0) return;  // Same point

    // For every step along the line
    int first_inside = -1;  // Track when we first enter
    int last_inside = -1;   // Track when we last exit

    for (int step = 0; step <= max_steps; step++) {
        // Linear interpolation: get point at this step
        double t = (double)step / max_steps;
        int px = (int)(x1 + t * dx_total);
        int py = (int)(y1 + t * dy_total);

        // Check if this pixel is inside the circle
        double dist = DistanceFromPoint(px, py, cx, cy);
        bool inside = (dist <= r);

        if (inside) {
            if (first_inside == -1) {
                first_inside = step;  // Remember first entry
            }
            last_inside = step;  // Keep updating last inside point
        }
    }

    // ===== Now draw the clipped segment =====
    if (first_inside == -1) {
        // Never entered circle
        cout << "[CIRCLE LINE] Line doesn't intersect circle - skipping" << endl;
        return;
    }

    // Calculate start and end points of clipped segment
    double t_start = (double)first_inside / max_steps;
    double t_end = (double)last_inside / max_steps;

    int clipped_x1 = (int)(x1 + t_start * dx_total);
    int clipped_y1 = (int)(y1 + t_start * dy_total);
    int clipped_x2 = (int)(x1 + t_end * dx_total);
    int clipped_y2 = (int)(y1 + t_end * dy_total);

    cout << "[CIRCLE LINE] Drawing clipped segment from step " << first_inside << " to " << last_inside << endl;
    DrawClippedLine(hdc, clipped_x1, clipped_y1, clipped_x2, clipped_y2, color);
}