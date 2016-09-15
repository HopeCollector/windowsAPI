#GDI结构
* 驱动程序类似于翻译，把GDI函数翻译成设备能理解的代码
* GDI就像是水，填平了设备与pc之间的沟壑，使得设备与pc连成平面，程序开发便能在平面进行，忽略设备间的差异
* GDI支持静态显示，对动画的支持能力有限

**GDI函数分类**
* 获取（创建）和释放（销毁）设备环境的函数
	* `BeginPaint()`-`EndPaint()`
	* `GetDC()`-`ReleaseDC()`
* 获取设备环境信息的函数
	* `GetTextMetrics()`获取选入设备环境的字体信息
* 绘图函数
	* `TextOut()`文本也算图像的一种
* 设置和获取设备环境属性的函数，用于确定绘图时的细节
	* `SetTextAlign()`
* 使用GDI对象的函数
	* 这些GDI对象的创建不需要设备环境句柄，就像封闭生态系统下发展出来的生物，虽然同样消耗自然资源，但自生自灭与外界无关

**GDI图像分类**
* 线条和曲线
* 可被填充的封闭区域
* 位图（点阵）
* 文本（由于其偏向艺术性的特质，使其成为了最复杂的图像）

#设备环境

**获取句柄的方法**
	* `Beginpaint()` `EndPaint()`
	* `GetDC()` `ReleaseDC()`
	* `GetWindowDC()`获取整个窗口的设备环境句柄 `ReleaseDC()`
	* `CreateDC()` `DeleteDC()`用起来挺复杂的，暂且不表
		* `CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL)`可以用来获取整个屏幕的设备环境句柄
	* `CreateIC()`(Information Context) 获取设备环境信息，快速，但是对GDI绘图函数来说，他没什么卵用...只是单纯的获取信息
	* `CreateCompatibleDC()` `DeleteDC()`获取内存设备环境句柄，处理位图用的，现在还不会用
	* `CreateMetaFile()` `CloseMetaFile()`类似于操作文件一样的操作图元文件

**获取设备环境信息**
* `GetDeviceCaps()`获取设备环境信息
	* 如果`hdc`指向一个屏幕设备，那么返回结果将和`GetSystemMetrics()`一样的结果

**设备的尺寸**
* 打印机的分辨率和显示器的不一样
	* 打印机dpi（dot oer inch),表示每英寸的点数
	* 显示器水平和垂直方向的像素总数
* 在Windows编程一书中对分辨率作了如下规定
	1. 分辨率=每度量单位（通常是英寸）含有的像素数
	2. 像素规模/像素尺寸 = 设备在水平方向和垂直方向上像素总数（显示器的分辨率）
	3. 度量尺寸/度量规模 = 以度量尺寸（英寸、毫米什么的）表示的大小
	4. **分辨率 = 像素尺寸 / 度量尺寸**
* 正方形像素：水平分辨率=垂直分辨率
* 字体大小由点值（磅值1/72in.）表示 = `tmHight` - `tmInternalLeading`
* Windows系统字体不管是小字体还是大字体，统统被假设为10磅字号，12磅行间距
	* 区别在于假定的显示器的分辨率不一样，大字体的分辨率大一些，用到的像素数量就多一些，看起来就更大
**GetDeviceCaps与尺寸相关的标识符**

标识符 | 含义
------- | -----
HORZRES/VERTRES | 两个方向的像素规模
LOGPIXELSX/LOGPIXELSY | 两个方向的分辨率
HORZSIZE/VERTSIZE | 两个方向的度量尺寸（由上面两个计算来的）
ASPECTX/ASPECTY/ASPECTXY | 每个像素的物理尺寸（长、宽、对角线）

**尺寸（mm）=25.4 * 像素规模/分辨率**
* Windows不关心物理尺寸，想获得准确的尺寸最好让用户自己输入...

