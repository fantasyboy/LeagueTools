#include "stdafx.h"
#include "utils.h"

void utils::log( char * _format, ...)
{
#ifdef _USER_DBG

	std::string temp;
	va_list marker = { 0 };
	va_start(marker, _format);
	size_t num_of_chars = _vscprintf(_format, marker);
	if (num_of_chars > temp.capacity())
	{
		temp.reserve(num_of_chars + 1);
	}
	vsprintf_s(const_cast<char*>(temp.c_str()), num_of_chars + 1, _format, marker);

	OutputDebugString(temp.c_str());

#endif // _USER_DBG
}


float utils::GetDistance(PEP_POINT_3D pt1, PEP_POINT_3D pt2)
{
	if (!pt1 || !pt2)
	{
		return 0.0f;
	}
	return (pt1->x - pt2->x)*(pt1->x - pt2->x) + (pt1->y - pt2->y) * (pt1->y - pt2->y);
}
