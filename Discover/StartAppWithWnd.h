#pragma once

/**
 Service在桌面创建具有System权限的窗口应用程序(貌似可以绕UAC)

 服务程序与用户桌面程序通信的解决办法
 1 如果服务需要通过发送消息的方式与用户交互，使用WTSSendMessage函数
     该函数几乎与MessageBox相同, 对于不需要过于完整的UI的服务，这是一种简单易行的方法, 而且足够安全, 因为所显示的信息框无法用于夺取对底层服务的控制
 2 如果服务需要更完整的UI, 请使用CreateProcessAsUser函数在发起请求的用户桌面创建进程
     需要注意, 依然需要在新创建的进程和原始服务之间进行通讯
 3 如果需要双向交互, 使用Windows Communication Foundation(WCF)
     一种用于.NET Remoting命名管线或任何其他进程间通讯(IPC)的机制(Windows消息除外), 可用于实现跨会话的通讯。
	 如果需要提升, WCF和Remoting是比直接提升用户(假设UAC没有过关闭)更安全的做法。
*/

class CStartAppWithWnd
{
public:
	explicit CStartAppWithWnd(void);
	virtual ~CStartAppWithWnd(void);

	DWORD _stdcall LaunchWin7SessionProcess(LPTSTR lpCommand);
};

