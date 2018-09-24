#pragma once
#include "stdafx.h"

#include <mutex>
#include <list>
#include <vector>
#include <deque>
typedef struct _MONSTER_OBJECT
{
	_MONSTER_OBJECT() : obj(0) , dangerous(3), name(""), heroname(""),_sid(""), _format(""){}
	DWORD obj; //怪物对象
	int dangerous; //危险等级
	std::string name;//玩家名字
	std::string heroname; //英雄名字
	std::string _sid;
	std::string _format;
	std::deque<EP_POINT_3D> LastOrientation;  //朝向
	std::deque<EP_POINT_3D> LastPosition;  //位置轨迹
}MONSTER_OBJECT, *PMONSTER_OBJECT;


typedef struct _GAME_MENU
{
	//技能抬手延迟
	float skDelay[4];
	//技能速度
	float skSpeed[4];
	//是否跟随鼠标
	bool bFollowMouse;
	//是否平A
	bool bAttack;
	//是否绘制线框
	bool bRenderAttackDis;
	//连招
	DWORD dwComboHotKey;
	//锁定技能QWER
	bool bLockSk[4];
	//连招使用技能
	bool bComboSk[4];
	//连招顺序
	int iComboSequence[4];
	//是否补刀
	bool bFarming;
	//补刀热键
	DWORD dwFarmingHotkey;
	//清兵使用平A
	bool bQingArmyWithAttack;
	//清兵使用技能
	bool bQingArmyWithSk[4];
	//清兵热键
	DWORD dwQingArmyHotkey;
	//目标选择器
	std::vector<MONSTER_OBJECT> monsList;
	//优先攻击血量低于比例的
	int dwTargetPercent;
	//连招热键是否按下
	bool bComboHotKeyDown;
	//清兵热键是否按下
	bool bQingArmyHotkeyDown;
	//补刀热键是否按下
	bool bFarmingHotkeyDown;
}GAME_MENU, *PGAME_MENU;

class CGameMenu
{
	CGameMenu();
	CGameMenu(const CGameMenu&) = delete;
	CGameMenu(const CGameMenu&&) = delete;
	void operator= (const CGameMenu&) = delete;
	void operator= (const CGameMenu&&) = delete;

	static std::mutex m_mutex;
	static CGameMenu* m_pInstance;
public:
	~CGameMenu();
	static CGameMenu* GetInstance();
	static void DestoryInstance();
	GAME_MENU* GetGameMenu();
	void SetMonsterList(std::vector<DWORD> list);
	bool GetComboKeyState();
	bool GetFarmingKeyState();
	bool GetQingArmyKeyState();
	void UpdateOrientation(); //更新最近的朝向
private:
	GAME_MENU m_gameMenu;
};

