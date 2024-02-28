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
	// 다이얼로그 화면을 설정한다
	SetBackgroundColor(RGB(50, 70, 90));
	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"맑은 고딕");
	HWND h_find_wnd = NULL;
	while (h_find_wnd = ::FindWindowExW(m_hWnd, h_find_wnd, L"Static", NULL)) {
		::SendMessage(h_find_wnd, WM_SETFONT, (WPARAM)(HFONT)m_font, 1);
	}
	SetWindowPos(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	// 데이터를 조회해서 값을 셋팅한다
	CreateDirectory(L".\\data", NULL);		// 폴더 만들기

	// 소켓 클라이언트를 실행해서 서버와 연결한다

	// 화면 그리기 초기작업을 셋팅한다

	// 컨트롤을 설정한다
	m_data_list.SubclassDlgItem(IDC_DATA_LIST, this);
	GetDlgItem(IDC_TEMP_RECT)->GetWindowRect(m_temp_rect);
	ScreenToClient(m_temp_rect);
	GetDlgItem(IDC_VIB_RECT)->GetWindowRect(m_vib_rect);
	ScreenToClient(m_vib_rect);

	m_tool_bar.AddButton(L"    설정    ", 24006, L".\\images\\setting_icon.png");			// 24006 발생
	m_tool_bar.AddButton(L"  사용자 관리  ", 24003, L".\\images\\user_icon.png");			// 24003 발생
	m_tool_bar.AddButton(L"  CSV파일 불러오기  ", 24004, L".\\images\\csv_icon.png");			// 24004 발생
	m_tool_bar.AddButton(L"  XLSX파일 불러오기  ", 24005, L".\\images\\xlsx_icon.png");			// 24005 발생
	m_tool_bar.AddButton(L"    종료    ", 24002, L".\\images\\exit_icon.png");			// 24002 발생
	
	m_tool_bar.CreateToolBar(IDC_TOOLBAR_RECT, this, 24001);

	// 투명화를 위해 자기자신 다이얼로그에 23000 메시지를 날린다
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
	if (::MessageBox(m_hWnd, _T("프로그램을 종료하시겠습니까?"), _T("프로그램 종료"), MB_OKCANCEL) == IDOK)
	{
		CDialogEx::OnCancel();
	}
}


void CTestServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	// 소켓 제거

	// 그리기 제거
	m_font.DeleteObject();

	// 강제종료시 로그인 다이얼로그 메모리 해제
	CloseLoginDialog();
}


void CTestServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
}

// 로그인을 위한 메인화면 투명처리
BOOL CTestServerDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CDialogEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	// TODO:  Add your specialized creation code here
	// 로그인 하기 전에 투명처리
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// 현재 윈도우의 속성을 가져옴
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style | WS_EX_LAYERED);					// LAYERED 속성을 추가함

	return TRUE;
}

// 메시지 발생 시 호출되는 함수
LRESULT CTestServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (message == 26001) {
		// 상세보기 눌림
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
			// 로그인 확인버튼
			TCHAR* id = mp_login_dlg->GetID();
			TCHAR* pw = mp_login_dlg->GetPW();
			UA account;
			memset(account.user_id, 0, sizeof(account.user_id));
			memset(account.user_pw, 0, sizeof(account.user_pw));
			memcpy(account.user_id, id, (wcslen(id)+1)*sizeof(TCHAR));
			memcpy(account.user_pw, pw, (wcslen(pw) + 1) * sizeof(TCHAR));
			int login_request = m_dbfile.SelectAllUser(account);

			if (login_request == 0) {
				AfxMessageBox(_T("아이디 또는 비밀번호가 일치하지 않습니다."));
			}
			else if(login_request == 1 || TESTER){
				CloseLoginDialog();
				LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// 현재 윈도우의 속성을 가져옴
				::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style & ~WS_EX_LAYERED);					// LAYERED 속성만 제거함
				Invalidate(TRUE);
				//m_listview1.Invalidate(1);
				//m_listview1.UpdateWindow();
			}
			else if(login_request == -1){
				AfxMessageBox(_T("데이터베이스에 접근할 수 없습니다."));
			}
		}
		else if ((int)wParam == 0) {
			// 로그인 취소버튼
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

// 툴바
BOOL CTestServerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == 24002)
	{
		// 툴바 - 종료
		OnBnClickedCancel();
	}
	else if (wParam == 24003) {
		// 툴바 - 사용자 관리
		AfxMessageBox(_T("사용자 계정관리 다이얼로그 실행"));
	}
	else if (wParam == 24004) {
		// 툴바 - CSV파일 불러오기
		AfxMessageBox(_T("선택 파일이 csv파일이면 DB 입력"));

	}
	else if (wParam == 24005) {
		// 툴바 - XLSX파일 불러오기
		AfxMessageBox(_T("선택 파일이 xlsx파일이면 DB 입력"));
	}
	else if (wParam == 24006) {
		// 툴바 - 설정
		AfxMessageBox(_T("설정 다이얼로그 실행"));
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

// 검색 버튼
void CTestServerDlg::OnBnClickedSeachBtn()
{
	// 기존 항목 삭제
	if (m_list_flag != 0) {
		m_data_list.DeleteAllItems();
		for (int i = m_data_list.GetHeaderCtrl()->GetItemCount() - 1; i >= 0; i--)
		{
			m_data_list.DeleteColumn(i);
		}
	}

	// 새로운 검색항목 셋팅
	if (m_list_flag % 2 == 0) {
		m_list_flag++;
		m_data_list.SetData();
	}
	else if (m_list_flag % 2 == 1) {
		m_list_flag++;
		m_data_list.SetData2();
	}
}
