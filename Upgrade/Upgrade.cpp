// Upgrade.cpp : WinMain ��ʵ��


#include "stdafx.h"
#include "resource.h"
#include "Upgrade_i.h"


#include <stdio.h>

class CUpgradeModule : public ATL::CAtlServiceModuleT< CUpgradeModule, IDS_SERVICENAME >
	{
public :
	DECLARE_LIBID(LIBID_UpgradeLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_UPGRADE, "{6CBCEE43-2A62-4DDD-9388-CDD126580E62}")
		HRESULT InitializeSecurity() throw()
	{
		// TODO : ���� CoInitializeSecurity ��Ϊ�����ṩ�ʵ��İ�ȫ����
		// ���� - PKT ����������֤��
		// RPC_C_IMP_LEVEL_IDENTIFY ��ģ�⼶��
		// �Լ��ʵ��ķ� null ��ȫ˵������

		return S_OK;
	}
	};

CUpgradeModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

