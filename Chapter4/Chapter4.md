#4.1�����ı����
* Windowsʲô�����ܾ�����ֻ��������������ֻ��ͨ��������ȷ���Լ��Ĵ����ж���Լ�������ַ��ж��
* Windows����Ϣ������һ�л��**Ͷ����Ϣ**��������У���**������Ϣ**���Ƕ�����Ϣ��
* �������`WM_PAINT`�ᱻ����
	* �û��ƶ����ڣ�ʹԭ���Ĵ��ڱ�¶����
	* �������ڳߴ�
	* �������`ScrollWindow`����`ScrollDC`��������
	* �������`InvalidateRect`����`InvalidateRgn`������ʽ����`WM_PAINT`��Ϣ
* �������`WM_PAINT`���ܱ����ͣ���Windows���顣����
	* Windows�ر���һ�������˲��ִ��ڵĶԻ������Ϣ��
	* �����˵����ջ�
	* ��ʾ��ʾ��Ϣ������ɶ��˼��
* �ػ�ʱ����Ҫ���������ͻ�����ֻҪ��**��Ч����**���и��¾Ϳ�����
* �ػ����
	* �ͻ����ڴ�����Ч����ʱWindows������Ϣ�����з���һ��`WM_PAINT`
	* ��Ϣ�����в�����ֶ���`WM_PAINT`
	* ���Ѿ�������Ч���ε�����£��������µ���Ч���򣬾��δ�С�������¼���
#4.2����GDI���
####�豸����
* ���ض��������豸������ĳЩ������������ͼ������
* �����ڻ�����ɺ����Ҫ�ͷ��豸����������ͷ�֮�������ã��ĺ󼴷٣�
* ����`CreateWindow()`��������������Ϣ֮�䴫���豸�������
**��ȡ����**
1. ʹ��`BeginPaint(), EndPaint()`,������Ҫ��������
* `hwnd`���ھ��
* `PAINTSTRUCT ps`���ƽṹ
```C
typedef struct tagPAINTSTRUCT {
    HDC         hdc;
    BOOL        fErase;
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT
```
* `HDC hdc`����������Ҫ���Ǹ�`hdc=BeginPaint()`
* `BOOL fErase`ָ���Ƿ���Ҫ�ػ棬ͨ������Ϊ���ò��������˼ҷ���
* `RECT rePaint`һ���ṹ�壬��������Ч���εı߽磬���л��ƶ�ֻ�����������
*`BeginPaint()`��������*
	* ����`wndclass.hbrBackground`��ȥ����
	* ���ps�ĸ����ֶ�
	* ����һ���豸��������
* ��`WM_PAINT`��Ϣ�У�һ��Ҫʹ�����ͻ�����Ч����������ػ淨��Windows��һֱͶ����Ϣ
* `InvalidateRect(LPRECT lpRect, BOOL bErase)`ָ��ĳһ����ʹ����Ч��
	* `LPRECT lpRect`ָ��һ��CRect����RECT�ṹ�壬ָ���������������`NULL`��Ϊ�����ͻ���
	* `BOOL bErase`ָ���Ƿ����ĳһ����
* ��ʡ�µ���ֱ���ػ������ͻ�������ȥ���κ���Ч����
* ��ʡʱ���ʹ����Ч���α��ⲻ��Ҫ��GDI��������
2. ʹ��`GetDC(hwnd)`��`ReleaseDC(hwnd, hdc)`
* `GetDC()`�����úܵ����������ܰ���Ч������Ч�������Һֱܴ����ῼ����Ч���Σ��õ���`hdc`ָ�������ͻ���
* `ValidateRect(hwnd, LPRECT lpRect)`������`InvalidateRect()`�෴��ǰ��������һ���������ˣ�
* `GetWindowDC(hwnd)`���Ի�ȡ�������ڵ��豸����������ȿͻ�����һ�㣩����ҲҪ����`WM_NEPAINT`��Ϣ
**����̽�֡�`TextOut(hdc, x, y, psText, iLength)`**
* `hdc`�豸���������Ҫ���������������Ӧ���ڼ�������GDI����
	* �豸�����е����Ծ������ı���ʾ�����Ա������壬�ֺţ��ı�����ɫ
	* �ı�����ɫ�����ı�����ʾ����֮һ�����Ϳͻ���������һ���������Ǳ��ı���Ȧ������ģ��ı�����ɫ��������ı����õ�
