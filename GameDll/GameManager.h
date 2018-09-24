#pragma once
#include <mutex>
#include <vector>
class CGameManager
{
	CGameManager();
	CGameManager(const CGameManager&) = delete;
	void operator= (const CGameManager&) = delete;
	CGameManager(const CGameManager&&) = delete;
	void operator= (const CGameManager&&) = delete;

	static CGameManager* m_pInstance;
	static std::mutex m_mutex;

public:
	~CGameManager();
	static CGameManager* GetInstance();
	static void DestoryInstance();

	bool Initialize();
	void setExitTag(bool _b);

	//通用连招线程
	void GeneralComboThread();
	//目标选择器线程
	void TargetSelecterThread();
	//蛇女连招线程
	void CassiopeiaComboThread();
	//麦林炮手连招线程
	void TristanaComboThread();
	//深渊巨口连招线程
	void KogMawComboThread();
	//滑板鞋连招线程
	void KalistaComboThread();
	//伊泽瑞尔连招线程
	void EzrealComboThread();
	//泽拉斯连招线程
	void XerathComboThread();
	//维克托连招线程
	void ViktorComboThread();
private:
	//通用走砍
	void GeneralZouKan();
	//补刀
	void Farming();
	//清兵
	void QingArmy();
	//使用技能1
	bool UseSkill1();
	//使用技能2
	bool UseSkill2();
	//使用技能3
	bool UseSkill3();
	//使用技能4
	bool UseSkill4();
	//获取危险等级最高的敌人玩家并且使用技能
	bool UseSkillByIndexWithDangerous(DWORD index);
	//获取血量最低的小兵并且使用技能
	bool UseSkill2MonsterHealthLowestByIndex(DWORD index);
private:
	std::thread m_targetSelecterTh;
	std::thread m_ComboSelecterTh;
	bool m_bExitGame;
	std::vector<DWORD> m_monsterList;
	std::vector<DWORD> m_personList;
	float m_skBeginDaley; //技能抬手时间
};

