#include <windows.h>
#include <math.h>

#define NUM 1000
#define TWOPI (2 * 3.14159)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("CLOVER");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("CLOVER"),
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
	HRGN hDrgn, hrgn1, hrgn2, hrgn3, hrgn4, hrgn5, hrgn6;
	RECT rect1, rect2, rect3, rect4;
	static int cxClient, cyClient;
	int i;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 1000, -1000, NULL);
		SetViewportExtEx(hdc, cxClient / 2, cyClient / 2, NULL);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);

		SetRect(&rect1, 0, cyClient / 3, cxClient / 2, 2 * cyClient / 3);
		SetRect(&rect2, cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3);
		SetRect(&rect3, cxClient / 3, 0, 2 * cxClient / 3, cyClient / 2);
		SetRect(&rect4, cxClient / 3, cyClient / 2, 2 * cxClient / 3, cyClient);

		hrgn5 = hrgn1 = CreateEllipticRgnIndirect(&rect1);
		hrgn6 = hrgn2 = CreateEllipticRgnIndirect(&rect2);
		hDrgn = hrgn3 = CreateEllipticRgnIndirect(&rect3);
		hrgn4 = CreateEllipticRgnIndirect(&rect4);
		CombineRgn(hrgn5, hrgn1, hrgn2, RGN_OR);
		CombineRgn(hrgn6, hrgn3, hrgn4, RGN_OR);
		CombineRgn(hDrgn, hrgn5, hrgn6, RGN_XOR);
		SelectObject(hdc, hDrgn);

		for (i = 0; i <= NUM; i++)
		{
			LineTo(hdc, cos(TWOPI * i / NUM) * NUM, sin(TWOPI * i / NUM) * NUM);
			MoveToEx(hdc, 0, 0, NULL);
		}

		DeleteObject(hDrgn);
		DeleteObject(hrgn1);
		DeleteObject(hrgn2);
		DeleteObject(hrgn3);
		DeleteObject(hrgn4);
		DeleteObject(hrgn5);
		DeleteObject(hrgn6);
		EndPaint(hwnd, &ps);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
	}
	DefWindowProc(hwnd, message, wParam, lParam);
}