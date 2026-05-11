//#include <Windows.h>
//#include<math.h>
//
//// Store the 3 click points
//int clickCount = 0;
//int cx, cy;       // Center (click 1)
//int p1x, p1y;    // Point on circle 1 (click 2)
//int p2x, p2y;    // Point on circle 2 (click 3)
//
//// ─────────────────────────────────────────────
//// Draw 8 symmetric points, each in a different color
//// ─────────────────────────────────────────────
//void Draw8Points(HDC hdc, int xc, int yc, int x, int y)
//{
//    // Each of the 8 octant pixels gets its own color
//    SetPixel(hdc, xc + x, yc + y, 255);  // Red
//    SetPixel(hdc, xc - x, yc + y, (0, 255, 0));  // Green
//    SetPixel(hdc, xc - x, yc - y, RGB(0, 0, 255));  // Blue
//    SetPixel(hdc, xc + x, yc - y, RGB(255, 255, 0));  // Yellow
//    SetPixel(hdc, xc + y, yc + x, RGB(255, 0, 255));  // Magenta
//    SetPixel(hdc, xc - y, yc + x, RGB(0, 255, 255));  // Cyan
//    SetPixel(hdc, xc - y, yc - x, RGB(255, 128, 0));  // Orange
//    SetPixel(hdc, xc + y, yc - x, RGB(128, 0, 128));  // Purple
//}
//
//// ─────────────────────────────────────────────
//// Bresenham circle algorithm using Draw8Points
//// ─────────────────────────────────────────────
//void DrawCircleBres(HDC hdc, int xc, int yc, int R)
//{
//    if (R <= 0) return;          // safety guard for tiny radii
//    int x = 0;
//    int y = R;
//    int d = 1 - R;
//    int ch1 = 3;
//    int ch2 = 5 - 2 * R;
//    Draw8Points(hdc, xc, yc, x, y);
//    while (x < y)
//    {
//        if (d < 0) {
//            d += ch1;
//            ch2 += 2;
//        }
//        else {
//            d += ch2;
//            ch2 += 4;
//            y--;
//        }
//        ch1 += 2;
//        x++;
//        Draw8Points(hdc, xc, yc, x, y);
//    }
//}
//
//// ─────────────────────────────────────────────
//// Bresenham LINE algorithm (your implementation)
//// Draws from (x1,y1) to (x2,y2) in white
//// ─────────────────────────────────────────────
//void DrawLineBres(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
//{
//    int dx = abs(x2 - x1);
//    int dy = abs(y2 - y1);
//    int sx = (x2 > x1) ? 1 : -1;   // step direction X
//    int sy = (y2 > y1) ? 1 : -1;   // step direction Y
//
//    // We generalise to all slopes by swapping roles of x and y when steep
//    bool steep = dy > dx;
//    if (steep) {
//        // swap dx/dy and drive along Y
//        int tmp = dx; dx = dy; dy = tmp;
//    }
//
//    // d = dx - 2*dy  (classic Bresenham decision param)
//    int d = dx - 2 * dy;
//    int dd1 = 2 * dx - 2 * dy;   // diagonal step correction
//    int dd2 = -2 * dy;             // axis-aligned step correction
//
//    int x = x1, y = y1;
//    SetPixel(hdc, x, y, color);
//
//    for (int i = 0; i < dx; i++)
//    {
//        if (d < 0) {
//            // Move diagonally
//            d += dd1;
//            if (steep) x += sx; else y += sy;  // minor-axis step
//        }
//        else {
//            d += dd2;
//        }
//        // Major-axis step
//        if (steep) y += sy; else x += sx;
//        SetPixel(hdc, x, y, color);
//    }
//}
//
//// ─────────────────────────────────────────────
//// Euclidean distance helper → gives us the radius
//// ─────────────────────────────────────────────
//int CalcRadius(int x1, int y1, int x2, int y2)
//{
//    int dx = x2 - x1;
//    int dy = y2 - y1;
//    return (int)sqrt((double)(dx * dx + dy * dy));
//}
//
//// ─────────────────────────────────────────────
//// Main drawing routine called after 3 clicks
//// ─────────────────────────────────────────────
//void Task(HDC hdc)
//{
//    // ── Compute the two radii ──────────────────
//    int R1 = CalcRadius(cx, cy, p1x, p1y);  // inner circle radius
//    int R2 = CalcRadius(cx, cy, p2x, p2y);  // outer circle radius
//
//    //// Make sure R1 < R2 (swap if the user clicked inner first)
//    if (R1 > R2) { int tmp = R1; R1 = R2; R2 = tmp; }
//
//    // ── 1. Fill the ring with concentric circles ──
//    // Loop from R2 down to R1+1, decrementing by 1 each step
//    // Each iteration draws one concentric circle, together they fill the ring
//    for (int var = R2; var > R1; var--)
//    {
//        DrawCircleBres(hdc, cx, cy, var);
//    }
//
//    // ── 2. Draw the two boundary circles (bold outline) ──
//    DrawCircleBres(hdc, cx, cy, R1);
//    DrawCircleBres(hdc, cx, cy, R2);
//
//    // ── 3. Line from center to point on circle 1 ──
//    DrawLineBres(hdc, cx, cy, p1x, p1y, RGB(255, 0, 0));
//
//    // ── 4. Line from center to point on circle 2 ──
//    DrawLineBres(hdc, cx, cy, p2x, p2y, RGB(0, 255, 0));
//}
//
//// ─────────────────────────────────────────────
//// Window Procedure
//// ─────────────────────────────────────────────
//LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
//{
//    HDC hdc;
//    switch (mcode)
//    {
//    case WM_LBUTTONDOWN:
//        clickCount++;
//        if (clickCount == 1)
//        {
//            // First click → save center
//            cx = LOWORD(lp);
//            cy = HIWORD(lp);
//        }
//        else if (clickCount == 2)
//        {
//            // Second click → point on inner/first circle
//            p1x = LOWORD(lp);
//            p1y = HIWORD(lp);
//        }
//        else if (clickCount == 3)
//        {
//            // Third click → point on outer/second circle
//            p2x = LOWORD(lp);
//            p2y = HIWORD(lp);
//
//            // All 3 clicks collected → draw everything
//            hdc = GetDC(hwnd);
//            Task(hdc);
//            ReleaseDC(hwnd, hdc);
//
//            // Reset so the user can try again
//            clickCount = 0;
//        }
//        break;
//
//    case WM_CLOSE:   DestroyWindow(hwnd); break;
//    case WM_DESTROY: PostQuitMessage(0);  break;
//    default: return DefWindowProc(hwnd, mcode, wp, lp);
//    }
//    return 0;
//}
//
//// ─────────────────────────────────────────────
//// Entry point
//// ─────────────────────────────────────────────
//int APIENTRY WinMain(HINSTANCE h, HINSTANCE p, LPSTR c, int nsh)
//{
//    WNDCLASS wc = {};
//    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
//    wc.hInstance = h;
//    wc.lpfnWndProc = WndProc;
//    wc.lpszClassName = L"myclass";
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    RegisterClass(&wc);
//
//    HWND hwnd = CreateWindow(L"myclass", L"Task 4 – Two Circles",
//        WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, h, 0);
//    ShowWindow(hwnd, nsh);
//    UpdateWindow(hwnd);
//
//    MSG msg;
//    while (GetMessage(&msg, NULL, 0, 0) > 0) {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//    return 0;
//}