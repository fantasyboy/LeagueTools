#pragma once
#include <mutex>
#include <vector>
#include "Monster.h"
#include "GameMenu.h"
extern std::vector<DWORD> g_personList;
extern std::vector<DWORD> g_monsterList;
extern std::mutex g_mutex;
extern std::vector<DWORD> g_personList2;
extern  HANDLE g_evt;


class CTargetSelecter
{
	CTargetSelecter();
	CTargetSelecter(const CTargetSelecter&) = delete;
	void operator= (const CTargetSelecter&) = delete;
	static CTargetSelecter* m_pInstance;
	static std::mutex m_mutex;
	std::vector<DWORD> m_personList;
	std::vector<DWORD> m_monsterList;
public:
	~CTargetSelecter();
	static CTargetSelecter* GetInstance();

	bool travse();
	void swapPersonList();
	void swapMonsterList();
	std::vector<DWORD>& GetPersonList();
};

//获取血量最低的怪物
DWORD GetHealthLowerestPerson(std::vector<DWORD>& list, float dis);
//获取蛇女中毒的怪物
DWORD GetCassiopeiaPoisonPerson(std::vector<DWORD> &list, float dis);
//判断是否有小兵遮挡 (小兵列表， 目标坐标 , 技能范围)
bool IsCoveredByMonster(std::vector<DWORD>& list, DWORD mons , float dis, float width,float delay, float misSpeed);

//获取危险等级最高的怪物
DWORD GetDangerousMostPerson(std::vector<MONSTER_OBJECT>& list, float dis, DWORD percent);