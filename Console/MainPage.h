#pragma once
#include "afxwin.h"


// CMainPage 对话框

class CMainPage : public CDialogEx
{
	DECLARE_DYNAMIC(CMainPage)

public:
	CMainPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
//	CStatic m_EndDateTextCtl;
	virtual BOOL OnInitDialog();
//	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_bmpStaticCtl;
	afx_msg void OnStnDblclickStaticbmp();
	static NOTIFYICONDATA m_nid; //初始化托盘相关
	static HMENU m_hMenu; //托盘菜单
protected:
	afx_msg LRESULT OnInject(WPARAM wParam, LPARAM lParam);
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg LRESULT OnTray(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
};

DWORD WINAPI InjectThreadProc(
	_In_ LPVOID lpParameter
);