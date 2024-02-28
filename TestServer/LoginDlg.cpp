// LoginDlg.cpp : implementation file
//

#include "pch.h"
#include "TestServer.h"
#include "afxdialogex.h"
#include "LoginDlg.h"
#include <array>

// LoginDlg dialog

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
{
}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LoginDlg::OnBnClickedCancel)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void LoginDlg::OnBnClickedOk()
{
	// 부모 대화상자에 message = 23001, wParam = 1 보내기
	GetParent()->PostMessage(23001, 1);
}


void LoginDlg::OnBnClickedCancel()
{
	// 부모 대화상자에 message = 23001, wParam = 0 보내기
	GetParent()->PostMessage(23001, 0);
}

TCHAR* LoginDlg::GetID()
{
	CString get_id;
	GetDlgItemText(IDC_ID, get_id);

	INT s_len = sizeof(m_id) / sizeof(m_id[0]);
	memset(m_id, 0x00, sizeof(TCHAR) * s_len);
	_tcscpy_s(m_id, s_len, get_id.GetBuffer(0));
	get_id.ReleaseBuffer();

	return m_id;
}

TCHAR* LoginDlg::GetPW()
{
	CString get_pw;
	GetDlgItemText(IDC_PW, get_pw);

	INT s_len = sizeof(m_pw) / sizeof(m_pw[0]);
	memset(m_pw, 0x00, sizeof(TCHAR) * s_len);
	_tcscpy_s(m_pw, s_len, get_pw.GetBuffer(0));
	get_pw.ReleaseBuffer();

	return m_pw;
}

int LoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rect;
	GetWindowRect(&rect);				// 현재 다이얼로그 위치와 크기를 가져온다

	// 화면 크기를 가져온다
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);

	// 다이얼로그 중앙 배치
	MoveWindow((x / 2) - (rect.Width()/2), (y / 2) - (rect.Height()/2), rect.Width(), rect.Height(), TRUE);

	return 0;
}
