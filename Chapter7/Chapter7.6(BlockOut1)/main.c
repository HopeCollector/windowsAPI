#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR dzCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("BlockOut1");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Mouse Button Demo"),
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
	static RECT rect, LasrPos, LastRect;
	static POINT pos1, pos2;
	static int cxClient, cyClient;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		pos1.x = LOWORD(lParam);
		pos1.y = HIWORD(lParam);
		SetCapture(hwnd);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_MOUSEMOVE:
		
		if (wParam == MK_LBUTTON)
		{
			
			InvalidateRect(hwnd, NULL, FALSE);
			hdc = BeginPaint(hwnd, &ps);
			
			SetROP2(hdc, R2_NOT);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, LasrPos.left, LasrPos.top, LasrPos.right, LasrPos.bottom);
			Rectangle(hdc, pos1.x, pos1.y, LOWORD(lParam), HIWORD(lParam));
			SetRect(&LasrPos, pos1.x, pos1.y, LOWORD(lParam), HIWORD(lParam));

			EndPaint(hwnd, &ps);
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_LBUTTONUP:
		pos2.x = LOWORD(lParam);
		pos2.y = HIWORD(lParam);

		hdc = GetDC(hwnd);
		
		SetROP2(hdc, R2_NOT);
		FillRect(hdc, &LastRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		SetRect(&rect, pos1.x, pos1.y, pos2.x, pos2.y);
		FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		CopyRect(&LastRect, &rect);

		ReleaseCapture();
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}