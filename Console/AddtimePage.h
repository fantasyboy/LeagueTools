#pragma once
#include "afxwin.h"


// CAddtimePage 对话框

class CAddtimePage : public CDialogEx
{
	DECLARE_DYNAMIC(CAddtimePage)

public:
	CAddtimePage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddtimePage();
	enum { IDD = IDD_DIALOG_ADDTIME};
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADDTIME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_username;
	CString m_key;
	CString m_recommand;
	CStatic m_LogCtl;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
};
