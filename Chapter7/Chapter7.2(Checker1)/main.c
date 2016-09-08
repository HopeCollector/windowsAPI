#include <windows.h>

#define PAINT 0x00
#define UNPAINT 0x01

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Checker1");
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
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Checker1"),
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
	static int cxClient, cyClient, cxColnum, cyColnum;
	static RECT rect[25];
	static TCHAR paint[25];
	static int last = 25;
	int i;
	int x, y;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		cxColnum = cxClient / 5;
		cyColnum = cyClient / 5;

		for (i = 0; i < 25; i++)
		{
			rect[i].left = cxColnum * (i % 5);
			rect[i].top = cyColnum * (i / 5);
			rect[i].right = cxColnum * (i % 5 + 1);
			rect[i].bottom = cyColnum * (i / 5 + 1);

			paint[i] = UNPAINT;
		}
		return 0;

	case WM_MOUSEMOVE:
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		i = x / cxColnum + y / cyColnum * 5;

		if (paint[i] == UNPAINT && last != i && last!= 25)
		{
			hdc = GetDC(hwnd);

			FillRect(hdc, &rect[i], CreateSolidBrush(RGB(250, 250, 250)));

			if (paint[last] == UNPAINT)
			{

				FillRect(hdc, &rect[last], (HBRUSH)GetStockObject(WHITE_BRUSH));

				FrameRect(hdc, &rect[last], (HBRUSH)GetStockObject(BLACK_BRUSH));
			}

			ReleaseDC(hwnd, hdc);
		}

		last = i;

		return 0;

	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		i = x / cxColnum + y / cyColnum * 5;

		if (paint[i] == UNPAINT)
		{
			hdc = GetDC(hwnd);
			SelectObject(hdc, GetStockObject(GRAY_BRUSH));

			FillRect(hdc, &rect[i], (HBRUSH)GetStockObject(GRAY_BRUSH));

			MoveToEx(hdc, rect[i].left, rect[i].top, NULL);
			LineTo(hdc, rect[i].right, rect[i].bottom);

			MoveToEx(hdc, rect[i].right, rect[i].top, NULL);
			LineTo(hdc, rect[i].left, rect[i].bottom);

			ReleaseDC(hwnd, hdc);
			paint[i] = PAINT;
		}
		else {
			hdc = GetDC(hwnd);

			FillRect(hdc, &rect[i], (HBRUSH)GetStockObject(WHITE_BRUSH));

			ReleaseDC(hwnd, hdc);
			paint[i] = UNPAINT ;
		}
		
		return 0;

	case WM_PAINT:
		
			hdc = BeginPaint(hwnd, &ps);

			for (i = 0; i < 5; i++)
			{
				MoveToEx(hdc, rect[i].left, rect[i].top, NULL);
				LineTo(hdc, rect[20 + i].left, rect[20 + i].bottom);

				MoveToEx(hdc, rect[5 * i].left, rect[5 * i].top, NULL);
				LineTo(hdc, rect[4 + i * 5].right, rect[4 + i * 5].top);

			}

			MoveToEx(hdc, rect[4].right, rect[4].top, NULL);
			LineTo(hdc, rect[24].right, rect[24].bottom);

			MoveToEx(hdc, rect[20].left, rect[20].bottom, NULL);
			LineTo(hdc, rect[24].right, rect[24].bottom);

			EndPaint(hwnd, &ps);
		
	
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}