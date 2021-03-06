#4.1——文本输出
* Windows什么都不能决定，只能随遇而安。他只能通过函数来确定自己的窗口有多大，自己输出的字符有多大
* Windows靠消息驱动，一切活动靠**投递消息**（进入队列）或**发送消息**（非队列消息）
* 以下情况`WM_PAINT`会被发送
	* 用户移动窗口，使原来的窗口暴露出来
	* 调整窗口尺寸
	* 程序调用`ScrollWindow`或者`ScrollDC`滚动窗口
	* 程序调用`InvalidateRect`或者`InvalidateRgn`函数显式生成`WM_PAINT`消息
* 以下情况`WM_PAINT`可能被发送，看Windows心情。。。
	* Windows关闭了一个覆盖了部分窗口的对话框或消息框
	* 下拉菜单被收回
	* 显示提示信息（这是啥意思）
* 重绘时不需要更新整个客户区，只要对**无效矩形**进行更新就可以了
* 重绘规则
	* 客户区内存在无效矩形时Windows将在消息队列中放置一条`WM_PAINT`
	* 消息队列中不会出现多条`WM_PAINT`
	* 在已经出现无效矩形的情况下，若出现新的无效区域，矩形大小将被重新计算
#4.2——GDI简介
####设备环境
* 与特定的物理设备关联，某些参数还决定了图形属性
* 程序在绘制完成后必须要释放设备环境句柄，释放之后不能再用（阅后即焚）
* 除了`CreateWindow()`不可以在两个消息之间传递设备环境句柄
**获取方法**
1. 使用`BeginPaint(), EndPaint()`,他们需要两个变量
* `hwnd`窗口句柄
* `PAINTSTRUCT ps`绘制结构
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
* `HDC hdc`就是我们需要的那个`hdc=BeginPaint()`
* `BOOL fErase`指定是否需要重绘，通常都设为零用不着他老人家费心
* `RECT rePaint`一个结构体，定义了无效矩形的边界，所有绘制都只能在里面进行
*`BeginPaint()`工作流程*
	* 利用`wndclass.hbrBackground`擦去背景
	* 填充ps的各个字段
	* 返回一个设备环境变量
* 在`WM_PAINT`消息中，一定要使整个客户区有效，否则根据重绘法则，Windows会一直投送消息
* `InvalidateRect(LPRECT lpRect, BOOL bErase)`指定某一区域使其无效化
	* `LPRECT lpRect`指向一个CRect或者RECT结构体，指定更新区域，如果用`NULL`则为整个客户区
	* `BOOL bErase`指定是否擦除某一区域
* 最省事的是直接重绘整个客户区，不去管任何无效区域
* 最省时间的使用无效矩形避免不必要的GDI函数调用
2. 使用`GetDC(hwnd)`和`ReleaseDC(hwnd, hdc)`
* `GetDC()`的作用很单纯，并不能把无效区域有效化，而且很粗暴不会考虑无效矩形，得到的`hdc`指向整个客户区
* `ValidateRect(hwnd, LPRECT lpRect)`作用与`InvalidateRect()`相反，前两个参数一样（不打了）
* `GetWindowDC(hwnd)`可以获取整个窗口的设备环境句柄（比客户区大一点）但是也要处理`WM_NEPAINT`消息
**深入探讨♂`TextOut(hdc, x, y, psText, iLength)`**
* `hdc`设备环境句柄，要明白向哪里输出，应用于几乎所有GDI函数
	* 设备环境中的属性决定了文本显示的特性比如字体，字号，文本背景色
	* 文本背景色即是文本的显示特性之一，它和客户区背景不一样，文字是被文本框圈在里面的，文本背景色就是填充文本框用的
* `x, y`文字出现的起始坐标，更细致的位置定位
	* 确定了第一个字符左上角的坐标
* `psText`指向字符串的指针，指明输出内容
	* 字符串中不应该有任何ASCII控制字符
	* 若为 Unicode 字符串则长度应该是`iLength`的两倍
* `iLength`字符串长度，他不知道自己应该输出多少。。。
	* `TextOut（）`并不检查字符串最后的`\0`，全靠`iLength`确定输出多少
