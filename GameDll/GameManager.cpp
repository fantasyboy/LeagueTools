#include "stdafx.h"
#include "GameManager.h"
#include "TargetSelecter.h"
#include "HookToMainThread.h"
#include "GameMenu.h"
#include "GameRender.h"
#include "utils.h"
#include "Player.h"
#include "GameCall.h"
CGameManager* CGameManager::m_pInstance = nullptr;
std::mutex CGameManager::m_mutex;
CGameManager::CGameManager() :  m_bExitGame(false), m_skBeginDaley(0.0f)
{

}

CGameManager::~CGameManager()
{
	//卸载菜单
	CGameRender::GetInstance()->DestoryInstance();

	//关闭连招，目标选择器线程
	if (m_ComboSelecterTh.joinable())
	{
		m_ComboSelecterTh.join();
	}

	//关闭目标选择器线程
	if (m_targetSelecterTh.joinable())
	{
		m_ComboSelecterTh.join();
	}
}

CGameManager * CGameManager::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CGameManager();
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

void CGameManager::DestoryInstance()
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



bool CGameManager::Initialize()
{

	if (!CTargetSelecter::GetInstance()->travse())
	{
		return false;
	}

	auto& list =  CTargetSelecter::GetInstance()->GetPersonList();
	CGameMenu::GetInstance()->SetMonsterList(list);

	if (!CGameRender::GetInstance()->Initialize())
	{
		return false;
	}

	m_targetSelecterTh = std::thread(std::bind(&CGameManager::TargetSelecterThread, this));


	std::string hero = Player::GetHeroName();
	VMProtectBegin(__FUNCTION__);
	if (hero.compare("Cassiopeia") == 0)
	{	
		//开启蛇女线程
		m_ComboSelecterTh = std::thread(std::bind(&CGameManager::CassiopeiaComboThread, this));
	}
	else if (hero.compare("KogMaw") == 0)
	{
		//开启大嘴线程
		m_ComboSelecterTh = std::thread(std::bind(&CGameManager::KogMawComboThread, this));
	}
	else if (hero.compare("Kalista") == 0)
	{
		//开启滑板鞋线程
		m_ComboSelecterTh = std::thread(std::bind(&CGameManager::KalistaComboThread, this));
	}
	else if (hero.compare("Tristana") == 0)
	{
		//开启小炮线程
		m_ComboSelecterTh = std::thread(std::bind(&CGameManager::TristanaComboThread, this));
	}
	else if (hero.compare("Ezreal") == 0)
	{
		//开启伊泽瑞尔线程
		m_ComboSelecterTh = std::thread(std::bind(&CGameManager::EzrealComboThread, this));
	}
	else if (hero.compare("Xerath") == 0)
	{
		//开启泽拉斯线程
		m_ComboSelecterTh = std::thread(std::bind(&CGameManager::XerathComboThread, this));
	}
	else if (hero.compare("Viktor") == 0)
	{
		//开启维克托线程
		m_ComboSelecterTh = std::thread(std::bind(&CGameManager::ViktorComboThread, this));
	}
	else {
		m_ComboSelecterTh = std::thread(std::bind(&CGameManager::GeneralComboThread, this));
	}
	
	VMProtectEnd();

	return true;
}

void CGameManager::setExitTag(bool _b)
{
	m_bExitGame = _b;
}

void CGameManager::CassiopeiaComboThread()
{

	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	m_monsterList.clear();
	m_personList.clear();

	menu->bComboSk[0] = true;
	menu->bComboSk[2] = true;
	menu->skDelay[0] = 0.35f;

	while (!m_bExitGame)
	{
		//如果收到交换数据的信号
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_evt, 1))
		{
			g_mutex.lock();
			m_personList.swap(g_personList);
			m_monsterList.swap(g_monsterList);
			g_mutex.unlock();
		}

		CGameMenu::GetInstance()->UpdateOrientation(); 

		//蛇女连招
		if (CGameMenu::GetInstance()->GetComboKeyState())
		{
			if (UseSkill1()) {
				auto index = menu->iComboSequence[0];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}

			if (UseSkill2()) {
				auto index = menu->iComboSequence[1];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}

			if (UseSkill3()) {
				auto index = menu->iComboSequence[2];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}

			if (UseSkill4()) {
				auto index = menu->iComboSequence[3];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}

			//通用走砍
			GeneralZouKan();
		}
		else
		{
			auto ske = Player::GetSkillByIndex(2);
			if (ske)
			{
				auto mons = GetHealthLowerestPerson(m_monsterList, ske->GetDistance());
				if (mons)
				{
					if (Monster::GetCurrentHealth(mons) < ske->GetBaseAggressivity() + Player::GetSpellPower() * 0.1f)
					{
						static DWORD timesec = 0;
						if (GetTickCount() - timesec > 300)
						{
							if (Player::UseSkillToMons(mons, 2, 0, 0, false))
							{
								timesec = GetTickCount();
							}
						}
					}
				}
			}
		}

	}
}

