#include "stdafx.h"
#include "TargetSelecter.h"
#include "Monster.h"
#include "utils.h"
#include "baseaddr.h"
#include "Player.h"

//测试用
#ifdef _USER_DBG
#include "GameMenu.h"
#endif // _USER_DBG

 CTargetSelecter* CTargetSelecter::m_pInstance = nullptr;
 std::mutex CTargetSelecter::m_mutex;

 //交换数据
 std::vector<DWORD> g_personList;
 std::vector<DWORD> g_monsterList;
 std::mutex g_mutex;
 HANDLE g_evt = { 0 };
CTargetSelecter::CTargetSelecter()
{
	g_evt = CreateEvent(NULL, FALSE, FALSE, NULL);
}


CTargetSelecter::~CTargetSelecter()
{
}

CTargetSelecter * CTargetSelecter::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CTargetSelecter();
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

bool CTargetSelecter::travse()
{
	m_personList.clear();
	m_monsterList.clear();

	auto startAddr = utils::read<DWORD>(Base_MonsterArrayAddr);
	auto endAddr = utils::read<DWORD>(Base_MonsterArrayAddr + 0x4);

	if (!startAddr || !endAddr)
	{
		return false;
	}

	for (auto i = startAddr; i <= endAddr; i += 4)
	{
		auto temp = utils::read<DWORD>(i);
		if (!temp) {
			continue;
		}

		if (Monster::GetCamp(temp) != Player::GetCamp())
		{
			//如果不是友军，并且是玩家对象
			if (Monster::GetType(temp) == 0x4) 
			{
				m_personList.push_back(temp);
			}
			//如果是小兵
			else if (Monster::GetType(temp) == 0x6) 
			{

				//过滤未知的东子
				auto MaxHp = Monster::GetMaxHealth(temp);
				if (MaxHp< 1001.0f && MaxHp > 999.0f)
				{
					continue;
				}

				if (MaxHp < 101.0f && MaxHp > 99.0f)
				{
					continue;
				}

				//过滤和谐死亡后的眼尸体
				if (MaxHp < 77.0f && MaxHp > 75.0f)
				{
					continue;
				}


				if (MaxHp < 10.0f && MaxHp > 0.0f)
				{
					continue;
				}


				if (utils::GetDistance(Monster::GetCurrentPos(temp), Player::GetCurrPostion()) < 2000.0f * 2000.0f &&
					!Monster::BDead(temp) &&
					Monster::BVisable(temp))
				{
					m_monsterList.push_back(temp);
				}
			}
			else
			{
#ifdef _USER_DBG
				//auto menu = CGameMenu::GetInstance()->GetGameMenu();
				//if (menu->bFarming)
				//{
				//	if (utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(temp)) < 1500.0f * 1500.0f)
				//	{
				//		utils::log("%x , %s , (%f,%f), %x , %x",
				//			temp,
				//			Monster::GetName(temp),
				//			Monster::GetCurrentHealth(temp),
				//			Monster::GetMaxHealth(temp),
				//			Monster::GetType(temp),
				//			Monster::GetCamp(temp));
				//	}
				//}

#endif // _USER_DBG
			}


		}
	}

	return true;
}



void CTargetSelecter::swapPersonList()
{
	g_mutex.lock();
	g_personList.swap(m_personList);
	g_mutex.unlock();
}

void CTargetSelecter::swapMonsterList()
{
	g_mutex.lock();
	g_monsterList.swap(m_monsterList);
	g_mutex.unlock();
}

std::vector<DWORD>& CTargetSelecter::GetPersonList()
{
	// TODO: 在此处插入 return 语句
	return m_personList;
}


DWORD GetHealthLowerestPerson(std::vector<DWORD>& list, float dis)
{
	if (dis <= 0)
	{
		return 0;
	}
	float minHealth = FLT_MAX;
	DWORD obj = 0;

	for (auto &temp : list)
	{
		if (!temp)
		{
			continue;
		}
		auto disTemp = dis + Monster::GetModelWidth(temp) * 1.5f;
		if (Monster::BVisable(temp) && 
			!Monster::BDead(temp) &&
			utils::GetDistance(Monster::GetCurrentPos(temp), Player::GetCurrPostion()) < disTemp  * disTemp)
		{
			auto currentHealth = Monster::GetCurrentHealth(temp);
			if (currentHealth < minHealth)
			{
				minHealth = currentHealth;
				obj = temp;
			}
		}
	}

	return obj;
}

DWORD GetCassiopeiaPoisonPerson(std::vector<DWORD>& list, float dis)
{

	if (dis <= 0)
	{
		return 0;
	}
	float minHealth = FLT_MAX;
	DWORD obj = 0;

	float PoisonHealth = FLT_MAX;
	DWORD poisonObj = 0;

	for (auto temp : list)
	{
		if (!temp)
		{
			continue;
		}
		auto disTemp = dis + Monster::GetModelWidth(temp) * 1.5f;
		if (Monster::BVisable(temp) &&
			!Monster::BDead(temp) &&
			utils::GetDistance(Monster::GetCurrentPos(temp), Player::GetCurrPostion()) < disTemp  * disTemp)
		{
			auto currentHealth = Monster::GetCurrentHealth(temp);
			if (currentHealth< minHealth)
			{
				minHealth = currentHealth;
				obj = temp;
			}


			//获取怪物buff
			CBufferService cbf(temp);
			for (auto obj : cbf.GetBufferList())
			{
				if (!Buffer::GetName(obj))
				{
					continue;
				}

				if (strstr(Buffer::GetName(obj), "assiopeia") != NULL)
				{
					//说明怪物已经中毒了
					if (currentHealth < PoisonHealth)
					{
						PoisonHealth = currentHealth;
						poisonObj = temp;
					}
				}
			}
		}
	}

	if (poisonObj)
	{
		return poisonObj;
	}

	return obj;

}

