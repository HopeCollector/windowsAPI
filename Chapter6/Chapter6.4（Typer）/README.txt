####消息顺序
1. `CreateWindow()`
	* 产生`WM_CREATE`，并立即调用窗口过程
2. `ShowWindow()`
	* 按顺序产生`WM_SETFOCUS`，`WM_SIZE`，并立即调用窗口过程处理
3. `UpDateWindow()`
	* `WM_PAINT`
4. 消息队列（正常运行）

####每条消息的作用
* `WM_CREATE` 获取字体尺寸
* `WM_SIZE` 获取客户区尺寸
	* 计算行/列可容纳最多字符数
	* 重置数组
	* 无效化客户区
* `WM_PAINT` 打印数组
* `WM_KEYDOWN` `WM_CHAR` 处理各种击键消息