void CGameManager::TristanaComboThread()
{
	//小炮连招线程
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	m_monsterList.clear();
	m_personList.clear();

	menu->bAttack = true;
	menu->bComboSk[0] = true;
	menu->bComboSk[2] = true;
	menu->bQingArmyWithAttack = true;
	
	{
		auto skq = Player::GetSkillByIndex(0);
		if (skq)
		{
			skq->SetDistance(900.0f);
		}


	}
	while (!m_bExitGame)
	{
		//如果收到交换数据的信号
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_evt, 1))
		{
			g_mutex.lock();
			m_personList.swap(g_personList);
			m_monsterList.swap(g_monsterList);
			g_mutex.unlock();
		}

		CGameMenu::GetInstance()->UpdateOrientation();
		if (CGameMenu::GetInstance()->GetComboKeyState())
		{
			//自动使用Q
			if(menu->bComboSk[0])
			{
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 300)
				{
					auto sk = Player::GetSkillByIndex(0);
					if (sk)
					{
						//获取普攻范围内的怪物
						auto dis = sk->GetDistance();
						DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
						if (mons)
						{
							if (Player::UseSkillToMons(mons, 0, 0, 0, 0))
							{
								timeSec = GetTickCount();
								m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[0];
							}
						}
					}
				}
			}//Q技能使用结束

			//自动使用E技能
			if (menu->bComboSk[2])
			{
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 300)
				{
					auto sk = Player::GetSkillByIndex(2);
					if (sk)
					{
						auto dis = sk->GetDistance();
						if (dis < 1)
						{
							dis = Player::GetAttackDistance();
							sk->SetDistance(dis);
						}

						DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
						if (mons)
						{
							if (Player::UseSkillToMons(mons, 2, 0, 0, false))
							{
								timeSec = GetTickCount();
								m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[2];
							}

						}
					}
				}
			} //自动使用技能E结束

			 //通用走砍
			GeneralZouKan();

		}

		Farming();

		QingArmy();

	}
}

