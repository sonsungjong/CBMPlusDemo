#include "pch.h"
#include "framework.h"
#include "CBMPlus.h"
#include "CBMPlusDlg.h"
#include "afxdialogex.h"



CCBMPlusDlg::CCBMPlusDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBMPLUS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	// ��ü �ʱ�ȭ
}

void CCBMPlusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTVIEW1, m_listview1);
}

BEGIN_MESSAGE_MAP(CCBMPlusDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCBMPlusDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCBMPlusDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCCREATE()
END_MESSAGE_MAP()


// CCBMPlusDlg message handlers

BOOL CCBMPlusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// MFC ���� ���� ����
	GetDlgItem(IDC_TITLE_RECT)->GetWindowRect(m_title_rect);
	ScreenToClient(m_title_rect);

	GetDlgItem(IDC_TEXT_RECT)->GetWindowRect(m_text_rect);
	ScreenToClient(m_text_rect);

	GetDlgItem(IDC_GRAPH1)->GetWindowRect(m_graph1);
	ScreenToClient(m_graph1);

	GetDlgItem(IDC_GRAPH2)->GetWindowRect(m_graph2);
	ScreenToClient(m_graph2);

	m_title_menu1.SetRect(3, m_title_rect.top + 3, 128, m_title_rect.bottom - 3);
	m_title_menu2.SetRect(141, m_title_rect.top + 3, 260, m_title_rect.bottom - 3);
	m_title_menu3.SetRect(300, m_title_rect.top + 3, 440, m_title_rect.bottom - 3);

	// �����ͺ��̽��� ��ȸ�ؼ� ��ü�� ���� �����Ѵ�.

	// ���� Ŭ���̾�Ʈ�� ������ ������ �����Ѵ�.
	// InitSocket();

	// ��ü�� ���� �׸��� �ʱ⼳��
	InitPaint();
	MyPaint();				// ��ü���� �޾� �׸���

	// ��ü�� ���� ����Ʈ ��Ʈ��(�׸���)�� �ִ´�
	SetListView();

	// Full HD ũ�� ����
	SetWindowPos(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);				//ShowWindow(SW_MAXIMIZE)

	// ����ȭ�� ���� �ڱ��ڽ� ���̾�α׿� 23000 �޽����� ������
	PostMessage(23000);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCBMPlusDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{

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
		// ����Ѵ�.
		m_dcp.Draw(dc);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCBMPlusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCBMPlusDlg::OnBnClickedOk()
{
	//CDialogEx::OnOK()
}


void CCBMPlusDlg::OnBnClickedCancel()
{
	if (::MessageBox(m_hWnd, _T("���α׷��� �����Ͻðڽ��ϱ�?"), _T("���α׷� ����"), MB_OKCANCEL) == IDOK)
	{
		CDialogEx::OnCancel();
	}
}


void CCBMPlusDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	if (m_client_socket.IsConnect() != 0) {
		m_client_socket.CloseSocket();				// ���� ����
	}
	//KillTimer(1)
	// ��������� �α��� ���̾�α� �޸� ����
	CloseLoginDialog();
}

// �޽��� �߻� �� ȣ��Ǵ� �Լ�
LRESULT CCBMPlusDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	// ���� ��ü�� �޽����� ó���� �� �ֵ��� �Լ��� ȣ��(26001, 26002)
	if (message == 26001 || message == 26002) {
		m_client_socket.MessageProc(message, wParam, lParam);
	}
	else if (message == 23000 || message == 23001) {
		DoLoginDialog(message, wParam, lParam);
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CCBMPlusDlg::MyPaint()
{
	//m_dcp.Draw(_T("./res/korea.png"), m_map_rect.left, m_map_rect.top, m_map_rect.Width(), m_map_rect.Height());
	m_dcp.FillSolidRect(m_title_rect.left, m_title_rect.top, m_title_rect.right, m_title_rect.bottom, RGB24(56, 56, 56));					// Ÿ��Ʋ �簢��
	m_dcp.Draw(_T("./res/bar_graph.png"), m_graph1.left, m_graph1.top, m_graph1.Width(), m_graph1.Height());
	m_dcp.Draw(_T("./res/line_graph.png"), m_graph2.left, m_graph2.top, m_graph2.Width(), m_graph2.Height());
	
	m_dcp.Draw(_T("./res/button27.png"), m_title_menu1.left, m_title_menu1.top, m_title_menu1.Width(), m_title_menu1.Height());
	m_dcp.Draw(_T("./res/button27.png"), m_title_menu2.left, m_title_menu2.top, m_title_menu2.Width(), m_title_menu2.Height());
	m_dcp.Draw(_T("./res/button27.png"), m_title_menu3.left, m_title_menu3.top, m_title_menu3.Width(), m_title_menu3.Height());

	m_dcp.GpTextOut(m_title_menu1.left+4, m_title_menu1.top+4, RGB24(255, 255, 255), _T("����� ����"), 6);
	m_dcp.GpTextOut(m_title_menu2.left+4, m_title_menu2.top+4, RGB24(255, 255, 255), _T("������ �Է�"), 6);
	m_dcp.GpTextOut(m_title_menu3.left+4, m_title_menu3.top+4, RGB24(210, 0, 0), _T("���α׷� ����"), 7);
	
	m_dcp.PieChart(10, 10, 200, 200, 0.7);
}


void CCBMPlusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1) {
		AfxMessageBox(_T("�׽�Ʈ Ÿ�̸�"));
		m_timer_count++;
		if (m_timer_count > 3) {
			KillTimer(1);
			m_timer_count = 0;
		}
	}
	else if (nIDEvent == 1000) {
		// GUI ������� ����
		if (m_client_socket.IsConnect() != 0) {
			int result = 0;
			TSTRING tstr = _T("sj");
			LONG64 data_size = tstr.length();
			if (data_size > 0) {
				result = m_client_socket.SendFrameData(1, const_cast<const TCHAR*>(tstr.c_str()), (data_size + 1) * sizeof(TCHAR));
				if (result == SOCKET_ERROR) {
					AfxMessageBox(_T("���� ���� ����"));
				}
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CCBMPlusDlg::Connect2Server()
{
	// �������Ͽ� ����
	if (m_client_socket.IsConnect() == 0) {
		m_client_socket.ConnectToServer(_T("127.0.0.1"), 65384, this, 26001, 26002);
	}
	else {
		AfxMessageBox(_T("������ �̹� ������ �õ��ϰ� �ְų� ���ӵ� �����Դϴ�."));
	}
}

// ����ȭ�� ����̹���
void CCBMPlusDlg::SetMainBk()
{
	m_main_bk_image.Load(_T(".\\res\\fhd_bg.png"));
	SetBackgroundImage(m_main_bk_image);
}

void CCBMPlusDlg::InitPaint()
{
	GetClientRect(m_rect);
	m_dcp.CreateDCP(m_rect.Width(), m_rect.Height());				// ��ü ����
	//m_dcp.Clear(RGB24(255, 255, 255))								// ���� ����
	SetMainBk();					// ����̹���
	m_dcp.SelectObject(_T("����ü"), 14);					// �۲�
}

void CCBMPlusDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDown(nFlags, point);
}

// �α����� ���� ����ȭ�� ����ó��
BOOL CCBMPlusDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CDialogEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	// TODO:  Add your specialized creation code here
	// �α��� �ϱ� ������ ����ó��
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// ���� �������� �Ӽ��� ������
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style | WS_EX_LAYERED);					// LAYERED �Ӽ��� �߰���

	return TRUE;
}

// �α���â ����
void CCBMPlusDlg::DoLoginDialog(UINT message, WPARAM wParam, LPARAM lParam)
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
			// ���̵� ��� ��ĭ
			if ((_tcscmp(id, _T("")) == 0) && (_tcscmp(pw, _T("")) == 0)) {
				CloseLoginDialog();
				LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// ���� �������� �Ӽ��� ������
				::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style & ~WS_EX_LAYERED);					// LAYERED �Ӽ��� ������
				Invalidate(TRUE);
				m_listview1.Invalidate(1);
				m_listview1.UpdateWindow();
			}
			else {
				AfxMessageBox(_T("���̵� �Ǵ� ��й�ȣ�� ��ġ���� �ʽ��ϴ�."));
			}
		}
		else if ((int)wParam == 0) {
			// �α��� ��ҹ�ư
			CloseLoginDialog();
			EndDialog(IDCANCEL);
		}
	}
}

