// Collecter.cpp : WinMain 的实现


#include "stdafx.h"
#include "resource.h"
#include "Collecter_i.h"


#include <stdio.h>

class CCollecterModule : public ATL::CAtlServiceModuleT< CCollecterModule, IDS_SERVICENAME >
	{
public :
	DECLARE_LIBID(LIBID_CollecterLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COLLECTER, "{8A430058-4864-4EC2-A989-17335166998E}")
		HRESULT InitializeSecurity() throw()
	{
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非 null 安全说明符。

		return S_OK;
	}
	};

CCollecterModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

