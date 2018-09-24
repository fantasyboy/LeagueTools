#include "stdafx.h"
#include "NetworkService.h"
#include "resource.h"
#include "VMProtectSDK.h"
#include "VAuth.h"

std::mutex CNetworkService::m_mutex;
CNetworkService* CNetworkService::m_pInstance = nullptr;

CNetworkService::CNetworkService()
{
	m_hInstVauthDll = { 0 };
}

bool CNetworkService::ResDllToWindowsPath()
{
	VMProtectBegin(__FUNCTION__);
	HRSRC hIdrDllALLp = ::FindResource(NULL, MAKEINTRESOURCE(IDR_VD_DLL), "vd");
	if (!hIdrDllALLp)
	{
		return false;
	}

	auto IdrSize = SizeofResource(NULL, hIdrDllALLp);
	if (!IdrSize)
	{
		return false;
	}

	HGLOBAL hGlobal = LoadResource(NULL, hIdrDllALLp);
	if (!hGlobal)
	{
		return false;
	}

	LPVOID pBuffer = LockResource(hGlobal);
	if (!pBuffer) 
	{
		return false;
	}

	auto hFile = CreateFile("C:\\Windows\\VAuth.dll", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hFile)
	{
		return false;
	}

	DWORD dwWriteSize;
	WriteFile(hFile, hGlobal, IdrSize, &dwWriteSize, NULL);
	CloseHandle(hFile);

	m_hInstVauthDll = LoadLibrary("C:\\Windows\\VAuth.dll");
	if (!m_hInstVauthDll)
	{
		return false;
	}

	VMProtectEnd();
	return true;
}

bool CNetworkService::ResToMemoryAndMemoryLoadVnc()
{
	VMProtectBegin(__FUNCTION__);
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_VD_VNC), "vd");
	if (!hRsrc) {
		return false;
	}

	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (!dwSize) {
		return false;
	}

	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (!hGlobal) {
		return false;
	}

	LPVOID pBuffer = LockResource(hGlobal);
	if (NULL == pBuffer) {
		return false;
	}

	auto pInputVNC = (InputVNC)GetProcAddress(m_hInstVauthDll, "InputVNC");
	if (!pInputVNC) {
		return false;
	}
	pInputVNC((BYTE*)pBuffer, dwSize);

	VMProtectEnd();
	return true;
}

bool CNetworkService::InitializeNetServer()
{
	VMProtectBegin(__FUNCTION__);
	auto pInitialize = (Initialize)GetProcAddress(m_hInstVauthDll, "Initialize");
	if (pInitialize) 
	{
		auto  ret = pInitialize("{C1E14188-7629-4B10-B2B5-E03B523C90E6}");
		if (ret >= 0)
		{
			return true;
		}
	}
	VMProtectEnd();

	return false;
}


CNetworkService::~CNetworkService()
{

}

CNetworkService * CNetworkService::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CNetworkService();
		}
		m_mutex.unlock();
	}
	return m_pInstance;
}

bool CNetworkService::Init()
{
	//ÊÍ·Ådll
	if (!ResDllToWindowsPath())
	{
		return false;
	}

	//ÄÚ´æ¼ÓÔØVNC
	if (!ResToMemoryAndMemoryLoadVnc())
	{
		return false;
	}

	if (!InitializeNetServer())
	{
		return false;
	}

	return true;
}

int CNetworkService::Login(char * username, char * password)
{
	VMProtectBegin(__FUNCTION__);
	auto pUserAuth = (UserAuth)GetProcAddress(m_hInstVauthDll, "UserAuth");
	VMProtectEnd();
	if (!pUserAuth)
	{
		return -1;
	}

	return  pUserAuth(username, password);


}

int CNetworkService::Register(char * username, char * password)
{
	VMProtectBegin(__FUNCTION__);
	auto pUserRegister = (UserRegister)GetProcAddress(m_hInstVauthDll, "UserRegister");
	VMProtectEnd();

	if (!pUserRegister)
	{
		return -1;
	}

	return pUserRegister(username, password, 0, 1, 1, 0);

}

int CNetworkService::UserAddTime(char * username, char * key, char * recommond, SHORT& days, SHORT& pnts)
{
	VMProtectBegin(__FUNCTION__);
	auto pAddTime = (AddTime)GetProcAddress(m_hInstVauthDll, "AddTime");
	VMProtectEnd();
	if (!pAddTime)
	{
		return -1;
	}

	return pAddTime(key, username, recommond, &days, &pnts);

}

bool CNetworkService::BValid()
{
	VMProtectBegin(__FUNCTION__);
	auto pIsValid = (IsValid)GetProcAddress(m_hInstVauthDll, "IsValid");
	VMProtectEnd();
	if (!pIsValid)
	{
		return false;
	}
	return pIsValid();
}

int CNetworkService::UserUnBind()
{
	//typedef int (CALLBACK* Unbind)();
	VMProtectBegin(__FUNCTION__);
	auto pUnbind = (Unbind)GetProcAddress(m_hInstVauthDll, "Unbind");
	VMProtectEnd();
	if (!pUnbind)
	{
		return -1;
	}
	return pUnbind();
}

char * CNetworkService::UserGetValiDate()
{
	//typedef char*  (CALLBACK* GetValidity)();
	VMProtectBegin(__FUNCTION__);
	auto pGetValidity = (GetValidity)GetProcAddress(m_hInstVauthDll, "GetValidity");
	VMProtectEnd();
	if (!pGetValidity)
	{
		return nullptr;
	}
	return pGetValidity();
}

int CNetworkService::UserDeductHour(int i)
{
	//typedef int  (CALLBACK* DeductHour)(DWORD hour);
	VMProtectBegin(__FUNCTION__);
	auto pDeductHour = (DeductHour)GetProcAddress(m_hInstVauthDll, "DeductHour");
	VMProtectEnd();
	if (!pDeductHour)
	{
		return -1;
	}
	return pDeductHour(i);
}
