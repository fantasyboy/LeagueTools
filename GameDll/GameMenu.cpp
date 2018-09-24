#include "stdafx.h"
#include "GameMenu.h"
#include "Monster.h"
#include "utils.h"
std::mutex CGameMenu::m_mutex;
CGameMenu* CGameMenu::m_pInstance = nullptr;

CGameMenu::CGameMenu()
{
	//memset(&m_gameMenu, 0, sizeof(GAME_MENU));
	m_gameMenu.skDelay[0] = 0.35f;
	m_gameMenu.skDelay[1] = 0.35f;
	m_gameMenu.skDelay[2] = 0.35f;
	m_gameMenu.skDelay[3] = 0.35f;
	m_gameMenu.skSpeed[0] = 0.0f;
	m_gameMenu.skSpeed[1] = 0.0f;
	m_gameMenu.skSpeed[2] = 0.0f;
	m_gameMenu.skSpeed[3] = 0.0f;
	m_gameMenu.bFollowMouse = true;
	m_gameMenu.bAttack = false;
	m_gameMenu.bRenderAttackDis = false;
	m_gameMenu.dwComboHotKey = VK_SPACE;
	m_gameMenu.bLockSk[0] = false;
	m_gameMenu.bLockSk[1] = false;
	m_gameMenu.bLockSk[2] = false;
	m_gameMenu.bLockSk[3] = false;
	m_gameMenu.bComboSk[0] = false;
	m_gameMenu.bComboSk[1] = false;
	m_gameMenu.bComboSk[2] = false;
	m_gameMenu.bComboSk[3] = false;
	m_gameMenu.iComboSequence[0] = 0;
	m_gameMenu.iComboSequence[1] = 1;
	m_gameMenu.iComboSequence[2] = 2;
	m_gameMenu.iComboSequence[3] = 3;
	m_gameMenu.bFarming = true;
	m_gameMenu.dwFarmingHotkey = 'C';
	m_gameMenu.bQingArmyWithAttack = false;
	m_gameMenu.bQingArmyWithSk[0] = false;
	m_gameMenu.bQingArmyWithSk[1] = false;
	m_gameMenu.bQingArmyWithSk[2] = false;
	m_gameMenu.bQingArmyWithSk[3] = false;
	m_gameMenu.dwQingArmyHotkey = 'V';
	m_gameMenu.monsList.clear();
	m_gameMenu.dwTargetPercent = 30;
	m_gameMenu.bComboHotKeyDown = false;
	m_gameMenu.bFarmingHotkeyDown = false;
	m_gameMenu.bQingArmyHotkeyDown = false;
}


CGameMenu::~CGameMenu()
{
}

CGameMenu * CGameMenu::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CGameMenu();
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

void CGameMenu::DestoryInstance()
{
	if (nullptr != m_pInstance)
	{
		m_mutex.lock();
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
		m_mutex.unlock();
	}
}

GAME_MENU * CGameMenu::GetGameMenu()
{
	// TODO: 在此处插入 return 语句
	return &m_gameMenu;
}

void CGameMenu::SetMonsterList(std::vector<DWORD> list)
{
	m_gameMenu.monsList.clear();
	for (auto temp : list)
	{
		MONSTER_OBJECT tempObj;
		tempObj.obj = temp;
		if (nullptr != Monster::GetName(temp))
		{
			tempObj.name = Monster::GetName(temp);
		}

		if (nullptr != Monster::GetHeroName(temp))
		{
			tempObj.heroname = Monster::GetHeroName(temp);
		}

		m_gameMenu.monsList.push_back(tempObj);
	}
}

bool CGameMenu::GetComboKeyState()
{
	return m_gameMenu.bComboHotKeyDown;
}

bool CGameMenu::GetFarmingKeyState()
{
	return m_gameMenu.bFarmingHotkeyDown;
}

bool CGameMenu::GetQingArmyKeyState()
{
	return m_gameMenu.bQingArmyHotkeyDown;
}

void CGameMenu::UpdateOrientation()
{
	static DWORD timesec[3] = {0};
	for (auto &temp : m_gameMenu.monsList)
	{

		if (GetTickCount() - timesec[0] > 100)
		{
			EP_POINT_3D Ori = { 0 };
			EP_POINT_3D Pos = { 0 };

			memcpy(&Ori, Monster::GetOrientation(temp.obj), sizeof(EP_POINT_3D));
			memcpy(&Pos, Monster::GetCurrentPos(temp.obj), sizeof(EP_POINT_3D));
			temp.LastOrientation.push_back(Ori);
			temp.LastPosition.push_back(Pos);

			if (temp.LastOrientation.size() > 10)
			{
				temp.LastOrientation.pop_front();
			}

			if (temp.LastPosition.size() > 10)
			{
				temp.LastPosition.pop_front();
			}

			timesec[0] = GetTickCount();
		}
	}

#ifdef _USER_DBG
	for (auto &temp : m_gameMenu.monsList)
	{
		//打印2次的朝向 和当前朝向
		//utils::log("HXL: 200ms: (%f,%f,%f) --- 100ms: (%f,%f,%f) 当前： (%f,%f,%f)",
		//	temp.LastOrientation[1].x,
		//	temp.LastOrientation[1].z,
		//	temp.LastOrientation[1].y,
		//	temp.LastOrientation[0].x,
		//	temp.LastOrientation[0].z,
		//	temp.LastOrientation[0].y,
		//	Monster::GetOrientation(temp.obj)->x,
		//	Monster::GetOrientation(temp.obj)->z,
		//	Monster::GetOrientation(temp.obj)->y);
	}

#endif // _USER_DBG


}
