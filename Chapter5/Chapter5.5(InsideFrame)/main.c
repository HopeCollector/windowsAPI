#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("InisdeFrame");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program Requires WindowsNT"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(
		szAppName,
		TEXT("LineDemo"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

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
	HPEN hpen;
	LOGPEN logpen;
	static int cxClient, cyClient;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, GetStockObject(NULL_BRUSH));

		hpen = CreatePen(PS_INSIDEFRAME, 10, RGB(255, 0, 0));
		SelectObject(hdc, hpen);
		Rectangle(hdc, cxClient / 6, cyClient / 3, cxClient / 3, cyClient / 3 * 2);
		
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		Rectangle(hdc, cxClient / 6, cyClient / 3, cxClient / 3, cyClient / 3 * 2);
		
		hpen = CreatePen(PS_SOLID, 10, RGB(255, 0, 0));
		SelectObject(hdc, hpen);
		Rectangle(hdc, cxClient / 6 * 4, cyClient / 3, cxClient / 6 * 5, cyClient / 3 * 2);

		SelectObject(hdc, GetStockObject(BLACK_PEN));
		Rectangle(hdc, cxClient / 6 * 4, cyClient / 3, cxClient / 6 * 5, cyClient / 3 * 2);

		EndPaint(hwnd, &ps);

		return 0;
	
	case WM_DESTROY:
		PostQuitMessage(0);
	}
	DefWindowProc(hwnd, message, wParam, lParam);
}