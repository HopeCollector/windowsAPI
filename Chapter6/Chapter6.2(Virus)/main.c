#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
		WS_POPUP | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	LONG style = GetWindowLong(hwnd, GWL_STYLE);//��ô��ڷ��   
	style = WS_CAPTION | WS_SIZEBOX;//����ȫ����ʾ�Ҳ��ɸı��С   
	SetWindowLong(hwnd, GWL_STYLE, style); //���ô��ڷ��   
	int screenX = GetSystemMetrics(SM_CXSCREEN);//��ȡ������Ļ���½�X����   
	int screenY = GetSystemMetrics(SM_CYSCREEN);//��ĻY����   
	SetWindowPos(hwnd, NULL, 0, 0, screenX, screenY, SWP_NOZORDER);//�ı䴰��λ�á��ߴ��Z��  

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
	RECT rect;

	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		GetClientRect(hwnd, &rect);
		rect.bottom = rect.left = rect.right = rect.top = 10;
		ClipCursor(&rect);

		return 0;

	
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_CHAR:
		GetClientRect(hwnd, &rect);
		rect.bottom = rect.left = rect.right = rect.top = 10;
		ClipCursor(&rect);

		return 0;//�޷�����alt+������ϼ�

	case WM_DESTROY:
		PostQuitMessage(0);
	}
	DefWindowProc(hwnd, message, wParam, lParam);
}