void CGameManager::KogMawComboThread()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	m_monsterList.clear();
	m_personList.clear();

	menu->bComboSk[0] = true;
	menu->bComboSk[1] = true;
	menu->bComboSk[3] = true;
	menu->bQingArmyWithAttack = true;
	menu->bAttack = true;
	menu->skDelay[0] = 0.25f;
	menu->skSpeed[0] = 1650.0f;
	menu->skDelay[2] = 0.25f;
	menu->skSpeed[2] = 800.0f;
	menu->skDelay[3] = 1.20f;

	auto skq = Player::GetSkillByIndex(0);
	if (skq)
	{
		//skq->SetDistance(1200.0f);
		skq->SetWidth(70.0f);
	}

	auto skr = Player::GetSkillByIndex(3);
	if (skr)
	{
		skr->SetDistance(1800.0f);
	}

	
	while (!m_bExitGame)
	{
		//如果收到交换数据的信号
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_evt, 1))
		{
			g_mutex.lock();
			m_personList.swap(g_personList);
			m_monsterList.swap(g_monsterList);
			g_mutex.unlock();
		}

		CGameMenu::GetInstance()->UpdateOrientation();
		//连招
		if (CGameMenu::GetInstance()->GetComboKeyState())
		{
			
			//1.自动使用W
			if(menu->bComboSk[1])
			{
				static DWORD timesec = 0;
				if (GetTickCount() - timesec > 300)
				{
					auto skw = Player::GetSkillByIndex(1);
					if (skw)
					{
						//获取W技能范围
						auto dis = Player::GetAttackDistance() + skw->GetSkillAggressivity();
						skw->SetDistance(dis);
						//获取怪物对象
						DWORD obj = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
						if (obj)
						{

							if (Player::UseSkillToMons(obj, skw->GetIndex(), 0, 0, 0))
							{
								m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[1];
								timesec = GetTickCount();
							}

						}
					}
				}
			}
			//2.自动判断遮挡Q
			if(menu->bComboSk[0])
			{
				static DWORD timesec = 0;
				if (GetTickCount() - timesec > 300)
				{
					if (skq)
					{
						auto dis = skq->GetDistance();
						DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
						if (mons)
						{
							//判断怪物遮挡
							if (!IsCoveredByMonster(m_monsterList, mons, skq->GetDistance(), skq->GetWidth(), menu->skDelay[0], menu->skSpeed[0]))
							{
								if (Player::UseSkillToMons(mons, skq->GetIndex(), menu->skDelay[0], menu->skSpeed[0], false))
								{
									m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[0];
									timesec = GetTickCount();
								}
							}
						}
					}
				}
			}//自动遮挡判断的Q

			//3.自动攻击血量低于百分之30的怪物
			if(menu->bComboSk[3])
			{
				static DWORD timesec = 0;
				if (GetTickCount() - timesec > 300)
				{
					//当怪物血量小于百分之30的时候，自动使用R
					auto skr = Player::GetSkillByIndex(3);
					if (skr)
					{
						//获取怪物对象
						DWORD mons = GetDangerousMostPerson(menu->monsList, skr->GetDistance(), menu->dwTargetPercent);
						if (mons)
						{
							//优先攻击血量低于百分比的怪物
							auto percent = (Monster::GetCurrentHealth(mons) / Monster::GetMaxHealth(mons)) * 100.0f;
							if (percent < menu->dwTargetPercent)
							{
								if (Player::UseSkillToMons(mons, skr->GetIndex(), menu->skDelay[3], menu->skSpeed[3], false))
								{
									m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[3];
									timesec = GetTickCount();
								}
								
							}
						}
					}
				}
			}

			//通用走砍
			GeneralZouKan();
		}

		//补刀
		Farming();
		//清兵
		QingArmy();
	}
}

void CGameManager::KalistaComboThread()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	m_monsterList.clear();
	m_personList.clear();
	menu->skDelay[0] = 0.25f;
	menu->skSpeed[0] = 1700.0f;
	auto skq = Player::GetSkillByIndex(0);
	if (skq)
	{
		//skq->SetDistance(1200.0f);
		skq->SetWidth(40.0f);
	}

	//滑板鞋配置
	menu->bFarming = true;
	menu->bQingArmyWithAttack = true;
	menu->bAttack = true;
	menu->bComboSk[0] = true;

	while (!m_bExitGame)
	{
		//如果收到交换数据的信号
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_evt, 1))
		{
			g_mutex.lock();
			m_personList.swap(g_personList);
			m_monsterList.swap(g_monsterList);
			g_mutex.unlock();
		}

		CGameMenu::GetInstance()->UpdateOrientation();
		if (CGameMenu::GetInstance()->GetComboKeyState())
		{
			if (menu->bComboSk[0])
			{
				static DWORD timesec = 0;
				if (GetTickCount() - timesec > 300)
				{
					if (skq)
					{
						auto dis = skq->GetDistance();
						DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
						if (mons)
						{
							//判断怪物遮挡
							if (!IsCoveredByMonster(m_monsterList, mons, skq->GetDistance(), skq->GetWidth(), menu->skDelay[0], menu->skSpeed[0]))
							{
								if (Player::UseSkillToMons(mons, skq->GetIndex(), menu->skDelay[0], menu->skSpeed[0], false))
								{
									m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[0];
									timesec = GetTickCount();
								}
							}
						}
					}
				}
			}//自动遮挡判断的Q

			 //通用走砍
			GeneralZouKan();
			
		}

		//清兵
		QingArmy();

		//滑板鞋自动补刀
		if (menu->bFarming)
		{
			static DWORD timeSec = 0;
			if (GetTickCount() - timeSec > 500)
			{
				auto ske = Player::GetSkillByIndex(2);
				if (ske)
				{

					//自动E人
					{
						auto Mons = GetDangerousMostPerson(menu->monsList, ske->GetDistance(), menu->dwTargetPercent);
						if (Mons)
						{

							if (Player::KalistaAutoE2Object(Mons))
							{
								m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[2];
								timeSec = GetTickCount();
							}

						}

						auto Mons2Att = GetDangerousMostPerson(menu->monsList, ske->GetDistance(), menu->dwTargetPercent);
						if (Mons2Att && Mons2Att != Mons)
						{

							if (Player::KalistaAutoE2Object(Mons2Att))
							{
								m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[2];
								timeSec = GetTickCount();
							}

						}
					}

					//自动E小兵
					{
						auto Mons = GetHealthLowerestPerson(m_monsterList, ske->GetDistance());
						if (Mons)
						{
							if (Player::KalistaAutoE2Object(Mons))
							{
								timeSec = GetTickCount();
								m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[2];
							}
						}
					}

				}
			}
		}

	}//退出循环
}

