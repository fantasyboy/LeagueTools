#pragma once
#include <mutex>

#include "d3d9.h"
#include "d3dx9.h"
typedef long(__stdcall * pReset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
typedef long(__stdcall * pPresent)(IDirect3DDevice9 * device, THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

class CGameRender 
{
	CGameRender();
	CGameRender(const CGameRender&) = delete;
	void operator= (const CGameRender&) = delete;
	CGameRender(const CGameRender&&) = delete;
	void operator= (const CGameRender&&) = delete;

	static CGameRender* m_pInstance;
	static std::mutex m_mutex;

public:
	~CGameRender();
	static CGameRender* GetInstance();
	static void DestoryInstance();
	//初始化函数
	void Render();
	bool Initialize();
	bool& BVisiable();
	LONG_PTR& GetOldWndProc();
	PVOID GetOldReset();
	PVOID GetOldPresent();
	static LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	void SetBVisiable();
private:
	void RenderTips(); //绘制提示项
	void RenderSkConfig(); //渲染技能延迟
	void RenderMoveAndAttack(); //渲染走砍
	void RenderLockSkill(); //渲染锁定技能
	void RenderCombo(); //渲染连招
	void RenderFarming(); //渲染补刀
	void RenderQingArmy(); //渲染清兵
	void RenderTargetSelecter(); //渲染目标选择器
	bool UseSkillByIndex(DWORD index);
private:
	HWND m_leagueHwnd; 
	LONG_PTR m_ulOldWndProc;
	bool m_bVisiable;
	PVOID m_oldReset;
	PVOID m_oldPresent;

	std::string m_sTipsText;
	std::string m_skConfigText; //技能设置
	std::string m_skDelayText; //技能前摇延迟
	std::string m_skSpeedText; //技能速度
	std::string m_sMoveAndAttackText; //走砍设置
	std::string m_sFollowMosueText; //跟随鼠标
	std::string m_sAttackText; //普通攻击
	std::string m_sRenderAttackDisText; //绘制普攻距离
	std::string m_sComboHotkeyText; //连招热键
	std::string m_sLockSkillText;  //锁定技能
	std::string m_sComboText; //连招设置
	std::string m_sFarmingText; //补刀设置
	std::string m_sOpenFramingText; //开启补刀
	std::string m_sFarmingHotkeyText; //补刀热键
	std::string m_sQingArmyText; //清兵设置
	std::string m_sQingArmtAttackText; //普攻清兵
	std::string m_sQingArmyHotkey; //清兵热键
	std::string m_sTargetSelecterText; //目标选择器设置
	std::string m_sTargetMinPercentText; //目标最低血量选择
	std::string m_sCombosk1Text; //连招使用技能1
	std::string m_sCombosk2Text; //连招使用技能2
	std::string m_sCombosk3Text; //连招使用技能3
	std::string m_sCombosk4Text; //连招使用技能4
};

/*
// API hook 函数
*/
long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
long __stdcall hkPresent(IDirect3DDevice9* device, THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
