#3.1�������ڴ���

* Windows���Կ��Ƴ��� 
*  **���������˴��ڹ��̣����д��ڶ��ǻ��ڴ����ഴ���ģ����������ڷ���һ�������ࣩ**
* ���ڹ���
	* �����Լ�дҲ����д�붯̬���ӿ�
	* ��Ϊһ����������������Windows����Ϣ�����Դ��ڽ��в���
	* �û���>windows��>���ڹ��̡�>����
* ��Ϣ����
	* ÿ�����򶼽���һ����Ϣ����
	* �����Ǹ���Ϣ�����ô��ڹ������ȶ�ִ��
*  *�ͻ���* �������û�������ӻ���Ϣ������
*  *��ʶ��* ��CS_HREDRAW��DT_CENTER�����»���_֮ǰ��ǰ׺��ʾ��ʶ�������
* windows �����кܶ����߰�����������ͣ�����`#define`����`typedef`����ģ�Ŀ���Ǽ��Ṥ����
*  *���* ������c���ļ�ָ��
*  *��������Ƿ�* �������Ա����ñ����������͵�Сд��ĸ��ͷ
	* �����ṹ����ʱ������ʹ�ýṹ����Сд��ʽ��Ϊ������ǰ׺��������������
* ��ʼ��wndclass->ע�ᴰ����->��������->��ʾ����
####������ע��
* ���ú���`RegisterClass(&WNDCLASS)`ע�ᴰ����
* `WNDCLASS`�ǽṹ�壬��Ϊ�����ַ�����Ա�������������汾��A�棬U�棬ͨ�ð棩��ͨ�ð�ͨ��ʶ��`#define UNICODE`������ʹ��ʲô�汾
* ע�ᴰ������Ҫ��`WNDCLASS`�ṹ��ʼ��
*  **WNDCLASS**
	* `wndclass.style = CS_HREDRAW|CS_VREDRAW`����*λ���*����������class style��
	* `wndclass.lpfnWndProc = WndProc`�������봰�����Ӧ�Ĵ��ڹ��̣���c�����൱��ʹ�ú���ָ��
	* `wndcalss.cbClsExtra = 0`��`wndclass.cbWndExtra = 0`����������ṹ��Windows�ڲ�ά���Ĵ��ڽṹԤ��һЩ�ռ�ģ������ף�
	* `wndcalss.hInstance = hInstance`��ʾӦ�ó����Լ���ʵ�����
	* `wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION)`����ͼ�꣨��������ߵ�ͼ�꣩������һ��ͼ����
	* `wndclass.hCursor = LoadCursor(NULL, IDC_ARROW)`������겢����һ�������
	* `wndclass.hbrBackground = GetStockObject(WHITE_BRUSH)`Ϊ�ͻ���ָ������ɫ��������һ����ˢ���
	* `wndclass.lpszMenuName = NULL`ָ��������˵��������ף�
	* `wndclass.lpszClassname = szAppName`��������һ������
* ��16λwindows�汾�У�������ʵ������һ�������ࣨ��Լ�ڴ棩,`hPrevInstance`�ͱ�����ǰһ�������ʵ���������32λ��`hPrevIndtance`��Ϊ`NULL`
####���ڴ���
* ������ĶԴ��ڵĶ�����һ���Եģ��޷����㴰�ڵ���Ҫ�������ڴ�����Ļ����϶��崰��
* `CreateWindow()`�������崰�ڷ��ظô��ڵ�**���ھ��**
```c
hwnd = CreateWindow(
		szAppName,			//����������
		TEXT(`The Hello Program`),		//���ڱ���
		WS_OVERLAPPEDWINDOW,		//���ڷ��
		CW_USEDEFAULT,			//��ʼx����
		CW_USEDEFAULT,			//��ʼy����
		CW_USEDEFAULT,			//��ʼx����ߴ�
		CW_USEDEFAULT,			//��ʼy����ߴ�
		NULL,				//�����ھ��
		NULL,				//�˵����ھ�� 
		hInstance,				//����ʵ�����
		NULL);				//�����������ڴ��ڴ�����֮ǰ�����ã�
```
* `WS_OVERLAPPEDWINDOW`��׼�Ĵ��ڷ��֮һ��ͨ��*λ���*�õ�
* `CW_USEDEFAULT`Ĭ��ֵ��0x80000000������ֻ������ȡĬ��ֵ���ߴ�Ҳ��������ȡĬ��ֵ
* �������ڴ��ڸ��ӹ�ϵʱ���Ӵ����ڸ�����ǰ��
* Ӧ�ô���λ�����洰��ǰ��
####��ʾ����
* `ShowWindow(hwnd, iCmdShow)`�����Ѵ�����ʾ����Ļ��
	* `hwnd`��`CreateWindow()`�õ��Ĵ��ھ��
	* `iCmdShow`��`WinMain`���������ܵĲ���
		* `SW_SHOWNORMAL`������С��ʾ
		* `SW_SHOWMAXIMIZED`�����ʾ
		* `SW_SHOWMINNOACTIVE`��С����ʾ