void CGameManager::EzrealComboThread()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	m_monsterList.clear();
	m_personList.clear();


	//默认配置
	menu->bAttack = true;
	menu->bComboSk[0] = true;
	menu->bComboSk[1] = true;
	menu->bQingArmyWithAttack = true;
	menu->bQingArmyWithSk[0] = true;
	menu->skDelay[0] = 0.1f;
	menu->skDelay[1] = 0.1f;
	menu->skDelay[3] = 1.0f;
	menu->skSpeed[0] = 2000.0f;
	menu->skSpeed[1] = 1600.0f;
	menu->skSpeed[3] = 2000.0f;
	//设置Q技能属性
	{
		auto skq = Player::GetSkillByIndex(0);
		if (skq)
		{
			skq->SetDistance(1100.0f);
			skq->SetWidth(65.0f);
		}
	}

	{
		auto sk2 = Player::GetSkillByIndex(1);
		if (sk2)
		{
			sk2->SetDistance(950.0f);
		}
	}
	while (!m_bExitGame)
	{
		//如果收到交换数据的信号
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_evt, 1))
		{
			g_mutex.lock();
			m_personList.swap(g_personList);
			m_monsterList.swap(g_monsterList);
			g_mutex.unlock();
		}

		CGameMenu::GetInstance()->UpdateOrientation();
		if (CGameMenu::GetInstance()->GetComboKeyState())
		{
			//自动预判Q
			if (menu->bComboSk[0])
			{
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 300)
				{
					auto sk = Player::GetSkillByIndex(0);
					if (sk)
					{
						auto dis = sk->GetDistance();
						//获取怪物对象
						DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
						if (mons)
						{
							if (!IsCoveredByMonster(m_monsterList, mons, dis, sk->GetWidth(), menu->skDelay[0] , menu->skSpeed[0]))
							{
								if (Player::UseSkillToMons(mons, sk->GetIndex(), menu->skDelay[0], menu->skSpeed[0], false))
								{
									timeSec = GetTickCount();
									m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[0];
								}
							}
						}

					}
				}
			}//自动预判Q结束

			//自动预判W
			if (menu->bComboSk[1])
			{
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 300)
				{
					auto sk = Player::GetSkillByIndex(1);
					if (sk)
					{
						auto dis = sk->GetDistance();
						if (dis < 1)
						{
							dis = Player::GetAttackDistance();
						}

						//获取怪物对象
						DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
						if (mons)
						{

							if (Player::UseSkillToMons(mons, sk->GetIndex(), menu->skDelay[1], menu->skSpeed[1], false))
							{
								m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[1];
								timeSec = GetTickCount();
							}

						}
					}
				}
			}//自动预判W结束

			 //通用走砍
			GeneralZouKan();
		}

		Farming();

		QingArmy();
	}
}

