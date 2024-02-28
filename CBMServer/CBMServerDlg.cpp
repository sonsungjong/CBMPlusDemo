#include "pch.h"
#include "framework.h"
#include "CBMServer.h"
#include "CBMServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// [소켓]클라이언트 접속 시 호출되는 함수
void MyServerSocket::AcceptUserProc(UserData* ap_user_data)
{
	CString str;
	str.Format(L"%s 에서 서버에 접속했습니다.", ap_user_data->ip_address);
	((CCBMServerDlg*)mp_notify_wnd)->AddEventString(str);
}

// [소켓]수신된 데이터를 처리하는 함수
int MyServerSocket::ProcessNetMessage()
{
	// m_net_msg_id값이 1이면 채팅 정보이므로 리스트 박스에 문자열 출력
	if (m_net_msg_id == 1) {
		((CCBMServerDlg*)mp_notify_wnd)->ProcessingSensorData(mp_net_body_data, m_net_body_size);
	}
	return 1;
}
// [소켓]클라이언트 접속이 해제된 경우에 호출되는 함수
void MyServerSocket::CloseUserProc(UserData* ap_user_data, int a_error_flag)
{
	if (a_error_flag == 0) {			// 에러가 없을때 0
		CString str;
		str.Format(L"%s에서 접속을 해제했습니다.", ap_user_data->ip_address);
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
		DEFAULT_PITCH | FF_SWISS, L"맑은 고딕");

	HWND h_find_wnd = NULL;
	while (h_find_wnd = ::FindWindowExW(m_hWnd, h_find_wnd, L"Static", NULL)) {
		::SendMessage(h_find_wnd, WM_SETFONT, (WPARAM)(HFONT)m_font, 1);
	}

	SetWindowPos(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	CreateDirectory(L".\\data", NULL);		// 폴더 만들기

	m_sensor_data_list.SubclassDlgItem(IDC_SENSOR_DATA_LIST, this);			// 리스트박스 서브클래싱
	m_search_btn.SubclassDlgItem(IDC_SEARCH_BTN, this);			// 버튼 서브클래싱
	m_term_btn[0].SubclassDlgItem(IDC_THREE_MONTH_BTN, this);
	m_term_btn[1].SubclassDlgItem(IDC_SIX_MONTH_BTN, this);
	m_term_btn[2].SubclassDlgItem(IDC_YEAR_BTN, this);
	m_data_list.SubclassDlgItem(IDC_DATA_LIST, this);				// 리스트컨트롤 서브클래싱

	// 컨트롤 설정
	((CComboBox*)GetDlgItem(IDC_GRADE_COMBO))->SetCurSel(0);
	OnBnClickedYearBtn();

	// 소켓 서버를 실행한다
	m_server_socket.StartListenService(IP, PORT, this, 25001, 25002);

	// 그래프가 출력될 위치를 얻는다.
	GetDlgItem(IDC_GRAPH_RECT)->GetWindowRect(m_temp_graph_rect);
	ScreenToClient(m_temp_graph_rect);
	GetDlgItem(IDC_VIB_RECT)->GetWindowRect(m_vib_graph_rect);
	ScreenToClient(m_vib_graph_rect);
	CRect r;
	GetDlgItem(IDC_HEADER_RECT)->GetWindowRect(r);
	ScreenToClient(r);

	// 헤더그리드 컨트롤을 Picture Control 좌표위치에 생성
	m_header_grid.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, r, this, 32000);
	// 6, 170, 240, 290, 360, 410, };
	m_header_grid.AddItemData(_T("날짜"), 164);
	m_header_grid.AddItemData(_T("온도"), 70);
	m_header_grid.AddItemData(_T("습도"), 60);
	m_header_grid.AddItemData(_T("진동"), 70);
	m_header_grid.AddItemData(_T("위도"), 60);
	m_header_grid.AddItemData(_T("경도"), 60);
	m_header_grid.RedrawItemData();
	m_header_grid.SetBuddyWnd(m_sensor_data_list.m_hWnd);
	
	//m_line_graph.SetD2dCore(&m_d2d_core);						// Direct2D 코어를 넘겨준다. (코어는 1개만 생성 가능해서 메인 대화 상자에서 생성한다.)
	m_line_graph.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_temp_graph_rect, this, 29001);					// 선 그래프를 그려줄 윈도우를 생성한다.
	//m_vib_graph.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(800, 800, 1200, 1000), this, 29002);					// 선 그래프를 그려줄 윈도우를 생성한다.
	m_vib_graph.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_vib_graph_rect, this, 29002);					// 선 그래프를 그려줄 윈도우를 생성한다.


	m_sensor_data_list.SetLineGraph(&m_line_graph, &m_vib_graph);
	m_tool_bar.AddButton(L"  유도탄 정보 입력  ", 24003, L".\\images\\setting_icon.png");			// 24003 발생
	m_tool_bar.AddButton(L"      사용자 관리      ", 24004, L".\\images\\user_icon.png");			// 24004 발생
	m_tool_bar.AddButton(L"  CSV파일 불러오기  ", 24005, L".\\images\\csv_icon.png");			// 24005 발생
	m_tool_bar.AddButton(L"  XLSX파일 불러오기  ", 24006, L".\\images\\xlsx_icon.png");			// 24006 발생
	m_tool_bar.AddButton(L"        종료        ", 24002, L".\\images\\exit_icon.png");			// 24002 발생
	m_tool_bar.CreateToolBar(IDC_TOOL_RECT, this, 24001);

	// 리스트컨트롤 임시 데이터 삽입
	ListCtrlSearch();
	
	// 투명화를 위해 자기자신 다이얼로그에 23000 메시지를 날린다
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
	if (::MessageBox(m_hWnd, _T("프로그램을 종료하시겠습니까?"), _T("프로그램 종료"), MB_OKCANCEL) == IDOK)
	{
		CDialogEx::OnCancel();
	}
}

void CCBMServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_line_graph.DestroyWindow(); // 선 그래프를 보여주는 윈도우를 제거한다.
	m_font.DeleteObject();

	// 강제종료시 로그인 다이얼로그 메모리 해제
	//CloseLoginDialog();
}

LRESULT CCBMServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	// 소켓 객체가 메시지를 처리할 수 있도록 함수를 호출
	m_server_socket.MessageProc(message, wParam, lParam);

	if (message == 26000) {
		// 리스트박스 상세보기
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
		// 리스트컨트롤 상세보기 눌림
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
	// 기존 항목 삭제
	if (m_listctrl_flag != 0) {
		m_data_list.DeleteAllItems();
		for (int i = m_data_list.GetHeaderCtrl()->GetItemCount() - 1; i >= 0; i--)
		{
			m_data_list.DeleteColumn(i);
		}
	}

	// 새로운 검색항목 셋팅
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
		// 리스트박스는 한번만
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
			fclose(p_file);  // 파일을 닫는다!
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
		// 툴바 종료버튼
		OnBnClickedCancel();
	}
	else if (wParam == 24003) {
		MessageBox(_T("유도탄 정보 입력"));
	}
	else if (wParam == 24004) {
		MessageBox(_T("사용자 관리"));
	}
	else if (wParam == 24005) {
		MessageBox(_T("CSV파일 불러오기"));
	}
	else if (wParam == 24006) {
		MessageBox(_T("XLSX파일 불러오기"));
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


BOOL CCBMServerDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CDialogEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	// TODO:  Add your specialized creation code here
	// 로그인 하기 전에 투명처리
	//LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// 현재 윈도우의 속성을 가져옴
	//::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style | WS_EX_LAYERED);					// LAYERED 속성을 추가함

	return TRUE;
}


void CCBMServerDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	// 리스트컨트롤의 줄간격
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