* `x, y`���ֳ��ֵ���ʼ���꣬��ϸ�µ�λ�ö�λ
	* ȷ���˵�һ���ַ����Ͻǵ�����
* `psText`ָ���ַ�����ָ�룬ָ���������
	* �ַ����в�Ӧ�����κ�ASCII�����ַ�
	* ��Ϊ Unicode �ַ����򳤶�Ӧ����`iLength`������
* `iLength`�ַ������ȣ�����֪���Լ�Ӧ��������١�����
	* `TextOut����`��������ַ�������`\0`��ȫ��`iLength`ȷ���������
####�ı�
* ��ȡ�ı���Ϣʹ��`GetTextMetrics(hdc�� LPTEXTMERTICS)`��������дһ���г�Ա�Զࣨ20����Ա����`TEXTMETRIC`�ṹ��
```c
TEXTMETRIC tm;
HDC hdc;
hdc = GetDC(hwnd);
GetTextMetric(hdc, &tm);
...
ReleaseDC(hwnd, hdc);
```
**TEXTMETRIC�е���Ҫ��Ա**
* `tmHight`���߶��ܺͣ�= `tmAscent`(�����ϣ�+`teDescent`�������£�
* `tmInternalLeading`�������Ÿ߶ȣ�������`tmHight`
* `tmExternalLeading`��������ĳ�Ա�����ʦ�����м��
* `tmAveCharWidth`Сд��ĸ�ļ�Ȩƽ�����ȣ�ʲô�Ǽ�Ȩ��
* `tmMaxCahrWidth`����������ַ��Ŀ���
* `tmPitchAndFamily`��ʶ�Ǳ�����壨1�����ǵȿ����壨0��
* ��д�ַ�ƽ������ʱСд��1.5������
* �ַ��ߴ���Բ����òµģ�
* `SetTextAlign(hdc, nFlags)`�趨�ı���**����**λ��
####�ͻ����ߴ�
* �����`GetClientRect()`��˵������`WM_SIZE`�����
	* lParam��**��λ�ֶ�**��ʾ�ͻ����Ŀ��ȣ�**��λ�ֶ�**��ʾ�߶�
```c
static int cxClient, cyClient;
...
case WM_SIZE:
	cxClient = LOWORD(lParam);
	cyClient = HIWORD(lParam);
	return 0;
```
* ������һ���ô�ֵĳ���Σ���Ȼ�ձ飬���Ǻ���
####������
* Windows���Ի�����Ϊ�����������Ϲ�����ζ�������ĵ��Ŀ�ʼ����֮���������
* ����һ���������൱��
```c
 hwnd = CreateWindow (szAppName, 
		TEXT ("Get System Metrics No. 2"),
                          	WS_OVERLAPPEDWINDOW | WS_VSCROLL,	//�������������
                          	CW_USEDEFAULT, CW_USEDEFAULT,
                          	CW_USEDEFAULT, CW_USEDEFAULT,
                         	 NULL, NULL, hInstance, NULL) ;
```
* ��������������*���ڷ��*���������������������һ������`WS_VSCROLL`����`WS_HSCROLL`�Ϳ����ˣ�ͬʱ��������Ҳû���⣩
* ��������λ���ڿͻ���֮��
**��������Χ**
* Ĭ��ֵΪ0~100��ʹ��`SetScrollRange(hwnd, iBar, iMin, iMax,BOOL bRedraw)`������
	* `iBar`���ߺ���Ҫ����ʲô����Ĺ���������������`SB_VERT`��`SB_HORZ`��
	* `iMin``iMax`�����ù�������Χ�ģ���������λ������ɢ������ֵ
	* `bRedraw`�������������Ƿ���Ҫ�����ػ棨֮�������������������ĺ�������ñ��ػ棬�˷ѣ���
	* ��������������Ĺ�������С���ܱ�.....
* ȷ���÷�Χ��Ҫ�趨λ��`SetScrollPos(hwnd, iBar, iPos, bRedraw)`
	* `iPos`Ҫ��`iMin`��`iMax`֮��
* ʹ��`GetScrollRange(hwnd, iBar, lpMin, lpMax)`��ȡ��������Χ
* ʹ��`GetScrollPos(hwnd, iBar)`��ȡ������λ�ã���һ��int�͵ķ���ֵ��
* ��������ʵ���ǳ����Windows���cp��ͬ�ĳɹ�
Windows����|��������
--------------|---------
�����������е������Ϣ | ��ʼ���������ķ�Χ��λ��
�ṩ����������ʱ�ķ�����˸ | �������͸����ڹ��̵Ĺ�������Ϣ
�û���ק����ʱ�ڹ��������ƶ����� | ���»���λ��
����Ӧ�Ĵ��ڹ��̷��͹�������Ϣ | ���ݹ������ı仯���¿ͻ���������
**��������Ϣ**
* `WM_VSCROLL`, `WM_HOCROLL`��`lParam`������`WM_SIZE`���������ã��ڶԻ��������ã���������`wParam`������
	* ���ֶ�λ��������ڹ������ϵĶ���������ʱ�ᷢ��������Ϣ������һ�������ɿ�ʱ�ᷢ����һ��`SB_ENGSCROLL`
	* �϶�����ʱ�ᷢ��`SB_THUMBTRACK`��Ϣ��`wParam`�ĵ�λ�ֶΣ�����ʱ��λ�ֶ��� **����ĵ�ǰλ��**
	* `wParam`�ĵ�λ�ֶ���`SB_THUMBPOSITION`ʱ����λ�ֶ����û��ɿ����ʱ���������λ��
	* ����`SB_THUMBTRACK`��������϶�����ʱ�ƶ��ͻ���������
	* ����`SB_THUMBPOSITION`��������û��ɿ����ʱ���¿ͻ���������
**��ţ�ƵĹ�����**
* `SetScrollInfo(hwnd, iBar, PSSCROLLINFO, bRedraw)`�������Ļ����ǿ��Ըı��С��
	* �����С/����������=�ĵ���ʾ��/�ĵ��ܴ�С=ҳ���С/��Χ��ʲô�Ƿ�Χ��
	* `PSSCROLLINFO`�ǽṹ��ָ�룬ָ��`SCROLLINFO`
```c
typedef struct tagSCROLLINFO
{
    UINT    cbSize;
    UINT    fMask;
    int     nMin;
    int     nMax;
    UINT    nPage;
    int     nPos;
    int     nTrackPos;
}   SCROLLINFO
```
	* `cdSize`��ʶ�ýṹ��Ĵ�С`sizeof(si)`�����ּ����ԣ���ôʵ�ֵģ�
	* `fMask`λ����ı�ʶ��
��ʶ�� | SetScrollInfo | GetScrollInfo
------- | ------------- | --------------
SIF_RANGE | nMin��nMaxָ����������Χ | nMin��nMax�Ϸ��ع�������Χ
SIF_POS | nPosָ������λ�� | nPos���ػ���λ��
SIF_PAGE | nPageָ��ҳ���С | nPage����ҳ���С
SIF_TRACKPOS | ���� | nTrackPos���ص�ǰ����λ��
SIF_DISABLENOSCROLL | ���ò���Ҫ�Ĺ�������������ʾ������ | ����
SIF_ALL | ������� | �������
**�������õ��ĺ���**
* `InvalidateRect()`ʹ�����ͻ�����Ч
* `UpdateWindow()`�˺�����ֱ�ӷ���`WM_PAINT`��Ϣ�����ڹ���
* `ScrollWindow(hwnd, XAmount, YAmount, lpRect, lpClipRect)`���������ͻ������ӿ�����ٶ�
	* `XAmount`��`YAmount`ָ�����������ظ���
	* `lpRect`ָ����������
	* `lpClipRect`����ͬ�ϣ�ֻ������������ʽ��