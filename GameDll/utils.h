#pragma once

#include<memory>
#include<string>
#include "StringConvert.h"


namespace utils
{
	//调试函数
	void log(char* _format, ...);

	//读内存数据
	template<typename T>
	T read(const DWORD& obj)
	{
		__try{
			T value = *(T*)(obj);
			return value;
		}
		__except (1){
			return T();
		}
	}

	float GetDistance(PEP_POINT_3D pt1, PEP_POINT_3D pt2);
}