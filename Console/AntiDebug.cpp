#include "stdafx.h"
#include "AntiDebug.h"



CAntiDebug::CAntiDebug()
{
}


CAntiDebug::~CAntiDebug()
{
	//释放检测调试器线程
	if (m_checkDebugerHandle.joinable())
	{
		m_checkDebugerHandle.join();
	}
}

bool CAntiDebug::Initialize()
{
	//设置VEH检测硬断
	::AddVectoredContinueHandler(1, VectoredHandler);
	//设置检测调试器线程。
	m_checkDebugerHandle = std::thread(std::bind(&CAntiDebug::CheckDebugerPresentThread, this));

	return false;
}

void CAntiDebug::CheckDebugerPresentThread()
{

	while (true)
	{
		//1.检测是否被调试

		Sleep(5000);
	}
}

bool CAntiDebug::IsDebugerPresent()
{
	bool ret = false;
	__asm
	{
		mov eax, fs: 18h;
		mov eax, [eax + 30h];
		movzx eax, byte ptr[eax + 2];
		//mov ret, eax;
	}
	return ret;
}

LONG CALLBACK VectoredHandler(PEXCEPTION_POINTERS ExceptionInfo)
{

	if (ExceptionInfo->ContextRecord->Dr0 != 0
		|| ExceptionInfo->ContextRecord->Dr1 != 0
		|| ExceptionInfo->ContextRecord->Dr2 != 0
		|| ExceptionInfo->ContextRecord->Dr3 != 0)
	{
		AfxMessageBox("检测到硬件断点");
		ExceptionInfo->ContextRecord->Dr0 = 0;
		ExceptionInfo->ContextRecord->Dr1 = 0;
		ExceptionInfo->ContextRecord->Dr2 = 0;
		ExceptionInfo->ContextRecord->Dr3 = 0;
	}
	//设置新的eip 让程序调转到safe执行
	//ExceptionInfo->ContextRecord->Eip = Vehaddr;
	return EXCEPTION_CONTINUE_EXECUTION;
}
