#include <Windows.h>

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TimerProc(HWND, UINT , UINT, DWORD);

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
	switch (message)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, 1000, TimerProc);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	static BOOL fColor = FALSE;
	HDC hdc;
	HBRUSH hBrush;
	RECT WholeClient;

	GetClientRect(hwnd, &WholeClient);
	fColor ^= 1;

	hBrush = CreateSolidBrush(fColor ? 0xFAEBD7 : 0xFAEBD7 ^ 0xFFFFFF);
	hdc = GetDC(hwnd);

	FillRect(hdc, &WholeClient, hBrush);
	
	ReleaseDC(hwnd, hdc);
	DeleteObject(hBrush);

	return 0;
}