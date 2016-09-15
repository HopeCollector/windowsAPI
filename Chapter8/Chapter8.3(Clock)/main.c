#include <Windows.h>
#include <math.h>
#include <stdlib.h>

#define SECOND 0
#define MINUTE 1
#define HOUR 2
#define PI 3.1415926

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID DrawDial(HDC);
VOID SetIsotropic(HDC, int, int);
VOID RotatePoint(LPPOINT, LPINT, INT);
VOID SysTimetoAngle(LPSYSTEMTIME, LPINT);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("CLOCK");
	MSG msg;
	HWND hwnd;
	WNDCLASS wndclass;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndclass);
	hwnd = CreateWindow(szAppName, TEXT("CLOCK"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int cxClient, cyClient;
	static SYSTEMTIME st;
	static POINT pt[3];
	static INT Firstangle[3];
	static INT Rotateangle[3] ;
	static INT HourCount = 0;

	switch (message)
	{
	case WM_CREATE:
		GetLocalTime(&st);
		SetTimer(hwnd, 1, 1000, NULL);
		SysTimetoAngle(&st, Firstangle);

		HourCount = st.wMinute % 12;

		pt[SECOND].x = 0;
		pt[SECOND].y = 850;

		pt[MINUTE].x = 0;
		pt[MINUTE].y = 650;

		pt[HOUR].x = 0;
		pt[HOUR].y = 450;

		RotatePoint(pt, Firstangle, 3);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_TIMER:
		Rotateangle[SECOND] = 6;
		Rotateangle[MINUTE] = 0;
		Rotateangle[HOUR] = 0;

		if ((Firstangle[SECOND] += 6) >= 360)
		{
			Firstangle[SECOND] = 0;
			Rotateangle[MINUTE] = 6;
			HourCount++;
			if (HourCount % 12 == 0)
			{
				Rotateangle[HOUR] = 6;
			}
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetIsotropic(hdc, cxClient, cyClient);
		DrawDial(hdc);
		RotatePoint(pt, Rotateangle, 3);

		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, pt[SECOND].x, pt[SECOND].y);
		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, pt[MINUTE].x, pt[MINUTE].y);
		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, pt[HOUR].x, pt[HOUR].y);


		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

VOID SetIsotropic(HDC hdc, int cxClient, int cyClient)
{
	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 1000, -1000, NULL);
	SetViewportExtEx(hdc, cxClient / 2, cyClient / 2, NULL);
	SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
}

VOID DrawDial(HDC hdc)
{
	int i;
	POINT pt;
	LPINT pass = malloc(sizeof(DOUBLE));

	SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	SelectObject(hdc, GetStockObject(NULL_PEN));

	for (i = 0; i < 360; i+=6)
	{
		pt.x = 0;
		pt.y = 900;
		*pass = (DOUBLE)i;
		RotatePoint(&pt, pass, 1);//有误差的东西不要用迭代算法
		if (i % 5 == 0)
			Ellipse(hdc, pt.x - 50, pt.y - 50, pt.x + 50, pt.y + 50);
		Ellipse(hdc, pt.x - 16, pt.y - 16, pt.x + 16, pt.y + 16);
	}
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
	DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
}

VOID RotatePoint(LPPOINT lppt, LPINT lpangle, int sum)
{
	int i;
	int x, y;
	for (i = 0; i < sum; i++)
	{
		x = lppt[i].x;
		y = lppt[i].y;
		lppt[i].x = (int)(x * cos(PI * lpangle[i] / 180) + 0.5) + (int)(y * sin(PI * lpangle[i] / 180) + 0.5);
		lppt[i].y = (int)(y * cos(PI * lpangle[i] / 180) + 0.5) - (int)(x * sin(PI * lpangle[i] / 180) + 0.5);
	}
}

VOID SysTimetoAngle(LPSYSTEMTIME lpst, LPINT lpangle)
{
	lpangle[SECOND] = (*lpst).wSecond * 6;
	lpangle[MINUTE] = (*lpst).wMinute * 6;
	lpangle[HOUR] = ((*lpst).wHour * 5 + (*lpst).wMinute) * 6;
}