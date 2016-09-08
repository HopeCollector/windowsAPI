#鼠标

* 鼠标只是附属品，它的存在让使用更方便，但是没了它也不会有问题（程序的存在与否和鼠标无关）
* 加载鼠标指针`wndclass.hCursor = LoadCursor(NULL, IDC_ARROW)`

**基本名词解释**

* hot spot：单像素精度（就一个像素），鼠标指针顶点
* 单击：按下鼠标按钮并松开
* 双击：连续两次按下鼠标按钮，并松开。物理位置上十分靠近(cxChar * cyChar / 2), 发生在*双击速度*内（特殊的时间间隔）
* 拖动：保持按下鼠标按钮，并移动鼠标
* 三键鼠标：左键（LBUTTON）右键（RBUTTON）中键（MBUTTON）
* 双键鼠标：左键，右键
* 单键鼠标：左键
* 单个鼠标-mouse，多个鼠标-mouse devices（避免使用复数）

####客户区鼠标消息

* 与键盘不同，即使窗口没有焦点，它仍然可以接收鼠标消息
* 除去`WM_MOUSEMOVE`和其他的11中非客户区鼠标消息，剩下的9中很好记
	* 单击-按下：`WM_LBUTTONDOWN` 键名+DOWN
	* 单击-松开：`WM_MBUTTONUP` 键名+UP
	* 双击-按下：`WM_RBUTTONDBLCLK` 键名+DBLCLK（double click忽略元音和重复的字母）
* `lParam`包含了位置信息，`wParam`包含了鼠标按钮，Shift，ctrl的状态信息
```c
//其他程序
case WM_LBUTTONDOWN:
	if(wParam & MK_CONTROL)
	//按下鼠标左键时ctrl被按下
```
* `WM_MOUSEMOVE` 的产生逻辑和`WM_PAINT`很想，在产生之前如果消息队列中有一条未被处理的，那么这条就不会产生（产生速度取决于处理速度）
* 程序未必会接收到成对鼠标消息比如`WM_...DOWN` `WM_...UP`
	* 在A窗口按下鼠标某个键，移到B窗口再松开，两个窗口收到的消息都不是成对的
* 程序是用来处理消息的，监视系统状态是操作系统的事，别指望程序处理消息时能监视某个键子是否被按下

**双击鼠标**

* 程序不是天生就能接收双击消息的，必须要在注册时定义清楚
	* `wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS`
	* 否则会接受到两对DOWN，up消息
	* 定义之后则将第二个DOWN消息简单的替换成DBLCLK，其它保持不变

####非客户区鼠标消息

* 与客户区鼠标消息的区别和键盘消息差不多，键盘消息多了SYS，鼠标消息多了NC（nonclient）
	* `wParam`储存以HT开头的标识符表示非客户区鼠标移动或单击的位置
	* `lParam`表示鼠标单击的位置，不过是屏幕坐标
	* 有多块显示器时提取坐标不要用`LOWORD()` `HIWORD()`，因为xy坐标可能是负的

**击中测试消息**

* 它的优先级高于其他所有的鼠标消息（它产生了所有的鼠标消息）
* 除去那些差不多的两类鼠标消息，还剩下一个`WM_NCHITTEST`表示非客户区击中测试
	* `lParam`记录鼠标位置
	* `wParam`没啥卵用
* 处理流程（消息产生消息）
1. `DefWindProc`接收消息
2. `DefWindProc`利用此消息产生其他和**鼠标位置**相关的**鼠标消息**
	* 若鼠标位于非客户区，则返回一个可用于鼠标消息的`wParam`的值
	* 若鼠标位于客户区，则返回`HTCLIENT`Windows会基于此发送一条**客户区鼠标消息**，并将屏幕坐标转成客户区坐标

####击中测试

* 击中测试：判断鼠标相对某些目标的位置，利用鼠标坐标进行计算
* `ShowCursor(BOOL)`控制鼠标的可见性
	* 鼠标的显示具有累加效应
	* TRUE-累加数+1
	* FALSE-累加数-1
	* 但累加数是负数时鼠标不可见

**以下两个函数用的都是屏幕坐标**

* `GetCursorPos(&pt)`
	* 获取当前鼠标位置，不同于`WM_...`消息用宏得到的坐标，他是用来获取调用函数时鼠标的坐标，而不是消息发出时的坐标
