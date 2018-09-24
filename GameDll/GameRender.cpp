#include "stdafx.h"
#include "GameRender.h"
#include "utils.h"
#include "baseaddr.h"
#include "detours.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "GameMenu.h"
#include "virtualKey.h"
#include "GameCall.h"
#include "Player.h"
#include "TargetSelecter.h"
#include <sstream>
CGameRender* CGameRender::m_pInstance = nullptr;
std::mutex CGameRender::m_mutex;
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CGameRender::CGameRender() : m_leagueHwnd(0), m_ulOldWndProc(0), m_bVisiable(false), m_oldReset(0), m_oldPresent(0)
{
	m_sTipsText = AnsiToUtf8("[ 脚本配置，当前版本v1.0.0 ]");
	m_skConfigText = AnsiToUtf8("技能设置");
	m_skDelayText = AnsiToUtf8("技能前摇");
	m_skSpeedText = AnsiToUtf8("技能速度");
	m_sMoveAndAttackText = AnsiToUtf8("走砍设置");
	m_sFollowMosueText = AnsiToUtf8("跟随鼠标");
	m_sAttackText = AnsiToUtf8("普通攻击");
	m_sRenderAttackDisText = AnsiToUtf8("绘制普攻距离");
	m_sComboHotkeyText = AnsiToUtf8("连招热键");
	m_sLockSkillText = AnsiToUtf8("锁定技能");
	m_sComboText = AnsiToUtf8("连招设置");
	m_sFarmingText = AnsiToUtf8("补刀设置");
	m_sQingArmyText = AnsiToUtf8("清兵设置");
	m_sOpenFramingText = AnsiToUtf8("开启补刀");
	m_sFarmingHotkeyText = AnsiToUtf8("补刀热键");
	m_sQingArmtAttackText = AnsiToUtf8("普攻清兵");
	m_sQingArmyHotkey = AnsiToUtf8("清兵热键");
	m_sTargetSelecterText = AnsiToUtf8("目标选择器");
	m_sTargetMinPercentText = AnsiToUtf8("优先选择血量低于百分之%.0f的怪物");
	m_sCombosk1Text = AnsiToUtf8("第一个使用的技能");
	m_sCombosk2Text = AnsiToUtf8("第二个使用的技能");
	m_sCombosk3Text = AnsiToUtf8("第三个使用的技能");
	m_sCombosk4Text = AnsiToUtf8("第四个使用的技能");
}

CGameRender::~CGameRender()
{
	//还原钩子消息
	if (m_ulOldWndProc)
	{
		SetWindowLongW(m_leagueHwnd, GWL_WNDPROC, m_ulOldWndProc);
		m_ulOldWndProc = 0;
	}

	//还原HOOK
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	if (m_oldReset)
	{
		DetourDetach(&m_oldReset, hkReset);
	}
	if (m_oldPresent)
	{
		DetourDetach(&m_oldPresent, hkPresent);
	}
	DetourTransactionCommit();

}

CGameRender * CGameRender::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CGameRender();
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

void CGameRender::DestoryInstance()
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



