// Discover.cpp : WinMain ��ʵ��


#include "stdafx.h"
#include "resource.h"
#include "Discover_i.h"


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

		return S_OK;
	}
	};

CDiscoverModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

