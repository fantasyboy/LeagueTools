// RegisterPage.cpp : 实现文件
//

#include "stdafx.h"
#include "Console.h"
#include "RegisterPage.h"
#include "afxdialogex.h"
#include "LoginPage.h"
#include "AddtimePage.h"
#include "NetworkService.h"

// CRegisterPage 对话框

IMPLEMENT_DYNAMIC(CRegisterPage, CDialogEx)

CRegisterPage::CRegisterPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_REGISTER, pParent)
	, m_username(_T(""))
	, m_password(_T(""))
{

}

CRegisterPage::~CRegisterPage()
{
}

void CRegisterPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOG, m_logCtl);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT2, m_password);
}


BEGIN_MESSAGE_MAP(CRegisterPage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CRegisterPage::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CRegisterPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CRegisterPage::OnBnClickedButton3)
END_MESSAGE_MAP()


// CRegisterPage 消息处理程序


void CRegisterPage::OnBnClickedButton2()
{
	// TODO: 切换到登录界面
	CDialog::OnOK();
	VMProtectBegin(__FUNCTION__);
	CLoginPage dlg;
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
	VMProtectEnd();
}


void CRegisterPage::OnBnClickedButton1()
{
	// TODO: 注册逻辑
	UpdateData(TRUE);
	auto ret = CNetworkService::GetInstance()->Register(const_cast<char*>(m_username.GetBuffer()), const_cast<char*>(m_password.GetBuffer()));
	switch (ret)
	{
	case 0:
		m_logCtl.SetWindowTextA("注册成功");
		break;
	case -1:
		m_logCtl.SetWindowTextA("注册失败");
		break;
	case -8:
		m_logCtl.SetWindowTextA("用户名重复");
		break;
	default:
		break;
	}
}


void CRegisterPage::OnBnClickedButton3()
{
	// TODO: 切换到充值界面
	CDialog::OnOK();
	VMProtectBegin(__FUNCTION__);
	CAddtimePage dlg;
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
	VMProtectEnd();
}
