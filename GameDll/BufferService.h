#pragma once
#include "Buffer.h"
#include <vector>
class CBufferService
{
public:
	CBufferService(DWORD dwObjectBase);
	~CBufferService();
	void travse();
	std::vector<DWORD>& GetBufferList();
private:
	DWORD m_dwObjectBase;
	std::vector<DWORD> m_bufferList;
};

