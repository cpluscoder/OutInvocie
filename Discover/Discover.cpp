// Discover.cpp : WinMain 的实现


#include "stdafx.h"
#include "resource.h"
#include "Discover_i.h"

#include "StartAppWithWnd.h"

#include <stdio.h>

class CDiscoverModule : public ATL::CAtlServiceModuleT< CDiscoverModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_DiscoverLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DISCOVER, "{FE7E7814-5E22-44B3-8EC9-B872DB1BF2BA}")
	
	HRESULT InitializeSecurity() throw()
	{
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非 null 安全说明符。

		// 创建安全说明符
		return ::CoInitializeSecurity(
			NULL, 
			-1, 
			NULL, 
			NULL, 
			RPC_C_AUTHN_LEVEL_NONE, 
			RPC_C_IMP_LEVEL_IDENTIFY, 
			NULL, 
			EOAC_NONE, 
			NULL);

		return S_OK;
	}

/// overload

	///	服务注册
	HRESULT RegisterAppId(bool bService = false) throw();

	///	暂停
	void OnPause() throw();

	///	停止
	void OnStop() throw();

	///	继续运行
	void OnContinue() throw();

	///	处理不同的服务控制消息
	void Handler(DWORD dwOpcode) throw();

	///	消息响应
	HRESULT PreMessageLoop(int nShowCmd) throw();

	///	启动服务
	HRESULT Start(_In_ int nShowCmd) throw();
};

CDiscoverModule _AtlModule;

/**
  确保需要跨会话共享的内核对象使用了带有Global\字符串的名称前缀
  该字符串意味着这个对象属于会话全局(Session-Global)命名空间
*/
HANDLE g_hSessionEvent;/// 全局变量, 事件对象
void CreateSessionEvent()
{
	///	创建事件对象
	g_hSessionEvent = ::CreateEvent(NULL, TRUE, FALSE, "Global\\ATLDemoGlobalSessionEvent");

	///	将事件对象设置为无信号状态
	::ResetEvent(g_hSessionEvent);
}

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

HRESULT CDiscoverModule::RegisterAppId( bool bService /*= false*/ ) throw()
{
	HRESULT hr = S_OK;
	BOOL res = __super::RegisterAppId(bService);
	if(bService)
	{
		/// 服务已经安装
		if(IsInstalled())
		{
			/// 打开服务管理器
			SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SERVICE_CHANGE_CONFIG);
			if(hSCM == NULL)
			{
				hr = ATL::AtlHresultFromLastError();
			}
			else
			{
				/// 打开服务, m_szServiceName为基类成员变量, 代表当前服务名称
				/// 可以在资源文件列表的String Table中修改
				SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_CHANGE_CONFIG);
				if(hService != NULL)
				{
					/// 修改服务配置
					::ChangeServiceConfig(
						hService,
						SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, /// 独立进程, 允许交互!!!
						SERVICE_AUTO_START, ///	服务自动启动
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						m_szServiceName);

					///	服务描述信息
					SERVICE_DESCRIPTION sDescription;
					TCHAR szDescription[1024];
					ZeroMemory(szDescription, sizeof(szDescription));
					ZeroMemory(&sDescription, sizeof(SERVICE_DESCRIPTION));

					/// 服务描述
					lstrcpy(szDescription, "DiscoverService");
					sDescription.lpDescription = szDescription;

					///	修改服务描述信息
					::ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sDescription);

					///	关闭服务句柄
					::CloseServiceHandle(hService);

				}
				else
				{
					hr = ATL::AtlHresultFromLastError();
				}
			}

			/// 关闭服务管理器句柄
			::CloseServiceHandle(hSCM);
		}
	}
	return hr;
}

HRESULT CDiscoverModule::Start(_In_ int nShowCmd) throw()
{
	CreateSessionEvent();

	//CStartAppWithWnd startApp;
	//startApp.LaunchWin7SessionProcess("C:\\Windows\\notepad.exe");

	return __super::Start(nShowCmd);
}

HRESULT CDiscoverModule::PreMessageLoop( int nShowCmd ) throw()
{
	/// 让服务允许暂停和继续操作
	m_status.dwControlsAccepted = m_status.dwControlsAccepted | SERVICE_ACCEPT_PAUSE_CONTINUE;

	HRESULT hr = __super::PreMessageLoop(nShowCmd);
	if(hr == S_FALSE)
	{
		hr = S_OK;//这里有Bug,必须这样写,后面才能继续
	}

	/// 将服务状态设置为启动
	SetServiceStatus(SERVICE_RUNNING);

	/// 写入系统日志
	LogEvent("Discover Service Start Successfully!");

	return hr;
}

void CDiscoverModule::Handler( DWORD dwOpcode ) throw()
{
	switch(dwOpcode)
	{
	case SERVICE_CONTROL_PAUSE:
		{
			/// 暂停
			OnPause();
		}
		break;
	case SERVICE_CONTROL_CONTINUE:
		{
			/// 继续
			OnContinue();
		}
		break;
	default:
		break;
	}

	__super::Handler(dwOpcode);
}

void CDiscoverModule::OnPause() throw()
{
	/// 设置服务状态为暂停
	SetServiceStatus(SERVICE_PAUSED);

	__super::OnPause();
}

void CDiscoverModule::OnStop() throw()
{
	/// 设置服务状态为停止
	SetServiceStatus(SERVICE_STOPPED);

	__super::OnStop();
}

void CDiscoverModule::OnContinue() throw()
{
	/// 设置服务状态为启动
	SetServiceStatus(SERVICE_RUNNING);

	__super::OnContinue();
}






