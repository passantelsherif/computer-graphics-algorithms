#include "Clipping.h"
#include <iostream>

// Get OutCode for a point relative to the clipping window
OutCode GetOutCode(double x, double y, const ClippingWindow& window)
{
    OutCode code = { 0 };

    if (x < window.left)
        code.left = 1;
    else if (x > window.right)
        code.right = 1;

    if (y < window.top)
        code.top = 1;
    else if (y > window.bottom)
        code.bottom = 1;

    return code;
}

// Vertical Edge Intersection: Find intersection with vertical edge at x = xedge
bool VIntersect(double x1, double y1, double x2, double y2,
    int xedge, double& x, double& y)
{
    double dx = x2 - x1;
    double dy = y2 - y1;

    if (dx == 0) return false; // Line is vertical, parallel to edge

    x = (double)xedge;
    y = y1 + (dy / dx) * (xedge - x1);

    return true;
}

// Horizontal Edge Intersection: Find intersection with horizontal edge at y = yedge
bool HIntersect(double x1, double y1, double x2, double y2,
    int yedge, double& x, double& y)
{
    double dx = x2 - x1;
    double dy = y2 - y1;

    if (dy == 0) return false; // Line is horizontal, parallel to edge

    y = (double)yedge;
    x = x1 + (dx / dy) * (yedge - y1);

    return true;
}

// Cohen-Sutherland Line Clipping Algorithm
void CohenSutherlandLineClipping(HDC hdc, int x1, int y1, int x2, int y2,
    const ClippingWindow& window, COLORREF color)
{
    double xs = x1, ys = y1, xe = x2, ye = y2;

    std::cout << "[LINE] P1 = (" << xs << ", " << ys << ")" << std::endl;
    std::cout << "[LINE] P2 = (" << xe << ", " << ye << ")" << std::endl;

    OutCode out1 = GetOutCode(xs, ys, window);
    OutCode out2 = GetOutCode(xe, ye, window);

    bool accept = false;
    bool done = false;

    while (!done)
    {
        // Both points inside
        if ((out1.left == 0 && out1.right == 0 && out1.top == 0 && out1.bottom == 0) &&
            (out2.left == 0 && out2.right == 0 && out2.top == 0 && out2.bottom == 0))
        {
            accept = true;
            done = true;
            std::cout << "Both points INSIDE - ACCEPT" << std::endl;
        }
        // Both points outside on same side
        else if ((out1.left && out2.left) || (out1.right && out2.right) ||
            (out1.top && out2.top) || (out1.bottom && out2.bottom))
        {
            done = true;
            std::cout << "Both points OUTSIDE same side - REJECT" << std::endl;
        }
        else
        {
            // At least one point is outside, need to clip it
            OutCode out;

            // Choose which point to clip (prefer the outside one)
            if (out1.left || out1.right || out1.top || out1.bottom)
            {
                out = out1;
            }
            else
            {
                out = out2;
            }

            double x = 0, y = 0;

            // Find intersection point with appropriate window edge
            if (out.left)
            {
                VIntersect(xs, ys, xe, ye, window.left, x, y);
                std::cout << "Clipping LEFT edge: x=" << x << " y=" << y << std::endl;
            }
            else if (out.right)
            {
                VIntersect(xs, ys, xe, ye, window.right, x, y);
                std::cout << "Clipping RIGHT edge: x=" << x << " y=" << y << std::endl;
            }
            else if (out.top)
            {
                HIntersect(xs, ys, xe, ye, window.top, x, y);
                std::cout << "Clipping TOP edge: x=" << x << " y=" << y << std::endl;
            }
            else if (out.bottom)
            {
                HIntersect(xs, ys, xe, ye, window.bottom, x, y);
                std::cout << "Clipping BOTTOM edge: x=" << x << " y=" << y << std::endl;
            }

            // Replace the outside point with intersection point
            if (out == out1)
            {
                xs = x;
                ys = y;
                out1 = GetOutCode(xs, ys, window);
                std::cout << "Updated P1 to (" << xs << ", " << ys << ")" << std::endl;
            }
            else
            {
                xe = x;
                ye = y;
                out2 = GetOutCode(xe, ye, window);
                std::cout << "Updated P2 to (" << xe << ", " << ye << ")" << std::endl;
            }
        }
    }

    if (accept)
    {
        std::cout << "Drawing line from (" << xs << ", " << ys << ") to (" << xe << ", " << ye << ")" << std::endl;
        DrawClippedLine(hdc, (int)xs, (int)ys, (int)xe, (int)ye, color);
    }

    std::cout << "[LINE] Done." << std::endl << std::endl;
}