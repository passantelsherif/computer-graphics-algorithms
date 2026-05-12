#include "Clipping.h"
#include <stack>

// Utility functions to test if vertex is on the "inside" of each edge

bool InLeft(Vertex v, const ClippingWindow& window)
{
    return v.x >= window.left;
}

bool InRight(Vertex v, const ClippingWindow& window)
{
    return v.x <= window.right;
}

bool InTop(Vertex v, const ClippingWindow& window)
{
    return v.y >= window.top;
}

bool InBottom(Vertex v, const ClippingWindow& window)
{
    return v.y <= window.bottom;
}



Vertex VIntersectPolygon(Vertex v1, Vertex v2, int edge, const ClippingWindow& window) {
    double dx = v2.x - v1.x;
    double dy = v2.y - v1.y;

    //  Check for edge == 0 (left), otherwise it's right
    double xedge = (edge == 0) ? window.left : window.right;

    double x = xedge;
    double y = v1.y + (dy / dx) * (xedge - v1.x);

    return Vertex(x, y);
}

Vertex HIntersectPolygon(Vertex v1, Vertex v2, int edge, const ClippingWindow& window) {
    double dx = v2.x - v1.x;
    double dy = v2.y - v1.y;

    //  Check for edge == 2 (top), otherwise it's bottom
    double yedge = (edge == 2) ? window.top : window.bottom;

    double y = yedge;
    double x = v1.x + (dx / dy) * (yedge - v1.y);

    return Vertex(x, y);
}

// Sutherland-Hodgman Polygon Clipping Algorithm
void SutherlandHodgmanClipping(HDC hdc, std::vector<Vertex>& polygon,
    const ClippingWindow& window, COLORREF color)
{
    std::vector<Vertex> inputList = polygon;
    std::vector<Vertex> outputList;

    // Clip against all 4 edges of the window
    // Edge 0 = left, Edge 1 = right, Edge 2 = top, Edge 3 = bottom

    for (int edge = 0; edge < 4; edge++)
    {
        if (inputList.empty()) break;

        outputList.clear();
        Vertex prevVertex = inputList.back();

        for (const auto& currVertex : inputList)
        {
            bool currInside = false;
            bool prevInside = false;

            // Check if vertices are inside relative to current edge
            if (edge == 0) // Left edge
            {
                currInside = InLeft(currVertex, window);
                prevInside = InLeft(prevVertex, window);
            }
            else if (edge == 1) // Right edge
            {
                currInside = InRight(currVertex, window);
                prevInside = InRight(prevVertex, window);
            }
            else if (edge == 2) // Top edge
            {
                currInside = InTop(currVertex, window);
                prevInside = InTop(prevVertex, window);
            }
            else if (edge == 3) // Bottom edge
            {
                currInside = InBottom(currVertex, window);
                prevInside = InBottom(prevVertex, window);
            }

            // Four cases for edge crossing
            if (currInside)
            {
                if (!prevInside)
                {
                    // Entering the inside region, add intersection point
                    Vertex intersection;
                    if (edge == 0 || edge == 1)
                        intersection = VIntersectPolygon(prevVertex, currVertex, edge, window);
                    else
                        intersection = HIntersectPolygon(prevVertex, currVertex, edge, window);

                    outputList.push_back(intersection);
                }
                // Add current vertex
                outputList.push_back(currVertex);
            }
            else if (prevInside)
            {
                // Leaving the inside region, add intersection point
                Vertex intersection;
                if (edge == 0 || edge == 1)
                    intersection = VIntersectPolygon(prevVertex, currVertex, edge, window);
                else
                    intersection = HIntersectPolygon(prevVertex, currVertex, edge, window);

                outputList.push_back(intersection);
            }

            prevVertex = currVertex;
        }

        inputList = outputList;
    }

    // Draw the clipped polygon
    if (!inputList.empty())
    {
        DrawClippedPolygon(hdc, inputList, color);
    }
}