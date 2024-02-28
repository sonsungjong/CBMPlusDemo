
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

// [소켓]클라이언트 접속 시 호출되는 함수
void MyServerSocket::AcceptUserProc(UserData* ap_user_data)
{
	//CString str;
	//str.Format(L"%s 에서 서버에 접속했습니다.", ap_user_data->ip_address);
	//((CBMBackendDlg*)mp_notify_wnd)->AddEventString(str);
}

// [소켓]수신된 데이터를 처리하는 함수
int MyServerSocket::ProcessNetMessage()
{
	// m_net_msg_id값이 1이면 채팅 정보이므로 리스트 박스에 문자열 출력
	if (m_net_msg_id == 1) {
		//((CBMBackendDlg*)mp_notify_wnd)->ProcessingSensorData(mp_net_body_data, m_net_body_size);
	}
	return 1;
}
// [소켓]클라이언트 접속이 해제된 경우에 호출되는 함수
void MyServerSocket::CloseUserProc(UserData* ap_user_data, int a_error_flag)
{
	if (a_error_flag == 0) {			// 에러가 없을때 0
		CString str;
		str.Format(L"%s에서 접속을 해제했습니다.", ap_user_data->ip_address);
		//((CBMBackendDlg*)mp_notify_wnd)->AddEventString(str);
	}
}


CCBMBackendDlg::CCBMBackendDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBMBACKEND_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	mp_environment = new EnvironmentModel[200000];			// 센서 데이터 총 20만개까지 저장가능
	m_dbfile.GetEnvironmentData(mp_environment);


}

void CCBMBackendDlg::ListCtrlSearch()
{
	// 기존 항목 삭제
	if (m_listctrl_flag != 0) {
		m_data_listctrl.DeleteAllItems();				// 모든 아이템 삭제
		for (int i = m_data_listctrl.GetHeaderCtrl()->GetItemCount() - 1; i >= 0; i--)
		{
			m_data_listctrl.DeleteColumn(i);				// 모든 데이터 삭제
		}
	}

	// 새로운 검색항목 셋팅
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
		DEFAULT_PITCH | FF_SWISS, L"맑은 고딕");

	// 네트워크 실행
	SetWindowPos(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	HWND h_find_wnd = NULL;
	while (h_find_wnd = ::FindWindowExW(m_hWnd, h_find_wnd, L"Static", NULL)) {
		::SendMessage(h_find_wnd, WM_SETFONT, (WPARAM)(HFONT)m_font, 1);
	}

	GetDlgItem(IDC_TEMP_HUMI_RECT)->GetWindowRect(m_temp_humi_graph);
	ScreenToClient(m_temp_humi_graph);

	CreateDirectory(L".\\data", NULL);		// 폴더 만들기

	// 서브클래싱
	m_data_listctrl.SubclassDlgItem(IDC_DATA_LIST, this);				// 중앙 리스트 컨트롤
	m_search_btn.SubclassDlgItem(IDC_SEARCH_BTN, this);			// 검색 버튼
	

	// 상단 툴바
	m_tool_bar.AddButton(L"  유도탄 정보 입력  ", 24003, L".\\images\\setting_icon.png");			// 24003 발생
	m_tool_bar.AddButton(L"      사용자 관리      ", 24004, L".\\images\\user_icon.png");			// 24004 발생
	m_tool_bar.AddButton(L"  CSV파일 불러오기  ", 24005, L".\\images\\csv_icon.png");			// 24005 발생
	m_tool_bar.AddButton(L"  XLSX파일 불러오기  ", 24006, L".\\images\\xlsx_icon.png");			// 24006 발생
	m_tool_bar.AddButton(L"        종료        ", 24002, L".\\images\\exit_icon.png");			// 24002 발생
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
	if (::MessageBox(m_hWnd, _T("프로그램을 종료하시겠습니까?"), _T("프로그램 종료"), MB_OKCANCEL) == IDOK)
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

	// 폰트 제거
	m_font.DeleteObject();

	// 강제종료시 다이얼로그 메모리 해제
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


LRESULT CCBMBackendDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == 26001) {
		// 리스트컨트롤 상세보기 눌림
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
		KillTimer(SELECT_LISTCTRL_ROW);				// 일회용 타이머

		// ToDo임시코드 : 이 부분을 그래프 그리는 코드로 바꾸면됨
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
