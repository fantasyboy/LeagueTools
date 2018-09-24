#include "stdafx.h"
#include "Skill.h"
#include "utils.h"
#include "baseaddr.h"
#include "GameCall.h"
Skill::Skill(DWORD dwNodeBase, DWORD _index) : m_dwNodeBase(dwNodeBase), m_index(_index) , m_Distance(0.0f), m_skWidth(0.0f), m_bJudgmentCover(false)
{
	auto temp = utils::read<DWORD>(dwNodeBase + Base_SkillOffset_Object);
	if (temp)
	{
		auto temp2 = utils::read<DWORD>(temp + 0x34);
		if (temp2)
		{
			//读取技能范围
			memcpy(m_skDistance1, (void*)(temp2 + Base_SkillOffset_Range), 0x1c);
			memcpy(m_skDistance2, (void*)(temp2 + Base_SkillOffset_Range + 0x1c), 0x1c);
			memcpy(m_skBreadth, (void*)(temp2 + Base_SkillOffset_Range + 0x1c + 0x1c), 0x1c);
			//读取技能攻击力
			memcpy(m_skAggressivity, (void*)(temp2 + Base_SkillAggressivityOffset), 0x1c);
		}
	}

}

Skill::~Skill()
{
}

DWORD Skill::GetIndex() const
{
	return m_index;
}

char * Skill::GetName() const
{
	auto base = utils::read<DWORD>(m_dwNodeBase + Base_SkillOffset_Object);
	if (!base)
	{
		return nullptr;
	}

	auto len = utils::read<DWORD>(base + 0x2c);
	if (len < 0x10)
	{
		return (char*)(base + 0x18);
	}
	else 
	{
		auto temp = utils::read<DWORD>(base + 0x18);
		if (temp)
		{
			return (char*)temp;
		}
	}
	return nullptr;
}

DWORD Skill::GetLevel() const
{
	return utils::read<DWORD>(m_dwNodeBase + 0x10);
}

bool Skill::BCoolDown() const
{
	auto time = utils::read<float>(m_dwNodeBase + 0x18);;
	return GameCall::GetGameTickTime() > time;
}

float Skill::GetExpendMp() const
{
	auto temp = utils::read<DWORD>(m_dwNodeBase + Base_SkillOffset_Object);
	if (!temp)
	{
		return 0.0f;
	}
	auto temp2 = utils::read<DWORD>(temp + 0x34);
	if (!temp2)
	{
		return 0.0f;
	}
	auto ret =  utils::read<float>(temp2 + Base_SkillMpExpendOffset + GetLevel() * 4);
	return ret;
}

float Skill::GetDistance() 
{
	if (m_Distance > 0)
	{
		return m_Distance;
	}

	m_Distance = GetDistance2();
	if (m_Distance > 0)
	{
		return m_Distance;
	}


	m_Distance = GetDistance1();
	if (m_Distance > 0)
	{
		
		return m_Distance;
	}

	return 0.0f;
}

float Skill::GetDistance1() 
{
	auto level = GetLevel();
	if (level < 7)
	{
		return  m_skDistance1[level];
	}

	return 0.0f;
}

float Skill::GetDistance2() 
{
	auto level = GetLevel();
	if (level < 7)
	{
		return  m_skDistance2[level];
	}

	return 0.0f;
}

float Skill::GetBreadth()
{
	auto level = GetLevel();
	if (level < 7)
	{
		return  m_skBreadth[level];
	}

	return 0.0f;
}

float Skill::GetSkillAggressivity() const
{
	auto level = GetLevel();
	if (level < 7) {
		return m_skAggressivity[level];
	}

	return 0.0f;
}

DWORD Skill::GetSkillType()
{
	auto temp = utils::read<DWORD>(m_dwNodeBase + Base_SkillOffset_Type1);
	if (temp)
	{
		auto temp1 = utils::read<DWORD>(temp + Base_SkillOffset_Type2);
		if (temp1)
		{
			auto temp2 =  utils::read<DWORD>(temp1 + 0x4);
			if (temp2)
			{
				auto temp3 = utils::read<DWORD>(temp2);
				if (temp3)
				{
					return utils::read<DWORD>(temp3 + 0x8);
				}
			}
		}
	}
	return 0;
}

float Skill::GetBaseAggressivity() const
{
	return utils::read<float>(m_dwNodeBase + Base_CassiopeiaAggressivity);
}

float Skill::GetXerathPowerTime()
{
	return  utils::read<float>(m_dwNodeBase + Base_XerathPowerTimeOffset);
}

DWORD Skill::GetNodeBase()
{
	return m_dwNodeBase;
}

void Skill::SetDistance(float dis)
{
	m_Distance = dis;
}

void Skill::SetWidth(float width)
{
	m_skWidth = width;
}

float Skill::GetWidth()
{
	return m_skWidth;
}

void Skill::SetJudgmentCover(bool _b)
{
	m_bJudgmentCover = _b;
}

bool Skill::GetJudgmentCover()
{
	return m_bJudgmentCover;
}
