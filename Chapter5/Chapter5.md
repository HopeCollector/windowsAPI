#GDI�ṹ
* �������������ڷ��룬��GDI����������豸�����Ĵ���
* GDI������ˮ����ƽ���豸��pc֮��Ĺ��֣�ʹ���豸��pc����ƽ�棬���򿪷�������ƽ����У������豸��Ĳ���
* GDI֧�־�̬��ʾ���Զ�����֧����������

**GDI��������**
* ��ȡ�����������ͷţ����٣��豸�����ĺ���
	* `BeginPaint()`-`EndPaint()`
	* `GetDC()`-`ReleaseDC()`
* ��ȡ�豸������Ϣ�ĺ���
	* `GetTextMetrics()`��ȡѡ���豸������������Ϣ
* ��ͼ����
	* `TextOut()`�ı�Ҳ��ͼ���һ��
* ���úͻ�ȡ�豸�������Եĺ���������ȷ����ͼʱ��ϸ��
	* `SetTextAlign()`
* ʹ��GDI����ĺ���
	* ��ЩGDI����Ĵ�������Ҫ�豸�����������������̬ϵͳ�·�չ�����������Ȼͬ��������Ȼ��Դ������������������޹�

**GDIͼ�����**
* ����������
* �ɱ����ķ������
* λͼ������
* �ı���������ƫ�������Ե����ʣ�ʹ���Ϊ����ӵ�ͼ��

#�豸����

**��ȡ����ķ���**
	* `Beginpaint()` `EndPaint()`
	* `GetDC()` `ReleaseDC()`
	* `GetWindowDC()`��ȡ�������ڵ��豸������� `ReleaseDC()`
	* `CreateDC()` `DeleteDC()`������ͦ���ӵģ����Ҳ���
		* `CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL)`����������ȡ������Ļ���豸�������
	* `CreateIC()`(Information Context) ��ȡ�豸������Ϣ�����٣����Ƕ�GDI��ͼ������˵����ûʲô����...ֻ�ǵ����Ļ�ȡ��Ϣ
	* `CreateCompatibleDC()` `DeleteDC()`��ȡ�ڴ��豸�������������λͼ�õģ����ڻ�������
	* `CreateMetaFile()` `CloseMetaFile()`�����ڲ����ļ�һ���Ĳ���ͼԪ�ļ�

**��ȡ�豸������Ϣ**
* `GetDeviceCaps()`��ȡ�豸������Ϣ
	* ���`hdc`ָ��һ����Ļ�豸����ô���ؽ������`GetSystemMetrics()`һ���Ľ��

**�豸�ĳߴ�**
* ��ӡ���ķֱ��ʺ���ʾ���Ĳ�һ��
	* ��ӡ��dpi��dot oer inch),��ʾÿӢ��ĵ���
	* ��ʾ��ˮƽ�ʹ�ֱ�������������
* ��Windows���һ���жԷֱ����������¹涨
	1. �ֱ���=ÿ������λ��ͨ����Ӣ�磩���е�������
	2. ���ع�ģ/���سߴ� = �豸��ˮƽ����ʹ�ֱ������������������ʾ���ķֱ��ʣ�
	3. �����ߴ�/������ģ = �Զ����ߴ磨Ӣ�硢����ʲô�ģ���ʾ�Ĵ�С
	4. **�ֱ��� = ���سߴ� / �����ߴ�**
* ���������أ�ˮƽ�ֱ���=��ֱ�ֱ���
* �����С�ɵ�ֵ����ֵ1/72in.����ʾ = `tmHight` - `tmInternalLeading`
* Windowsϵͳ���岻����С���廹�Ǵ����壬ͳͳ������Ϊ10���ֺţ�12���м��
	* �������ڼٶ�����ʾ���ķֱ��ʲ�һ����������ķֱ��ʴ�һЩ���õ������������Ͷ�һЩ���������͸���
**GetDeviceCaps��ߴ���صı�ʶ��**

��ʶ�� | ����
------- | -----
HORZRES/VERTRES | ������������ع�ģ
LOGPIXELSX/LOGPIXELSY | ��������ķֱ���
HORZSIZE/VERTSIZE | ��������Ķ����ߴ磨�����������������ģ�
ASPECTX/ASPECTY/ASPECTXY | ÿ�����ص�����ߴ磨�������Խ��ߣ�

**�ߴ磨mm��=25.4 * ���ع�ģ/�ֱ���**
* Windows����������ߴ磬����׼ȷ�ĳߴ�������û��Լ�����...

