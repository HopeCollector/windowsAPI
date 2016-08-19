#3.1——窗口创建

* Windows可以控制程序 
*  **窗口类标记了窗口过程，所有窗口都是基于窗口类创建的（允许多个窗口分享一个窗口类）**
* 窗口过程
	* 可以自己写也可以写入动态链接库
	* 作为一个函数，它将接收Windows的消息，并对窗口进行操作
	* 用户—>windows—>窗口过程—>窗口
* 消息队列
	* 每个程序都仅有一个消息队列
	* 作用是给消息排序，让窗口过程能稳定执行
*  *客户区* 程序向用户传达可视化消息的区域
*  *标识符* （CS_HREDRAW，DT_CENTER），下划线_之前的前缀表示标识符的类别
* windows 总是有很多乱七八糟的数据类型，都是`#define`或者`typedef`定义的，目的是减轻工作量
*  *句柄* 类似于c的文件指针
*  *匈牙利标记法* 变量名以表明该变量数据类型的小写字母开头
	* 命名结构变量时，可以使用结构名的小写形式作为变量名前缀或者整个变量名
* 初始化wndclass->注册窗口类->创建窗口->显示窗口
####窗口类注册
* 利用函数`RegisterClass(&WNDCLASS)`注册窗口类
* `WNDCLASS`是结构体，因为包含字符串成员，所以有三个版本（A版，U版，通用版），通用版通过识别`#define UNICODE`来决定使用什么版本
* 注册窗口类需要对`WNDCLASS`结构初始化
*  **WNDCLASS**
	* `wndclass.style = CS_HREDRAW|CS_VREDRAW`采用*位标记*定义了类风格（class style）
	* `wndclass.lpfnWndProc = WndProc`定义了与窗口类对应的窗口过程，在c中这相当于使用函数指针
	* `wndcalss.cbClsExtra = 0`和`wndclass.cbWndExtra = 0`是用来在类结构和Windows内部维护的窗口结构预留一些空间的（不明白）
	* `wndcalss.hInstance = hInstance`表示应用程序自己的实例句柄
	* `wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION)`加载图标（标题栏左边的图标）并返回一个图标句柄
	* `wndclass.hCursor = LoadCursor(NULL, IDC_ARROW)`加载鼠标并返回一个鼠标句柄
	* `wndclass.hbrBackground = GetStockObject(WHITE_BRUSH)`为客户区指定背景色，并返回一个画刷句柄
	* `wndclass.lpszMenuName = NULL`指定窗口类菜单（不明白）
	* `wndclass.lpszClassname = szAppName`给窗口类一个名称
* 在16位windows版本中，允许多个实例共享一个窗口类（节约内存）,`hPrevInstance`就保存了前一个程序的实例句柄，在32位中`hPrevIndtance`总为`NULL`
####窗口创建
* 窗口类的对窗口的定义是一般性的，无法满足窗口的需要，必须在窗口类的基础上定义窗口
* `CreateWindow()`用来定义窗口返回该窗口的**窗口句柄**
```c
hwnd = CreateWindow(
		szAppName,			//窗口类名称
		TEXT(`The Hello Program`),		//窗口标题
		WS_OVERLAPPEDWINDOW,		//窗口风格
		CW_USEDEFAULT,			//初始x坐标
		CW_USEDEFAULT,			//初始y坐标
		CW_USEDEFAULT,			//初始x方向尺寸
		CW_USEDEFAULT,			//初始y方向尺寸
		NULL,				//父窗口句柄
		NULL,				//菜单窗口句柄 
		hInstance,				//程序实例句柄
		NULL);				//创建参数（在窗口创建号之前起作用）
```
* `WS_OVERLAPPEDWINDOW`标准的窗口风格之一，通过*位标记*得到
* `CW_USEDEFAULT`默认值（0x80000000），不只是坐标取默认值，尺寸也可以用它取默认值
* 两个窗口存在父子关系时，子窗口在父窗口前面
* 应用窗口位于桌面窗口前方
####显示窗口
* `ShowWindow(hwnd, iCmdShow)`用来把窗口显示在屏幕中
	* `hwnd`用`CreateWindow()`得到的窗口句柄
	* `iCmdShow`是`WinMain`主函数接受的参数
		* `SW_SHOWNORMAL`正常大小显示
		* `SW_SHOWMAXIMIZED`最大化显示
		* `SW_SHOWMINNOACTIVE`最小化显示
