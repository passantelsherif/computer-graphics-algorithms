//#include <Windows.h>
//#include <math.h>
//
//// ─────────────────────────────────────────────
//// State: collect 4 clicks, then draw
//// ─────────────────────────────────────────────
//int  clickCount = 0;
//POINT cp[4];   // cp[0]=P0, cp[1]=P1, cp[2]=P2, cp[3]=P3
//
//
//// ─────────────────────────────────────────────
//// Bresenham LINE — draws from (x1,y1) to (x2,y2)
//// Uses pure pixel-setting algorithm, no GDI pens
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
//
//// ─────────────────────────────────────────────
//// STEP 3 helper:
////   Apply the Hermite basis matrix to one axis.
////
////   The Hermite basis matrix multiplication:
////
////   [α0]     [1    0    0    0 ]   [x1]
////   [α1]  =  [0    1    0    0 ] * [u1]
////   [α2]     [-3  -2    3   -1 ]   [x2]
////   [α3]     [2    1   -2    1 ]   [u2]
////
////   Result: x(t) = α0 + α1*t + α2*t^2 + α3*t^3
////
////   Parameters:
////     x1 = position at t=0
////     u1 = tangent/velocity at t=0
////     x2 = position at t=1
////     u2 = tangent/velocity at t=1
////     alpha = output array [α0, α1, α2, α3]
//// ─────────────────────────────────────────────
//void GetCoeffs(double x1, double u1, double x2, double u2, double alpha[4])
//{
//    // Apply Hermite basis matrix (row by row)
//
//    // Row 1: α0 = 1*x1 + 0*u1 + 0*x2 + 0*u2
//    alpha[0] = x1;
//
//    // Row 2: α1 = 0*x1 + 1*u1 + 0*x2 + 0*u2
//    alpha[1] = u1;
//
//    // Row 3: α2 = (-3)*x1 + (-2)*u1 + 3*x2 + (-1)*u2
//    alpha[2] = -3 * x1 - 2 * u1 + 3 * x2 - u2;
//
//    // Row 4: α3 = 2*x1 + 1*u1 + (-2)*x2 + 1*u2
//    alpha[3] = 2 * x1 + u1 - 2 * x2 + u2;
//}
//
//
//// ─────────────────────────────────────────────
//// Main Hermite Curve drawing function
////
////   STEP 1 — receive the 4 control points P0..P3
////             (already stored in cp[] from the clicks)
////
////   STEP 2 — compute tangent vectors from the points:
////             u1 = 3 * (P1 - P0)   (tangent at t=0)
////             u2 = 3 * (P3 - P2)   (tangent at t=1)
////
////   STEP 3 — call GetCoeffs() for x and y separately.
////             This applies the Hermite basis matrix to compute
////             [α0, α1, α2, α3] from [x1, u1, x2, u2].
////
////   STEP 4 — loop t from 0 → 1 in small steps.
////             At each t evaluate:
////               x(t) = α0 + α1*t + α2*t^2 + α3*t^3
////               y(t) = same with y coefficients
////
////   STEP 5 — connect consecutive evaluated points using
////             DrawLineBres (Bresenham line algorithm).
////
////   STEP 6 — draw the control polygon as red lines
////             and mark the 4 points so the user can see
////             how the handles influence the curve.
//// ─────────────────────────────────────────────
//void DrawHermiteCurve(HDC hdc, POINT p[4])
//{
//    // ── STEP 2: tangent vectors ───────────────
//    // x1 = P0.x, u1 = 3*(P1.x - P0.x)
//    // x2 = P3.x, u2 = 3*(P3.x - P2.x)
//    double x1_x = (double)p[0].x;
//    double u1_x = 3.0 * (p[1].x - p[0].x);
//    double x2_x = (double)p[3].x;
//    double u2_x = 3.0 * (p[3].x - p[2].x);
//
//    double x1_y = (double)p[0].y;
//    double u1_y = 3.0 * (p[1].y - p[0].y);
//    double x2_y = (double)p[3].y;
//    double u2_y = 3.0 * (p[3].y - p[2].y);
//
//    // ── STEP 3: polynomial coefficients using Hermite basis matrix ──────
//    // Input to matrix: [x1, u1, x2, u2]
//    double alpha_x[4], alpha_y[4];
//    GetCoeffs(x1_x, u1_x, x2_x, u2_x, alpha_x);   // x-axis
//    GetCoeffs(x1_y, u1_y, x2_y, u2_y, alpha_y);   // y-axis
//
//    // ── STEP 4 + 5: evaluate and connect using DrawLineBres ─────
//    const int N = 500;  // Number of segments for smooth curve
//    int prevX = 0, prevY = 0;
//
//    for (int i = 0; i <= N; i++)
//    {
//        double t = (double)i / N;
//        double t2 = t * t;
//        double t3 = t2 * t;
//
//        // Evaluate cubic polynomial for x and y at parameter t
//        // x(t) = α0 + α1*t + α2*t² + α3*t³
//        int x = (int)round(alpha_x[0] + alpha_x[1] * t + alpha_x[2] * t2 + alpha_x[3] * t3);
//        int y = (int)round(alpha_y[0] + alpha_y[1] * t + alpha_y[2] * t2 + alpha_y[3] * t3);
//
//        // Draw line from previous point to current point
//        if (i > 0) {
//            DrawLineBres(hdc, prevX, prevY, x, y, RGB(0, 0, 0));  // Black curve
//        }
//
//        prevX = x;
//        prevY = y;
//    }
//
//    // ── STEP 6: control polygon + point markers ──
//
//    // Handle lines: P0→P1 and P3→P2 (red)
//    DrawLineBres(hdc, p[0].x, p[0].y, p[1].x, p[1].y, RGB(255, 0, 0));
//    DrawLineBres(hdc, p[3].x, p[3].y, p[2].x, p[2].y, RGB(255, 0, 0));
//
//    // Small filled circles on each control point
//    // P0 and P3 (endpoints the curve passes through) → blue
//    // P1 and P2 (handles)                            → green
//    COLORREF dotColor[4] = {
//        RGB(0, 0, 255),   // P0 — start, curve passes through
//        RGB(0, 255, 0),   // P1 — handle near start
//        RGB(0, 255, 0),   // P2 — handle near end
//        RGB(0, 0, 255),   // P3 — end, curve passes through
//    };
//
//    for (int i = 0; i < 4; i++)
//    {
//        HBRUSH hBrush = CreateSolidBrush(dotColor[i]);
//        HBRUSH hOld = (HBRUSH)SelectObject(hdc, hBrush);
//        Ellipse(hdc, p[i].x - 5, p[i].y - 5,
//            p[i].x + 5, p[i].y + 5);
//        SelectObject(hdc, hOld);
//        DeleteObject(hBrush);
//    }
//}
//
//
//// ─────────────────────────────────────────────
//// Window Procedure
//// ─────────────────────────────────────────────
//LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
//{
//    HDC hdc;
//
//    switch (mcode)
//    {
//    case WM_LBUTTONDOWN:
//    {
//        // Collect clicks
//        cp[clickCount].x = LOWORD(lp);
//        cp[clickCount].y = HIWORD(lp);
//        clickCount++;
//
//        // Draw a small dot so the user sees their click landed
//        //hdc = GetDC(hwnd);
//        //HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));
//        //HBRUSH hOld = (HBRUSH)SelectObject(hdc, hBrush);
//        //Ellipse(hdc,
//        //    cp[clickCount - 1].x - 3, cp[clickCount - 1].y - 3,
//        //    cp[clickCount - 1].x + 3, cp[clickCount - 1].y + 3);
//        //SelectObject(hdc, hOld);
//        //DeleteObject(hBrush);
//        //ReleaseDC(hwnd, hdc);
//
//        // After the 4th click → run the full drawing pipeline
//        if (clickCount == 4)
//        {
//            hdc = GetDC(hwnd);
//            DrawHermiteCurve(hdc, cp);
//            ReleaseDC(hwnd, hdc);
//
//            clickCount = 0;   // reset → user can draw another curve
//        }
//        break;
//    }
//
//    case WM_CLOSE:   DestroyWindow(hwnd); break;
//    case WM_DESTROY: PostQuitMessage(0);  break;
//    default: return DefWindowProc(hwnd, mcode, wp, lp);
//    }
//    return 0;
//}
//
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
//    wc.lpszClassName = L"HermiteClass";
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    RegisterClass(&wc);
//
//    HWND hwnd = CreateWindow(
//        L"HermiteClass", L"Hermite Curves – Click 4 Points",
//        WS_OVERLAPPEDWINDOW,
//        0, 0, 800, 600,
//        NULL, NULL, h, 0);
//
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