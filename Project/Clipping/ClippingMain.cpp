#include <Windows.h>
#include <iostream>
#include <vector>
#include "Clipping.h"

using namespace std;

// globals
int  clickCount = 0;
vector<Vertex> polygonPoints;

ClippingWindow rectWindow = { 150, 100, 650, 500 }; // 500 x 400  rectangle
ClippingWindow squareWindow = { 200, 100, 600, 500 }; // 400 x 400  square

int testMode = 0; // 0 = Point | 1 = Line | 2 = Polygon
int windowType = 0; // 0 = Rectangle | 1 = Square

COLORREF testColor = RGB(0, 200, 255);
COLORREF rectColor = RGB(100, 100, 100);
COLORREF squareColor = RGB(100, 180, 80);

extern void bresenhamLine(HDC hdc, int x1, int y1,
    int x2, int y2, COLORREF color);

//helpers
ClippingWindow& ActiveWindow()
{
    return (windowType == 0) ? rectWindow : squareWindow;
}

void RefreshWindow(HWND hwnd) 
{
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);
}

void PrintStatus()
{
    const char* modes[] = { "Point", "Line", "Polygon" };
    const char* windows[] = { "Rectangle", "Square" };
    cout << "[STATUS] Mode: " << modes[testMode]
        << " | Window: " << windows[windowType] << "\n";
}

// console menu
void ShowConsoleMenu()
{
  
    cout << "  CLIPPING ALGORITHM  \n";
   

    cout << "\nSelect Clipping Mode:\n"
        << "  1 = Point Clipping\n"
        << "  2 = Line Clipping\n"
        << "  3 = Polygon Clipping\n"
        << "Enter choice (1-3): ";
    int mChoice = 0;
    cin >> mChoice;
    testMode = (mChoice >= 1 && mChoice <= 3) ? mChoice - 1 : 0;

    if (testMode == 2)
    {
        
        cout << "Polygon clipping only supports Rectangle window.\n";
        windowType = 0;
    }
    else
    {
        cout << "\nSelect Clipping Window Shape:\n"
            << "  1 = Rectangle\n"
            << "  2 = Square\n"
            << "Enter choice (1-2): ";
        int wChoice = 0;
        cin >> wChoice;
        windowType = (wChoice == 2) ? 1 : 0;
    }

    cout << "\n------------------------------\n";
    PrintStatus();
    cout << "------------------------------\n";
    cout << "Keyboard shortcuts (window must be focused):\n"
        << "  1  = Point Clipping mode\n"
        << "  2  = Line Clipping mode\n"
        << "  3  = Polygon Clipping mode\n"
        << "  R  = Rectangle clipping window\n"
        << "  S  = Square clipping window  (Point & Line only)\n"
        << "  C  = Clear & redraw\n"
        << "Polygon: left-click vertices, right-click to clip\n"
        << "------------------------------\n\n";
}

