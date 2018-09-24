#include "stdafx.h"
#include "SkillService.h"
#include "baseaddr.h"
#include "utils.h"

CSkillService::CSkillService(DWORD dwObjectBase) : m_dwObjectBase(dwObjectBase)
{
}

CSkillService::~CSkillService()
{
}

void CSkillService::travse()
{
	m_skillList.clear();

	auto dwBase = m_dwObjectBase + Base_SkillTravseOffset1 + Base_SkillTravseOffset2;
	for (auto i = 0; i != 0x3f; i++)
	{
		auto skillBase = utils::read<DWORD>(dwBase + i * 4);
		//技能地址 或者 技能对象 不存在，就继续遍历
		if (skillBase == 0 || 0 == utils::read<DWORD>(skillBase + Base_SkillOffset_Object))
		{
			continue;
		}
		Skill sk(skillBase, i);
		m_skillList.push_back(sk);
	}
}

Skill* CSkillService::GetSkillByIndex(DWORD index)
{
	//travse();
	if (index >= m_skillList.size())
	{
		return nullptr;
	}
	return &m_skillList.at(index);
}

