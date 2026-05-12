#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <math.h>
#include <commdlg.h>
#include <algorithm>

using namespace std;

// --- CUSTOM MODULES ---
#include "Clipping/Clipping.h"
#include "Filling/Fill.h"
#include "Shapes/Lines.h"
#include "Shapes/Circle.h"
#include "Shapes/Ellipse.h"
#include "Shapes/Curve.h"


// ============ Menu command IDs =============
#define FILE_CLEAR       1001
#define FILE_SAVE        1002
#define FILE_LOAD        1003
#define PREF_BGWHITE     2001
#define PREF_CURSORSHAPE 2002
#define PREF_CHOOSECOLOR 2003
#define LINE_DDA          3001
#define LINE_MIDPOINT     3002
#define LINE_PARAMETRIC   3003
#define CIRCLE_DIRECT     4001
#define CIRCLE_POLAR      4002
#define CIRCLE_ITERPOLAR  4003
#define CIRCLE_MIDPOINT   4004
#define CIRCLE_MODMID     4005
#define ELLIPSE_DIRECT    5001
#define ELLIPSE_POLAR     5002
#define ELLIPSE_MIDPOINT  5003
#define CARDINAL_SPLINE   6001
#define FILL_CIRCLELINES   7001
#define FILL_CIRCLECIRCLES 7002
#define FILL_SQHERMIT      7003
#define FILL_RECTBEZIER    7004
#define FILL_CONVEX        7005
#define FILL_NON_CONVEX    7006
#define FILL_FLOOD         7007
#define FILL_FLOOD_REC     7008
#define CLIP_RECTPOINT     8001
#define CLIP_RECTLINE      8002
#define CLIP_RECTPOLYGON   8003
#define CLIP_SQPOINT       8004
#define CLIP_SQLINE        8005
#define CLIP_CIRCLEPOINT   8006
#define CLIP_CIRCLELINE    8007

// ============== Data Structures ==================
enum ShapeType { SHAPE_LINE, SHAPE_CIRCLE, SHAPE_ELLIPSE, SHAPE_CURVE };

struct Color {
    int r, g, b;
    Color() : r(0), g(0), b(0) {}
    Color(int red, int green, int blue) : r(red), g(green), b(blue) {}
};

struct ShapeData {
    ShapeType type;
    Color color;
    int x1, y1, x2, y2;
    int hRadius, vRadius;
    int pointsCount;
    vector<POINT> curvePoints;
};

// ================= Globals ==================
vector<ShapeData> shapes;
Color currColor(0, 0, 0);
int currCommand = 0;

ClippingWindow rectWindow = { 150, 100, 650, 500 };
ClippingWindow squareWindow = { 200, 100, 600, 500 };
CircleClippingWindow circleWindow = { 400, 300, 150 };  // center(400,300), radius 150
vector<Vertex> clipPolygonPoints;

// ================ Helper functions ================
bool IsInsideCircle(int px, int py, int cx, int cy, int r) {
    int dx = px - cx, dy = py - cy;
    return (dx * dx + dy * dy) <= (r * r);
}

bool IsInsideEllipse(int px, int py, int cx, int cy, int a, int b) {
    double dx = px - cx, dy = py - cy;
    return (dx * dx) / (a * a) + (dy * dy) / (b * b) <= 1.0;
}

// ================ File Operations ==================
void ClearScreen(HWND hwnd) {
    shapes.clear();
    clipPolygonPoints.clear();
    InvalidateRect(hwnd, NULL, TRUE);
    printf("Screen cleared.\n");
}

string OpenFileDialog(HWND hwnd, bool save) {
    char filename[MAX_PATH] = "";
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Drawing Files (*.drw)\0*.drw\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrDefExt = "drw";

    if (save) {
        ofn.Flags = OFN_OVERWRITEPROMPT;
        if (GetSaveFileNameA(&ofn)) return string(filename);
    }
    else {
        ofn.Flags = OFN_FILEMUSTEXIST;
        if (GetOpenFileNameA(&ofn)) return string(filename);
    }
    return "";
}

