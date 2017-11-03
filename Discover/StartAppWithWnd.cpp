#include "StdAfx.h"
#include "StartAppWithWnd.h"

#include <Wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")
#include <Userenv.h>
#pragma comment(lib, "Userenv.lib")


CStartAppWithWnd::CStartAppWithWnd(void)
{
}


CStartAppWithWnd::~CStartAppWithWnd(void)
{
}


DWORD CStartAppWithWnd::LaunchWin7SessionProcess(LPTSTR lpCommand)
{
	DWORD dwRet = 0;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	DWORD dwSessionId = 0;/// 当前会话的ID
	HANDLE hUserToken = NULL;/// 当前登录用户的令牌
	HANDLE hUserTokenDup = NULL;/// 复制的用户令牌
	HANDLE hPToken = NULL;/// 进程令牌
	DWORD dwCreationFlags = 0;

	/// 得到当前活动的会话ID, 即登录用户的会话ID
	dwSessionId = ::WTSGetActiveConsoleSessionId();
	do
	{
		///	读取当前登录用户的令牌信息
		if(!::WTSQueryUserToken(dwSessionId, &hUserToken))
		{
			dwRet = ::GetLastError();
			break;
		}

		///	创建参数
		dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;

		ZeroMemory(&si,sizeof(STARTUPINFO));
		ZeroMemory(&pi,sizeof(pi));

		si.cb = sizeof(STARTUPINFO);

		///	指定创建进程的窗口站, Windows下唯一可交互的窗口站就是WinSta0\Default
		si.lpDesktop = "winsta0\\default";

		///	打开进程令牌
		if(!::OpenProcessToken(
			GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES
			| TOKEN_QUERY
			| TOKEN_DUPLICATE
			| TOKEN_ASSIGN_PRIMARY
			| TOKEN_ADJUST_SESSIONID
			| TOKEN_READ
			| TOKEN_WRITE,
			&hPToken))
		{
			dwRet = ::GetLastError();
			break;
		}

		///	查找DEBUG权限的UID
		LUID luid;
		if(!::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			dwRet = ::GetLastError();
			break;
		}

		///	设置令牌信息
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		///	复制当前用户的令牌
		if(!::DuplicateTokenEx(
			hPToken,
			MAXIMUM_ALLOWED,
			NULL,
			SecurityIdentification,
			TokenPrimary,
			&hUserTokenDup))
		{
			dwRet = ::GetLastError();
			break;
		}

		///	设置当前进程的令牌信息
		if(!::SetTokenInformation(hUserTokenDup, TokenSessionId, (void *)&dwSessionId, sizeof(DWORD)))
		{
			dwRet = ::GetLastError();
			break;
		}

		///	应用令牌权限
		if(!AdjustTokenPrivileges(
			hUserTokenDup,
			FALSE,
			&tp,
			sizeof(TOKEN_PRIVILEGES),
			(PTOKEN_PRIVILEGES)NULL,
			NULL))
		{
			dwRet = ::GetLastError();
			break;
		}

		///	创建进程环境块, 保证环境块是在用户桌面的环境下
		LPVOID pEnv = NULL;
		if(::CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
		{
			dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
		}
		else
		{
			pEnv = NULL;
		}

		///	创建用户进程
		if(!::CreateProcessAsUser(
			hUserTokenDup,
			NULL,
			lpCommand,
			NULL,
			NULL,
			FALSE,
			dwCreationFlags,
			pEnv,
			NULL,
			&si,
			&pi))
		{
			dwRet = ::GetLastError();
			break;
		}
	} while (false);

	///	关闭句柄
	if(NULL != hUserToken)
	{
		::CloseHandle(hUserToken);
		hUserToken = NULL;
	}

	if(NULL != hUserTokenDup)
	{
		::CloseHandle(hUserTokenDup);
		hUserTokenDup = NULL;
	}

	if(NULL != hPToken)
	{
		::CloseHandle(hPToken);
		hPToken = NULL;
	}

	return dwRet;
}
