#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define DRAW 1;
#define ERASE 0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void RandRect(HWND, POINT, int );

static int cxClient, cyClient;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("AltWind");
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

	hwnd = CreateWindow(szAppName, TEXT("Alternate and Winding Fill Modes"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	POINT pt;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON || wParam & MK_RBUTTON)
		{

			switch (wParam)
			{
			case MK_LBUTTON:
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				RandRect(hwnd, pt, 1 );
				break;

			case MK_RBUTTON:
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				RandRect(hwnd, pt, 0);
				break;
			}

			

		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
	}
	DefWindowProc(hwnd, message, wParam, lParam);
}

void RandRect(HWND hwnd, POINT pt, int bool)
{
	HDC hdc;
	HBRUSH hb;
	RECT rect;
	int r, g, b;

	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;

	hdc = GetDC(hwnd);
	if (bool)
	{
		hb = CreateSolidBrush(RGB(r, g, b));

		switch (rand() % 4)
		{
		case 0:
			SetRect(&rect,  pt.x, pt.y,  pt.x + 20, pt.y + 20);
			break;							  			   
											  			   
		case 1:								  			   
			SetRect(&rect,  pt.x, pt.y,  pt.x - 20, pt.y + 20);
			break;							  			   
											  			   
		case 2:								  			   
			SetRect(&rect,  pt.x, pt.y,  pt.x + 20, pt.y - 20);
			break;							  			   
											  			   
		case 3:								  			   
			SetRect(&rect,  pt.x, pt.y,  pt.x - 20, pt.y - 20);
			break;

		}

		FillRect(hdc, &rect, hb);
		DeleteObject(hb);

	}
		else {
			SetRect(&rect, pt.x + 50, pt.y + 50, pt.x - 50, pt.y - 50);
			FrameRect(hdc, &rect, GetStockObject(BLACK_BRUSH));
			FillRect(hdc, &rect, GetStockObject(WHITE_BRUSH));
		}
	

	ReleaseDC(hwnd, hdc);
}