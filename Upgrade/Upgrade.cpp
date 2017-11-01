// Upgrade.cpp : WinMain 的实现


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
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非 null 安全说明符。

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