void CGameManager::XerathComboThread()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	m_monsterList.clear();
	m_personList.clear();

	menu->bComboSk[0] = true;
	menu->bComboSk[2] = true;
	menu->bComboSk[3] = true;
	menu->skDelay[0] = 0.6f;
	menu->skDelay[1] = 0.7f;
	menu->skDelay[2] = 0.2f;
	menu->skSpeed[2] = 1400.0f;


	//设置连招技能
	{
		auto skq = Player::GetSkillByIndex(0);
		if (skq)
		{
			skq->SetDistance(1500.0f);
		}

		auto ske = Player::GetSkillByIndex(2);
		if (ske)
		{
			ske->SetWidth(65.0f);
		}
	}
	while (!m_bExitGame)
	{
		//如果收到交换数据的信号
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_evt, 1))
		{
			g_mutex.lock();
			m_personList.swap(g_personList);
			m_monsterList.swap(g_monsterList);
			g_mutex.unlock();
		}

		CGameMenu::GetInstance()->UpdateOrientation();
		if (CGameMenu::GetInstance()->GetComboKeyState())
		{
			//自动预判E
			if (menu->bComboSk[2])
			{
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 300)
				{
					auto sk = Player::GetSkillByIndex(2);
					if (sk)
					{
						auto dis = sk->GetDistance();
						if (dis < 1)
						{
							dis = Player::GetAttackDistance();
						}

						//获取怪物对象
						DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
						if (mons)
						{
							if (!IsCoveredByMonster(m_monsterList, mons, dis, sk->GetWidth(), menu->skDelay[2], menu->skSpeed[2]))
							{

								if (Player::UseSkillToMons(mons, sk->GetIndex(), menu->skDelay[2], menu->skSpeed[2], false))
								{
									m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[2];
									timeSec = GetTickCount();
								}

							}
						}

					}
				}
			}//自动预判Q结束

			 //自动预判W
			if (menu->bComboSk[1])
			{
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 300)
				{
					auto sk = Player::GetSkillByIndex(1);
					if (sk)
					{
						auto dis = sk->GetDistance();
						if (dis < 1)
						{
							dis = Player::GetAttackDistance();
						}


						//获取怪物对象
						DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
						if (mons)
						{

							if (Player::UseSkillToMons(mons, sk->GetIndex(), menu->skDelay[1], menu->skSpeed[1], false))
							{
								m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[1];
								timeSec = GetTickCount();
							}
						}
					}
				}
			}//自动预判W结束


			if (menu->bComboSk[0])
			{
				//自动使用技能Q
				auto sk = Player::GetSkillByIndex(0);
				if (sk)
				{
					auto dis = sk->GetDistance();
					if (dis < 1)
					{
						dis = Player::GetAttackDistance();
					}

					DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
					if (mons)
					{
						//没有还没有开始蓄力，就蓄力
						if (Player::GetXerathPowerTime() < 0.0001)
						{
							static DWORD timeSec = 0;
							if (GetTickCount() - timeSec > 300)
							{
								if (Player::UseSkillToMons(mons, 0, menu->skDelay[0], 0, false))
								{
									timeSec = GetTickCount();
								}
							}
						}
						else
						{
							//判断技能的距离知否达到了释放距离，如果是就释放技能
							auto currDis = Player::GetXerathSkillCurrDistance();
							if (utils::GetDistance(Player::GetCurrPostion(), Monster::GetCurrentPos(mons)) < currDis * currDis)
							{
								static DWORD timeSec = 0;
								if (GetTickCount() - timeSec > 300)
								{
									Player::UseSkillToMons2(mons, 0, menu->skDelay[0], menu->skSpeed[0]);
									timeSec = GetTickCount();
								}
							}
						}
					}
				}
			}


			//通用走砍
			GeneralZouKan();
			
		}

		//按T自动R
		if (GetAsyncKeyState('T') & 0x8000)
		{
			if (menu->bComboSk[3])
			{
				auto sk = Player::GetSkillByIndex(3);
				if (sk)
				{
					auto dis = sk->GetDistance();
					if (dis < 1)
					{
						dis = Player::GetAttackDistance();
					}

					//获取怪物对象
					DWORD mons = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
					if (mons)
					{
						static DWORD timeSec = 0;
						if (GetTickCount() - timeSec > 100)
						{
							if (Player::UseSkillToMons(mons, sk->GetIndex(), menu->skDelay[3], menu->skSpeed[1], false))
							{
								m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[3];
								timeSec = GetTickCount();
							}
						}
					}
				}
			}
		}


		Farming();

		QingArmy();

	}
}

