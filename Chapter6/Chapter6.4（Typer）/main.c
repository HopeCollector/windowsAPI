#include <windows.h>

#define Buffer(x,y) *(pBuffer + y * (cxBuffer-1) + x)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Typer");
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

	hwnd = CreateWindow(szAppName, TEXT("Typing Program"),
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
	TEXTMETRIC tm;
	static TCHAR * pBuffer;
	static POINT pos;
	static DWORD   dwCharSet = DEFAULT_CHARSET;
	static int cxChar, cyChar, cxClient, cyClient;
	static int cxBuffer, cyBuffer;
	int x, y;

	switch (message)
	{
		case WM_INPUTLANGCHANGE:
			dwCharSet = wParam;

		case WM_CREATE:
			hdc = GetDC(hwnd);

			SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0,
				dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
			GetTextMetrics(hdc, &tm);

			cxChar = tm.tmAveCharWidth + 1;
			cyChar = tm.tmHeight + tm.tmExternalLeading;

			DeleteObject(GetStockObject(SYSTEM_FONT));
			ReleaseDC(hwnd, hdc);

		case WM_SIZE:
			if (message == WM_SIZE)
			{
				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
			}

			cxBuffer = cxClient / cxChar;
			cyBuffer = cyClient / cyChar;

			if (pBuffer != NULL)
				free(pBuffer);

			pBuffer = (TCHAR*)malloc(cxBuffer * cyBuffer * sizeof(TCHAR));

			for (x = 0; x < cxBuffer; x++)
				for (y = 0; y < cyBuffer; y++)
					Buffer(x, y) = ' ';

			InvalidateRect(hwnd, NULL, TRUE);
			return 0;

		case WM_KEYDOWN:
			HideCaret(hwnd);
			if (lParam != 3)
			{
				GetCaretPos(&pos);
				pos.x /= cxChar;
				pos.y /= cyChar;
			}

			switch (wParam)
			{
			case VK_UP:
				pos.y = max(pos.y - 1, 0);
				break;

			case VK_DOWN:
				pos.y = min(pos.y + 1, cyBuffer);
				break;

			case VK_LEFT:
				pos.x = max(pos.x - 1, 0);
				break;

			case VK_RIGHT:
				pos.x = min(pos.x + 1, cxBuffer);
				break;

			case VK_DELETE:
				for (y = pos.y; y < cyBuffer; y++)//ÓÐÎÊÌâ
					for (x = y == pos.y ? pos.x : 0; x < cxBuffer; x++)
					{
						if (x < cxBuffer - 1)
						{
							Buffer(x, y) = Buffer(x + 1, y);
						}
						else if(pos.y + 1 < cyBuffer){
							Buffer(x, y) = Buffer(0, y + 1);
						}
					}

				Buffer(cxBuffer * cxChar, cyBuffer * cyChar) = ' ';
				hdc = GetDC(hwnd);

				SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0,
					dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
				SetMapMode(hdc, MM_ANISOTROPIC);
				SetWindowExtEx(hdc, 1, 1, NULL);
				SetViewportExtEx(hdc, cxChar, cyChar, NULL);

				for(y = 0; y < cyBuffer; y++)
					for(x = 0; x < cxBuffer - 1; x++)
						TextOut(hdc, x, y, &Buffer(x, y), 1);

				DeleteObject(GetStockObject(SYSTEM_FONT));
				ReleaseDC(hwnd, hdc);
				break;

			default:
				break;
			}
			SetCaretPos(pos.x * cxChar, pos.y * cyChar);
			ShowCaret(hwnd);
			return 0;

		case WM_CHAR:
			HideCaret(hwnd);
			if (lParam != 3)
			{
				GetCaretPos(&pos);
				pos.x /= cxChar;
				pos.y /= cyChar;
			}
			switch (wParam)
			{
			case '\b':
				if (pos.x > 0)
				{
					pos.x--;
					SendMessage(hwnd, WM_KEYDOWN, VK_DELETE, 3);
				}
				break;

			case '\r':
				if (++pos.y <= cyBuffer)
					pos.x = 0;
				else
					pos.y = cyBuffer;
				
				break;

			case '\t':
				for (x = 0; x < 4 && pos.x < cxBuffer; x++)
					SendMessage(hwnd, WM_CHAR, ' ', 3);
				break;

			default:
				
				if (wParam < 32 || wParam > 126)
					break;
				
				if (pos.x + 1 > cxBuffer)
					break;

				Buffer(pos.x, pos.y) = (TCHAR)wParam;
				hdc = GetDC(hwnd);

				SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0,
					dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
				SetMapMode(hdc, MM_ANISOTROPIC);
				SetWindowExtEx(hdc, 1, 1, NULL);
				SetViewportExtEx(hdc, cxChar, cyChar, NULL);

				TextOut(hdc, pos.x, pos.y, &Buffer(pos.x, pos.y), 1);
				pos.x++;

				DeleteObject(GetStockObject(SYSTEM_FONT));
				ReleaseDC(hwnd, hdc);
				
				break;
			}
			SetCaretPos(pos.x * cxChar, pos.y * cyChar);
			ShowCaret(hwnd);
			return 0;

		case WM_PAINT:
			HideCaret(hwnd);
			hdc = BeginPaint(hwnd, &ps);

			for (x = 0; x < cxBuffer; x++)
				for (y = 0; y < cyBuffer; y++)
					TextOut(hdc, x, y, &Buffer(x, y), 1);

			EndPaint(hwnd, &ps);
			pos.x = 0;
			pos.y = 0;
			SetCaretPos(pos.x, pos.y);
			ShowCaret(hwnd);
			return 0;

		case WM_SETFOCUS:
			CreateCaret(hwnd, NULL, cxChar, cyChar);
			GetCaretPos(&pos);
			SetCaretPos(pos.x, pos.y);
			ShowCaret(hwnd);
			return 0;

		case WM_KILLFOCUS:
			HideCaret(hwnd);
			DestroyCaret();
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
		}
		DefWindowProc(hwnd, message, wParam, lParam);
	}
