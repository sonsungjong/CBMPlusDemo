
// CBMBackendDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CBMBackend.h"
#include "CBMBackendDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// [����]Ŭ���̾�Ʈ ���� �� ȣ��Ǵ� �Լ�
void MyServerSocket::AcceptUserProc(UserData* ap_user_data)
{
	//CString str;
	//str.Format(L"%s ���� ������ �����߽��ϴ�.", ap_user_data->ip_address);
	//((CBMBackendDlg*)mp_notify_wnd)->AddEventString(str);
}

// [����]���ŵ� �����͸� ó���ϴ� �Լ�
int MyServerSocket::ProcessNetMessage()
{
	// m_net_msg_id���� 1�̸� ä�� �����̹Ƿ� ����Ʈ �ڽ��� ���ڿ� ���
	if (m_net_msg_id == 1) {
		//((CBMBackendDlg*)mp_notify_wnd)->ProcessingSensorData(mp_net_body_data, m_net_body_size);
	}
	return 1;
}
// [����]Ŭ���̾�Ʈ ������ ������ ��쿡 ȣ��Ǵ� �Լ�
void MyServerSocket::CloseUserProc(UserData* ap_user_data, int a_error_flag)
{
	if (a_error_flag == 0) {			// ������ ������ 0
		CString str;
		str.Format(L"%s���� ������ �����߽��ϴ�.", ap_user_data->ip_address);
		//((CBMBackendDlg*)mp_notify_wnd)->AddEventString(str);
	}
}


CCBMBackendDlg::CCBMBackendDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBMBACKEND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	mp_environment = new EnvironmentModel[200000];			// ���� ������ �� 20�������� ���尡��
	m_dbfile.GetEnvironmentData(mp_environment);


}

void CCBMBackendDlg::ListCtrlSearch()
{
	// ���� �׸� ����
	if (m_listctrl_flag != 0) {
		m_data_listctrl.DeleteAllItems();				// ��� ������ ����
		for (int i = m_data_listctrl.GetHeaderCtrl()->GetItemCount() - 1; i >= 0; i--)
		{
			m_data_listctrl.DeleteColumn(i);				// ��� ������ ����
		}
	}

	// ���ο� �˻��׸� ����
	if (m_listctrl_flag % 2 == 0) {
		m_listctrl_flag++;
		m_data_listctrl.SetData(mp_environment);
	}
	else if (m_listctrl_flag % 2 == 1) {
		m_listctrl_flag++;
		m_data_listctrl.SetData2();
	}
}

void CCBMBackendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCBMBackendDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCBMBackendDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCBMBackendDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SEARCH_BTN, &CCBMBackendDlg::OnBnClickedSearchBtn)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_NCCREATE()
	ON_WM_MEASUREITEM()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCBMBackendDlg message handlers

BOOL CCBMBackendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetBackgroundColor(RGB(50, 70, 90));
	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"���� ���");

	// ��Ʈ��ũ ����
	SetWindowPos(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	HWND h_find_wnd = NULL;
	while (h_find_wnd = ::FindWindowExW(m_hWnd, h_find_wnd, L"Static", NULL)) {
		::SendMessage(h_find_wnd, WM_SETFONT, (WPARAM)(HFONT)m_font, 1);
	}

	GetDlgItem(IDC_TEMP_HUMI_RECT)->GetWindowRect(m_temp_humi_graph);
	ScreenToClient(m_temp_humi_graph);

	CreateDirectory(L".\\data", NULL);		// ���� �����

	// ����Ŭ����
	m_data_listctrl.SubclassDlgItem(IDC_DATA_LIST, this);				// �߾� ����Ʈ ��Ʈ��
	m_search_btn.SubclassDlgItem(IDC_SEARCH_BTN, this);			// �˻� ��ư
	

	// ��� ����
	m_tool_bar.AddButton(L"  ����ź ���� �Է�  ", 24003, L".\\images\\setting_icon.png");			// 24003 �߻�
	m_tool_bar.AddButton(L"      ����� ����      ", 24004, L".\\images\\user_icon.png");			// 24004 �߻�
	m_tool_bar.AddButton(L"  CSV���� �ҷ�����  ", 24005, L".\\images\\csv_icon.png");			// 24005 �߻�
	m_tool_bar.AddButton(L"  XLSX���� �ҷ�����  ", 24006, L".\\images\\xlsx_icon.png");			// 24006 �߻�
	m_tool_bar.AddButton(L"        ����        ", 24002, L".\\images\\exit_icon.png");			// 24002 �߻�
	m_tool_bar.CreateToolBar(IDC_TOOL_RECT, this, 24001);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCBMBackendDlg::OnPaint()
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
HCURSOR CCBMBackendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCBMBackendDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}


void CCBMBackendDlg::OnBnClickedCancel()
{
	if (::MessageBox(m_hWnd, _T("���α׷��� �����Ͻðڽ��ϱ�?"), _T("���α׷� ����"), MB_OKCANCEL) == IDOK)
	{
		CDialogEx::OnCancel();
	}
}


void CCBMBackendDlg::OnBnClickedSearchBtn()
{
	// TODO: Add your control notification handler code here
	

	ListCtrlSearch();
}


void CCBMBackendDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	delete[] mp_environment;

	// ��Ʈ ����
	m_font.DeleteObject();

	// ��������� ���̾�α� �޸� ����
	// 	CloseLoginDialog();
}


HBRUSH CCBMBackendDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0, 128, 0));
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


BOOL CCBMBackendDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CDialogEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}


void CCBMBackendDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	if (IDC_DATA_LIST == nIDCtl) {
		lpMeasureItemStruct->itemHeight += 15;
	}

	CDialogEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


BOOL CCBMBackendDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == 24002)
	{
		// ���� �����ư
		OnBnClickedCancel();
	}
	else if (wParam == 24003) {
		MessageBox(_T("����ź ���� �Է�"));
	}
	else if (wParam == 24004) {
		MessageBox(_T("����� ����"));
	}
	else if (wParam == 24005) {
		MessageBox(_T("CSV���� �ҷ�����"));
	}
	else if (wParam == 24006) {
		MessageBox(_T("XLSX���� �ҷ�����"));
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


LRESULT CCBMBackendDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == 26001) {
		// ����Ʈ��Ʈ�� �󼼺��� ����
		int select_item = m_data_listctrl.GetSelectionMark();
		wchar_t str[128];
		swprintf_s(str, 128, L"%d", select_item + 1);
		//str.Format(_T("%d"), select_item+1);
		if (select_item != LB_ERR)
		{
			AfxMessageBox(str);
		}
	}
	else if (message == 23000 || message == 23001) {
		//DoLoginDialog(message, wParam, lParam);
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CCBMBackendDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == SELECT_LISTCTRL_ROW) {
		KillTimer(SELECT_LISTCTRL_ROW);				// ��ȸ�� Ÿ�̸�

		// ToDo�ӽ��ڵ� : �� �κ��� �׷��� �׸��� �ڵ�� �ٲٸ��
		HDC hdc = ::GetDC(m_hWnd);
		Rectangle(hdc, m_temp_humi_graph.left + m_move, m_temp_humi_graph.top, m_temp_humi_graph.left + 100 + m_move, m_temp_humi_graph.top + 100);
		m_move += 10;
		::ReleaseDC(m_hWnd, hdc);
		//


	}
	else {
		CDialogEx::OnTimer(nIDEvent);
	}
}
