// Discover.cpp : WinMain ��ʵ��


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
		// TODO : ���� CoInitializeSecurity ��Ϊ�����ṩ�ʵ��İ�ȫ����
		// ���� - PKT ����������֤��
		// RPC_C_IMP_LEVEL_IDENTIFY ��ģ�⼶��
		// �Լ��ʵ��ķ� null ��ȫ˵������

		// ������ȫ˵����
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

	///	����ע��
	HRESULT RegisterAppId(bool bService = false) throw();

	///	��ͣ
	void OnPause() throw();

	///	ֹͣ
	void OnStop() throw();

	///	��������
	void OnContinue() throw();

	///	����ͬ�ķ��������Ϣ
	void Handler(DWORD dwOpcode) throw();

	///	��Ϣ��Ӧ
	HRESULT PreMessageLoop(int nShowCmd) throw();

	///	��������
	HRESULT Start(_In_ int nShowCmd) throw();
};

CDiscoverModule _AtlModule;

/**
  ȷ����Ҫ��Ự������ں˶���ʹ���˴���Global\�ַ���������ǰ׺
  ���ַ�����ζ������������ڻỰȫ��(Session-Global)�����ռ�
*/
HANDLE g_hSessionEvent;/// ȫ�ֱ���, �¼�����
void CreateSessionEvent()
{
	///	�����¼�����
	g_hSessionEvent = ::CreateEvent(NULL, TRUE, FALSE, "Global\\ATLDemoGlobalSessionEvent");

	///	���¼���������Ϊ���ź�״̬
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
		/// �����Ѿ���װ
		if(IsInstalled())
		{
			/// �򿪷��������
			SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SERVICE_CHANGE_CONFIG);
			if(hSCM == NULL)
			{
				hr = ATL::AtlHresultFromLastError();
			}
			else
			{
				/// �򿪷���, m_szServiceNameΪ�����Ա����, ����ǰ��������
				/// ��������Դ�ļ��б��String Table���޸�
				SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_CHANGE_CONFIG);
				if(hService != NULL)
				{
					/// �޸ķ�������
					::ChangeServiceConfig(
						hService,
						SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, /// ��������, ������!!!
						SERVICE_AUTO_START, ///	�����Զ�����
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL,
						m_szServiceName);

					///	����������Ϣ
					SERVICE_DESCRIPTION sDescription;
					TCHAR szDescription[1024];
					ZeroMemory(szDescription, sizeof(szDescription));
					ZeroMemory(&sDescription, sizeof(SERVICE_DESCRIPTION));

					/// ��������
					lstrcpy(szDescription, "DiscoverService");
					sDescription.lpDescription = szDescription;

					///	�޸ķ���������Ϣ
					::ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sDescription);

					///	�رշ�����
					::CloseServiceHandle(hService);

				}
				else
				{
					hr = ATL::AtlHresultFromLastError();
				}
			}

			/// �رշ�����������
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
	/// �÷���������ͣ�ͼ�������
	m_status.dwControlsAccepted = m_status.dwControlsAccepted | SERVICE_ACCEPT_PAUSE_CONTINUE;

	HRESULT hr = __super::PreMessageLoop(nShowCmd);
	if(hr == S_FALSE)
	{
		hr = S_OK;//������Bug,��������д,������ܼ���
	}

	/// ������״̬����Ϊ����
	SetServiceStatus(SERVICE_RUNNING);

	/// д��ϵͳ��־
	LogEvent("Discover Service Start Successfully!");

	return hr;
}

void CDiscoverModule::Handler( DWORD dwOpcode ) throw()
{
	switch(dwOpcode)
	{
	case SERVICE_CONTROL_PAUSE:
		{
			/// ��ͣ
			OnPause();
		}
		break;
	case SERVICE_CONTROL_CONTINUE:
		{
			/// ����
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
	/// ���÷���״̬Ϊ��ͣ
	SetServiceStatus(SERVICE_PAUSED);

	__super::OnPause();
}

void CDiscoverModule::OnStop() throw()
{
	/// ���÷���״̬Ϊֹͣ
	SetServiceStatus(SERVICE_STOPPED);

	__super::OnStop();
}

void CDiscoverModule::OnContinue() throw()
{
	/// ���÷���״̬Ϊ����
	SetServiceStatus(SERVICE_RUNNING);

	__super::OnContinue();
}