**色彩**
* 显示色彩越丰富，每个像素用于储存颜色信息的内存越大（黑白显示器只要1byte/pi）
* 显示器支持的色彩数 = 1<<(GetDeviceCaps(hdc, PLANES) * GetDeviceCaps(hdc, BITSPIXEL))(2的n次方）
* 调用GDI函数时一般使用COLORREF（无符号长整形）表示一种特定颜色（RGB色彩，从右到左每8位表示一种颜色，按照红绿蓝排序，最后8位全是0）
* `RGB(r, g, b)`宏用来取得一个COLORREF色彩值
* `GetR/G/BValue`宏用来提取相应颜色的值
* 抖动：能让显示器显示更多色彩的方法，用不同色彩的相邻像素组成一种色彩的方法
	* `GetNearestColor(hdc, crColor)`用来获取与该颜色最接近的非合成色

**保存设备环境变量**
1. 在`wndclass.style`中加入`CS_OWNDC`,使得初始化一次之后的设备环境变量在使用`GetDC()`或者`BeginPaint()`之后还能继续使用（其他获取 方式不可以）
2. 使用`idSaved = SaveDC(hdc)`（返回之前的设备环境变量）和`RestoreDC(hdc, idSaved)`（重载之前的设备环境变量）

#点和线的绘制
**点**
* `SetPixel(hdc, x, y, crColor)`在点（x，y）处画一个颜色为`crColor`的点

**线**
Windows的画线函数

函数 | 功能
---- | -----
LineTo(hdc, x, y) | 从当前位置画一条线到（x，y）
Polyline(hdc, lpPoints, nCounts) | 连接`lpPoints`指定的点，连成一条线（和当前位置无关）
PolylineTo(hdc, lpPoints, nCounts) | 从当前位置开始，其它同上
PolyPolyline(hdc, lpPoints, lpPolyPoints, nCount) | 一次画多个折线,`lpPolyPoints`表示每个折线包含的点数
Arc(x1,y1,x2,y2,x3,y3,x4,y4) | 前两个点确定椭圆范围，后两个点确定画线的起始位置
Arc(lpRect, ptStart, ptEnd) | 作用和上面一样
ArcTo(参数和上面两个相同) | 作用一样，但是会更新当前点的位置
PolyBezier(lpPoints, nCounts) | 用来画贝塞尔样条曲线
PolyBezierTo(lpPoints, nCounts) | 作用同上 
PolyDraw(lpPoints, lpTypes, nCount) | 画多条贝塞尔样条曲线，或者多条折线

* 对绘制线条有影响的5个设备环境中的属性
	* 当前位置（仅对带To的函数有影响 ）
	* 画笔
	* 背景模式
	* 背景颜色
	* 绘制模式
* `MoveToEX(hdc, x, y, lpPoint)`改变当前位置，并且lpPoint将接收到之前位置的坐标，所有坐标都是32位的
	* `GetCurrentPositionEX(hdc, lpPoint)`取得当前位置
*  画圆曲线的方向是逆时针

**面**

函数 | 功能
----- | ----
Rectangle(hdc, x1, y1, x2, y2) | 画一个矩形
Ellipse(hdc, x1, y1, x2, y2) | 画一个椭圆
RoundRect(x1, y1, x2, y2, x3, y3) | 乔布斯最爱的圆角矩形，第三对不是坐标，表示圆角椭圆的长宽
RoundRect(lpRect, POINT) | POINT就是上面的第三对参数，他们不是坐标！！！
Pie(x1, y1, x2, y2, x3, y3, x4, y4) | 画椭圆的一部分，并与中心连接看起来像扇形
Pie(lpRect, ptStart, ptEnd) | 同上
Chord(参数和上面两个一样) | 画出由弦割出的部分椭圆

* 边界偏差：绘制函数给出的两点限制的边框内画矩形框
* 对于`Pie` `Chord`  `Arc`指定的起始点不需要在矩形框里指定，Windows将绘制两点连线与椭圆相交两点间的图像

**画笔**
* 设备环境变量的默认画笔是`BLACK_PEN`
	* Windows的备用画笔（stock pen）有三种`BLACK_PEN` `WHITE_PEN` `NULL_PEN`（什么也不画，怒刷存在感而已）
	* `GetStockPen()`获取**备用画笔**句柄
	* `SelectObject(hdc, hgobject)`把某个对象的句柄选入设备环境变量，将会返回之前被替换掉的那个对象的相应句柄（仅限于非Region对象）
* `CreatePen(fnPenStyle, nWidth, crColor)`创建自己的画笔
	* `fnPenStyle`画笔风格标识符

标识符 | 含义
------ | -----
PS_SOLID | 实线
PS_DASH | 短划线
PS_DOT | 虚线
PS_DASHDOT | 点划线
PS_DASHDOTDOT | 两点一划
PS_NULL | 什么也不画怒刷存在感
PS_INSIDEFRAME | [框内实线](http://www.cnblogs.com/cgwolver/archive/2009/12/29/1635358.html)

	* `nWidth`画笔宽度，设为0的话默认为1像素宽
	* `crColor`选择画笔颜色
	* `PS_INSIDEFRAME`宽度超过1时，允许使用抖动显示颜色
* `CreatePenIndirect(LOGPEN *lplgpn)`和上面的作用一样，只不过是指针形式
```c
typedef struct tagLOGPEN {
  UINT     lopnStyle;
  POINT    lopnWidth;
  COLORREF lopnColor;
} LOGPEN
```
	* 三个字段和上面的没啥区别
* `DeleteObject()`删除创建的GDI对象
* `GetCurrentObject(hdc, uObjectType)`
	* `uObjectType`标识符，标记要取得的对象

标识符 | 含义
------ | -----
OBJ_BITMAP | 位图文件
OBJ_BRUSH | 画刷
OBJ_COLORSPACE | 色彩空间（这是啥）
OBJ_FONT | 字体
OBJ_PAL | 调色板
OBJ_PEN | 画笔

* `GetObject(hgdiobj, cbBuffer, lpvObject)`获取`hgdiobj`对象的详细信息
	* `hgdiobj`是某一对象的句柄
	* `cbBuffer`是用来接受信息变量的大小
	* `lpvObject`用来接收信息的变量，必须强制转换成（LPVOID)类型的才可以
* 虚线类型的画笔空隙使用`wndclass`的背景色填充
	* `SetBKColor(hdc, crColor)`可以改变填充色
	* `SetBKMode(hdc, nBKMode)`设定填充模式
		* `OPAQUE`不透明
		* `TRANSPARENT`透明
* `SetRop2(hdc, iDrawMode)`设置绘图模式
	* `iDrawMode`是绘图模式的[布尔运算规则](https://msdn.microsoft.com/en-us/library/windows/desktop/dd145088(v=vs.85).aspx)
	* 对应的还有`iDrawMode = GetROP2(hdc)`用来获取当前的运算规则

**绘制填充区域**
* 和画笔的用法一样`HBRUSH hbrush`画刷句柄
	* `GetStockBrush()`同样适用于备用画刷
* 填充`PolyGon()`画出的图像是个麻烦事
	* `SetPolyFillMode(hdc, iMode)`设置填充模式
		* ALTERNATE交替模式：判断一个区域是否会被填充，从这个区域开始向无穷远画射线，穿过偶数条边框线时才**不会**被填充
		* WINDING螺旋模式：同上，但是只有在绘制方向不同的边框线数量相同时，才不会被填充
		* 上面两种填充模式只识别绘制边框，和实际看上去有几条边框没关系
* 画刷是8x8的位图，Windows允许使用5种函数建立画刷
1. `CreateSolidBrush(crColor)`创建一个实心画刷，好用但是功能不强大
2. `CreatHatchBrush(iHatchStyle, crColor)`创建有阴影线的画刷
3. `CreatBrushIndirect(&logBrush)`建立逻辑画刷
```c
typedef struct tagLOGBRUSH
  {
    UINT        lbStyle;
    COLORREF    lbColor;
    ULONG_PTR   lbHatch;
  } LOGBRUSH
```
	* `lbStyle`决定了下面两个字段怎么理解
lbStyle | lbColor | lbHatch
------- | -------- | --------
BS_SOLID | 画刷颜色 | 忽略
BS_HOLLOW | 忽略 | 忽略
BS_HATCHED | 阴影线颜色 | 阴影线方向
BS_PATTERN | 忽略 | 位图句柄
BS_DIBPATTERNPT | 忽略 | 指向DIB的指针

#GDI映射模式
* 客户区的坐标系是可以变的，这种坐标系叫映射模式，改变它就能改变坐标系
* `SetMapMode(hdc, iMapMode)`设置映射模式
	* 对应的还有`iMapMode = GetMapMode(hdc)`用来获取当前映射模式

映射模式 | 逻辑单位 | x轴 | y轴 
--------- | ------ | --- | ---
MM_TEXT | 像素 | 右 | 下
MM_LOMETRIC | 0.1mm | 右 | 上
MM_HIMETRIC | 0.01mm | 右 | 上
MM_LOENGLISH | 0.01in. | 右 | 上
MM_HIENGLISH | 0.001in. | 右 | 上
MM_TWIPS | 1/1440in. | 右 | 上
MM_ISOTROPIC | 随意(x=y) | 随意 | 随意
MM_ANISOTROPIC | 随意(x!=y) | 随意 | 随意

* `Twip`的意思是1/20磅
* 默认映射模式是第一个`MM_TEXT`
* 映射模式并不应用在一切函数上，比如非GDI函数
	* `GetSystemMetrics()` `GetDeviceCaps()` 都使用设备坐标
	* `GetTextMetrics()` 会受到映射模式的影响，所以尽量让绘制文本时的映射模式和获得尺寸的当前映射模式一致
**设备坐标系统**
* 坐标系和原点位置类似于`MM_TEXT`模式
* 设备坐标系统取决于获取设备环境变量时使用的函数
	* 屏幕坐标：用于与窗口无关的函数，根据屏幕上的点移动或寻找窗口的函数（带有DISPALY参数的CreateDC逻辑坐标将默认为屏幕坐标）
	* 全窗口坐标：整个窗口坐标(包括标题栏，滚动条什么的），从`GetWindowsDC()`获取的设备环境逻辑坐标会被默认为全窗口坐标
	* 客户区坐标：从`GetDC()` `BeginPaint()`获得的设备环境逻辑坐标被默认为客户区坐标
* `ScreenToClient(lpRect)` `ClientToScreen(lpRect)`实现了客户区坐标和屏幕坐标的相互转换
	* `GetWindowRect(hwnd, lpRect)`用于获取屏幕坐标下矩形（窗口）的位置信息
**视口与窗口**
* 视口->设备坐标（像素），窗口->逻辑坐标（随意）（仅在第五章使用）
* 映射模式：从窗口到视口的映射
* `DPtoLP(hdc, pPoints, iNumber)` `LPtoDP()`视口坐标与逻辑坐标互转
**MM_TEXT**
* 视口原点和窗口原点都可以重新设置
* `SetViewportOrgEx(hdc, x, y, lpPoint)` `SetWindowOrgEx(hdc, x, y, lpPoint)`设置视口和窗口的原点
	* 一般来说不同时使用
	* 设置视口原点的使用设备坐标，设置窗口原点的使用逻辑坐标
	* **设备点（0,0）永远位于左上角**
	* 相应的`GetViewportOrgEx(hdc, &pt)` `GetWindowOrgEx(hdc, &pt)`用来获取位置（多此一举）
**度量映射模式**
* 视口原点和窗口原点都可以改变
* 逻辑单位都是物理单位
* 窗口范围中纵轴都是负值！
**MM_ISOTROPIC**
* 可以控制逻辑单位的物理尺寸
* 允许改变窗口和视口范围（换算比例，即缩放），但是Windows会进行干涉，使得两坐标轴的逻辑单位表示相同的物理尺寸
* 第一次设置映射模式为`MM_ISOTROPIC`时窗口和视口范围与`MM_LOMETRIC`相同
* `SetWindowExtEx(hdc, x, y, lpSize)`使用逻辑坐标 `SetViewportExtEx(hdc, x, y, lpSize)`使用窗口坐标，用来设置范围
	* 设置范围意思是，一个逻辑单位代表几个物理单位
	* 一般都需要先设置窗口范围再设置视口范围
	* 设置好的范围只是换算比例，不能限制x, y的范围
**MM_ANISOTROPIC**
* 这种映射模式不要求两轴的逻辑单位表示相同的物理尺寸
* 设定这种映射模式时，它总是继承前面映射模式的范围
#矩形、区域、剪裁
函数 | 功能
---- | -----
FillRect(hdc, &rect, hBrush) | 用画刷填充矩形
FrameRect(hdc, &rect, hBrush) | 用画刷绘制矩形边框（不填充内部）
InvertRect(hdc, &rect) | 反转矩形框内的颜色
SetRect(&rect, x1, y1, x2, y2) | 设置矩形的坐标（比较快）
OffsetRect(&rect, x, y) | 沿坐标轴移动几个单位
InflateRect(&rect, x, y) | 缩放矩形的尺寸
SetRectEmpty(&rect) | 把矩形的各个字段设置为空
CopyRect(&DesRect, &SrcRect) | 复制
InstersertRect(&DesRect, &SrcRect1, &SrcRect2) | 取两个矩形的交集
UnionRect(&DesRect, &SrcRect1, &SrcRect2) | 取两个矩形的并集
BOOL IsRectEmpty(&rect) | 判断矩形是否为空
BOOL PtInRect(&rect, point) | 判断点是否在矩形内

**随即矩形**
* `BOOL PeekMessage(&msg, hwnd, wMsgFilterMin, wMsgFilterMasx, wRemoveMsg)`
	* 和`GetMessage()`类似，不同点是PeekMessage不会一直霸占程序控制权
	* `hwnd`指向你希望检查消息的那个窗口，输入NULL会检查所有窗口，输入-1会检查只有窗口句柄是NULL的消息
	* 三四两个参数表示要检查信息的种类，输入0表示全都检查
	* 最后一个参数表示如何处理处理后的消息
	* `PM_NOREMOVE`不删除处理后的消息
	* `PM_REMOVE`这不用说了
	* `PM_NOYIELD`不让系统闲下来（不明白啥意思）
**区域**
* 区域也是GDI对象，`HRGN`是句柄（用DeleteObject（）就能删的）
* `Create(图形)Rgn(Indirect)()`可以用来创建区域(图形)表示画面时用到的函数名字（Rect。。。）
* `CombineRgn(hDesRgn, hSrcRgn1, hSrcRgn2, iCombine)`用来合并两个区域
	* `hDesRgn`原来的区域会被销毁
	* `iCombine`结合方式标识符

iCombine | 新区域
---------- | -------
RGN_AND | 两个源的公共部分
RGN_OR | 两个源全部区域
RGN_XOR | 两个源的全部区域除了公共部分
RGN_DIFF | hSrcRgn1不在hSrcRgn2的部分
RGN_COPY | 忽略hSRCRgn2

* 返回值有四个
	* `NULLREGION`空区域
	* `SIMPLE`简单区域，基础图形
	* `COMPLEXREGION`复杂区域，基础图形组合
	* `ERROR`出错...
* 与`InvalidateRect()`类似，`InvalidateRgn()` `ValidateRgn()`对区域操作
	* 这个区域即为剪裁区域，不能在他外面绘图