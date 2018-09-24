#pragma once
#include "afxwin.h"


// CRegisterPage 对话框

class CRegisterPage : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterPage)

public:
	CRegisterPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisterPage();
	enum { IDD = IDD_DIALOG_REGISTER};
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	CStatic m_logCtl;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CString m_username;
	CString m_password;
};