bool IsCoveredByMonster(std::vector<DWORD>& list, DWORD mons, float dis, float width, float delay, float MissileSpeed)
{
	//预判坐标
	auto pnt = Player::ClacAnticipationPnt(mons, delay, MissileSpeed);

	//计算玩家到目标的角度
	//float anglesTarget = atan2(Monster::GetCurrentPos(mons)->y - Player::GetCurrPostion()->y, Monster::GetCurrentPos(mons)->x - Player::GetCurrPostion()->x);
	float anglesTarget = atan2(pnt.y - Player::GetCurrPostion()->y, pnt.x - Player::GetCurrPostion()->x);
	if (anglesTarget < 0)
	{
		anglesTarget = PI * 2.0f + anglesTarget;
	}
	//计算玩家到目标的距离
	auto dis2Player = sqrt(utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(mons)));
	auto monsWidth = Monster::GetModelWidth(mons);

	//获取到小兵列表
	for (auto &temp : list)
	{
		//如果小兵死亡，或者小兵不可见，就继续找下一个小兵,说明没有遮挡
		if (Monster::BDead(temp) || !Monster::BVisable(temp))
		{
			continue;
		}

		//计算小兵到玩家的圆半径
		auto disMons2Player = utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(temp));
		//1.如果小兵在技能范围外，就没有遮挡
		auto modelWidth = Monster::GetModelWidth(temp);
		if (disMons2Player > (dis + modelWidth)* (dis + modelWidth))
		{
			continue;
		}

		//2.如果小兵和玩家的距离 > (玩家到敌人得距离 + 敌人模型大小）,说明没有遮挡。
		if (disMons2Player > (dis2Player + monsWidth)* (dis2Player + monsWidth))
		{
			continue;
		}

		//计算小兵的弧度角大小,并且将角度转化为 0-2PI之间
		float angles = atan2(Monster::GetCurrentPos(temp)->y - Player::GetCurrPostion()->y, Monster::GetCurrentPos(temp)->x - Player::GetCurrPostion()->x);
		if (angles < 0)
		{
			angles = PI * 2.0f + angles;
		}
		//计算小兵和技能之间的夹角,如果绝对值 > 90度，直接返回
		auto destAngles = abs(anglesTarget - angles);
		if (destAngles > (PI / 2.0f))
		{
			continue;
		}

		//根据夹角求垂线距离
		auto dis2Pnt = sin(destAngles) * (sqrt(disMons2Player));

		//如果小兵中心点的位置 到 技能中心垂线的距离 < 技能宽度 + 怪物半径，说明技能碰撞
		if (dis2Pnt <= (modelWidth + width))
		{
			return true;
		}

	}

	return false;
}

DWORD GetDangerousMostPerson(std::vector<MONSTER_OBJECT>& list, float dis, DWORD percent)
{
	if (dis <= 0)
	{
		return 0;
	}

	//保存血量最低 & 低于设置的血量比例的怪物
	float minHealth = FLT_MAX;
	DWORD HealthMostLowObj = 0;

	//保存危险等级最高的怪物
	DWORD Dangerousobj = 0;
	int iDangerous = -1;
	for (auto &temp : list)
	{
		if (!temp.obj)
		{
			continue;
		}
		//判断怪物敌人是否在技能范围内
		auto disTemp = dis + Monster::GetModelWidth(temp.obj) * 1.5f;
		if (Monster::BVisable(temp.obj) &&
			!Monster::BDead(temp.obj) &&
			utils::GetDistance(Monster::GetCurrentPos(temp.obj), Player::GetCurrPostion()) < disTemp  * disTemp)
		{

			auto curHealth = Monster::GetCurrentHealth(temp.obj);

			//保存血量最低的怪物
			if (curHealth < minHealth)
			{
				minHealth = curHealth;
				HealthMostLowObj = temp.obj;
			}

			//保存危险等级最高的怪物
			if (temp.dangerous > iDangerous)
			{
				iDangerous = temp.dangerous;
				Dangerousobj = temp.obj;
			}

			//如果危险等级 == 当前最大危险级
			if (temp.dangerous == iDangerous)
			{
				if (Monster::GetCurrentHealth(temp.obj) < Monster::GetCurrentHealth(Dangerousobj))
				{
					iDangerous = temp.dangerous;
					Dangerousobj = temp.obj;
				}
			}
		}
	}

	//判断血量最低的怪物是否低于设置的最低百分比
	auto curHealth = Monster::GetCurrentHealth(HealthMostLowObj);
	auto maxHealth = Monster::GetMaxHealth(HealthMostLowObj);
	if (maxHealth > 0)
	{
		auto curPercent = curHealth / maxHealth;
		if (curPercent < (percent / 100.0f))
		{
			return HealthMostLowObj;
		}
	}

	return Dangerousobj;
}
