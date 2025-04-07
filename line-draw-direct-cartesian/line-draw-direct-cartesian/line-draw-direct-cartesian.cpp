#include <Windows.h>
#include <cmath>
#include <iostream>

using namespace std;

void drawSimpleLine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c) {
	int dx = xe - xs;
	int dy = ye - ys;

	if (abs(dx) > abs(dy)) {
		// Line is more horizontal, so iterate over x
		if (xe == xs) { // edge case drawing vertical line
			int y_start = min(ys, ye);
			int y_end = max(ys, ye);
			for (int y = y_start; y <= y_end; y++) {
				SetPixel(hdc, xs, y, c);
			}
			return;
		}

		if (xs > xe) {
			swap(xs, xe);
			swap(ys, ye);
		}
		double slope = static_cast<double>(ye - ys) / (xe - xs);
		for (int x = xs; x <= xe; x++) {
			int y = round(ys + slope * (x - xs));
			SetPixel(hdc, x, y, c);
		}
	}
	else {
		// Line is more vertical, so iterate over y
		if (ys == ye) { // edge case drawing horizontal line
			int x_start = min(xs, xe);
			int x_end = max(xs, xe);
			for (int x = x_start; x <= x_end; x++) {
				SetPixel(hdc, xs, x, c);
			}
			return;
		}
		if (ys > ye) {
			swap(xs, xe);
			swap(ys, ye);
		}
		double invSlope = static_cast<double>(xe - xs) / (ye - ys);  // dx / dy
		for (int y = ys; y <= ye; y++) {
			int x = round(xs + invSlope * (y - ys));
			SetPixel(hdc, x, y, c);
		}
	}
}


LRESULT WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
	static int x1 = 0, y1 = 0;

	switch (m)
	{
	case WM_LBUTTONDOWN: {
		x1 = LOWORD(lp);
		y1 = HIWORD(lp);
		break;
	}
	case WM_LBUTTONUP: {
		int x2 = LOWORD(lp);
		int y2 = HIWORD(lp);
		HDC hdc = GetDC(hwnd);
		COLORREF c1 = 0x00FF0000, c2 = 0x000000FF;
		drawSimpleLine(hdc, x1, y1, x2, y2, c1);
		ReleaseDC(hwnd, hdc);
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd); break;
	case WM_DESTROY:
		PostQuitMessage(0); break;
	default:return DefWindowProc(hwnd, m, wp, lp);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR cmd, int nsh)
{
	WNDCLASS wc = {};
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hi;

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindow(L"MyClass", L"Hello World!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hi, 0);
	if (hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
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