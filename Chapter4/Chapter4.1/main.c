#include <Windows.h>
#include "SysMets.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets1");
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
		TEXT("Get System Metrics No.1"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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
	TEXTMETRIC tm;
	HDC hdc;
	TCHAR szBuffer[10];
	PAINTSTRUCT ps;
	int i;
	int y;
	static int cxChar;
	static int cxCap;
	static int cyChar;
	static int cxClient;
	static int cyClient;
	static int iVscrollpos;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAscent;
		cxCap = (tm.tmPitchAndFamily & 1 ? 3 : 2) / 2 * cxChar;
		cyChar = tm.tmHeight;
		
		SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
		SetScrollPos(hwnd, SB_VERT, GetScrollPos(hwnd, SB_VERT), TRUE);
		
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		y = -iVscrollpos * cyChar;

		for (i = 0; i < NUMLINES; i++)
		{
			TextOut(hdc, 0, y + cyChar * i, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));

			TextOut(hdc, 22 * cxCap, y + cyChar * i, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, 22 * cxCap + 40 * cxChar, y + cyChar * i, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);

		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iVscrollpos -= 1;
			break;
		case SB_LINEDOWN:
			iVscrollpos += 1;
			break;
		case SB_PAGEUP:
			iVscrollpos -= cyClient / cyChar;
			break;
		case SB_PAGEDOWN:
			iVscrollpos += cyClient / cyChar;
			break;
		case SB_THUMBTRACK:
			iVscrollpos = HIWORD(wParam);
			break;
		default:
			break;
		}
		iVscrollpos = max(0,min(NUMLINES-1, iVscrollpos));
		SetScrollPos(hwnd, SB_VERT, iVscrollpos, TRUE);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}