####文本
* 获取文本信息使用`GetTextMetrics(hdc， LPTEXTMERTICS)`，它会填写一个有成员略多（20个成员）的`TEXTMETRIC`结构体
```c
TEXTMETRIC tm;
HDC hdc;
hdc = GetDC(hwnd);
GetTextMetric(hdc, &tm);
...
ReleaseDC(hwnd, hdc);
```
**TEXTMETRIC中的重要成员**
* `tmHight`（高度总和）= `tmAscent`(基线上）+`teDescent`（基线下）
* `tmInternalLeading`重音符号高度，包含在`tmHight`
* `tmExternalLeading`跳出三界的成员，设计师建议行间距
* `tmAveCharWidth`小写字母的加权平均宽度（什么是加权）
* `tmMaxCahrWidth`字体中最宽字符的宽度
* `tmPitchAndFamily`标识是变宽字体（1）还是等宽字体（0）
* 大写字符平均宽度时小写的1.5倍左右
* 字符尺寸绝对不能用猜的！
* `SetTextAlign(hdc, nFlags)`设定文本的**对齐**位置
####客户区尺寸
* 相比于`GetClientRect()`来说，处理`WM_SIZE`更快捷
	* lParam的**低位字段**表示客户区的宽度，**高位字段**表示高度
```c
static int cxClient, cyClient;
...
case WM_SIZE:
	cxClient = LOWORD(lParam);
	cyClient = HIWORD(lParam);
	return 0;
```
* 上面是一个烂大街的程序段，虽然普遍，但是好用
####滚动条
* Windows人性化的认为，滚动条向上滚动意味着移向文档的开始，反之则移向结束
* 制造一个滚动条相当简单
```c
 hwnd = CreateWindow (szAppName, 
		TEXT ("Get System Metrics No. 2"),
                          	WS_OVERLAPPEDWINDOW | WS_VSCROLL,	//这样制造滚动条
                          	CW_USEDEFAULT, CW_USEDEFAULT,
                          	CW_USEDEFAULT, CW_USEDEFAULT,
                         	 NULL, NULL, hInstance, NULL) ;
```
* 像上面那样，在*窗口风格*（第三个参数）那里包括一个参数`WS_VSCROLL`或者`WS_HSCROLL`就可以了（同时包括两者也没问题）
* 滚动条的位置在客户区之外
**滚动条范围**
* 默认值为0~100，使用`SetScrollRange(hwnd, iBar, iMin, iMax,BOOL bRedraw)`来设置
	* `iBar`告诉函数要设置什么方向的滚动条（两个参数`SB_VERT`和`SB_HORZ`）
	* `iMin``iMax`是设置滚动条范围的，滚动条的位置是离散的整数值
	* `bRedraw`调整滚动条后是否需要立即重绘（之后有其他调整滚动条的函数是最好别重绘，浪费！）
	* 这个函数做出来的滚动条大小不能变.....
* 确定好范围后要设定位置`SetScrollPos(hwnd, iBar, iPos, bRedraw)`
	* `iPos`要在`iMin`和`iMax`之间
* 使用`GetScrollRange(hwnd, iBar, lpMin, lpMax)`获取滚动条范围
* 使用`GetScrollPos(hwnd, iBar)`获取滚动条位置（是一个int型的返回值）
* 滚动条的实现是程序和Windows这对cp共同的成果

Windows做的 | 程序做的
----------- | -------
处理滚动条中的鼠标信息 | 初始化滚动条的范围和位置
提供单击滚动条时的反向闪烁 | 处理传送给窗口过程的滚动条信息
用户拖拽滑块时在滚动条内移动滑块 | 更新滑块位置
向相应的窗口过程发送滚动条消息 | 根据滚动条的变化更新客户区的内容

**滚动条消息**

* `WM_VSCROLL`, `WM_HOCROLL`，`lParam`不再像`WM_SIZE`里那样有用（在对话框中有用），反而是`wParam`更有用
	* 低字段位代表鼠标在滚动条上的动作，按下时会发出多条消息（至少一条），松开时会发出另一条`SB_ENGSCROLL`
	* 拖动滑块时会发出`SB_THUMBTRACK`消息（`wParam`的低位字段），这时高位字段是 **滑块的当前位置**
	* `wParam`的低位字段是`SB_THUMBPOSITION`时，高位字段是用户松开鼠标时滑块的最终位置
	* 处理`SB_THUMBTRACK`程序会在拖动滑块时移动客户区的内容
	* 处理`SB_THUMBPOSITION`程序会在用户松开鼠标时更新客户区的内容
**更牛逼的滚动条**
* `SetScrollInfo(hwnd, iBar, PSSCROLLINFO, bRedraw)`做出来的滑块是可以改变大小的
	* 滑块大小/滚动条长度=文档显示量/文档总大小=页面大小/范围（什么是范围）
	* `PSSCROLLINFO`是结构体指针，指向`SCROLLINFO`
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
	* `cdSize`标识该结构体的大小`sizeof(si)`，保持兼容性（怎么实现的）
	* `fMask`位运算的标识符

标识符 | SetScrollInfo | GetScrollInfo
------- | ------------- | --------------
SIF_RANGE | nMin和nMax指定滚动条范围 | nMin和nMax上返回滚动条范围
SIF_POS | nPos指定滑块位置 | nPos返回滑块位置
SIF_PAGE | nPage指定页面大小 | nPage返回页面大小
SIF_TRACKPOS | 忽略 | nTrackPos返回当前滑块位置
SIF_ALL | 以上组合 | 以上组合
SIF_DISABLENOSCROLL | 禁用不需要的滚动条（但会显示出来） | 忽略

**滚动条用到的函数**

* `InvalidateRect()`使整个客户区无效
* `UpdateWindow()`此函数将直接发送`WM_PAINT`消息到窗口过程
* `ScrollWindow(hwnd, XAmount, YAmount, lpRect, lpClipRect)`用来滚动客户区，加快程序速度
	* `XAmount`和`YAmount`指定滚动的像素个数
	* `lpRect`指定滚动区域
	* `lpClipRect`作用同上，只不过是坐标形式的