void CGameRender::RenderSkConfig()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	//绘制技能延迟
	if (ImGui::TreeNode(m_skConfigText.c_str()))
	{
		if (ImGui::TreeNode(m_skDelayText.c_str()))
		{
			ImGui::SliderFloat("##QDelay", &menu->skDelay[0], 0, 1, "Q = %.3f");
			ImGui::SliderFloat("##WDelay", &menu->skDelay[1], 0, 1, "W = %.3f");
			ImGui::SliderFloat("##EDelay", &menu->skDelay[2], 0, 1, "E = %.3f");
			ImGui::SliderFloat("##RDelay", &menu->skDelay[3], 0, 1, "R = %.3f");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode(m_skSpeedText.c_str()))
		{
			ImGui::SliderFloat("##QSpeed", &menu->skSpeed[0], 0, 3000, "Q = %.3f");
			ImGui::SliderFloat("##WSpeed", &menu->skSpeed[1], 0, 3000, "W = %.3f");
			ImGui::SliderFloat("##ESpeed", &menu->skSpeed[2], 0, 3000, "E = %.3f");
			ImGui::SliderFloat("##RSpeed", &menu->skSpeed[3], 0, 3000, "R = %.3f");
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void CGameRender::RenderTips()
{
	//绘制提示项
	ImGui::PushItemWidth(350.0f);
	ImGui::LabelText("", "%s", m_sTipsText.c_str());
	ImGui::PopItemWidth();
}

void CGameRender::RenderMoveAndAttack()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();

	//绘制走砍
	if (ImGui::TreeNode(m_sMoveAndAttackText.c_str()))
	{
		//跟随鼠标
		static char* szFollowMouse[2] = { "True##FollowMouse" ,"False##FollowMouse" };
		ImGui::LabelText("", "%s", m_sFollowMosueText.c_str()); ImGui::SameLine();
		if (ImGui::Button(menu->bFollowMouse ? szFollowMouse[0] : szFollowMouse[1], ImVec2(50, 0)))
		{
			menu->bFollowMouse ^= 1;
		}

		//开启平A
		static char* szAttack[2] = { "True##Attack" ,"False##Attack" };
		ImGui::LabelText("", "%s", m_sAttackText.c_str()); ImGui::SameLine();
		if (ImGui::Button(menu->bAttack ? szAttack[0] : szAttack[1], ImVec2(50, 0)))
		{
			menu->bAttack ^= 1;
		}

		//绘制线框 bRenderAttackDis
		static char* szRenderAttackDis[2] = { "True##RenderAttackDis" ,"False##RenderAttackDis" };
		ImGui::LabelText("", "%s", m_sRenderAttackDisText.c_str()); ImGui::SameLine();
		if (ImGui::Button(menu->bRenderAttackDis ? szRenderAttackDis[0] : szRenderAttackDis[1], ImVec2(50, 0)))
		{
			menu->bRenderAttackDis ^= 1;
		}

		ImGui::LabelText("", "%s", m_sComboHotkeyText.c_str()); ImGui::SameLine();
		ImGui::Button(KeyNames[menu->dwComboHotKey].text, ImVec2(50, 0));
		//如果鼠标悬停在button上面，就设置热键
		if (ImGui::IsItemHovered())
		{
			ImGuiIO& io = ImGui::GetIO();
			for (int i = 0; i < 256; i++)
			{
				if (io.KeysDown[i] == 1 && i != VK_PROCESSKEY)
				{
					menu->dwComboHotKey = i;
				}
			}
		}

		ImGui::TreePop();
	}
}

void CGameRender::RenderLockSkill()
{
	//绘制锁定锁定技能
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	if (ImGui::TreeNode(m_sLockSkillText.c_str()))
	{
		{
			static char* szLockSkQ[2] = { "True##LockQ" , "False##LockQ" };
			ImGui::LabelText("", "Q"); ImGui::SameLine();
			if (ImGui::Button(menu->bLockSk[0] ? szLockSkQ[0] : szLockSkQ[1], ImVec2(50, 0)))
			{
				menu->bLockSk[0] ^= 1;
			}
		}

		{
			static char* szLockSkW[2] = { "True##LockW" , "False##LockW" };
			ImGui::LabelText("", "W"); ImGui::SameLine();
			if (ImGui::Button(menu->bLockSk[1] ? szLockSkW[0] : szLockSkW[1], ImVec2(50, 0)))
			{
				menu->bLockSk[1] ^= 1;
			}
		}

		{
			static char* szLockSkE[2] = { "True##LockE" , "False##LockE" };
			ImGui::LabelText("", "E"); ImGui::SameLine();
			if (ImGui::Button(menu->bLockSk[2] ? szLockSkE[0] : szLockSkE[1],ImVec2(50, 0)))
			{
				menu->bLockSk[2] ^= 1;
			}
		}

		{
			static char* szLockSkR[2] = { "True##LockR" , "False##LockR" };
			ImGui::LabelText("", "R"); ImGui::SameLine();
			if (ImGui::Button(menu->bLockSk[3] ? szLockSkR[0] : szLockSkR[1], ImVec2(50, 0)))
			{
				menu->bLockSk[3] ^= 1;
			}
		}

		ImGui::TreePop();
	}
}

void CGameRender::RenderCombo()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	if (ImGui::TreeNode(m_sComboText.c_str()))
	{
		{//q
			static char* szSkQ[2] = { "True##SkQ", "False##SkQ" };
			ImGui::LabelText("", "Q"); ImGui::SameLine();
			if (ImGui::Button(menu->bComboSk[0] ? szSkQ[0] : szSkQ[1], ImVec2(50, 0)))
			{
				menu->bComboSk[0] ^= 1;
			}
		}

		{//w
			static char* szSkW[2] = { "True##SkW", "False##SkW" };
			ImGui::LabelText("", "W"); ImGui::SameLine();
			if (ImGui::Button(menu->bComboSk[1] ? szSkW[0] : szSkW[1], ImVec2(50, 0)))
			{
				menu->bComboSk[1] ^= 1;
			}
		}

		{//e
			static char* szSkE[2] = { "True##SkE", "False##SkE" };
			ImGui::LabelText("", "E"); ImGui::SameLine();
			if (ImGui::Button(menu->bComboSk[2] ? szSkE[0] : szSkE[1], ImVec2(50, 0)))
			{
				menu->bComboSk[2] ^= 1;
			}
		}

		{//r
			static char* szSkR[2] = { "True##SkR", "False##SkR" };
			ImGui::LabelText("", "R"); ImGui::SameLine();
			if (ImGui::Button(menu->bComboSk[3] ? szSkR[0] : szSkR[1], ImVec2(50, 0)))
			{
				menu->bComboSk[3] ^= 1;
			}
		}


		{//技能1
			static char* szSk1ToUse[2] = { "True##Sk1ToUse", "False##Sk1ToUse" };
			ImGui::LabelText("", "%s", m_sCombosk1Text.c_str()); ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::Combo("##Sk1ToUse", &menu->iComboSequence[0], "Q\0W\0E\0R\0\0");
			ImGui::PopItemWidth();
		}

		{//技能2
			static char* szSk2ToUse[2] = { "True##Sk2ToUse", "False##Sk2ToUse" };
			ImGui::LabelText("", "%s", m_sCombosk2Text.c_str()); ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::Combo("##Sk2ToUse", &menu->iComboSequence[1], "Q\0W\0E\0R\0\0");
			ImGui::PopItemWidth();
		}

		{//技能3
			static char* szSk3ToUse[2] = { "True##Sk3ToUse", "False##Sk3ToUse" };
			ImGui::LabelText("", "%s", m_sCombosk3Text.c_str()); ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::Combo("##Sk3ToUse", &menu->iComboSequence[2], "Q\0W\0E\0R\0\0");
			ImGui::PopItemWidth();
		}

		{//技能4
			static char* szSk4ToUse[2] = { "True##Sk4ToUse", "False##Sk4ToUse" };
			ImGui::LabelText("", "%s", m_sCombosk4Text.c_str()); ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::Combo("##Sk4ToUse", &menu->iComboSequence[3], "Q\0W\0E\0R\0\0");
			ImGui::PopItemWidth();
		}

		ImGui::TreePop();
	}
}

