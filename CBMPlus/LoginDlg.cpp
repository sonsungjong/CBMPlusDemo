// LoginDlg.cpp : implementation file
//

#include "pch.h"
#include "CBMPlus.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// LoginDlg dialog

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

LoginDlg::~LoginDlg()
{
}

TCHAR* LoginDlg::GetID()
{
	CString get_id;
	GetDlgItemText(IDC_ID, get_id);

	INT s_len = sizeof(m_id)/sizeof(m_id[0]);
	memset(m_id, 0x00, sizeof(TCHAR) * s_len);
	_tcscpy_s(m_id, s_len, get_id.GetBuffer(0));
	get_id.ReleaseBuffer();

	return m_id;
}

TCHAR* LoginDlg::GetPW()
{
	CString get_pw;
	GetDlgItemText(IDC_PW, get_pw);

	INT s_len = sizeof(m_pw)/sizeof(m_pw[0]);
	memset(m_pw, 0x00, sizeof(TCHAR) * s_len);
	_tcscpy_s(m_pw, s_len, get_pw.GetBuffer(0));
	get_pw.ReleaseBuffer();

	return m_pw;
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &LoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;
}


void LoginDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void LoginDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// 부모 대화상자에 message = 23001, wParam = 1 보내기
	GetParent()->PostMessage(23001, 1);
	//CDialogEx::OnOK()
}


void LoginDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	// 부모 대화상자에 message = 23001, wParam = 0 보내기
	GetParent()->PostMessage(23001, 0);
	//CDialogEx::OnCancel()
}