void CGameManager::ViktorComboThread()
{
	/*
	//1.保存数据
	*/
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	m_monsterList.clear();
	m_personList.clear();

	//配置连招信息
	menu->bComboSk[0] = true;
	menu->bComboSk[2] = true;
	
	{
		auto ske = Player::GetSkillByIndex(2);
		if (ske)
		{
			ske->SetDistance(ske->GetDistance() * 2.0f);
		}
	}

	while (!m_bExitGame)
	{
		//如果收到交换数据的信号
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_evt, 1))
		{
			g_mutex.lock();
			m_personList.swap(g_personList);
			m_monsterList.swap(g_monsterList);
			g_mutex.unlock();
		}

		CGameMenu::GetInstance()->UpdateOrientation();
		//通用连招
		if (CGameMenu::GetInstance()->GetComboKeyState())
		{
			//连招技能
			if (UseSkill1()) {
				auto index = menu->iComboSequence[0];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}
			if (UseSkill2()) {
				auto index = menu->iComboSequence[1];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}
			if (UseSkill3()) {
				auto index = menu->iComboSequence[2];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}
			if (UseSkill4()) {
				auto index = menu->iComboSequence[3];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}
			else {
			}

			//通用走砍
			GeneralZouKan();
		}
		//补刀
		Farming();
		//清兵
		QingArmy();

	}
}

void CGameManager::GeneralComboThread()
{
	/*
	//1.保存数据
	*/
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	m_monsterList.clear();
	m_personList.clear();
	while (!m_bExitGame)
	{			
		//如果收到交换数据的信号
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_evt, 1))
		{
			g_mutex.lock();
			m_personList.swap(g_personList);
			m_monsterList.swap(g_monsterList);
			g_mutex.unlock();
		}

		//预判计算点采集
		CGameMenu::GetInstance()->UpdateOrientation();
		//通用连招
		if (CGameMenu::GetInstance()->GetComboKeyState())
		{
			//连招技能
			if (UseSkill1()) {
				auto index = menu->iComboSequence[0];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}

			if (UseSkill2()) {
				auto index = menu->iComboSequence[1];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}


			if (UseSkill3()) {
				auto index = menu->iComboSequence[2];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}


			if (UseSkill4()) {
				auto index = menu->iComboSequence[3];
				m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[index];
			}

			//通用走砍
			GeneralZouKan();
		}
		//补刀
		Farming();
		//清兵
		QingArmy();

	}
}

void CGameManager::TargetSelecterThread()
{
	while (!m_bExitGame)
	{
		//怪物遍历
		if (!CTargetSelecter::GetInstance()->travse())
		{
			Sleep(10);
			continue;
		}

		//交换小兵数据 和 玩家数据 到全局的表中
		CTargetSelecter::GetInstance()->swapPersonList();
		CTargetSelecter::GetInstance()->swapMonsterList();
		SetEvent(g_evt);

		Sleep(30);
	}
}




void CGameManager::GeneralZouKan()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	if (menu->bFollowMouse)
	{
		static DWORD timesec = 0;
		if (GetTickCount() - timesec > 15)
		{
			auto obj = GetDangerousMostPerson(menu->monsList, Player::GetAttackDistance(), menu->dwTargetPercent);
			if (obj && GameCall::GetGameTickTime() > m_skBeginDaley)
			{
				Player::ZouA2Mons(obj, menu->bAttack);
			}
			else
			{
				Player::ZouA2Mons(obj, false);
			}
			timesec = GetTickCount();
		}
	}
}

void CGameManager::Farming()
{		
	//补刀
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	if (CGameMenu::GetInstance()->GetFarmingKeyState())
	{
		if (menu->bFarming)
		{
			static DWORD timesec = 0;
			if (GetTickCount() - timesec > 15)
			{
				auto obj = GetHealthLowerestPerson(m_monsterList, Player::GetAttackDistance());
				if (obj
					&& Monster::GetCurrentHealth(obj) < Player::GetAttackPower() 
					&& GameCall::GetGameTickTime() > m_skBeginDaley)
				{
					Player::ZouA2Mons(obj, true);
				}
				else {
					Player::ZouA2Mons(obj, false);
				}
				timesec = GetTickCount();
			}
		}
	}
}

