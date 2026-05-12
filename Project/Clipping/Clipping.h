#pragma once
#include <Windows.h>
#include <vector>

// Data structures
struct Vertex {
    double x, y;
    Vertex(double x = 0, double y = 0) : x(x), y(y) {}
};

struct ClippingWindow {
    int left, top, right, bottom;
};

// OutCode for Cohen-Sutherland
union OutCode {
    unsigned All : 4;
    struct {
        unsigned left : 1;
        unsigned top : 1;
        unsigned right : 1;
        unsigned bottom : 1;
    };

    // for comparison
    bool operator==(const OutCode& other) const {
        return left == other.left &&
            right == other.right &&
            top == other.top &&
            bottom == other.bottom;
    }
};

// =========== point clipping
void PointClippingRect(int x, int y, const ClippingWindow& window, HDC hdc, COLORREF color);
void PointClippingSquare(int x, int y, const ClippingWindow& window, HDC hdc, COLORREF color);

// =========== Line clipping
OutCode GetOutCode(double x, double y, const ClippingWindow& window);


bool VIntersect(double x1, double y1, double x2, double y2, int xedge, double& x, double& y);
bool HIntersect(double x1, double y1, double x2, double y2, int yedge, double& x, double& y);

void CohenSutherlandLineClipping(HDC hdc, int x1, int y1, int x2, int y2, const ClippingWindow& window, COLORREF color);

// =============== polygon clipping

typedef bool (*IsInFunc)(Vertex v, const ClippingWindow& window);
typedef Vertex(*IntersectFunc)(Vertex v1, Vertex v2, int edge, const ClippingWindow& window);

bool InLeft(Vertex v, const ClippingWindow& window);
bool InRight(Vertex v, const ClippingWindow& window);
bool InTop(Vertex v, const ClippingWindow& window);
bool InBottom(Vertex v, const ClippingWindow& window);

Vertex VIntersectPolygon(Vertex v1, Vertex v2, int edge, const ClippingWindow& window);
Vertex HIntersectPolygon(Vertex v1, Vertex v2, int edge, const ClippingWindow& window);

std::vector<Vertex> ClipWithEdge(std::vector<Vertex> inputList, int edge,
    IsInFunc InFunc, IntersectFunc IntersectFunc,
    const ClippingWindow& window);

void SutherlandHodgmanClipping(HDC hdc, std::vector<Vertex>& polygon, const ClippingWindow& window, COLORREF color);

// ===== utility drawing
void DrawClippedLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);
void DrawClippedPoint(HDC hdc, int x, int y, COLORREF color);
void DrawClippedPolygon(HDC hdc, const std::vector<Vertex>& polygon, COLORREF color);
void DrawWindow(HDC hdc, const ClippingWindow& window, COLORREF color);

//bonus
struct CircleClippingWindow {
    int centerX, centerY, radius;
};

void PointClippingCircle(int x, int y, const CircleClippingWindow& circle,
    HDC hdc, COLORREF color);

void CircleLineClipping(HDC hdc, int x1, int y1, int x2, int y2,
    const CircleClippingWindow& circle, COLORREF color);

double DistanceFromPoint(int x, int y, int cx, int cy);

bool FindLineCircleIntersections(int x1, int y1, int x2, int y2,
    const CircleClippingWindow& circle,
    double& t1, double& t2);