bool SaveToFile(HWND hwnd) {
    string filepath = OpenFileDialog(hwnd, true);
    if (filepath.empty()) return false;

    ofstream file(filepath);
    if (!file.is_open()) return false;

    file << shapes.size() << "\n";
    for (const ShapeData& s : shapes) {
        file << s.type << " " << s.color.r << " " << s.color.g << " " << s.color.b << " "
            << s.x1 << " " << s.y1 << " " << s.x2 << " " << s.y2 << " "
            << s.hRadius << " " << s.vRadius << " " << s.pointsCount;

        if (s.type == SHAPE_CURVE) {
            file << " " << s.curvePoints.size();
            for (const POINT& pt : s.curvePoints) file << " " << pt.x << " " << pt.y;
        }
        file << "\n";
    }
    file.close();
    printf("Saved %d shapes\n", (int)shapes.size());
    return true;
}

bool LoadFromFile(HWND hwnd) {
    string filepath = OpenFileDialog(hwnd, false);
    if (filepath.empty()) return false;

    ifstream file(filepath);
    if (!file.is_open()) return false;

    shapes.clear();
    int count;
    file >> count;

    for (int i = 0; i < count; i++) {
        ShapeData s;
        int typeInt;
        file >> typeInt;
        s.type = (ShapeType)typeInt;
        file >> s.color.r >> s.color.g >> s.color.b;
        file >> s.x1 >> s.y1 >> s.x2 >> s.y2;
        file >> s.hRadius >> s.vRadius >> s.pointsCount;

        if (s.type == SHAPE_CURVE && s.pointsCount > 0) {
            int ptCount;
            file >> ptCount;
            for (int j = 0; j < ptCount; j++) {
                POINT pt;
                file >> pt.x >> pt.y;
                s.curvePoints.push_back(pt);
            }
        }
        shapes.push_back(s);
    }
    file.close();
    printf("Loaded %d shapes\n", count);
    InvalidateRect(hwnd, NULL, TRUE);
    return true;
}

// ================= Preferences Operations ===================
void ChangeBGWhite(HWND hwnd) {
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)GetStockObject(WHITE_BRUSH));
    InvalidateRect(hwnd, NULL, TRUE);
    printf("Background changed to white.\n");
}

void ChangeCursorShape(HWND hwnd) {
    static int cursorIndex = 0;
    LPCSTR cursorNames[] = { (LPCSTR)IDC_CROSS, (LPCSTR)IDC_HAND, (LPCSTR)IDC_HELP, (LPCSTR)IDC_IBEAM, (LPCSTR)IDC_ARROW };
    const int numCursors = 5;
    HCURSOR cursor = LoadCursorA(NULL, cursorNames[cursorIndex]);
    SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)cursor);
    cursorIndex = (cursorIndex + 1) % numCursors;
}

void ChooseColorRGB(HWND hwnd) {
    CHOOSECOLORA cc;
    static COLORREF acrCustClr[16];
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = RGB(currColor.r, currColor.g, currColor.b);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColorA(&cc)) {
        currColor.r = GetRValue(cc.rgbResult);
        currColor.g = GetGValue(cc.rgbResult);
        currColor.b = GetBValue(cc.rgbResult);
        printf("Color changed to RGB(%d, %d, %d).\n", currColor.r, currColor.g, currColor.b);
    }
}