void CGameManager::QingArmy()
{		
	//清兵
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	if (CGameMenu::GetInstance()->GetQingArmyKeyState())
	{

		//如果开启了清兵
		if (menu->bQingArmyWithAttack 
			|| menu->bQingArmyWithSk[0] 
			|| menu->bQingArmyWithSk[1] 
			|| menu->bQingArmyWithSk[2] 
			|| menu->bQingArmyWithSk[3])
		{

			if (menu->bQingArmyWithSk[0])
			{
				static DWORD timesec = 0;
				if (GetTickCount() - timesec > 300)
				{
					if (UseSkill2MonsterHealthLowestByIndex(0))
					{
						m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[0];
						timesec = GetTickCount();
					}
					
				}
			}

			if (menu->bQingArmyWithSk[1])
			{
				static DWORD timesec = 0;
				if (GetTickCount() - timesec > 300)
				{
					if (UseSkill2MonsterHealthLowestByIndex(1))
					{
						m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[1];
						timesec = GetTickCount();
					}
				}
			}


			if (menu->bQingArmyWithSk[2])
			{
				static DWORD timesec = 0;
				if (GetTickCount() - timesec > 300)
				{
					if (UseSkill2MonsterHealthLowestByIndex(2))
					{
						m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[2];
						timesec = GetTickCount();
					}
					
				}
			}


			if (menu->bQingArmyWithSk[3])
			{
				static DWORD timesec = 0;
				if (GetTickCount() - timesec > 300)
				{
					if (UseSkill2MonsterHealthLowestByIndex(3))
					{
						m_skBeginDaley = GameCall::GetGameTickTime() + menu->skDelay[3];
						timesec = GetTickCount();
					}
				}
			}

			//如果开启普攻清兵
			if (menu->bQingArmyWithAttack)
			{
				static DWORD timesec = 0;
				if (GetTickCount() - timesec > 15)
				{
					//获取到血量最低的怪物
					auto obj = GetHealthLowerestPerson(m_monsterList, Player::GetAttackDistance());
					if (obj && GameCall::GetGameTickTime() > m_skBeginDaley) {
						Player::ZouA2Mons(obj, true);
					}
					else {
						Player::ZouA2Mons(obj, false);
					}
					timesec = GetTickCount();
				}

			}
		}


	}
}

bool CGameManager::UseSkill1()
{
	static DWORD timesec = 0;
	if (GetTickCount() - timesec > 300)
	{
		if (UseSkillByIndexWithDangerous(0))
		{
			timesec = GetTickCount();
			return true;
		}
	}
	return false;
}

bool CGameManager::UseSkill2()
{
	static DWORD timesec = 0;
	if (GetTickCount() - timesec > 300)
	{
		if (UseSkillByIndexWithDangerous(1))
		{
			timesec = GetTickCount();
			return true;
		}
		
	}
	return false;
}

bool CGameManager::UseSkill3()
{
	static DWORD timesec = 0;
	if (GetTickCount() - timesec > 300)
	{
		if (UseSkillByIndexWithDangerous(2))
		{
			timesec = GetTickCount();
			return true;
		}
	}
	return false;
}

bool CGameManager::UseSkill4()
{

	static DWORD timesec = 0;
	if (GetTickCount() - timesec > 300)
	{
		if (UseSkillByIndexWithDangerous(3))
		{
			timesec = GetTickCount();
			return true;
		}
	}
	return false;
}


bool CGameManager::UseSkillByIndexWithDangerous(DWORD index)
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	if (index < 0 && index > 3)
	{
		return false;
	}

	auto skIndex = menu->iComboSequence[index];
	//判断是否是QWER
	if (skIndex >= 0 && skIndex <= 3)
	{
		//判断是否开启连招
		if (menu->bComboSk[skIndex])
		{
			auto sk = Player::GetSkillByIndex(skIndex);
			if (sk)
			{
				//获取到技能距离
				auto dis = sk->GetDistance();
				if (dis < 1)
				{
					dis = Player::GetAttackDistance();
					sk->SetDistance(dis);
				}

				auto obj = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
				if (obj)
				{
					if (Player::UseSkillToMons(obj, sk->GetIndex(), menu->skDelay[skIndex], menu->skSpeed[skIndex], false))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool CGameManager::UseSkill2MonsterHealthLowestByIndex(DWORD index)
{
	auto sk = Player::GetSkillByIndex(index);
	if (sk)
	{
		//获取到技能距离
		auto dis = sk->GetDistance();
		if (dis < 1)
		{
			dis = Player::GetAttackDistance();
		}
		//获取小兵列表中血量最低的小兵
		auto obj = GetHealthLowerestPerson(m_monsterList, dis);
		if (obj)
		{
			if (Player::UseSkillToMons(obj, sk->GetIndex(), 0, 0, false))
			{
				return true;
			}
	
		}
	}

	return false;
}
