#include "stdafx.h"
#include "HookToMainThread.h"
#include "utils.h"
#include "GameCall.h"
#include "Player.h"
std::mutex CHookToMainThread::m_mutex;
CHookToMainThread* CHookToMainThread::m_pInstance = nullptr;
HHOOK CHookToMainThread::m_hHook = { 0 };
DWORD CHookToMainThread::m_msgCode = RegisterWindowMessage("CHookToMainThread");

CHookToMainThread::CHookToMainThread()
{
	Hook();
}

CHookToMainThread * CHookToMainThread::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CHookToMainThread();
		}
		m_mutex.unlock();
	}
	return m_pInstance;
}

void CHookToMainThread::DestoryInstance()
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


CHookToMainThread::~CHookToMainThread()
{
	if (m_hHook) {
		UnhookWindowsHookEx(m_hHook);
	}
}

bool CHookToMainThread::Hook()
{
	m_leagueHwnd = ::FindWindow("RiotWindowClass", NULL);
	if (!m_leagueHwnd) 
	{
		return false;
	}

	auto threadId = GetWindowThreadProcessId(m_leagueHwnd, NULL);
	if (!threadId) 
	{
		return false;
	}

	//挂载到主线程
	m_hHook = SetWindowsHookExA(WH_CALLWNDPROC,CHookToMainThread::CallWndProc,NULL, threadId);
	if (NULL == m_hHook) 
	{
		return false;
	}

	return true;
}


void CHookToMainThread::SendMessageToMainThread(MESSAGE msg, LPARAM lparam)
{
	::SendMessage(m_leagueHwnd, m_msgCode, msg, lparam);
}

LRESULT CHookToMainThread::CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT *pArg = (PCWPSTRUCT)(lParam);
	if (nCode == HC_ACTION)
	{
		if (pArg->message == CHookToMainThread::m_msgCode)
		{
			switch (pArg->wParam)
			{
			case MESSAGE::MSG_FINDWAY:
			{
				auto mousePnt = GameCall::GetMousePoint();
				auto playerpnt = Player::GetCurrPostion();
				if (mousePnt && playerpnt) {
					if (utils::GetDistance(mousePnt, playerpnt) < 2000.0f * 2000.0f) {
						GameCall::FindWay(mousePnt);
					}
				}
				return 1;
			}
			case MESSAGE::MSG_ATTACKCALL:
			{
				DWORD monObj = *(DWORD*)(pArg->lParam);
				GameCall::Attack(monObj);
				return 1;
			}
			case MESSAGE::MSG_USESKILL:
			{
				SKILL_TO_MONS sk2mons = *(SKILL_TO_MONS*)(pArg->lParam);
				GameCall::UseSkill(sk2mons.targetId, &sk2mons.pAskPnt, &sk2mons.pDestPnt, sk2mons.skillIndex, sk2mons.skillBase);
				//GameCall::Defense();
				return 1;
			}
			case MSG_USESKILL2:
			{
				SKILL_TO_MONS sk2mons = *(SKILL_TO_MONS*)(pArg->lParam);
				GameCall::useSkill2(&sk2mons.pAskPnt, sk2mons.skillIndex, sk2mons.skillBase);
				return 1;
			}
			case  MESSAGE::MSG_DEFENSE:
			{
				GameCall::Defense();
				return 1;
			}
			default:
				break;
			}
		}
	}
	return CallNextHookEx(CHookToMainThread::GetInstance()->m_hHook, nCode, wParam, lParam);
}