* 如果`iCmdShow`得到的参数时`SW_SHOWNORMAL`则整个客户区会被`winclass.hbrBackground`选定的画刷擦除
* `UpdateWindow(hwnd)`用来重绘客户区，它将向窗口过程发送一条`WM_PAINT`信息
####消息循环
* Windows会把来自用户的输入当作`消息`，放到应用程序对应的消息队列中
* 消息循环
```c
while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
```
* `GetMessage(&msg, NULL, 0, 0)`用来从消息队列中检索消息，并把消息填入`msg`
	* `MSG msg`是结构体
	* `msg.message`取得`WM_QUIT`消息时返回0
```c
typedef struct tagMSG {
    HWND        hwnd;	//窗口句柄
    UINT        message;	//每条消息的数值
    WPARAM      wParam;	//依消息不同而作用不同
    LPARAM      lParam;	//同上
    DWORD       time;	//消息进入消息队列的时间
    POINT       pt;		//结构体，表示一个点，消息进入队列时鼠标的位置
} MSG;
```
* `POINT`结构体
```c
typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT
```
* `TranslateMessage(&msg)`翻译某些键盘消息
* `DispatchMessage(&msg)`将`msg`再次返还给Windows等着填入下一条消息，而消息则进入窗口过程进行处理（msg就像一辆车，来回运消息）
####窗口过程
* 应用程序一般不会调用窗口过程，实在想用那就用`SendMessage()`函数
* 定义窗口过程的一般方法
```c
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
```
* 前四个参数与`MSG`的前四个参数一一对应（后两个也没办法对应啊。。。）
* 不管有几个基于相同窗口类创建的窗口，hwnd总是标识接收消息的窗口（废话，标识别的消息谁处理啊）
*由于窗口过程需要处理大量消息，使得它变成了一个巨型的switch-case...
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
return DefWindowProc(hwnd, iMsg, wParam, lParam);  //对窗口过程没有处理的消息进行默认处理
```
* `WM_CREATE`
	* Windows处理`CreateWindow()`时，WM_CREATE消息就开始处理了（不经过GeiMessage）
	* `PlaySound`要在消息队列里执行，不执行完窗口无法显示（显示窗口也是消息）
* `WM_PAINT`
	* 客户区“无效”或者必须“更新”时，程序会得到此消息
	* 以下情况客户区无效
		* 窗口被首次创建
		* 调整窗口尺寸（最小化，最大化）
		* 窗口被重叠部分遮挡，再次显现
	* `BeginPaint(hwnd, &ps)`开始绘制
		* 绘制期间若背景尚未擦除，则Windows会使用`winclass.hbrBackground`来擦除背景
		* `BeginPaint()`可以使整个客户区有效
		* 返回一个“设备环境句柄”，与物理设备（显示器）关联
	* `EndPaint(hwnd, &ps)`结束绘制，断开`hwnd`与物理设备的连接
	* `GetClientRect(hwnd, &rect)`依据窗口尺寸来设置`rect`的四个字段（left, top, right, bottom)
	* `DrawText()`绘制文本
* `WM_DESTORY`
	* 用户关闭窗口时程序会接到这条消息
	* `PostQuitMessage(0)`将一条`WM_QUIT`消息强行插入♂队列，`GetMessage()`将返回0
# 3.2
* WinMain只包含了必要的一部分操作，真正的操作都是在WinProc里实现的
* Windows通过调用窗口过程来向窗口发消息，部分函数也可以调用WinProc（以消息的形式）
* 以下情况WinProc会被Windows调用，所有调用都以消息形式出现 
	* 新建窗口
	* 窗口被最终销毁
	* 窗口尺寸发生变化（包括最小化）
	* 鼠标在窗口执行单击或双击
	* 键盘消息传入
	* 鼠标操作或单击滚动条
	* 客户区需要重绘
* 消息分两种
	* 队列消息，进入消息队列等待被分发的消息
	* 非队列消息，直接被送到WinProc进行处理
* 消息循环与窗口过程并不是并发进行的`DispatchMeaasge()`执行时就会调用WinProc，当WinProc将控制权返回给Windows时`DispatchMessage()`才会返回
* 窗口过程在处理某一消息时，程序不会被其他消息中断