#include "stdafx.h"
#include "Buffer.h"
#include "utils.h"
#include "baseaddr.h"
char * Buffer::GetName(DWORD obj)
{
	auto temp = utils::read<DWORD>(obj + 0x4);
	if (!temp)
	{
		return nullptr;
	}
	return (char*)(temp + 0x8);
}

DWORD Buffer::GetBufferCount(DWORD obj)
{
	auto temp1 = utils::read<DWORD>(obj + 0x20);
	auto temp2 = utils::read<DWORD>(obj + 0x1C);
	auto temp3 = (temp1 - temp2) >> 3;
	if (temp3 < 0x2)
	{
		return utils::read<DWORD>(obj + Base_BufferCountOffset);
	}
	else
	{
		return  temp3;
	}
}
