#include <Windows.h>
#include "devcaps.h"


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
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
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
	SCROLLINFO si;
	int i;
	int y;
	int x;
	int iPaintBeg;
	int iPaintEnd;
	static int cxChar;
	static int cxCap;
	static int cyChar;
	static int cxClient;
	static int cyClient;
	static int iVscrollpos;
	static int iHscrollpos;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAscent;
		cxCap = (tm.tmPitchAndFamily & 1 ? 3 : 2) / 2 * cxChar;
		cyChar = tm.tmHeight;
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVscrollpos = si.nPos;

		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHscrollpos = si.nPos;

		iPaintBeg = max(0, ps.rcPaint.top / cyChar + iVscrollpos);
		iPaintEnd = min(NUMLINES - 1, iVscrollpos + ps.rcPaint.bottom / cyChar);

		for (i = iPaintBeg; i <= iPaintEnd; i++)
		{
			x = cxChar* (0 - iHscrollpos);
			y = cyChar * (i - iVscrollpos);

			TextOut(hdc, x, y, devcaps[i].szLabel, lstrlen(devcaps[i].szLabel));

			TextOut(hdc, x + 22 * cxCap, y, devcaps[i].szDesc, lstrlen(devcaps[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, x + 22 * cxCap + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(devcaps[i].iIndex)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);

		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_VSCROLL:
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVscrollpos = si.nPos;
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hwnd, SB_VERT, &si);
		if (si.nPos != iVscrollpos)
		{
			ScrollWindow(hwnd, 0, cyChar * (iVscrollpos - si.nPos), NULL, NULL);
		}
		return 0;
	case WM_HSCROLL:
		si.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHscrollpos = si.nPos;
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			si.nPos -= 1;
			break;
		case SB_LINERIGHT:
			si.nPos += 1;
			break;
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		GetScrollInfo(hwnd, SB_HORZ, &si);
		if (si.nPos != iHscrollpos)
		{
			ScrollWindow(hwnd, cxChar * (iHscrollpos - si.nPos), 0, NULL, NULL);
		}

		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE | SIF_TRACKPOS;
		si.nMin = 0;
		si.nMax = NUMLINES;
		si.nPage = cyClient / cyChar;
		si.nPos = 0;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		si.nMax = (22 * cxCap + 40 * cxChar) / cxChar;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}