void CGameRender::RenderFarming()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();

	if (ImGui::TreeNode(m_sFarmingText.c_str()))
	{
		static char* szFarming[2] = { "True##Farming", "False##Farming" };
		ImGui::LabelText("", "%s", m_sOpenFramingText.c_str()); ImGui::SameLine();
		if (ImGui::Button(menu->bFarming ? szFarming[0] : szFarming[1], ImVec2(50, 0)))
		{
			menu->bFarming ^= 1;
		}

		ImGui::LabelText("", "%s", m_sFarmingHotkeyText.c_str()); ImGui::SameLine();
		std::stringstream _btn;
		_btn << KeyNames[menu->dwFarmingHotkey].text << "##dwQingArmyHotkey";
		ImGui::Button(_btn.str().c_str(), ImVec2(50, 0));
		//如果鼠标悬停在button上面，就设置热键
		if (ImGui::IsItemHovered())
		{
			ImGuiIO& io = ImGui::GetIO();
			for (int i = 0; i < 256; i++)
			{
				if (io.KeysDown[i] == 1 && i != VK_PROCESSKEY)
				{
					menu->dwFarmingHotkey = i;
				}
			}
		}
		ImGui::TreePop();
	}
}

void CGameRender::RenderQingArmy()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();

	if (ImGui::TreeNode(m_sQingArmyText.c_str()))
	{
		{
			static char* szQingArmy[2] = { "True##QingArmy", "False##QingArmy" };
			ImGui::LabelText("", "%s", m_sQingArmtAttackText.c_str()); ImGui::SameLine();
			if (ImGui::Button(menu->bQingArmyWithAttack ? szQingArmy[0] : szQingArmy[1], ImVec2(50, 0)))
			{
				menu->bQingArmyWithAttack ^= 1;
			}
		}


		{ //Q
			char* szQingArmyQ[2] = { "True##QingArmyQ","False##QingArmyQ" };
			ImGui::LabelText("", "Q"); ImGui::SameLine();
			if (ImGui::Button(menu->bQingArmyWithSk[0] ? szQingArmyQ[0] : szQingArmyQ[1], ImVec2(50, 0)))
			{
				menu->bQingArmyWithSk[0] ^= 1;
			}

		}

		{// W
			char* szQingArmyW[2] = { "True##QingArmyW","False##QingArmyW" };
			ImGui::LabelText("", "W"); ImGui::SameLine();
			if (ImGui::Button(menu->bQingArmyWithSk[1] ? szQingArmyW[0] : szQingArmyW[1], ImVec2(50, 0)))
			{
				menu->bQingArmyWithSk[1] ^= 1;
			}

		}


		{//E
			char* szQingArmyE[2] = { "True##QingArmyE","False##QingArmyE" };
			ImGui::LabelText("", "E"); ImGui::SameLine();
			if (ImGui::Button(menu->bQingArmyWithSk[2] ? szQingArmyE[0] : szQingArmyE[1], ImVec2(50, 0)))
			{
				menu->bQingArmyWithSk[2] ^= 1;
			}
		}
	
		{//R
			char* szQingArmyR[2] = { "True##QingArmyR","False##QingArmyR" };
			ImGui::LabelText("", "R"); ImGui::SameLine();
			if (ImGui::Button(menu->bQingArmyWithSk[3] ? szQingArmyR[0] : szQingArmyR[1], ImVec2(50, 0)))
			{
				menu->bQingArmyWithSk[3] ^= 1;
			}
		}

		{
			ImGui::LabelText("", "%s", m_sQingArmyHotkey.c_str()); ImGui::SameLine();
			std::stringstream _btn;
			_btn << KeyNames[menu->dwQingArmyHotkey].text << "##dwQingArmyHotkey";
			ImGui::Button(_btn.str().c_str(), ImVec2(50, 0));
			//如果鼠标悬停在button上面，就设置热键
			if (ImGui::IsItemHovered())
			{
				ImGuiIO& io = ImGui::GetIO();
				for (int i = 0; i < 256; i++)
				{
					if (io.KeysDown[i] == 1 && i != VK_PROCESSKEY)
					{
						menu->dwQingArmyHotkey = i;
					}
				}
			}
		}

		ImGui::TreePop();
	}
}

