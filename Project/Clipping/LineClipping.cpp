#include "Clipping.h"
#include <iostream>
using namespace std;

// Get OutCode for a point relative to the clipping window
OutCode GetOutCode(double x, double y, const ClippingWindow& window)
{
    OutCode code = { 0 }; // start with 0000 assuming the point is inside the window

    if (x < window.left)
        code.left = 1; //point is left
    else if (x > window.right)
        code.right = 1; // point is right

    if (y < window.top)
        code.top = 1; //top
    else if (y > window.bottom)
        code.bottom = 1;//bottom

    return code;
}

// Vertical Edge Intersection: Find intersection with vertical edge at x = xedge
bool VIntersect(double x1, double y1, double x2, double y2,
    int xedge, double& x, double& y)
{
    double dx = x2 - x1;
    double dy = y2 - y1;

    if (dx == 0) return false; // Line is vertical, parallel to edge

    x = (double)xedge; //x is the edge of the window
    y = y1 + (dy / dx) * (xedge - x1); //the exact y coordinat where it intersect with xedge

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

    cout << "[LINE] P1 = (" << xs << ", " << ys << ")" << endl;
    cout << "[LINE] P2 = (" << xe << ", " << ye << ")" << endl;

    //to know which of the 9 regions they in
    OutCode out1 = GetOutCode(xs, ys, window);
    OutCode out2 = GetOutCode(xe, ye, window);

    bool accept = false;
    bool done = false;

    //loops until the line is rejected or sliced until it perfectly fits inside the window
    while (!done)
    {
        //the first 2 trivial cases
        // Both points inside
        if ((out1.left == 0 && out1.right == 0 && out1.top == 0 && out1.bottom == 0) &&
            (out2.left == 0 && out2.right == 0 && out2.top == 0 && out2.bottom == 0))
        {
            accept = true;
            done = true;
            cout << "Both points INSIDE - ACCEPT" << endl;
        }
        // Both points outside on same side
        else if ((out1.left && out2.left) || (out1.right && out2.right) ||
            (out1.top && out2.top) || (out1.bottom && out2.bottom))
        {
            done = true;
            cout << "Both points OUTSIDE same side - REJECT" << endl;
        }
        else
        {
            // At least one point is outside, need to clip it
            OutCode out;

            // pick the point that is outside the window to clip
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
                cout << "Clipping LEFT edge: x=" << x << " y=" << y <<endl;
            }
            else if (out.right)
            {
                VIntersect(xs, ys, xe, ye, window.right, x, y);
                cout << "Clipping RIGHT edge: x=" << x << " y=" << y <<endl;
            }
            else if (out.top)
            {
                HIntersect(xs, ys, xe, ye, window.top, x, y);
                cout << "Clipping TOP edge: x=" << x << " y=" << y <<endl;
            }
            else if (out.bottom)
            {
                HIntersect(xs, ys, xe, ye, window.bottom, x, y);
                cout << "Clipping BOTTOM edge: x=" << x << " y=" << y << endl;
            }

            // Replace the outside point with intersection point
            if (out == out1)
            {
                xs = x;
                ys = y;
                out1 = GetOutCode(xs, ys, window);
                cout << "Updated P1 to (" << xs << ", " << ys << ")" <<endl;
            }
            else
            {
                xe = x;
                ye = y;
                out2 = GetOutCode(xe, ye, window);
                cout << "Updated P2 to (" << xe << ", " << ye << ")" <<endl;
            }
        }
    }

    if (accept)
    {
        cout << "Drawing line from (" << xs << ", " << ys << ") to (" << xe << ", " << ye << ")" <<endl;
        DrawClippedLine(hdc, (int)xs, (int)ys, (int)xe, (int)ye, color);
    }

    cout << "[LINE] Done." << endl << endl;
}