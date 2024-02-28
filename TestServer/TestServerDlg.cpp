#include "pch.h"
#include "framework.h"
#include "TestServer.h"
#include "TestServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTestServerDlg::CTestServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}



void CTestServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestServerDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_NCCREATE()
	ON_WM_MEASUREITEM()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SEACH_BTN, &CTestServerDlg::OnBnClickedSeachBtn)
END_MESSAGE_MAP()


// CTestServerDlg message handlers

BOOL CTestServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// ���̾�α� ȭ���� �����Ѵ�
	SetBackgroundColor(RGB(50, 70, 90));
	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"���� ���");
	HWND h_find_wnd = NULL;
	while (h_find_wnd = ::FindWindowExW(m_hWnd, h_find_wnd, L"Static", NULL)) {
		::SendMessage(h_find_wnd, WM_SETFONT, (WPARAM)(HFONT)m_font, 1);
	}
	SetWindowPos(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	// �����͸� ��ȸ�ؼ� ���� �����Ѵ�
	CreateDirectory(L".\\data", NULL);		// ���� �����

	// ���� Ŭ���̾�Ʈ�� �����ؼ� ������ �����Ѵ�

	// ȭ�� �׸��� �ʱ��۾��� �����Ѵ�

	// ��Ʈ���� �����Ѵ�
	m_data_list.SubclassDlgItem(IDC_DATA_LIST, this);
	GetDlgItem(IDC_TEMP_RECT)->GetWindowRect(m_temp_rect);
	ScreenToClient(m_temp_rect);
	GetDlgItem(IDC_VIB_RECT)->GetWindowRect(m_vib_rect);
	ScreenToClient(m_vib_rect);

	m_tool_bar.AddButton(L"    ����    ", 24006, L".\\images\\setting_icon.png");			// 24006 �߻�
	m_tool_bar.AddButton(L"  ����� ����  ", 24003, L".\\images\\user_icon.png");			// 24003 �߻�
	m_tool_bar.AddButton(L"  CSV���� �ҷ�����  ", 24004, L".\\images\\csv_icon.png");			// 24004 �߻�
	m_tool_bar.AddButton(L"  XLSX���� �ҷ�����  ", 24005, L".\\images\\xlsx_icon.png");			// 24005 �߻�
	m_tool_bar.AddButton(L"    ����    ", 24002, L".\\images\\exit_icon.png");			// 24002 �߻�
	
	m_tool_bar.CreateToolBar(IDC_TOOLBAR_RECT, this, 24001);

	// ����ȭ�� ���� �ڱ��ڽ� ���̾�α׿� 23000 �޽����� ������
	PostMessage(23000);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestServerDlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CTestServerDlg::OnBnClickedCancel()
{
	if (::MessageBox(m_hWnd, _T("���α׷��� �����Ͻðڽ��ϱ�?"), _T("���α׷� ����"), MB_OKCANCEL) == IDOK)
	{
		CDialogEx::OnCancel();
	}
}


void CTestServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	// ���� ����

	// �׸��� ����
	m_font.DeleteObject();

	// ��������� �α��� ���̾�α� �޸� ����
	CloseLoginDialog();
}


void CTestServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
}

// �α����� ���� ����ȭ�� ����ó��
BOOL CTestServerDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CDialogEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	// TODO:  Add your specialized creation code here
	// �α��� �ϱ� ���� ����ó��
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// ���� �������� �Ӽ��� ������
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style | WS_EX_LAYERED);					// LAYERED �Ӽ��� �߰���

	return TRUE;
}

// �޽��� �߻� �� ȣ��Ǵ� �Լ�
LRESULT CTestServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (message == 26001) {
		// �󼼺��� ����
		int select_item = m_data_list.GetSelectionMark();
		wchar_t str[128];
		swprintf_s(str, 128, L"%d", select_item + 1);
		//str.Format(_T("%d"), select_item+1);
		if (select_item != LB_ERR)
		{
			AfxMessageBox(str);
		}
	}
	else if(message == 23000 || message == 23001){
		DoLoginDialog(message, wParam, lParam);
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CTestServerDlg::DoLoginDialog(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == 23000) {
		mp_login_dlg = new LoginDlg;
		mp_login_dlg->Create(IDD_LOGIN, this);
	}
	else if (message == 23001) {
		if ((int)wParam == 1) {
			// �α��� Ȯ�ι�ư
			TCHAR* id = mp_login_dlg->GetID();
			TCHAR* pw = mp_login_dlg->GetPW();
			UA account;
			memset(account.user_id, 0, sizeof(account.user_id));
			memset(account.user_pw, 0, sizeof(account.user_pw));
			memcpy(account.user_id, id, (wcslen(id)+1)*sizeof(TCHAR));
			memcpy(account.user_pw, pw, (wcslen(pw) + 1) * sizeof(TCHAR));
			int login_request = m_dbfile.SelectAllUser(account);

			if (login_request == 0) {
				AfxMessageBox(_T("���̵� �Ǵ� ��й�ȣ�� ��ġ���� �ʽ��ϴ�."));
			}
			else if(login_request == 1 || TESTER){
				CloseLoginDialog();
				LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// ���� �������� �Ӽ��� ������
				::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style & ~WS_EX_LAYERED);					// LAYERED �Ӽ��� ������
				Invalidate(TRUE);
				//m_listview1.Invalidate(1);
				//m_listview1.UpdateWindow();
			}
			else if(login_request == -1){
				AfxMessageBox(_T("�����ͺ��̽��� ������ �� �����ϴ�."));
			}
		}
		else if ((int)wParam == 0) {
			// �α��� ��ҹ�ư
			CloseLoginDialog();
			EndDialog(IDCANCEL);
		}
	}
}

void CTestServerDlg::CloseLoginDialog()
{
	if (mp_login_dlg != NULL) {
		mp_login_dlg->DestroyWindow();
		delete mp_login_dlg;
		mp_login_dlg = NULL;
	}
}


void CTestServerDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	if (IDC_DATA_LIST == nIDCtl) {
		lpMeasureItemStruct->itemHeight += 15;
	}

	CDialogEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


HBRUSH CTestServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0, 128, 0));
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

// ����
BOOL CTestServerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == 24002)
	{
		// ���� - ����
		OnBnClickedCancel();
	}
	else if (wParam == 24003) {
		// ���� - ����� ����
		AfxMessageBox(_T("����� �������� ���̾�α� ����"));
	}
	else if (wParam == 24004) {
		// ���� - CSV���� �ҷ�����
		AfxMessageBox(_T("���� ������ csv�����̸� DB �Է�"));

	}
	else if (wParam == 24005) {
		// ���� - XLSX���� �ҷ�����
		AfxMessageBox(_T("���� ������ xlsx�����̸� DB �Է�"));
	}
	else if (wParam == 24006) {
		// ���� - ����
		AfxMessageBox(_T("���� ���̾�α� ����"));
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

// �˻� ��ư
void CTestServerDlg::OnBnClickedSeachBtn()
{
	// ���� �׸� ����
	if (m_list_flag != 0) {
		m_data_list.DeleteAllItems();
		for (int i = m_data_list.GetHeaderCtrl()->GetItemCount() - 1; i >= 0; i--)
		{
			m_data_list.DeleteColumn(i);
		}
	}

	// ���ο� �˻��׸� ����
	if (m_list_flag % 2 == 0) {
		m_list_flag++;
		m_data_list.SetData();
	}
	else if (m_list_flag % 2 == 1) {
		m_list_flag++;
		m_data_list.SetData2();
	}
}