void CGameRender::RenderTargetSelecter()
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();

	if (ImGui::TreeNode(m_sTargetSelecterText.c_str()))
	{

		for (auto& temp : menu->monsList)
		{
			if (temp.obj)
			{
				if (temp._sid.empty())
				{
					std::stringstream _sid;
					_sid << "##" << temp.obj;
					temp._sid = _sid.str();
				}
				
				if (temp._format.empty())
				{
					std::stringstream _format;
					_format << temp.name << AnsiToUtf8(" , 危险等级为 : %.0f");
					temp._format = _format.str();
				}
				ImGui::SliderInt(temp._sid.c_str(), &temp.dangerous,1, 5, temp._format.c_str());
			}
		}

		ImGui::SliderInt("##dwTargetPercent", &menu->dwTargetPercent, 1, 100, m_sTargetMinPercentText.c_str());

		ImGui::TreePop();
	}
}

bool CGameRender::UseSkillByIndex(DWORD index)
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	//判断是否是正常的索引
	if (index > 3 || index < 0)
	{
		return false;
	}

	//如果玩家死亡
	if (Player::BDead())
	{
		return false;
	}
	//获取技能
	auto sk = Player::GetSkillByIndex(index);
	if (!sk) 
	{
		return false;
	}

	//获取技能范围
	auto dis = sk->GetDistance();
	if (dis < 1)
	{
		dis = Player::GetAttackDistance();
	}

	//获取怪物对象
	DWORD obj = GetDangerousMostPerson(menu->monsList, dis, menu->dwTargetPercent);
	if (obj)
	{
		if (Player::UseSkillToMons(obj, sk->GetIndex(), menu->skDelay[index], menu->skSpeed[index], false))
		{
			return true;
		}

	}
	return false;
}


