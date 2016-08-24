#include <Windows.h>
#define NUM 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Bezier(HDC hdc, POINT apt[]);
void MyPolyBezier(HDC hdc, POINT * apt, int cp);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Bezier");
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
		TEXT("Bezier"),
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
	static POINT apt[4];
	static int cxClient, cyClient;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		apt[0].x = cxClient / 4;
		apt[0].y = cyClient / 2;
		apt[3].x = 3 * cxClient / 4;
		apt[3].y = cyClient / 2;
		apt[1].x = apt[1].y = 0;
		apt[2].x = apt[2].y = cyClient;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON || wParam & MK_RBUTTON)
		{
			hdc = GetDC(hwnd);

			SelectObject(hdc, GetStockObject(WHITE_PEN));
			Bezier(hdc, apt);

			switch (wParam)
			{
			case MK_LBUTTON:
				apt[1].x = LOWORD(lParam);
				apt[1].y = HIWORD(lParam);
				break;

			case MK_RBUTTON:
				apt[2].x = LOWORD(lParam);
				apt[2].y = HIWORD(lParam);
				break;
			}

			SelectObject(hdc, GetStockObject(BLACK_PEN));
			Bezier(hdc, apt);

		}
		return 0;

	case WM_PAINT:
		InvalidateRect(hwnd, NULL, FALSE);
		hdc = BeginPaint(hwnd, &ps);
		Bezier(hdc, apt);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void Bezier(HDC hdc, POINT apt[])
{
	MyPolyBezier(hdc, apt, 4);

	MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
	LineTo(hdc, apt[1].x, apt[1].y);

	MoveToEx(hdc, apt[3].x, apt[3].y, NULL);
	LineTo(hdc, apt[2].x, apt[2].y);
}

void MyPolyBezier(HDC hdc, POINT apt[], int cp)
{
	POINT aptm[NUM];
	POINT aptt[NUM -1];
	POINT pt;
	int i;
	switch (cp)
	{
	case 2:
		break;

	case 3:
		break;

	case 4:
		for (i = 1; i < NUM ; i++)  // 当1/i接近1时得出来的结果变化也来越小，导致画出来的线越来越像直线，但是按照c语言的除法规则，得到的结果就是直线！
		{
			aptm[i].x = (1.0 - 1.0 / i)*(1.0 - 1.0 / i)*(1.0 - 1.0 / i) * apt[3].x + 1.0 / i * (1.0 - 1.0 / i)*(1.0 - 1.0 / i) * 3 * apt[2].x + 3.0 / i / i * (1.0 - 1.0 / i) * apt[1].x + 1.0 / i / i / i * apt[0].x;
			aptm[i].y = (1.0 - 1.0 / i)*(1.0 - 1.0 / i)*(1.0 - 1.0 / i) * apt[3].y + 1.0 / i * (1.0 - 1.0 / i)*(1.0 - 1.0 / i) * 3 * apt[2].y + 3.0 / i / i * (1.0 - 1.0 / i) * apt[1].y + 1.0 / i / i / i * apt[0].y;
		}

		for (i = 0; i < NUM - 1; i++)
		{
			aptt[i] = aptm[i + 1];
		}
		Polyline(hdc, aptt, NUM - 1);
		break;
	}
}