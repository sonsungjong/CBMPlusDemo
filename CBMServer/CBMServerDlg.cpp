#include "pch.h"
#include "framework.h"
#include "CBMServer.h"
#include "CBMServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// [����]Ŭ���̾�Ʈ ���� �� ȣ��Ǵ� �Լ�
void MyServerSocket::AcceptUserProc(UserData* ap_user_data)
{
	CString str;
	str.Format(L"%s ���� ������ �����߽��ϴ�.", ap_user_data->ip_address);
	((CCBMServerDlg*)mp_notify_wnd)->AddEventString(str);
}

// [����]���ŵ� �����͸� ó���ϴ� �Լ�
int MyServerSocket::ProcessNetMessage()
{
	// m_net_msg_id���� 1�̸� ä�� �����̹Ƿ� ����Ʈ �ڽ��� ���ڿ� ���
	if (m_net_msg_id == 1) {
		((CCBMServerDlg*)mp_notify_wnd)->ProcessingSensorData(mp_net_body_data, m_net_body_size);
	}
	return 1;
}
// [����]Ŭ���̾�Ʈ ������ ������ ��쿡 ȣ��Ǵ� �Լ�
void MyServerSocket::CloseUserProc(UserData* ap_user_data, int a_error_flag)
{
	if (a_error_flag == 0) {			// ������ ������ 0
		CString str;
		str.Format(L"%s���� ������ �����߽��ϴ�.", ap_user_data->ip_address);
		((CCBMServerDlg*)mp_notify_wnd)->AddEventString(str);
	}
}

CCBMServerDlg::CCBMServerDlg(CWnd* pParent /*=nullptr*/)
		: CDialogEx(IDD_CBMSERVER_DIALOG, pParent), m_line_graph(2), m_vib_graph(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCBMServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_LIST, m_event_list);
}

BEGIN_MESSAGE_MAP(CCBMServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCBMServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCBMServerDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SEARCH_BTN, &CCBMServerDlg::OnBnClickedSearchBtn)
	ON_WM_CTLCOLOR()
	ON_WM_NCCREATE()
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_THREE_MONTH_BTN, &CCBMServerDlg::OnBnClickedThreeMonthBtn)
	ON_BN_CLICKED(IDC_SIX_MONTH_BTN, &CCBMServerDlg::OnBnClickedSixMonthBtn)
	ON_BN_CLICKED(IDC_YEAR_BTN, &CCBMServerDlg::OnBnClickedYearBtn)
END_MESSAGE_MAP()


// CCBMServerDlg message handlers

