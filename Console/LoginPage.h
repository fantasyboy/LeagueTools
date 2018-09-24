#pragma once
#include "afxwin.h"


// CLoginPage 对话框

typedef struct _LOGIN_DATA_ST
{
	CString username;
	CString password;
	bool bRem;
}LOGIN_DATA_ST,*PLOGIN_DATA_ST;

class CLoginPage : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginPage)

public:
	CLoginPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginPage();
	enum { IDD = IDD_DIALOG_LOGIN };

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_username;
	CString m_password;
	BOOL m_bRemberPassword;
	afx_msg void OnBnClickedButtonLogin();
protected:
	afx_msg LRESULT OnLogin(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
protected:
//	afx_msg LRESULT OnLoginsuccess(WPARAM wParam, LPARAM lParam);
public:
	CStatic m_logCtl;
protected:
	afx_msg LRESULT OnSetlog(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
};