// ===== WINDOW PROCEDURE =====
LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (mcode)
    {
        // --------------------------------------------------
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);

        COLORREF wColor = (windowType == 0) ? rectColor : squareColor;
        DrawWindow(hdc, ActiveWindow(), wColor);

        EndPaint(hwnd, &ps);
        break;
    }

    // --------------------------------------------------
    case WM_LBUTTONDOWN:
    {
        int mx = LOWORD(lp);
        int my = HIWORD(lp);
        hdc = GetDC(hwnd);
        ClippingWindow& cw = ActiveWindow();

        cout << "[CLICK] (" << mx << ", " << my << ")\n";

        // --- Point Clipping ---
        if (testMode == 0)
        {
            bool inside = (mx >= cw.left && mx <= cw.right &&
                my >= cw.top && my <= cw.bottom);

            cout << "[POINT] (" << mx << ", " << my << ") -> "
                << (inside ? "INSIDE  -> drawn\n"
                    : "OUTSIDE -> clipped\n");

            // Draw red dot regardless so user sees where they clicked
            HBRUSH hb = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH ho = (HBRUSH)SelectObject(hdc, hb);
            Ellipse(hdc, mx - 3, my - 3, mx + 3, my + 3);
            SelectObject(hdc, ho);
            DeleteObject(hb);

            // Draw clipped (cyan) pixel only if inside
            PointClippingRect(mx, my, cw, hdc, testColor);
        }

        // --- Line Clipping ---
        else if (testMode == 1)
        {
            HBRUSH hb = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH ho = (HBRUSH)SelectObject(hdc, hb);
            Ellipse(hdc, mx - 3, my - 3, mx + 3, my + 3);
            SelectObject(hdc, ho);
            DeleteObject(hb);

            if (clickCount == 0)
            {
                cout << "[LINE] P1 = (" << mx << ", " << my << ")\n";
                polygonPoints.push_back(Vertex(mx, my));
                clickCount++;
            }
            else
            {
                cout << "[LINE] P2 = (" << mx << ", " << my << ")\n";
                cout << "[LINE] Running Cohen-Sutherland...\n";

                CohenSutherlandLineClipping(
                    hdc,
                    (int)polygonPoints[0].x, (int)polygonPoints[0].y,
                    mx, my,
                    cw, testColor);

                cout << "[LINE] Done.\n";
                clickCount = 0;
                polygonPoints.clear();
            }
        }

        // --- Polygon Clipping ---
        else if (testMode == 2)
        {
            if (clickCount < 6)
            {
                cout << "[POLYGON] Vertex " << clickCount + 1
                    << " = (" << mx << ", " << my << ")\n";

                polygonPoints.push_back(Vertex(mx, my));

                HBRUSH hb = CreateSolidBrush(RGB(255, 0, 0));
                HBRUSH ho = (HBRUSH)SelectObject(hdc, hb);
                Ellipse(hdc, mx - 3, my - 3, mx + 3, my + 3);
                SelectObject(hdc, ho);
                DeleteObject(hb);

                if (clickCount > 0)
                {
                    bresenhamLine(hdc,
                        (int)polygonPoints[clickCount - 1].x,
                        (int)polygonPoints[clickCount - 1].y,
                        mx, my,
                        RGB(255, 100, 0));
                }

                clickCount++;
                cout << "[POLYGON] " << clickCount
                    << " vertices so far. Right-click to clip.\n";
            }
            else
            {
                cout << "[POLYGON] Max 6 vertices reached. "
                    "Right-click to clip.\n";
            }
        }

        ReleaseDC(hwnd, hdc);
        break;
    }

    // --------------------------------------------------
    case WM_RBUTTONDOWN:
    {
        // Only act on right-click in Polygon mode
        if (testMode != 2)
            break;

        if (clickCount > 2)
        {
            hdc = GetDC(hwnd);
            cout << "[POLYGON] Clipping polygon ("
                << clickCount << " vertices)...\n";

            vector<Vertex> poly = polygonPoints;
            SutherlandHodgmanClipping(hdc, poly, rectWindow, testColor);

            ReleaseDC(hwnd, hdc);

            clickCount = 0;
            polygonPoints.clear();
            cout << "[POLYGON] Done. Click to start a new polygon.\n";
        }
        else
        {
            cout << "[POLYGON] Need at least 3 vertices (have "
                << clickCount << ").\n";
            MessageBox(hwnd,
                L"Need at least 3 vertices.\nKeep left-clicking, then right-click.",
                L"Polygon Clipping", MB_OK);
        }
        break;
    }

    // --------------------------------------------------
    case WM_KEYDOWN:
    {
        if (wp == '1')
        {
            testMode = 0;
            clickCount = 0; polygonPoints.clear();
            cout << "\n[MODE] Point Clipping\n";
            PrintStatus();
            RefreshWindow(hwnd);
        }
        else if (wp == '2')
        {
            testMode = 1;
            clickCount = 0; polygonPoints.clear();
            cout << "\n[MODE] Line Clipping\n";
            PrintStatus();
            RefreshWindow(hwnd);
        }
        else if (wp == '3')
        {
            testMode = 2;
            windowType = 0; // force rectangle for polygon
            clickCount = 0; polygonPoints.clear();
            cout << "\n[MODE] Polygon Clipping (Rectangle window forced)\n";
            PrintStatus();
            RefreshWindow(hwnd);
        }
        else if (wp == 'R')
        {
            windowType = 0;
            clickCount = 0; polygonPoints.clear();
            cout << "\n[WINDOW] Rectangle\n";
            PrintStatus();
            RefreshWindow(hwnd);
        }
        else if (wp == 'S')
        {
            if (testMode == 2)
            {
                cout << "[WINDOW] Square not available in Polygon mode.\n";
            }
            else
            {
                windowType = 1;
                clickCount = 0; polygonPoints.clear();
                cout << "\n[WINDOW] Square\n";
                PrintStatus();
                RefreshWindow(hwnd);
            }
        }
        else if (wp == 'C')
        {
            clickCount = 0; polygonPoints.clear();
            cout << "\n[CLEAR] Screen cleared.\n";
            RefreshWindow(hwnd);
        }
        break;
    }

    case WM_CLOSE:   DestroyWindow(hwnd); break;
    case WM_DESTROY: PostQuitMessage(0);  break;
    default: return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}

// ===== ENTRY POINT =====
int APIENTRY WinMain(HINSTANCE h, HINSTANCE p, LPSTR c, int nsh)
{
    // Open a console window for logs + user input
    AllocConsole();
    FILE* fOut; freopen_s(&fOut, "CONOUT$", "w", stdout);
    FILE* fIn;  freopen_s(&fIn, "CONIN$", "r", stdin);

    // Ask user for initial mode & window type BEFORE showing the window
    ShowConsoleMenu();

    // Register window class
    WNDCLASS wc = {};
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hInstance = h;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"ClippingTestClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        L"ClippingTestClass", L"Clipping Algorithms Test",
        WS_OVERLAPPEDWINDOW, 0, 0, 800, 600,
        NULL, NULL, h, 0);

    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}