BOOL CCBMServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetBackgroundColor(RGB(50, 70, 90));
	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"���� ���");

	HWND h_find_wnd = NULL;
	while (h_find_wnd = ::FindWindowExW(m_hWnd, h_find_wnd, L"Static", NULL)) {
		::SendMessage(h_find_wnd, WM_SETFONT, (WPARAM)(HFONT)m_font, 1);
	}

	SetWindowPos(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	CreateDirectory(L".\\data", NULL);		// ���� �����

	m_sensor_data_list.SubclassDlgItem(IDC_SENSOR_DATA_LIST, this);			// ����Ʈ�ڽ� ����Ŭ����
	m_search_btn.SubclassDlgItem(IDC_SEARCH_BTN, this);			// ��ư ����Ŭ����
	m_term_btn[0].SubclassDlgItem(IDC_THREE_MONTH_BTN, this);
	m_term_btn[1].SubclassDlgItem(IDC_SIX_MONTH_BTN, this);
	m_term_btn[2].SubclassDlgItem(IDC_YEAR_BTN, this);
	m_data_list.SubclassDlgItem(IDC_DATA_LIST, this);				// ����Ʈ��Ʈ�� ����Ŭ����

	// ��Ʈ�� ����
	((CComboBox*)GetDlgItem(IDC_GRADE_COMBO))->SetCurSel(0);
	OnBnClickedYearBtn();

	// ���� ������ �����Ѵ�
	m_server_socket.StartListenService(IP, PORT, this, 25001, 25002);

	// �׷����� ��µ� ��ġ�� ��´�.
	GetDlgItem(IDC_GRAPH_RECT)->GetWindowRect(m_temp_graph_rect);
	ScreenToClient(m_temp_graph_rect);
	GetDlgItem(IDC_VIB_RECT)->GetWindowRect(m_vib_graph_rect);
	ScreenToClient(m_vib_graph_rect);
	CRect r;
	GetDlgItem(IDC_HEADER_RECT)->GetWindowRect(r);
	ScreenToClient(r);

	// ����׸��� ��Ʈ���� Picture Control ��ǥ��ġ�� ����
	m_header_grid.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, r, this, 32000);
	// 6, 170, 240, 290, 360, 410, };
	m_header_grid.AddItemData(_T("��¥"), 164);
	m_header_grid.AddItemData(_T("�µ�"), 70);
	m_header_grid.AddItemData(_T("����"), 60);
	m_header_grid.AddItemData(_T("����"), 70);
	m_header_grid.AddItemData(_T("����"), 60);
	m_header_grid.AddItemData(_T("�浵"), 60);
	m_header_grid.RedrawItemData();
	m_header_grid.SetBuddyWnd(m_sensor_data_list.m_hWnd);
	
	//m_line_graph.SetD2dCore(&m_d2d_core);						// Direct2D �ھ �Ѱ��ش�. (�ھ�� 1���� ���� �����ؼ� ���� ��ȭ ���ڿ��� �����Ѵ�.)
	m_line_graph.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_temp_graph_rect, this, 29001);					// �� �׷����� �׷��� �����츦 �����Ѵ�.
	//m_vib_graph.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(800, 800, 1200, 1000), this, 29002);					// �� �׷����� �׷��� �����츦 �����Ѵ�.
	m_vib_graph.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_vib_graph_rect, this, 29002);					// �� �׷����� �׷��� �����츦 �����Ѵ�.


	m_sensor_data_list.SetLineGraph(&m_line_graph, &m_vib_graph);
	m_tool_bar.AddButton(L"  ����ź ���� �Է�  ", 24003, L".\\images\\setting_icon.png");			// 24003 �߻�
	m_tool_bar.AddButton(L"      ����� ����      ", 24004, L".\\images\\user_icon.png");			// 24004 �߻�
	m_tool_bar.AddButton(L"  CSV���� �ҷ�����  ", 24005, L".\\images\\csv_icon.png");			// 24005 �߻�
	m_tool_bar.AddButton(L"  XLSX���� �ҷ�����  ", 24006, L".\\images\\xlsx_icon.png");			// 24006 �߻�
	m_tool_bar.AddButton(L"        ����        ", 24002, L".\\images\\exit_icon.png");			// 24002 �߻�
	m_tool_bar.CreateToolBar(IDC_TOOL_RECT, this, 24001);

	// ����Ʈ��Ʈ�� �ӽ� ������ ����
	ListCtrlSearch();
	
	// ����ȭ�� ���� �ڱ��ڽ� ���̾�α׿� 23000 �޽����� ������
	//PostMessage(23000);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCBMServerDlg::OnPaint()
{
	if (IsIconic())	{
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
	} else {
		CDialogEx::OnPaint();
	}
}

HCURSOR CCBMServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCBMServerDlg::OnBnClickedOk()
{
}


void CCBMServerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (::MessageBox(m_hWnd, _T("���α׷��� �����Ͻðڽ��ϱ�?"), _T("���α׷� ����"), MB_OKCANCEL) == IDOK)
	{
		CDialogEx::OnCancel();
	}
}

void CCBMServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_line_graph.DestroyWindow(); // �� �׷����� �����ִ� �����츦 �����Ѵ�.
	m_font.DeleteObject();

	// ��������� �α��� ���̾�α� �޸� ����
	//CloseLoginDialog();
}

