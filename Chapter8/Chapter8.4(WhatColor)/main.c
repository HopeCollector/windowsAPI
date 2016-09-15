#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID FindWindowRect(LPINT, LPINT);

int cxWindow, cyWindow;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("Define Color");
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
	
	FindWindowRect(&cxWindow, &cyWindow);

	RegisterClass(&wndclass);
	hwnd = CreateWindow(szAppName, TEXT("Let's  see what color it is"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT, cxWindow, cyWindow,
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

VOID FindWindowRect(LPINT x, LPINT y)
{
	HDC hdcScreen;
	TEXTMETRIC tm;

	hdcScreen = CreateIC(TEXT("DISPLAY"), NULL, NULL, NULL);
	GetTextMetrics(hdcScreen, &tm);
	DeleteDC(hdcScreen);

	*x = 2 * GetSystemMetrics(SM_CXBORDER) + 20 * tm.tmAveCharWidth + 100;
	*y = 2 * GetSystemMetrics(SM_CYCAPTION) + 2 * tm.tmHeight;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR szBuffer[20];
	int r, g, b;
	static RECT rect, simple;
	static HDC hdcScreen;
	 COLORREF color;
	static POINT MousePos;

	switch (message)
	{
	case WM_CREATE:
		hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
		GetClientRect(hwnd, &rect);
		rect.right -= 100;

		SetTimer(hwnd, 1, USER_TIMER_MINIMUM, NULL);
		return 0;

	case WM_TIMER:

		InvalidateRect(hwnd, NULL, FALSE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetCursorPos(&MousePos);

		color = GetPixel(hdcScreen, MousePos.x, MousePos.y);

		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		SetRect(&simple, cxWindow - 100, 0, cxWindow, cyWindow);
		FillRect(hdc, &simple, CreateSolidBrush(RGB(r, g, b)));

		wsprintf(szBuffer, TEXT("R:%02X G:%02X B:%02X"),
			GetRValue(color), GetGValue(color), GetBValue(color));
		DrawText(hdc, szBuffer, -1, &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hwnd, &ps);

		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}