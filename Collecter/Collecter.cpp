// Collecter.cpp : WinMain ��ʵ��


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
		// TODO : ���� CoInitializeSecurity ��Ϊ�����ṩ�ʵ��İ�ȫ����
		// ���� - PKT ����������֤��
		// RPC_C_IMP_LEVEL_IDENTIFY ��ģ�⼶��
		// �Լ��ʵ��ķ� null ��ȫ˵������

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