// ================== WINDOW PROCEDURE ==================
LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    switch (mcode)
    {
    case WM_CREATE:
    {
        AllocConsole();
        FILE* fOut;
        freopen_s(&fOut, "CONOUT$", "w", stdout);
        FILE* fIN;
        freopen_s(&fIN, "CONIN$", "r", stdin);
        printf("=== 2D Drawing Package ===\n");

        HMENU hMenuBar = CreateMenu();

        HMENU hFileMenu = CreatePopupMenu();
        AppendMenu(hFileMenu, MF_STRING, FILE_CLEAR, L"Clear Screen");
        AppendMenu(hFileMenu, MF_STRING, FILE_SAVE, L"Save");
        AppendMenu(hFileMenu, MF_STRING, FILE_LOAD, L"Load");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"File");

        HMENU hPrefMenu = CreatePopupMenu();
        AppendMenu(hPrefMenu, MF_STRING, PREF_BGWHITE, L"White Background");
        AppendMenu(hPrefMenu, MF_STRING, PREF_CURSORSHAPE, L"Change Cursor Shape");
        AppendMenu(hPrefMenu, MF_STRING, PREF_CHOOSECOLOR, L"Choose Shape Color");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hPrefMenu, L"Preferences");

        HMENU hLinesMenu = CreatePopupMenu();
        AppendMenu(hLinesMenu, MF_STRING, LINE_DDA, L"DDA");
        AppendMenu(hLinesMenu, MF_STRING, LINE_MIDPOINT, L"Midpoint");
        AppendMenu(hLinesMenu, MF_STRING, LINE_PARAMETRIC, L"Parametric");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hLinesMenu, L"Lines");

        HMENU hCirclesMenu = CreatePopupMenu();
        AppendMenu(hCirclesMenu, MF_STRING, CIRCLE_DIRECT, L"Direct");
        AppendMenu(hCirclesMenu, MF_STRING, CIRCLE_POLAR, L"Polar");
        AppendMenu(hCirclesMenu, MF_STRING, CIRCLE_ITERPOLAR, L"Iterative Polar");
        AppendMenu(hCirclesMenu, MF_STRING, CIRCLE_MIDPOINT, L"Midpoint");
        AppendMenu(hCirclesMenu, MF_STRING, CIRCLE_MODMID, L"Modified Midpoint");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hCirclesMenu, L"Circles");

        HMENU hEllipseMenu = CreatePopupMenu();
        AppendMenu(hEllipseMenu, MF_STRING, ELLIPSE_DIRECT, L"Direct");
        AppendMenu(hEllipseMenu, MF_STRING, ELLIPSE_POLAR, L"Polar");
        AppendMenu(hEllipseMenu, MF_STRING, ELLIPSE_MIDPOINT, L"Midpoint");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hEllipseMenu, L"Ellipse");

        HMENU hCurvesMenu = CreatePopupMenu();
        AppendMenu(hCurvesMenu, MF_STRING, CARDINAL_SPLINE, L"Cardinal Spline Curve");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hCurvesMenu, L"Curves");

        HMENU hFillingMenu = CreatePopupMenu();
        AppendMenu(hFillingMenu, MF_STRING, FILL_CIRCLELINES, L"Fill Circle with Lines");
        AppendMenu(hFillingMenu, MF_STRING, FILL_CIRCLECIRCLES, L"Fill Circle with Circles");
        AppendMenu(hFillingMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hFillingMenu, MF_STRING, FILL_SQHERMIT, L"Fill Square - Hermit (Vertical)");
        AppendMenu(hFillingMenu, MF_STRING, FILL_RECTBEZIER, L"Fill Rectangle - Bezier (Horizontal)");
        AppendMenu(hFillingMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hFillingMenu, MF_STRING, FILL_CONVEX, L"Convex Filling");
        AppendMenu(hFillingMenu, MF_STRING, FILL_NON_CONVEX, L"Non-Convex Filling");
        AppendMenu(hFillingMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hFillingMenu, MF_STRING, FILL_FLOOD_REC, L"Flood Fill (Recursive)");
        AppendMenu(hFillingMenu, MF_STRING, FILL_FLOOD, L"Flood Fill (Non-Recursive)");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFillingMenu, L"Filling");

        HMENU hClippingMenu = CreatePopupMenu();
        AppendMenu(hClippingMenu, MF_STRING, CLIP_RECTPOINT, L"Rectangle Window - Point");
        AppendMenu(hClippingMenu, MF_STRING, CLIP_RECTLINE, L"Rectangle Window - Line");
        AppendMenu(hClippingMenu, MF_STRING, CLIP_RECTPOLYGON, L"Rectangle Window - Polygon");
        AppendMenu(hClippingMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hClippingMenu, MF_STRING, CLIP_SQPOINT, L"Square Window - Point");
        AppendMenu(hClippingMenu, MF_STRING, CLIP_SQLINE, L"Square Window - Line");
        AppendMenu(hClippingMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hClippingMenu, MF_STRING, CLIP_CIRCLEPOINT, L"Circle Window - Point");
        AppendMenu(hClippingMenu, MF_STRING, CLIP_CIRCLELINE, L"Circle Window - Line");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hClippingMenu, L"Clipping");

        SetMenu(hwnd, hMenuBar);
        break;
    }

    case WM_COMMAND:
        currCommand = LOWORD(wp);
        if (currCommand == FILE_CLEAR) ClearScreen(hwnd);
        else if (currCommand == FILE_SAVE) SaveToFile(hwnd);
        else if (currCommand == FILE_LOAD) LoadFromFile(hwnd);
        else if (currCommand == PREF_BGWHITE) ChangeBGWhite(hwnd);
        else if (currCommand == PREF_CURSORSHAPE) ChangeCursorShape(hwnd);
        else if (currCommand == PREF_CHOOSECOLOR) ChooseColorRGB(hwnd);

        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lp);
        int y = HIWORD(lp);
        COLORREF col = RGB(currColor.r, currColor.g, currColor.b);
        hdc = GetDC(hwnd);

        // --- LINES ---
        static int lineClicks = 0;
        static int lx1, ly1, lx2, ly2;
        if (currCommand == LINE_DDA || currCommand == LINE_MIDPOINT || currCommand == LINE_PARAMETRIC) {
            lineClicks++;
            if (lineClicks == 1) { lx1 = x; ly1 = y; printf("Start point: (%d, %d). Click end point.\n", lx1, ly1); }
            else {
                lx2 = x; ly2 = y;
                if (currCommand == LINE_DDA)             DrawLineDDA(hdc, lx1, ly1, lx2, ly2, col);
                else if (currCommand == LINE_MIDPOINT)   bresenhamLine(hdc, lx1, ly1, lx2, ly2, col);
                else if (currCommand == LINE_PARAMETRIC) DrawLineParametric(hdc, lx1, ly1, lx2, ly2, col);

                ShapeData shape; shape.type = SHAPE_LINE; shape.color = currColor;
                shape.x1 = lx1; shape.y1 = ly1; shape.x2 = lx2; shape.y2 = ly2;
                shapes.push_back(shape);
                lineClicks = 0;
            }
        }

        // --- CIRCLES ---
        static int circleclicks = 0;
        static int cx, cy;
        if (currCommand >= CIRCLE_DIRECT && currCommand <= CIRCLE_MODMID) {
            circleclicks++;
            if (circleclicks == 1) { cx = x; cy = y; printf("Center: (%d, %d). Click radius point.\n", cx, cy); }
            else {
                int r = (int)sqrt((double)((x - cx) * (x - cx) + (y - cy) * (y - cy)));
                switch (currCommand) {
                case CIRCLE_DIRECT:    DrawCircleDirect(hdc, cx, cy, r, col); break;
                case CIRCLE_POLAR:     DrawCirclePolar(hdc, cx, cy, r, col);  break;
                case CIRCLE_ITERPOLAR: DrawCircleIterPolar(hdc, cx, cy, r, col); break;
                case CIRCLE_MIDPOINT:  DrawCircleMidpoint(hdc, cx, cy, r, col); break;
                case CIRCLE_MODMID:    DrawCircleMod(hdc, cx, cy, r, col); break;
                }
                ShapeData shape; shape.type = SHAPE_CIRCLE; shape.color = currColor;
                shape.x1 = cx; shape.y1 = cy; shape.hRadius = r;
                shapes.push_back(shape);
                circleclicks = 0;
            }
        }

        // --- ELLIPSES ---
        static int ellipseClick = 0;
        static int ex, ey, ea, eb;
        if (currCommand >= ELLIPSE_DIRECT && currCommand <= ELLIPSE_MIDPOINT) {
            ellipseClick++;
            if (ellipseClick == 1) { ex = x; ey = y; printf("Center: (%d, %d). Click horizontal.\n", ex, ey); }
            else if (ellipseClick == 2) { ea = (int)sqrt(pow(x - ex, 2) + pow(y - ey, 2)); printf("a=%d. Click vertical.\n", ea); }
            else {
                eb = (int)sqrt(pow(x - ex, 2) + pow(y - ey, 2));
                if (currCommand == ELLIPSE_DIRECT)   DrawEllipseDirect(hdc, ex, ey, ea, eb, col);
                if (currCommand == ELLIPSE_POLAR)    DrawEllipsePolar(hdc, ex, ey, ea, eb, col);
                if (currCommand == ELLIPSE_MIDPOINT) DrawEllipseMidpoint(hdc, ex, ey, ea, eb, col);

                ShapeData shape; shape.type = SHAPE_ELLIPSE; shape.color = currColor;
                shape.x1 = ex; shape.y1 = ey; shape.hRadius = ea; shape.vRadius = eb;
                shapes.push_back(shape);
                ellipseClick = 0;
            }
        }

        // --- CURVES ---
        static vector<POINT> splinePoints;
        if (currCommand == CARDINAL_SPLINE) {
            splinePoints.push_back({ x, y });
            if (splinePoints.size() == 4) {
                DrawCardinalSpline(hdc, splinePoints, 0.5, col);
                ShapeData shape; shape.type = SHAPE_CURVE; shape.color = currColor;
                shape.pointsCount = 4; shape.curvePoints = splinePoints;
                shapes.push_back(shape);
                splinePoints.clear();
            }
        }

        // --- FILLING ---
        if (currCommand == FILL_FLOOD_REC || currCommand == FILL_FLOOD) {
            bool inside = false;
            for (const ShapeData& s : shapes) {
                if (s.type == SHAPE_CIRCLE && IsInsideCircle(x, y, s.x1, s.y1, s.hRadius)) inside = true;
                if (s.type == SHAPE_ELLIPSE && IsInsideEllipse(x, y, s.x1, s.y1, s.hRadius, s.vRadius)) inside = true;
            }
            if (!inside) printf("Click inside the shape!\n");
            else {
                if (currCommand == FILL_FLOOD_REC) FloodFillRecursive(hdc, x, y, col, RGB(128, 0, 32));
                else FloodFillNonRecursive(hdc, x, y, col, RGB(128, 0, 32));
            }
        }

        static int fillClicks = 0;
        static int fx, fy;
        if (currCommand == FILL_SQHERMIT || currCommand == FILL_RECTBEZIER) {
            fillClicks++;
            if (fillClicks == 1) { fx = x; fy = y; printf("Top-left: (%d,%d). Click bottom-right.\n", fx, fy);
            }
            else {
                if (currCommand == FILL_SQHERMIT) FillSquareHermiteVertical(hdc,  min(fx, x), min(fy, y), max(abs(x - fx), abs(y - fy)), col);
                if (currCommand == FILL_RECTBEZIER) FillRectangleBezierHorizontal(hdc, min(fx, x), min(fy, y), abs(x - fx), abs(y - fy), col);
                fillClicks = 0;
            }
        }

        static POINT points[5];
        static int polyClick = 0;
        if (currCommand == FILL_CONVEX || currCommand == FILL_NON_CONVEX) {
            points[polyClick] = { x, y };
            polyClick++;

            // Draw a red dot for the vertex
            SetPixel(hdc, x, y, RGB(255, 0, 0));

            // Draw the boundary line from the previous point to the current point
            if (polyClick > 1) {
                DrawLineDDA(hdc, points[polyClick - 2].x, points[polyClick - 2].y,
                    points[polyClick - 1].x, points[polyClick - 1].y, col);
            }

            if (polyClick == 5) {
                // Close the polygon boundary by connecting the 5th point back to the 1st point
                DrawLineDDA(hdc, points[4].x, points[4].y, points[0].x, points[0].y, col);

                // Execute the filling algorithm
                if (currCommand == FILL_CONVEX) PolyFill(hdc, points, 5, col);
                else nonPolyFill(hdc, points, 5, col);

                polyClick = 0;
            }
        }

        int fillQuarter;
        if (currCommand == FILL_CIRCLECIRCLES || currCommand == FILL_CIRCLELINES) {
            for (const ShapeData& s : shapes) {
                if (s.type == SHAPE_CIRCLE && (x - s.x1) * (x - s.x1) + (y - s.y1) * (y - s.y1) <= s.hRadius * s.hRadius) {
                    printf("Enter quarter (1=TopRight, 2=TopLeft, 3=BottomLeft, 4=BottomRight): ");
                    cin >> fillQuarter;
                    if (currCommand == FILL_CIRCLECIRCLES) FillCircleWithCircles(hdc, s.x1, s.y1, s.hRadius, col, fillQuarter);
                    else FillCircleWithLines(hdc, s.x1, s.y1, s.hRadius, col, fillQuarter);
                    break;
                }
            }
        }

        // --- CLIPPING ---
                // --- CLIPPING ---
        if (currCommand >= CLIP_RECTPOINT && currCommand <= CLIP_CIRCLELINE) {
            ClippingWindow cw = (currCommand == CLIP_SQPOINT || currCommand == CLIP_SQLINE) ? squareWindow : rectWindow;

            if (currCommand == CLIP_RECTPOINT || currCommand == CLIP_SQPOINT) {
                SetPixel(hdc, x, y, RGB(255, 0, 0));
                if (currCommand == CLIP_RECTPOINT) PointClippingRect(x, y, cw, hdc, col);
                else PointClippingSquare(x, y, cw, hdc, col);
            }
            else if (currCommand == CLIP_RECTLINE || currCommand == CLIP_SQLINE) {
                static int clipLineClicks = 0;
                static int clx1, cly1;
                SetPixel(hdc, x, y, RGB(255, 0, 0));
                clipLineClicks++;
                if (clipLineClicks == 1) { clx1 = x; cly1 = y; }
                else { CohenSutherlandLineClipping(hdc, clx1, cly1, x, y, cw, col); clipLineClicks = 0; }
            }
            else if (currCommand == CLIP_RECTPOLYGON) {
                clipPolygonPoints.push_back(Vertex(x, y));
                SetPixel(hdc, x, y, RGB(255, 0, 0));
                printf("Polygon vertex added. Right click to clip.\n");
            }
            // ===== circle clipping
            else if (currCommand == CLIP_CIRCLEPOINT) {
                SetPixel(hdc, x, y, RGB(255, 0, 0));
                PointClippingCircle(x, y, circleWindow, hdc, col);
                printf("Circle clipping - Point: (%d, %d)\n", x, y);
            }
            else if (currCommand == CLIP_CIRCLELINE) {
                static int circleLineClicks = 0;
                static int clx1c, cly1c;
                SetPixel(hdc, x, y, RGB(255, 0, 0));
                circleLineClicks++;
                if (circleLineClicks == 1) {
                    clx1c = x; cly1c = y;
                    printf("Circle line clipping - P1: (%d, %d). Click P2.\n", clx1c, cly1c);
                }
                else {
                    CircleLineClipping(hdc, clx1c, cly1c, x, y, circleWindow, col);
                    circleLineClicks = 0;
                }
            }
        }

        ReleaseDC(hwnd, hdc);
        break;
    }

    case WM_RBUTTONDOWN:
    {
        if (currCommand == CLIP_RECTPOLYGON && clipPolygonPoints.size() >= 3) {
            HDC hdc = GetDC(hwnd);
            COLORREF col = RGB(currColor.r, currColor.g, currColor.b);
            SutherlandHodgmanClipping(hdc, clipPolygonPoints, rectWindow, col);
            clipPolygonPoints.clear();
            ReleaseDC(hwnd, hdc);
        }
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Draw Saved Shapes
        for (const ShapeData& shape : shapes) {
            COLORREF c = RGB(shape.color.r, shape.color.g, shape.color.b);
            if (shape.type == SHAPE_CIRCLE) DrawCircleMidpoint(hdc, shape.x1, shape.y1, shape.hRadius, c);
            else if (shape.type == SHAPE_ELLIPSE) DrawEllipseMidpoint(hdc, shape.x1, shape.y1, shape.hRadius, shape.vRadius, c);
            else if (shape.type == SHAPE_CURVE) DrawCardinalSpline(hdc, shape.curvePoints, 0.5, c);
            else if (shape.type == SHAPE_LINE) bresenhamLine(hdc, shape.x1, shape.y1, shape.x2, shape.y2, c);
        }

        // Draw active clipping window
        if (currCommand >= CLIP_RECTPOINT && currCommand <= CLIP_SQLINE) {
            ClippingWindow cw = (currCommand == CLIP_SQPOINT || currCommand == CLIP_SQLINE) ? squareWindow : rectWindow;
            DrawWindow(hdc, cw, RGB(150, 150, 150));
        }
        //draw circle clipping window if active
        else if (currCommand == CLIP_CIRCLEPOINT || currCommand == CLIP_CIRCLELINE) {
            DrawCircleMidpoint(hdc, circleWindow.centerX, circleWindow.centerY, circleWindow.radius,RGB(0,0,0));
        }

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_CLOSE: DestroyWindow(hwnd); break;
    case WM_DESTROY: PostQuitMessage(0);  break;
    default: return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE h, HINSTANCE p, LPSTR c, int nsh)
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hInstance = h;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"myclass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(L"myclass", L"2D Drawing Package - Fully Modular",
        WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, NULL, NULL, h, 0);

    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}