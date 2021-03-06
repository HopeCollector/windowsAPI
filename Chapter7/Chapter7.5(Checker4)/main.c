#include <Windows.h>

#define LIMATE 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szChildClassName[] = TEXT("Checker3_Child");
int idFocus = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Checker3");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	wndclass.cbWndExtra = sizeof(long);
	wndclass.hIcon = NULL;
	wndclass.lpfnWndProc = ChildWndProc;
	wndclass.lpszClassName = szChildClassName;

	RegisterClass(&wndclass);

	hwnd = CreateWindow(szAppName, TEXT("Checker"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL,
		hInstance, NULL);

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
	static HWND hChildWnd[LIMATE][LIMATE];
	static int cxBlock, cyBlock;
	int x, y;

	switch (message)
	{
	case WM_CREATE:
		for (x = 0; x < LIMATE; x++)
			for (y = 0; y < LIMATE; y++)
				hChildWnd[x][y] = CreateWindow(szChildClassName,
					NULL,
					WS_CHILDWINDOW | WS_VISIBLE,
					0, 0, 0, 0,
					hwnd,
					(HMENU)(y << 8 | x),
					(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					NULL);
		return 0;

	case WM_SETFOCUS:
		SetFocus(GetDlgItem(hwnd, idFocus));
		return 0;

	case WM_KEYDOWN:
		x = idFocus & 0xFF;
		y = idFocus >> 8;

		switch (wParam)
		{
		case VK_UP:
			y--;
			break;

		case VK_DOWN:
			y++;
			break;

		case VK_LEFT:
			x--;
			break;

		case VK_RIGHT:
			x++;
			break;

		default:
			break;

		}
		x = (x + LIMATE) % LIMATE;
		y = (y + LIMATE) % LIMATE;

		idFocus = y << 8 | x;

		SetFocus(hChildWnd[x][y]);

		return 0;

	case WM_SIZE:
		cxBlock = LOWORD(lParam) / LIMATE;
		cyBlock = HIWORD(lParam) / LIMATE;

		for (x = 0; x < LIMATE; x++)
			for (y = 0; y < LIMATE; y++)
				MoveWindow(hChildWnd[x][y],
					cxBlock * x, cyBlock * y,
					cxBlock, cyBlock, TRUE);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HPEN hpen;

	switch (message)
	{
	case WM_CREATE:
		SetWindowLong(hwnd, 0, 0);
		return 0;

	case WM_LBUTTONDOWN:
		SetWindowLong(hwnd, 0, 1 ^ GetWindowLong(hwnd, 0));
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;

	case WM_KEYDOWN:
		if (wParam != VK_SPACE && wParam != VK_RETURN)
		{
			SendMessage(GetParent(hwnd), message, wParam, lParam);
			return 0;
		}
		SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
		return 0;

	case WM_SETFOCUS:
		idFocus = GetWindowLong(hwnd, GWL_ID);

	case WM_KILLFOCUS:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		Rectangle(hdc, 0, 0, rect.right, rect.bottom);
		if (hwnd == GetFocus())
		{
			hpen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
			SelectObject(hdc, hpen);

			Rectangle(hdc, rect.right / 10, rect.bottom / 10, rect.right / 10 * 9, rect.bottom / 10 * 9);

			SelectObject(hdc, GetStockObject(BLACK_PEN));
			DeleteObject(hpen);

		}
		if (GetWindowLong(hwnd, 0))
		{
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, rect.right, rect.bottom);

			MoveToEx(hdc, rect.right, rect.top, NULL);
			LineTo(hdc, rect.left, rect.bottom);
		}

		EndPaint(hwnd, &ps);
		return 0;

	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}