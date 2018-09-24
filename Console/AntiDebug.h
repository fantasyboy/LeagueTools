#pragma once
/*
加载反调试
*/
#include <thread>
#include <functional>

class CAntiDebug
{
public:
	CAntiDebug();
	~CAntiDebug();

	
	bool Initialize();

	//检测调试器线程
	void CheckDebugerPresentThread();
private:
	//检测调试器
	bool IsDebugerPresent();
	//检测硬件断点


private:
	std::thread m_checkDebugerHandle;
};

LONG CALLBACK VectoredHandler(
	_In_ PEXCEPTION_POINTERS ExceptionInfo
);

