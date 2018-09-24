#include "stdafx.h"
#include "BufferService.h"
#include "baseaddr.h"
#include "utils.h"
CBufferService::CBufferService(DWORD dwObjectBase): m_dwObjectBase(dwObjectBase)
{
}

CBufferService::~CBufferService()
{
}

void CBufferService::travse()
{
	m_bufferList.clear();

	auto startAddr = utils::read<DWORD>(m_dwObjectBase + Base_BufferOffset);
	auto endAddr = utils::read<DWORD>(m_dwObjectBase + Base_BufferOffset + 0x4);

	auto size = (endAddr - startAddr) >> 3;
	for (auto i = 0; i != size; i++)
	{
		auto tempObj = utils::read<DWORD>(startAddr + 8 * i);
		if (tempObj == 0) 
		{
			continue;
		}

		if (utils::read<DWORD>(tempObj + 0x4) == 0)
		{
			//不是图标对象 继续
			continue;
		}

		m_bufferList.push_back(tempObj);
	}
}

std::vector<DWORD>& CBufferService::GetBufferList()
{
	travse();
	return m_bufferList;
}