**ɫ��**
* ��ʾɫ��Խ�ḻ��ÿ���������ڴ�����ɫ��Ϣ���ڴ�Խ�󣨺ڰ���ʾ��ֻҪ1byte/pi��
* ��ʾ��֧�ֵ�ɫ���� = 1<<(GetDeviceCaps(hdc, PLANES) * GetDeviceCaps(hdc, BITSPIXEL))(2��n�η���
* ����GDI����ʱһ��ʹ��COLORREF���޷��ų����Σ���ʾһ���ض���ɫ��RGBɫ�ʣ����ҵ���ÿ8λ��ʾһ����ɫ�����պ������������8λȫ��0��
* `RGB(r, g, b)`������ȡ��һ��COLORREFɫ��ֵ
* `GetR/G/BValue`��������ȡ��Ӧ��ɫ��ֵ
* ������������ʾ����ʾ����ɫ�ʵķ������ò�ͬɫ�ʵ������������һ��ɫ�ʵķ���
	* `GetNearestColor(hdc, crColor)`������ȡ�����ɫ��ӽ��ķǺϳ�ɫ

**�����豸��������**
1. ��`wndclass.style`�м���`CS_OWNDC`,ʹ�ó�ʼ��һ��֮����豸����������ʹ��`GetDC()`����`BeginPaint()`֮���ܼ���ʹ�ã�������ȡ ��ʽ�����ԣ�
2. ʹ��`idSaved = SaveDC(hdc)`������֮ǰ���豸������������`RestoreDC(hdc, idSaved)`������֮ǰ���豸����������

#����ߵĻ���
**��**
* `SetPixel(hdc, x, y, crColor)`�ڵ㣨x��y������һ����ɫΪ`crColor`�ĵ�

**��**
Windows�Ļ��ߺ���

���� | ����
---- | -----
LineTo(hdc, x, y) | �ӵ�ǰλ�û�һ���ߵ���x��y��
Polyline(hdc, lpPoints, nCounts) | ����`lpPoints`ָ���ĵ㣬����һ���ߣ��͵�ǰλ���޹أ�
PolylineTo(hdc, lpPoints, nCounts) | �ӵ�ǰλ�ÿ�ʼ������ͬ��
PolyPolyline(hdc, lpPoints, lpPolyPoints, nCount) | һ�λ��������,`lpPolyPoints`��ʾÿ�����߰����ĵ���
Arc(x1,y1,x2,y2,x3,y3,x4,y4) | ǰ������ȷ����Բ��Χ����������ȷ�����ߵ���ʼλ��
Arc(lpRect, ptStart, ptEnd) | ���ú�����һ��
ArcTo(����������������ͬ) | ����һ�������ǻ���µ�ǰ���λ��
PolyBezier(lpPoints, nCounts) | ��������������������
PolyBezierTo(lpPoints, nCounts) | ����ͬ�� 
PolyDraw(lpPoints, lpTypes, nCount) | �������������������ߣ����߶�������

* �Ի���������Ӱ���5���豸�����е�����
	* ��ǰλ�ã����Դ�To�ĺ�����Ӱ�� ��
	* ����
	* ����ģʽ
	* ������ɫ
	* ����ģʽ
* `MoveToEX(hdc, x, y, lpPoint)`�ı䵱ǰλ�ã�����lpPoint�����յ�֮ǰλ�õ����꣬�������궼��32λ��
	* `GetCurrentPositionEX(hdc, lpPoint)`ȡ�õ�ǰλ��
*  ��Բ���ߵķ�������ʱ��

**��**

���� | ����
----- | ----
Rectangle(hdc, x1, y1, x2, y2) | ��һ������
Ellipse(hdc, x1, y1, x2, y2) | ��һ����Բ
RoundRect(x1, y1, x2, y2, x3, y3) | �ǲ�˹���Բ�Ǿ��Σ������Բ������꣬��ʾԲ����Բ�ĳ���
RoundRect(lpRect, POINT) | POINT��������ĵ����Բ��������ǲ������꣡����
Pie(x1, y1, x2, y2, x3, y3, x4, y4) | ����Բ��һ���֣������������ӿ�����������
Pie(lpRect, ptStart, ptEnd) | ͬ��
Chord(��������������һ��) | �������Ҹ���Ĳ�����Բ

* �߽�ƫ����ƺ����������������Ƶı߿��ڻ����ο�
* ����`Pie` `Chord`  `Arc`ָ������ʼ�㲻��Ҫ�ھ��ο���ָ����Windows������������������Բ�ཻ������ͼ��

**����**
* �豸����������Ĭ�ϻ�����`BLACK_PEN`
	* Windows�ı��û��ʣ�stock pen��������`BLACK_PEN` `WHITE_PEN` `NULL_PEN`��ʲôҲ������ŭˢ���ڸж��ѣ�
	* `GetStockPen()`��ȡ**���û���**���
	* `SelectObject(hdc, hgobject)`��ĳ������ľ��ѡ���豸�������������᷵��֮ǰ���滻�����Ǹ��������Ӧ����������ڷ�Region����
* `CreatePen(fnPenStyle, nWidth, crColor)`�����Լ��Ļ���
	* `fnPenStyle`���ʷ���ʶ��

��ʶ�� | ����
------ | -----
PS_SOLID | ʵ��
PS_DASH | �̻���
PS_DOT | ����
PS_DASHDOT | �㻮��
PS_DASHDOTDOT | ����һ��
PS_NULL | ʲôҲ����ŭˢ���ڸ�
PS_INSIDEFRAME | [����ʵ��](http://www.cnblogs.com/cgwolver/archive/2009/12/29/1635358.html)

	* `nWidth`���ʿ�ȣ���Ϊ0�Ļ�Ĭ��Ϊ1���ؿ�
	* `crColor`ѡ�񻭱���ɫ
	* `PS_INSIDEFRAME`��ȳ���1ʱ������ʹ�ö�����ʾ��ɫ
* `CreatePenIndirect(LOGPEN *lplgpn)`�����������һ����ֻ������ָ����ʽ
```c
typedef struct tagLOGPEN {
  UINT     lopnStyle;
  POINT    lopnWidth;
  COLORREF lopnColor;
} LOGPEN
```
	* �����ֶκ������ûɶ����
* `DeleteObject()`ɾ��������GDI����
* `GetCurrentObject(hdc, uObjectType)`
	* `uObjectType`��ʶ�������Ҫȡ�õĶ���

��ʶ�� | ����
------ | -----
OBJ_BITMAP | λͼ�ļ�
OBJ_BRUSH | ��ˢ
OBJ_COLORSPACE | ɫ�ʿռ䣨����ɶ��
OBJ_FONT | ����
OBJ_PAL | ��ɫ��
OBJ_PEN | ����

* `GetObject(hgdiobj, cbBuffer, lpvObject)`��ȡ`hgdiobj`�������ϸ��Ϣ
	* `hgdiobj`��ĳһ����ľ��
	* `cbBuffer`������������Ϣ�����Ĵ�С
	* `lpvObject`����������Ϣ�ı���������ǿ��ת���ɣ�LPVOID)���͵Ĳſ���
* �������͵Ļ��ʿ�϶ʹ��`wndclass`�ı���ɫ���
	* `SetBKColor(hdc, crColor)`���Ըı����ɫ
	* `SetBKMode(hdc, nBKMode)`�趨���ģʽ
		* `OPAQUE`��͸��
		* `TRANSPARENT`͸��
* `SetRop2(hdc, iDrawMode)`���û�ͼģʽ
	* `iDrawMode`�ǻ�ͼģʽ��[�����������](https://msdn.microsoft.com/en-us/library/windows/desktop/dd145088(v=vs.85).aspx)
	* ��Ӧ�Ļ���`iDrawMode = GetROP2(hdc)`������ȡ��ǰ���������

**�����������**
* �ͻ��ʵ��÷�һ��`HBRUSH hbrush`��ˢ���
	* `GetStockBrush()`ͬ�������ڱ��û�ˢ
* ���`PolyGon()`������ͼ���Ǹ��鷳��
	* `SetPolyFillMode(hdc, iMode)`�������ģʽ
		* ALTERNATE����ģʽ���ж�һ�������Ƿ�ᱻ��䣬���������ʼ������Զ�����ߣ�����ż�����߿���ʱ��**����**�����
		* WINDING����ģʽ��ͬ�ϣ�����ֻ���ڻ��Ʒ���ͬ�ı߿���������ͬʱ���Ų��ᱻ���
		* �����������ģʽֻʶ����Ʊ߿򣬺�ʵ�ʿ���ȥ�м����߿�û��ϵ
* ��ˢ��8x8��λͼ��Windows����ʹ��5�ֺ���������ˢ
1. `CreateSolidBrush(crColor)`����һ��ʵ�Ļ�ˢ�����õ��ǹ��ܲ�ǿ��
2. `CreatHatchBrush(iHatchStyle, crColor)`��������Ӱ�ߵĻ�ˢ
3. `CreatBrushIndirect(&logBrush)`�����߼���ˢ
```c
typedef struct tagLOGBRUSH
  {
    UINT        lbStyle;
    COLORREF    lbColor;
    ULONG_PTR   lbHatch;
  } LOGBRUSH
```
	* `lbStyle`���������������ֶ���ô���
lbStyle | lbColor | lbHatch
------- | -------- | --------
BS_SOLID | ��ˢ��ɫ | ����
BS_HOLLOW | ���� | ����
BS_HATCHED | ��Ӱ����ɫ | ��Ӱ�߷���
BS_PATTERN | ���� | λͼ���
BS_DIBPATTERNPT | ���� | ָ��DIB��ָ��

#GDIӳ��ģʽ
* �ͻ���������ϵ�ǿ��Ա�ģ���������ϵ��ӳ��ģʽ���ı������ܸı�����ϵ
* `SetMapMode(hdc, iMapMode)`����ӳ��ģʽ
	* ��Ӧ�Ļ���`iMapMode = GetMapMode(hdc)`������ȡ��ǰӳ��ģʽ

ӳ��ģʽ | �߼���λ | x�� | y�� 
--------- | ------ | --- | ---
MM_TEXT | ���� | �� | ��
MM_LOMETRIC | 0.1mm | �� | ��
MM_HIMETRIC | 0.01mm | �� | ��
MM_LOENGLISH | 0.01in. | �� | ��
MM_HIENGLISH | 0.001in. | �� | ��
MM_TWIPS | 1/1440in. | �� | ��
MM_ISOTROPIC | ����(x=y) | ���� | ����
MM_ANISOTROPIC | ����(x!=y) | ���� | ����

* `Twip`����˼��1/20��
* Ĭ��ӳ��ģʽ�ǵ�һ��`MM_TEXT`
* ӳ��ģʽ����Ӧ����һ�к����ϣ������GDI����
	* `GetSystemMetrics()` `GetDeviceCaps()` ��ʹ���豸����
	* `GetTextMetrics()` ���ܵ�ӳ��ģʽ��Ӱ�죬���Ծ����û����ı�ʱ��ӳ��ģʽ�ͻ�óߴ�ĵ�ǰӳ��ģʽһ��
**�豸����ϵͳ**
* ����ϵ��ԭ��λ��������`MM_TEXT`ģʽ
* �豸����ϵͳȡ���ڻ�ȡ�豸��������ʱʹ�õĺ���
	* ��Ļ���꣺�����봰���޹صĺ�����������Ļ�ϵĵ��ƶ���Ѱ�Ҵ��ڵĺ���������DISPALY������CreateDC�߼����꽫Ĭ��Ϊ��Ļ���꣩
	* ȫ�������꣺������������(������������������ʲô�ģ�����`GetWindowsDC()`��ȡ���豸�����߼�����ᱻĬ��Ϊȫ��������
	* �ͻ������꣺��`GetDC()` `BeginPaint()`��õ��豸�����߼����걻Ĭ��Ϊ�ͻ�������
* `ScreenToClient(lpRect)` `ClientToScreen(lpRect)`ʵ���˿ͻ����������Ļ������໥ת��
	* `GetWindowRect(hwnd, lpRect)`���ڻ�ȡ��Ļ�����¾��Σ����ڣ���λ����Ϣ
**�ӿ��봰��**
* �ӿ�->�豸���꣨���أ�������->�߼����꣨���⣩�����ڵ�����ʹ�ã�
* ӳ��ģʽ���Ӵ��ڵ��ӿڵ�ӳ��
* `DPtoLP(hdc, pPoints, iNumber)` `LPtoDP()`�ӿ��������߼����껥ת
**MM_TEXT**
* �ӿ�ԭ��ʹ���ԭ�㶼������������
* `SetViewportOrgEx(hdc, x, y, lpPoint)` `SetWindowOrgEx(hdc, x, y, lpPoint)`�����ӿںʹ��ڵ�ԭ��
	* һ����˵��ͬʱʹ��
	* �����ӿ�ԭ���ʹ���豸���꣬���ô���ԭ���ʹ���߼�����
	* **�豸�㣨0,0����Զλ�����Ͻ�**
	* ��Ӧ��`GetViewportOrgEx(hdc, &pt)` `GetWindowOrgEx(hdc, &pt)`������ȡλ�ã����һ�٣�
**����ӳ��ģʽ**
* �ӿ�ԭ��ʹ���ԭ�㶼���Ըı�
* �߼���λ��������λ
* ���ڷ�Χ�����ᶼ�Ǹ�ֵ��
**MM_ISOTROPIC**
* ���Կ����߼���λ������ߴ�
* ����ı䴰�ں��ӿڷ�Χ����������������ţ�������Windows����и��棬ʹ������������߼���λ��ʾ��ͬ������ߴ�
* ��һ������ӳ��ģʽΪ`MM_ISOTROPIC`ʱ���ں��ӿڷ�Χ��`MM_LOMETRIC`��ͬ
* `SetWindowExtEx(hdc, x, y, lpSize)`ʹ���߼����� `SetViewportExtEx(hdc, x, y, lpSize)`ʹ�ô������꣬�������÷�Χ
	* ���÷�Χ��˼�ǣ�һ���߼���λ����������λ
	* һ�㶼��Ҫ�����ô��ڷ�Χ�������ӿڷ�Χ
	* ���úõķ�Χֻ�ǻ����������������x, y�ķ�Χ
**MM_ANISOTROPIC**
* ����ӳ��ģʽ��Ҫ��������߼���λ��ʾ��ͬ������ߴ�
* �趨����ӳ��ģʽʱ�������Ǽ̳�ǰ��ӳ��ģʽ�ķ�Χ
#���Ρ����򡢼���
���� | ����
---- | -----
FillRect(hdc, &rect, hBrush) | �û�ˢ������
FrameRect(hdc, &rect, hBrush) | �û�ˢ���ƾ��α߿򣨲�����ڲ���
InvertRect(hdc, &rect) | ��ת���ο��ڵ���ɫ
SetRect(&rect, x1, y1, x2, y2) | ���þ��ε����꣨�ȽϿ죩
OffsetRect(&rect, x, y) | ���������ƶ�������λ
InflateRect(&rect, x, y) | ���ž��εĳߴ�
SetRectEmpty(&rect) | �Ѿ��εĸ����ֶ�����Ϊ��
CopyRect(&DesRect, &SrcRect) | ����
InstersertRect(&DesRect, &SrcRect1, &SrcRect2) | ȡ�������εĽ���
UnionRect(&DesRect, &SrcRect1, &SrcRect2) | ȡ�������εĲ���
BOOL IsRectEmpty(&rect) | �жϾ����Ƿ�Ϊ��
BOOL PtInRect(&rect, point) | �жϵ��Ƿ��ھ�����

**�漴����**
* `BOOL PeekMessage(&msg, hwnd, wMsgFilterMin, wMsgFilterMasx, wRemoveMsg)`
	* ��`GetMessage()`���ƣ���ͬ����PeekMessage����һֱ��ռ�������Ȩ
	* `hwnd`ָ����ϣ�������Ϣ���Ǹ����ڣ�����NULL�������д��ڣ�����-1����ֻ�д��ھ����NULL����Ϣ
	* ��������������ʾҪ�����Ϣ�����࣬����0��ʾȫ�����
	* ���һ��������ʾ��δ���������Ϣ
	* `PM_NOREMOVE`��ɾ����������Ϣ
	* `PM_REMOVE`�ⲻ��˵��
	* `PM_NOYIELD`����ϵͳ��������������ɶ��˼��
**����**
* ����Ҳ��GDI����`HRGN`�Ǿ������DeleteObject��������ɾ�ģ�
* `Create(ͼ��)Rgn(Indirect)()`����������������(ͼ��)��ʾ����ʱ�õ��ĺ������֣�Rect��������
* `CombineRgn(hDesRgn, hSrcRgn1, hSrcRgn2, iCombine)`�����ϲ���������
	* `hDesRgn`ԭ��������ᱻ����
	* `iCombine`��Ϸ�ʽ��ʶ��

iCombine | ������
---------- | -------
RGN_AND | ����Դ�Ĺ�������
RGN_OR | ����Դȫ������
RGN_XOR | ����Դ��ȫ��������˹�������
RGN_DIFF | hSrcRgn1����hSrcRgn2�Ĳ���
RGN_COPY | ����hSRCRgn2

* ����ֵ���ĸ�
	* `NULLREGION`������
	* `SIMPLE`�����򣬻���ͼ��
	* `COMPLEXREGION`�������򣬻���ͼ�����
	* `ERROR`����...
* ��`InvalidateRect()`���ƣ�`InvalidateRgn()` `ValidateRgn()`���������
	* �������Ϊ�������򣬲������������ͼ