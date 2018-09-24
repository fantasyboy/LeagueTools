#pragma once
#include "Skill.h"
#include <vector>

class CSkillService
{
public:
	CSkillService(DWORD dwObjectBase);
	~CSkillService();
	void travse();
	Skill* GetSkillByIndex(DWORD index);

private:
	DWORD m_dwObjectBase;
	std::vector<Skill> m_skillList;
};

