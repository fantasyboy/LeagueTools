#pragma once

#include <mutex>
#include "Monster.h"

enum MESSAGE
{
	MSG_FINDWAY = 100,
	MSG_ATTACKCALL,
	MSG_USESKILL,
	MSG_USESKILL2,
	MSG_DEFENSE
};

struct SKILL_TO_MONS
{
	DWORD targetId;
	EP_POINT_3D pCurrPnt;
	EP_POINT_3D pDestPnt;
	EP_POINT_3D pAskPnt;
	DWORD skillIndex;
	DWORD skillBase;
};


class CHookToMainThread
{
	CHookToMainThread();
	CHookToMainThread(const CHookToMainThread&) = delete;
	void operator= (const CHookToMainThread&) = delete;
	CHookToMainThread(const CHookToMainThread&&) = delete;
	void operator= (const CHookToMainThread&&) = delete;

	static std::mutex m_mutex;
	static CHookToMainThread* m_pInstance;
public:
	static CHookToMainThread* GetInstance();
	static void DestoryInstance();
	~CHookToMainThread();

	//挂载到主线程
	bool Hook();

	//向主线程发送消息
	void SendMessageToMainThread(MESSAGE msg, LPARAM lparam);
	static LRESULT CALLBACK CallWndProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

public:
	static HHOOK m_hHook;
	static DWORD m_msgCode;
private:
	HWND m_leagueHwnd;
};

