//#include <Windows.h>
//#include <stack>
//
//int   clickCount = 0;
//POINT pts[5];
//
//const COLORREF BORDER_COLOR = RGB(0, 0, 0);
//const COLORREF FILL_COLOR = RGB(0, 200, 255);
//
//void DrawLineBres(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
//{
//    int dx = abs(x2 - x1), dy = abs(y2 - y1);
//    int sx = (x2 > x1) ? 1 : -1;
//    int sy = (y2 > y1) ? 1 : -1;
//
//    bool steep = dy > dx;
//    if (steep) { int t = dx; dx = dy; dy = t; }
//
//    int d = dx - 2 * dy, dd1 = 2 * dx - 2 * dy, dd2 = -2 * dy;
//    int x = x1, y = y1;
//    SetPixel(hdc, x, y, color);
//
//    for (int i = 0; i < dx; i++)
//    {
//        if (d < 0) 
//        { 
//            d += dd1; 
//            if (steep) x += sx; 
//            else y += sy; 
//        }
//        else 
//        { 
//            d += dd2; 
//        }
//
//        if (steep) y += sy; else x += sx;
//
//        SetPixel(hdc, x, y, color);
//    }
//}
//
//void DrawPolygon(HDC hdc)
//{
//    for (int i = 0; i < 5; i++)
//        DrawLineBres(hdc,pts[i].x, pts[i].y,pts[(i + 1) % 5].x, pts[(i + 1) % 5].y,BORDER_COLOR);
//}
//
//void FloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc)
//{
//    std::stack<POINT> s;
//    s.push({ x, y });
//
//    while (!s.empty())
//    {
//        POINT p = s.top();  s.pop();
//
//        COLORREF c = GetPixel(hdc, p.x, p.y);
//        if (c == bc || c == fc) continue;
//
//        SetPixel(hdc, p.x, p.y, fc);
//        s.push({ p.x,     p.y + 1 });
//        s.push({ p.x,     p.y - 1 });
//        s.push({ p.x + 1, p.y });
//        s.push({ p.x - 1, p.y });
//    }
//}
//
//LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
//{
//    HDC hdc;
//    switch (mcode)
//    {
//    case WM_LBUTTONDOWN:
//    {
//        int mx = LOWORD(lp);
//        int my = HIWORD(lp);
//
//        if (clickCount < 5)
//        {
//            pts[clickCount] = { mx, my };
//            clickCount++;
//
//            hdc = GetDC(hwnd);
//
//            HBRUSH hb = CreateSolidBrush(RGB(255, 0, 0));
//            HBRUSH ho = (HBRUSH)SelectObject(hdc, hb);
//            Ellipse(hdc, mx - 4, my - 4, mx + 4, my + 4);
//            SelectObject(hdc, ho);
//            DeleteObject(hb);
//
//            if (clickCount == 5)
//                DrawPolygon(hdc);
//
//            ReleaseDC(hwnd, hdc);
//        }
//        else
//        {
//            hdc = GetDC(hwnd);
//            FloodFill(hdc, mx, my, BORDER_COLOR, FILL_COLOR);
//            ReleaseDC(hwnd, hdc);
//            clickCount = 0;
//        }
//        break;
//    }
//    case WM_CLOSE:   DestroyWindow(hwnd); break;
//    case WM_DESTROY: PostQuitMessage(0);  break;
//    default: return DefWindowProc(hwnd, mcode, wp, lp);
//    }
//    return 0;
//}
//
//int APIENTRY WinMain(HINSTANCE h, HINSTANCE p, LPSTR c, int nsh)
//{
//    WNDCLASS wc = {};
//    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
//    wc.hInstance = h;
//    wc.lpfnWndProc = WndProc;
//    wc.lpszClassName = L"PolyFillClass";
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    RegisterClass(&wc);
//
//    HWND hwnd = CreateWindow(L"PolyFillClass", L"Polygon Fill",
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