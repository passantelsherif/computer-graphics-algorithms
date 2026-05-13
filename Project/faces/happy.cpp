#include"faces.h"
#include"../Shapes/Lines.h"
#include"../Shapes/Curve.h"
#include"../Shapes/Circle.h"

void DrawHappyFace(HDC hdc, int cx, int cy, int R, COLORREF color)
{
    DrawCircleMod(hdc, cx, cy, R, color);
    int eyeR = R / 8, eyeY = cy - R / 4, eyeX = R / 3;
    DrawCircleMod(hdc, cx - eyeX, eyeY, eyeR, color);
    DrawCircleMod(hdc, cx + eyeX, eyeY, eyeR, color);
    DrawLineDDA(hdc, cx, cy - R / 10, cx, cy + R / 10, color);
    int mouthY = cy + R / 3, W = R / 2, H = R / 4;
    DrawBezier(hdc, cx - W, mouthY, cx - W / 2, mouthY + H, cx + W / 2, mouthY + H, cx + W, mouthY, color);
}