void CGameRender::Render()
{
	if (!m_bVisiable)
	{
		return;
	}

	//初始化imgui绘制
	ImGui_ImplDX9_NewFrame();

	//设置窗口属性
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	//设置窗口位置 & 创建窗口
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_Once);
	if (!ImGui::Begin("", NULL, ImVec2(0, 0), 0.0f, window_flags))
	{
		ImGui::End();
		return;
	}
	
	ImGui::PushStyleColor(ImGuiCol_Text, ImColor(0, 255, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor(68, 123,153));

	RenderTips();
	RenderSkConfig();
	RenderMoveAndAttack();
	RenderLockSkill();
	RenderCombo();
	RenderFarming();
	RenderQingArmy();
	RenderTargetSelecter();

	ImGui::PopStyleColor(2);
	ImGui::End();

	ImGui::Render();
}

bool CGameRender::Initialize()
{
	//1.获取游戏窗口句柄
	m_leagueHwnd = ::FindWindow("RiotWindowClass", NULL);
	if (!m_leagueHwnd)
	{
		return false;
	}


	//2.设置窗口消息回调
	m_ulOldWndProc = SetWindowLongW(m_leagueHwnd, GWL_WNDPROC, (LONG)WindowProc);
	if (0 == m_ulOldWndProc)
	{
		return false;
	}

	
	//3.获取设备对象
	auto deviceObject = utils::read<DWORD>(Base_DirectDeviceAddr);
	if (!deviceObject)
	{
		return false;
	}

	//4.读取到设备对象的虚表函数地址
	auto VirtualTableObject = utils::read<DWORD>(deviceObject);
	if (!VirtualTableObject)
	{
		return false;
	}

	auto VirtualTableObjectPtr = (DWORD*)VirtualTableObject;


	//5.绑定并初始化imgui,加载字体
	if (!ImGui_ImplDX9_Init(m_leagueHwnd, (IDirect3DDevice9*)(deviceObject)))
	{
		return false;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesChinese());


	//6.获取到Present Reset 函数地址 , 并开始hook
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (VirtualTableObjectPtr[0x10])
	{
		m_oldReset = (PVOID)VirtualTableObjectPtr[0x10];
		DetourAttach(&m_oldReset, hkReset);
	}

	if (VirtualTableObjectPtr[0x11])
	{
		m_oldPresent = (PVOID)VirtualTableObjectPtr[0x11];
		DetourAttach(&m_oldPresent, hkPresent);
	}

	if (DetourTransactionCommit() != NO_ERROR)
	{
		return false;
	}

	return true;
}

