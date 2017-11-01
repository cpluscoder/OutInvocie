// Discover.cpp : WinMain 的实现


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
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非 null 安全说明符。

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