LRESULT CCBMServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	// ���� ��ü�� �޽����� ó���� �� �ֵ��� �Լ��� ȣ��
	m_server_socket.MessageProc(message, wParam, lParam);

	if (message == 26000) {
		// ����Ʈ�ڽ� �󼼺���
		//int index = m_sensor_data_list.GetCurSel();
		//if (index != LB_ERR) {
		//	SDF* p_data = (SDF*)m_sensor_data_list.GetItemDataPtr(index);
		//	wchar_t str[128];
		//	if (p_data->type == 1) {
		//		SensorData* p_sensor = (SensorData*)p_data->p_data;
		//		int len = swprintf_s(str, 128, L"%.1f, %d%%, %.1f, %.1f, %.1f", p_sensor->temp, p_sensor->humi, p_sensor->acc, p_sensor->lat, p_sensor->lon);

		//		AfxMessageBox(str);
		//	}
		//}
	}
	else if (message == 26001) {
		// ����Ʈ��Ʈ�� �󼼺��� ����
		int select_item = m_data_list.GetSelectionMark();
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


void CCBMServerDlg::ProcessingSensorData(char* ap_data, unsigned short a_data_size)
{
	m_sensor_data_list.AddSensorData(ap_data, a_data_size, 1);
}

void CCBMServerDlg::ListCtrlSearch()
{
	// ���� �׸� ����
	if (m_listctrl_flag != 0) {
		m_data_list.DeleteAllItems();
		for (int i = m_data_list.GetHeaderCtrl()->GetItemCount() - 1; i >= 0; i--)
		{
			m_data_list.DeleteColumn(i);
		}
	}

	// ���ο� �˻��׸� ����
	if (m_listctrl_flag % 2 == 0) {
		m_listctrl_flag++;
		m_data_list.SetData2();
	}
	else if (m_listctrl_flag % 2 == 1) {
		m_listctrl_flag++;
		m_data_list.SetData();
	}
}

void CCBMServerDlg::OnBnClickedSearchBtn()
{
	// TODO: Add your control notification handler code here

	if (m_listctrl_flag <= 1) {
		// ����Ʈ�ڽ��� �ѹ���
		FILE* p_file = NULL;

		UINT8 count = 0;
		if (0 == fopen_s(&p_file, ".\\data\\cbm.dat", "rb")) {

			char temp[2500];

			while (1 == fread(&count, 1, 1, p_file)) {
				temp[0] = count;
				fread(temp + 1, 25, count, p_file);
				m_sensor_data_list.AddSensorData(temp, 25 * count + 1, 0);
			}

			m_sensor_data_list.AddMiData(time(NULL), 0.5f, 20);
			fclose(p_file);  // ������ �ݴ´�!
		}
	}

	ListCtrlSearch();
}


HBRUSH CCBMServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


BOOL CCBMServerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
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


BOOL CCBMServerDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CDialogEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	// TODO:  Add your specialized creation code here
	// �α��� �ϱ� ���� ����ó��
	//LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// ���� �������� �Ӽ��� ������
	//::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style | WS_EX_LAYERED);					// LAYERED �Ӽ��� �߰���

	return TRUE;
}


void CCBMServerDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	// ����Ʈ��Ʈ���� �ٰ���
	if (IDC_DATA_LIST == nIDCtl) {
		lpMeasureItemStruct->itemHeight += 15;
	}

	CDialogEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CCBMServerDlg::OnBnClickedThreeMonthBtn()
{
	// TODO: Add your control notification handler code here
	m_choice_term = 0;
	m_term_btn[0].ChangeColor(RGB(59, 163, 67), RGB(109, 113, 117), RGB(39, 143, 47), RGB(59, 63, 67), RGB(157, 171, 196), RGB(232, 248, 248));
	m_term_btn[1].ChangeColor(RGB(59, 63, 67), RGB(109, 113, 117), RGB(39, 43, 47), RGB(59, 63, 67), RGB(157, 171, 196), RGB(232, 248, 248));
	m_term_btn[2].ChangeColor(RGB(59, 63, 67), RGB(109, 113, 117), RGB(39, 43, 47), RGB(59, 63, 67), RGB(157, 171, 196), RGB(232, 248, 248));
	Invalidate(FALSE);
}


void CCBMServerDlg::OnBnClickedSixMonthBtn()
{
	// TODO: Add your control notification handler code here
	m_choice_term = 1;
	m_term_btn[1].ChangeColor(RGB(59, 163, 67), RGB(109, 113, 117), RGB(39, 143, 47), RGB(59, 63, 67), RGB(157, 171, 196), RGB(232, 248, 248));
	m_term_btn[0].ChangeColor(RGB(59, 63, 67), RGB(109, 113, 117), RGB(39, 43, 47), RGB(59, 63, 67), RGB(157, 171, 196), RGB(232, 248, 248));
	m_term_btn[2].ChangeColor(RGB(59, 63, 67), RGB(109, 113, 117), RGB(39, 43, 47), RGB(59, 63, 67), RGB(157, 171, 196), RGB(232, 248, 248));
	Invalidate(FALSE);
}


void CCBMServerDlg::OnBnClickedYearBtn()
{
	// TODO: Add your control notification handler code here
	m_choice_term = 2;
	m_term_btn[2].ChangeColor(RGB(59, 163, 67), RGB(109, 113, 117), RGB(39, 143, 47), RGB(59, 63, 67), RGB(157, 171, 196), RGB(232, 248, 248));
	m_term_btn[0].ChangeColor(RGB(59, 63, 67), RGB(109, 113, 117), RGB(39, 43, 47), RGB(59, 63, 67), RGB(157, 171, 196), RGB(232, 248, 248));
	m_term_btn[1].ChangeColor(RGB(59, 63, 67), RGB(109, 113, 117), RGB(39, 43, 47), RGB(59, 63, 67), RGB(157, 171, 196), RGB(232, 248, 248));
	Invalidate(FALSE);
}
