#include <windows.h>
#include <math.h>

#define NUM 100
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

	hwnd = CreateWindow(szAppName, TEXT("Heart"),
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
	HRGN hDrgn;
	POINT apt[50], aptUp[50], aptDown[50], pt;
	static int cxClient, cyClient;
	int i, u = 0, d = 0;
	int conUp = 0, conDown = 0;
	double x, y, t;
	int flag;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 300, -300, NULL);
		SetViewportExtEx(hdc, cxClient / 2, cyClient / 2, NULL);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);

		for(x = -1.5 ; x <= 1.5 ; x += 0.1)
		{
			flag = 1;
			for (y = -1.5 ; y <=1.5 ; y += 0.1)
			{
				t = (x*x + y*y - 1)*(x*x + y*y - 1)*(x*x + y*y - 1);
				if ((t - x*x*y*y*y) <= 0.0f && flag)
				{
					aptDown[u].x = (int)(x * NUM);
					aptDown[u].y = (int)(y * NUM);
					u++;
					flag = 0;
				}
				if ((t - x*x*y*y*y) <= 0.0f)
				{
					pt.x = (int)(x * NUM);
					pt.y = (int)(y * NUM);
				}
			}
			if (aptDown[--u].y != pt.y && flag == 0)
			{
				aptUp[d].x = pt.x;
				aptUp[d].y = pt.y;
				d++;
			}
			u++;
		}
		conDown = d - 1;

		for (i = 0; i < u + d; i++)
		{
			switch (i < u)
			{
			case 1:
				apt[i].x = aptUp[conUp].x;
				apt[i].y = aptUp[conUp].y;
				conUp++;
				break;
			case 0:
				apt[i].x = aptDown[conDown].x;
				apt[i].y = aptDown[conDown].y;
				conDown--;
				break;
			}
		}

		Polygon(hdc, apt, i);
		
		EndPaint(hwnd, &ps);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
	}
	DefWindowProc(hwnd, message, wParam, lParam);
}