#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID ShowDigit(HDC hdc,int iNumber);
VOID ShowPoint(HDC hdc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("DigClock");
	HWND hwnd;
	MSG msg;
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
	hwnd = CreateWindow(szAppName, TEXT("DigClock"),
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
	PAINTSTRUCT  ps;
	static HBRUSH hBrush;
	static int cxClient, cyClient;
	SYSTEMTIME st;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 1000, NULL);
		hBrush = CreateSolidBrush(0x0000FF);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_TIMER:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, 276, 72, NULL);
		SetViewportExtEx(hdc, cxClient, cyClient, NULL);
		SetWindowOrgEx(hdc, 138, 36, NULL);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
		SelectObject(hdc, hBrush);
		SelectObject(hdc, GetStockObject(NULL_PEN));


		GetLocalTime(&st);
		ShowDigit(hdc, st.wHour);
		ShowPoint(hdc);
		ShowDigit(hdc, st.wMinute);
		ShowPoint(hdc);
		ShowDigit(hdc, st.wSecond);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		DeleteObject(hBrush);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

VOID ShowDigit(HDC hdc, int iNumber)
{
	static  BOOL fNum[10][7] =
	{
		1,1,1,0,1,1,1,
		0,0,1,0,0,1,0,
		1,0,1,1,1,0,1,
		1,0,1,1,0,1,1,
		0,1,1,1,0,1,0,
		1,1,0,1,0,1,1,
		1,1,0,1,1,1,1,
		1,0,1,0,0,1,0,
		1,1,1,1,1,1,1,
		1,1,1,1,0,1,1,
	};
	static POINT ptSegment[7][6] = {
		7,  6,  11,  2,  31,  2,  35,  6,  31, 10,  11, 10,
		6,  7,  10, 11,  10, 31,   6, 35,   2, 31,   2, 11,
		36,  7,  40, 11,  40, 31,  36, 35,  32, 31,  32, 11,
		7, 36,  11, 32,  31, 32,  35, 36,  31, 40,  11, 40,
		6, 37,  10, 41,  10, 61,   6, 65,   2, 61,   2, 41,
		36, 37,  40, 41,  40, 61,  36, 65,  32, 61,  32, 41,
		7, 66,  11, 62,  31, 62,  35, 66,  31, 70,  11, 70 };
	int i, flag = 0;

	for (i = 0; i < 7; i++)
		if (fNum[iNumber / 10][i])
			Polygon(hdc, ptSegment[i], 6);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);

	for (i = 0; i < 7; i++)
		if (fNum[iNumber % 10][i])
			Polygon(hdc, ptSegment[i], 6);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);	
}

VOID ShowPoint(HDC hdc)
{
	static POINT ptColon[2][4] = { 2,  21,  6,  17,  10, 21,  6, 25,
		2,  51,  6,  47,  10, 51,  6, 55 };

	Polygon(hdc, ptColon[0], 4);
	Polygon(hdc, ptColon[1], 4);
	OffsetWindowOrgEx(hdc, -12, 0, NULL);
}