void CCBMPlusDlg::CloseLoginDialog()
{
	if (mp_login_dlg != NULL) {
		mp_login_dlg->DestroyWindow();
		delete mp_login_dlg;
		mp_login_dlg = NULL;
	}
}

// ��ü�� �޾� ����Ʈ ��Ʈ�� �ʱ⼳��
void CCBMPlusDlg::SetListView()
{
	// ����Ʈ��Ʈ�� Font
	mh_font = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_MODERN, _T("����"));
	m_listview1.SendMessage(WM_SETFONT, (WPARAM)mh_font, (LPARAM)TRUE);

	// ����Ʈ ��Ʈ�� �⺻ ����
	LVITEM item;
	::ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	item.pszText = _T("LIG");
	item.iItem = 5;
	item.iImage = 1;
	item.state = LVIS_SELECTED | LVIS_FOCUSED;

	// ���� ��� �߰� �� �÷� �߰�
	m_listview1.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);							// ���� �߰�
	m_listview1.InsertColumn(0, _T("No"), LVCFMT_CENTER, 40);
	m_listview1.InsertColumn(1, _T("�δ��"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(2, _T("����ȣ"), LVCFMT_CENTER, 180);
	m_listview1.InsertColumn(3, _T("�Ϸù�ȣ"), LVCFMT_CENTER, 180);
	m_listview1.InsertColumn(4, _T("ź��"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(5, _T("�µ�"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(6, _T("����"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(7, _T("��ݷ�"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(8, _T("����"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(9, _T("�̻󿩺�"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(10, _T("����������"), LVCFMT_CENTER, 180);
	m_listview1.InsertColumn(11, _T("����"), LVCFMT_CENTER, 60);
	m_listview1.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	// �׸� ���� 30��
	CString strItem = _T("");
	for (int i = 1; i <= 30; i++) {
		strItem.Format(_T("%d"), i);
		m_listview1.InsertItem(i, strItem, i);
	}

	// �� �׸���� ���� �Է� 30row
	for (int i = 0; i < 30; i++) {
		m_listview1.SetItem(i, 1, LVIF_TEXT, _T("�ѱ�"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 2, LVIF_TEXT, _T("1430010876337"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 3, LVIF_TEXT, _T("JN17148851"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 4, LVIF_TEXT, _T("õ��"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 5, LVIF_TEXT, _T("-36.5��"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 6, LVIF_TEXT, _T("20��"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 7, LVIF_TEXT, _T("9.8"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 8, LVIF_TEXT, _T("����"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 9, LVIF_TEXT, _T("�̻�"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 10, LVIF_TEXT, _T("2028-03-21"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 11, LVIF_TEXT, _T("C"), NULL, NULL, NULL, NULL);
	}
}