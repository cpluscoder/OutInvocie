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
	DWORD dwSessionId = 0;/// ��ǰ�Ự��ID
	HANDLE hUserToken = NULL;/// ��ǰ��¼�û�������
	HANDLE hUserTokenDup = NULL;/// ���Ƶ��û�����
	HANDLE hPToken = NULL;/// ��������
	DWORD dwCreationFlags = 0;

	/// �õ���ǰ��ĻỰID, ����¼�û��ĻỰID
	dwSessionId = ::WTSGetActiveConsoleSessionId();
	do
	{
		///	��ȡ��ǰ��¼�û���������Ϣ
		if(!::WTSQueryUserToken(dwSessionId, &hUserToken))
		{
			dwRet = ::GetLastError();
			break;
		}

		///	��������
		dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;

		ZeroMemory(&si,sizeof(STARTUPINFO));
		ZeroMemory(&pi,sizeof(pi));

		si.cb = sizeof(STARTUPINFO);

		///	ָ���������̵Ĵ���վ, Windows��Ψһ�ɽ����Ĵ���վ����WinSta0\Default
		si.lpDesktop = "winsta0\\default";

		///	�򿪽�������
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

		///	����DEBUGȨ�޵�UID
		LUID luid;
		if(!::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			dwRet = ::GetLastError();
			break;
		}

		///	����������Ϣ
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		///	���Ƶ�ǰ�û�������
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

		///	���õ�ǰ���̵�������Ϣ
		if(!::SetTokenInformation(hUserTokenDup, TokenSessionId, (void *)&dwSessionId, sizeof(DWORD)))
		{
			dwRet = ::GetLastError();
			break;
		}

		///	Ӧ������Ȩ��
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

		///	�������̻�����, ��֤�����������û�����Ļ�����
		LPVOID pEnv = NULL;
		if(::CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
		{
			dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
		}
		else
		{
			pEnv = NULL;
		}

		///	�����û�����
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

	///	�رվ��
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