* ���`iCmdShow`�õ��Ĳ���ʱ`SW_SHOWNORMAL`�������ͻ����ᱻ`winclass.hbrBackground`ѡ���Ļ�ˢ����
* `UpdateWindow(hwnd)`�����ػ�ͻ����������򴰿ڹ��̷���һ��`WM_PAINT`��Ϣ
####��Ϣѭ��
* Windows��������û������뵱��`��Ϣ`���ŵ�Ӧ�ó����Ӧ����Ϣ������
* ��Ϣѭ��
```c
while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
```
* `GetMessage(&msg, NULL, 0, 0)`��������Ϣ�����м�����Ϣ��������Ϣ����`msg`
	* `MSG msg`�ǽṹ��
	* `msg.message`ȡ��`WM_QUIT`��Ϣʱ����0
```c
typedef struct tagMSG {
    HWND        hwnd;	//���ھ��
    UINT        message;	//ÿ����Ϣ����ֵ
    WPARAM      wParam;	//����Ϣ��ͬ�����ò�ͬ
    LPARAM      lParam;	//ͬ��
    DWORD       time;	//��Ϣ������Ϣ���е�ʱ��
    POINT       pt;		//�ṹ�壬��ʾһ���㣬��Ϣ�������ʱ����λ��
} MSG;
```
* `POINT`�ṹ��
```c
typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT
```
* `TranslateMessage(&msg)`����ĳЩ������Ϣ
* `DispatchMessage(&msg)`��`msg`�ٴη�����Windows����������һ����Ϣ������Ϣ����봰�ڹ��̽��д���msg����һ��������������Ϣ��
####���ڹ���
* Ӧ�ó���һ�㲻����ô��ڹ��̣�ʵ�������Ǿ���`SendMessage()`����
* ���崰�ڹ��̵�һ�㷽��
```c
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
```
* ǰ�ĸ�������`MSG`��ǰ�ĸ�����һһ��Ӧ��������Ҳû�취��Ӧ����������
* �����м���������ͬ�����ഴ���Ĵ��ڣ�hwnd���Ǳ�ʶ������Ϣ�Ĵ��ڣ��ϻ�����ʶ�����Ϣ˭������
*���ڴ��ڹ�����Ҫ���������Ϣ��ʹ���������һ�����͵�switch-case...
```c
switch (iMsg)
{
case WM_CREATE:
	...
	return 0;
case WM_PAINT:
	...
	return 0;
case WM_DESTROY:
	...
	retuen 0;
}
return DefWindowProc(hwnd, iMsg, wParam, lParam);  //�Դ��ڹ���û�д������Ϣ����Ĭ�ϴ���
```
* `WM_CREATE`
	* Windows����`CreateWindow()`ʱ��WM_CREATE��Ϣ�Ϳ�ʼ�����ˣ�������GeiMessage��
	* `PlaySound`Ҫ����Ϣ������ִ�У���ִ���괰���޷���ʾ����ʾ����Ҳ����Ϣ��
* `WM_PAINT`
	* �ͻ�������Ч�����߱��롰���¡�ʱ�������õ�����Ϣ
	* ��������ͻ�����Ч
		* ���ڱ��״δ���
		* �������ڳߴ磨��С������󻯣�
		* ���ڱ��ص������ڵ����ٴ�����
	* `BeginPaint(hwnd, &ps)`��ʼ����
		* �����ڼ���������δ��������Windows��ʹ��`winclass.hbrBackground`����������
		* `BeginPaint()`����ʹ�����ͻ�����Ч
		* ����һ�����豸������������������豸����ʾ��������
	* `EndPaint(hwnd, &ps)`�������ƣ��Ͽ�`hwnd`�������豸������
	* `GetClientRect(hwnd, &rect)`���ݴ��ڳߴ�������`rect`���ĸ��ֶΣ�left, top, right, bottom)
	* `DrawText()`�����ı�
* `WM_DESTORY`
	* �û��رմ���ʱ�����ӵ�������Ϣ
	* `PostQuitMessage(0)`��һ��`WM_QUIT`��Ϣǿ�в������У�`GetMessage()`������0
# 3.2
* WinMainֻ�����˱�Ҫ��һ���ֲ����������Ĳ���������WinProc��ʵ�ֵ�
* Windowsͨ�����ô��ڹ������򴰿ڷ���Ϣ�����ֺ���Ҳ���Ե���WinProc������Ϣ����ʽ��
* �������WinProc�ᱻWindows���ã����е��ö�����Ϣ��ʽ���� 
	* �½�����
	* ���ڱ���������
	* ���ڳߴ緢���仯��������С����
	* ����ڴ���ִ�е�����˫��
	* ������Ϣ����
	* �������򵥻�������
	* �ͻ�����Ҫ�ػ�
* ��Ϣ������
	* ������Ϣ��������Ϣ���еȴ����ַ�����Ϣ
	* �Ƕ�����Ϣ��ֱ�ӱ��͵�WinProc���д���
* ��Ϣѭ���봰�ڹ��̲����ǲ������е�`DispatchMeaasge()`ִ��ʱ�ͻ����WinProc����WinProc������Ȩ���ظ�Windowsʱ`DispatchMessage()`�Ż᷵��
* ���ڹ����ڴ���ĳһ��Ϣʱ�����򲻻ᱻ������Ϣ�ж