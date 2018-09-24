#include "stdafx.h"
#include "utils.h"
#include "Monster.h"
#include "baseaddr.h"
char * Monster::GetName(DWORD obj)
{
	auto len = utils::read<DWORD>(obj + 0x30);
	if (!len)
	{
		return nullptr;
	}

	if (len < 0x10)
	{
		return (char*)(obj + 0x20);
	}
	else {
		auto tem = utils::read<DWORD>(obj + 0x20);
		if (!tem) {
			return nullptr;
		}

		return (char*)(tem);
	}
}

char * Monster::GetHeroName(DWORD obj)
{
	auto base = utils::read<DWORD>(obj + 0x38);
	if (!base)
	{
		return nullptr;
	}

	auto len = utils::read<DWORD>(base + 0x30);
	if (!len) 
	{
		return nullptr;
	}
	if (len < 0x10)
	{
		return (char*)(base + 0x20);
	}
	else
	{
		auto temp = utils::read<DWORD>(base + 0x20);
		if (temp)
		{
			return (char*)(temp);
		}
	}
	return nullptr;
}

float Monster::GetCurrentHealth(DWORD obj)
{
	return utils::read<float>(obj + Base_MonsterCurrentHpOffset);
}

float Monster::GetMaxHealth(DWORD obj)
{
	return utils::read<float>(obj + Base_MonsterCurrentHpOffset + 0x10);
}

float Monster::GetCurrentMagic(DWORD obj)
{
	return utils::read<float>(obj + Base_MonsterCurrentMpOffset);
}

float Monster::GetMaxMagic(DWORD obj)
{
	return utils::read<float>(obj + Base_MonsterCurrentMpOffset + 0x10);
}

PEP_POINT_3D Monster::GetCurrentPos(DWORD obj)
{
	 return PEP_POINT_3D(obj + 0x104);
}

PEP_POINT_3D Monster::GetModelPos(DWORD obj)
{
	return PEP_POINT_3D(obj + 0x7C);
}

bool Monster::BDead(DWORD obj)
{
	return utils::read<bool>(obj + Base_MonsterDeadTagOffset) || GetCurrentHealth(obj) < 1.0f;
}

bool Monster::BMoving(DWORD obj)
{
	return utils::read<bool>(obj + Base_MonsterBMovingOffset);
}

bool Monster::BVisable(DWORD obj)
{
	return utils::read<bool>(obj + Base_MonsterBVisableOffset + Base_MonsterBVisableOffset2);
}

DWORD Monster::GetType(DWORD obj)
{
	//return utils::read<DWORD>(obj + 0x18);
	auto temp = utils::read<DWORD>(obj + 0x4/*0x18*/);
	if (temp)
	{
		return utils::read<DWORD>(temp);
	}

	return 0;
}

DWORD Monster::GetCamp(DWORD obj)
{
	return utils::read<DWORD>(obj + 0x14);
}

PEP_POINT_3D Monster::GetOrientation(DWORD obj)
{
	return (PEP_POINT_3D)(obj + Base_MonsterOrientationXOffset);
}

float Monster::GetDefensePower(DWORD obj)
{
	return utils::read<float>(obj + Base_MonsterDefenseOffset);
}

float Monster::GetMoveSpeed(DWORD obj)
{
	return utils::read<float>(obj + Base_MonsterMoveSpeedOffset);
}

DWORD Monster::GetObjectForSkill(DWORD obj)
{
	return utils::read<DWORD>(obj + 0x0e4);
}

float Monster::GetModelWidth(DWORD obj)
{
	auto monsPnt = GetCurrentPos(obj);
	auto modelPnt = GetModelPos(obj);

	if (!monsPnt || !modelPnt)
	{
		return 0.0f;
	}

	auto ret = monsPnt->x - modelPnt->x;
	return ret > 0.0f ? ret : -ret;
}