bool& CGameRender::BVisiable()
{
	return m_bVisiable;
}

LONG_PTR& CGameRender::GetOldWndProc()
{
	return m_ulOldWndProc;
}

PVOID CGameRender::GetOldReset()
{
	return m_oldReset;
}

PVOID CGameRender::GetOldPresent()
{
	return m_oldPresent;
}


LRESULT CGameRender::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto menu = CGameMenu::GetInstance()->GetGameMenu();
	if (CGameRender::GetInstance()->BVisiable())
	{
		ImGui_ImplDX9_WndProcHandler(hwnd, uMsg, wParam, lParam);
	}


	switch (uMsg)
	{
	case WM_KEYDOWN:
	{

		if(wParam ==  'Q' || wParam == 'q'){
			if (menu->bLockSk[0]){
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 100){
					if (CGameRender::GetInstance()->UseSkillByIndex(0)){
						timeSec = GetTickCount();
						
					}
				}
			}
			return true;
		}

		if(wParam == 'W' || wParam == 'w'){
			if (menu->bLockSk[1]){
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 100){
					if (CGameRender::GetInstance()->UseSkillByIndex(1)){
						timeSec = GetTickCount();
					}
				}
			}
			return true;
		}

		if(wParam == 'E' || wParam == 'e'){
			if (menu->bLockSk[2]){
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 100){
					if (CGameRender::GetInstance()->UseSkillByIndex(2)){
						timeSec = GetTickCount();
					}
				}
			}
			return true;
		}

		if (wParam == 'R'|| wParam == 'r'){			
			if (menu->bLockSk[3]){
				static DWORD timeSec = 0;
				if (GetTickCount() - timeSec > 100){
					if (CGameRender::GetInstance()->UseSkillByIndex(3)){
						timeSec = GetTickCount();
					}
				}
			}
			return true;
		}

		if (wParam == menu->dwComboHotKey){
			menu->bComboHotKeyDown = true;
		}

		if (wParam == menu->dwFarmingHotkey){
			menu->bFarmingHotkeyDown = true;
		}

		if (wParam == menu->dwQingArmyHotkey){
			menu->bQingArmyHotkeyDown = true;
		}

		break;
	}
	case WM_KEYUP:

		if (wParam == menu->dwComboHotKey){
			menu->bComboHotKeyDown = false;
		}

		if (wParam == menu->dwFarmingHotkey){
			menu->bFarmingHotkeyDown = false;
		}

		if (wParam == menu->dwQingArmyHotkey){
			menu->bQingArmyHotkeyDown = false;
		}

		if (wParam == VK_HOME){
			CGameRender::GetInstance()->SetBVisiable();
		}
		break;
	}

	return CallWindowProcW((WNDPROC)CGameRender::GetInstance()->GetOldWndProc(), hwnd, uMsg, wParam, lParam);
}



void CGameRender::SetBVisiable()
{
	m_bVisiable = !m_bVisiable;
}





long __stdcall hkReset(IDirect3DDevice9 * device, D3DPRESENT_PARAMETERS * pPresentationParameters)
{

	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto ret = (pReset(CGameRender::GetInstance()->GetOldReset()))(device, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();
	return ret;
}

long __stdcall hkPresent(IDirect3DDevice9 * device, CONST RECT * pSourceRect, CONST RECT * pDestRect, HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion)
{
	//完成自己界面的渲染
	CGameRender::GetInstance()->Render();
	return (pPresent(CGameRender::GetInstance()->GetOldPresent()))(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

