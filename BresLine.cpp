#include <windows.h>
#include <iostream>
using namespace std;

// Global variables
int x1_line = 200, y1_line = 300, x2_line = 450, y2_line = 150;

// Bresenham's Midpoint Line Algorithm — Case 1: 0 <= slope <= 1, x1 < x2
void bresenhamLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Initial decision parameter and increments
    int d = dx - 2 * dy;       // d = Δx - 2Δy
    int dd1 = 2 * dx - 2 * dy;   // Δd1: diagonal step (d < 0)
    int dd2 = -2 * dy;            // Δd2: horizontal step (d >= 0)

    int x = x1;
    int y = y1;

    // Plot the first pixel
    SetPixel(hdc, x, y, color);
    cout << "Plot (" << x << ", " << y << ")  d = " << d << endl;

    while (x < x2) {
        if (d < 0) {
            // Line is above midpoint -> go diagonal (NE)
            y--;        // y-- because in Windows, y increases DOWNWARD
            d += dd1;
        }
        else {
            // Line is below midpoint -> go horizontal (E)
            d += dd2;
        }
        x++;
        SetPixel(hdc, x, y, color);
        cout << "Plot (" << x << ", " << y << ")  d = " << d << endl;
    }
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Draw Bresenham line in white
        bresenhamLine(hdc, x1_line, y1_line, x2_line, y2_line, RGB(255, 255, 255));

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("BresenhamWindow");
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  // black background
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindow(
        TEXT("BresenhamWindow"),
        TEXT("Bresenham Line Algorithm - Case 1"),
        WS_OVERLAPPEDWINDOW,
        100, 100,           // window position
        600, 500,           // window size
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}