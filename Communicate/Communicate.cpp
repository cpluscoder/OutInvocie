// Communicate.cpp : WinMain ��ʵ��


#include "stdafx.h"
#include "resource.h"
#include "Communicate_i.h"


#include <stdio.h>

class CCommunicateModule : public ATL::CAtlServiceModuleT< CCommunicateModule, IDS_SERVICENAME >
	{
public :
	DECLARE_LIBID(LIBID_CommunicateLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMMUNICATE, "{FCF92B35-C451-464C-B9CF-34C3E04CE4A7}")
		HRESULT InitializeSecurity() throw()
	{
		// TODO : ���� CoInitializeSecurity ��Ϊ�����ṩ�ʵ��İ�ȫ����
		// ���� - PKT ����������֤��
		// RPC_C_IMP_LEVEL_IDENTIFY ��ģ�⼶��
		// �Լ��ʵ��ķ� null ��ȫ˵������

		return S_OK;
	}
	};

CCommunicateModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

