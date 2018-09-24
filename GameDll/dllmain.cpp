// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "dllmain.h"
#include "GameManager.h"
#include "utils.h"
#include "HookToMainThread.h"
#include "ReflectiveLoader.h"

extern "C" HINSTANCE hAppInstance;


HANDLE g_mainHandle = { 0 };
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_QUERY_HMODULE:
		if (lpReserved != NULL)
			*(HMODULE *)lpReserved = hAppInstance;
		break;
	case DLL_PROCESS_ATTACH:
	{
		hAppInstance = hModule;
		DisableThreadLibraryCalls(hModule);

		g_mainHandle = ::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(ThreadProc), NULL, NULL, NULL);
	
		break;
	}
	case DLL_THREAD_ATTACH:
	{
		break;
	}
	case DLL_THREAD_DETACH:
	{
		break;
	}
	case DLL_PROCESS_DETACH:
	{	
		if (g_mainHandle)
		{
			CloseHandle(g_mainHandle);
		}
		
		CHookToMainThread::GetInstance()->DestoryInstance();
		CGameManager::GetInstance()->setExitTag(true);
		CGameManager::GetInstance()->DestoryInstance();
		break;
	}
		
	}
	return TRUE;
}


DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	//Sleep(1000);
	CGameManager::GetInstance()->Initialize();
	return 0;
}