* `SetCursorPos(&pt)`
	* 设置鼠标位置

**子窗口**

* 正如第一章说的，处理不同事情的窗口用不同的窗口过程，而窗口过程与窗口类又是一一对应的，
所以需要注册两个窗口类`RegisterClass()`

>*  **WNDCLASS**
>	* `wndclass.style = CS_HREDRAW|CS_VREDRAW`采用*位标记*定义了类风格（class style）
>	* `wndclass.lpfnWndProc = WndProc`定义了与窗口类对应的窗口过程，在c中这相当于使用函数指针
>	* `wndcalss.cbClsExtra = 0`和`wndclass.cbWndExtra = 0`是用来在类结构和Windows内部维护的窗口结构预留一些空间的（不明白）
>	* `wndcalss.hInstance = hInstance`表示应用程序自己的实例句柄
>	* `wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION)`加载图标（标题栏左边的图标）并返回一个图标句柄
>	* `wndclass.hCursor = LoadCursor(NULL, IDC_ARROW)`加载鼠标并返回一个鼠标句柄
>	* `wndclass.hbrBackground = GetStockObject(WHITE_BRUSH)`为客户区指定背景色，并返回一个画刷句柄
>	* `wndclass.lpszMenuName = NULL`指定窗口类菜单（不明白）
>	* `wndclass.lpszClassname = szAppName`给窗口类一个名称

* 注册子窗口时需要改动几个成员，重点是前两个
	* `wndclass.lpfnWndProc = ChildWndProc`定义了子窗口类对应的窗口过程
	* `wndclass.cbWndExtra = sizeof(LONG)`为每个基于此窗口类创建的窗口在内部结构中预留4字节空间（wndclass.cbClsExtra是为窗口类预留的空间）
	* `wndclass.Hicon = NULL`子窗口不需要图标
	* `wndclass.pszClassName = "Checker3_Child"`子窗口类的名称
* 创建子窗口`CreateWindow()`
```c
CreateWindow (szChildClass, NULL,
                              WS_CHILDWINDOW | WS_VISIBLE,
                              0, 0, 0, 0,
                              hwnd, (HMENU) (y << 8 | x),
                              (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
                              NULL)
```
* 子窗口ID-`(HMENU)(y<<8 | X)`
	* 只是一个唯一用来表示子窗口的数值，上面的语句是把两个坐标值拼到一起了
	* 处理对话框的子窗口控件时特别重要（不明白）
* 实例句柄-`(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE)`
	* 因为不能直接获取实例句柄，只能用这种方式

* `MoveWindow(hwnd, x, y, Width, Hight, bRepaint)`
	* 如果要求重绘则会在此函数调用完成后，直接调用`UpDateWindow()`
* `SetWindowLong(hwnd, nIdex, dwNewLong)`
	* 可以改变窗口的属性，也可以改变之前为窗口预留的存储区域（wndclass.cdWndExtra)
	* `nIndex`如果大于零，则用来访问预留内存的指定字段（都是4的倍数int）
	* 如果小于零，则用来访问窗口的数据结构
	* `dwNewLong`设置存储在指定字段的数据
* `GetWindowLong(hwnd, nIndex)`
	* `nIndex`的作用和上面函数的差不多，只不过把修改换成了获取
* 使用子窗口可以不用纠结击中测试

**在子窗口加入键盘接口**

* 单击子窗口时获得焦点的是父窗口(但是子窗口能接受鼠标消息)，需要用`SetFocus(hwnd)`,为子窗口设定焦点
* 多数情况下子窗口句柄难以获得，用`GetDlgItem(hwnd, id)`
	* `id`是但是创建窗口为子窗口设定的**子窗口ID**
* `HWND GetFocus(void)`用来确定当前的窗口是否具有焦点
* 输入是输向具有焦点的窗口，不管它是是子窗口

**捕获鼠标**

* `SetCapture(hwnd)`强制函数把一切 鼠标消息发送给`hwnd`的窗口过程
	* 相应的`ReleaseCapture()`释放鼠标，恢复正常

**鼠标滚轮**

* 需要捕获`WM_MOUSEWHEEL`消息
	* `lParam`表示了鼠标位置
	* `wParam`比较复杂
		* 低位表示按键状态，和`WM_MOUSEMOVE`的`wParam`相似
		* 高位表示增量数值