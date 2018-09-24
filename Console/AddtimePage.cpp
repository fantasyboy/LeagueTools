// AddtimePage.cpp : 实现文件
//

#include "stdafx.h"
#include "Console.h"
#include "AddtimePage.h"
#include "afxdialogex.h"
#include "LoginPage.h"
#include "RegisterPage.h"
#include "NetworkService.h"

// CAddtimePage 对话框

IMPLEMENT_DYNAMIC(CAddtimePage, CDialogEx)

CAddtimePage::CAddtimePage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ADDTIME, pParent)
	, m_username(_T(""))
	, m_key(_T(""))
	, m_recommand(_T(""))
{

}

CAddtimePage::~CAddtimePage()
{
}

void CAddtimePage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT2, m_key);
	DDX_Text(pDX, IDC_EDIT3, m_recommand);
	DDX_Control(pDX, IDC_STATIC_LOG, m_LogCtl);
}


BEGIN_MESSAGE_MAP(CAddtimePage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddtimePage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CAddtimePage::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CAddtimePage::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAddtimePage 消息处理程序


void CAddtimePage::OnBnClickedButton1()
{
	// TODO: 充值界面
	UpdateData(TRUE);

	if (m_recommand.IsEmpty())
	{
		m_recommand = "fantasy";
	}

	short days = 0;
	short points = 0;
	auto ret = CNetworkService::GetInstance()->UserAddTime(const_cast<char*>(m_username.GetBuffer()),
		const_cast<char*>(m_key.GetBuffer()),
		const_cast<char*>(m_recommand.GetBuffer()),
		days, points);

	switch (ret)
	{
	case 0:
	{
		CString _format;
		_format.Format("加时成功，共计%d天！", days);
		m_LogCtl.SetWindowTextA(_format);
	}
		break;
	default:
		CString _format;
		_format.Format("加时失败，卡密错误，或者账户无效", days);
		m_LogCtl.SetWindowTextA(_format);
		break;
	}
}


void CAddtimePage::OnBnClickedButton3()
{
	// TODO: 切换到登录界面
	VMProtectBegin(__FUNCTION__);
	CDialog::OnOK();
	CLoginPage dlg;
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
	VMProtectEnd();
}


void CAddtimePage::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	VMProtectBegin(__FUNCTION__);
	CDialog::OnOK();
	CRegisterPage dlg;
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
	VMProtectEnd();
}
