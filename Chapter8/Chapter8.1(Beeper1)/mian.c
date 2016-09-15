#include <Windows.h>

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Beeper1");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.lpfnWndProc = WinProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;

	RegisterClass(&wndclass);
	hwnd = CreateWindow(szAppName, TEXT("Beeper1"),
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

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	PAINTSTRUCT	ps;
	HDC hdc;
	HBRUSH hBrush;
	static int fColor;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 500, NULL);
		GetClientRect(hwnd, &rect);
		return 0;

	case WM_TIMER:
		InvalidateRect(hwnd, NULL, FALSE);
		fColor ^= 1;
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		hBrush = CreateSolidBrush(fColor ?  0xD2691E: 0xD2691E ^ 0xFFFFFF);
		SelectObject(hdc, hBrush);
		Rectangle(hdc, 0, 0, rect.right, rect.bottom);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}