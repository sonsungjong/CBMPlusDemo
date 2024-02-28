#include "pch.h"
#include "framework.h"
#include "CBMPlus.h"
#include "CBMPlusDlg.h"
#include "afxdialogex.h"



CCBMPlusDlg::CCBMPlusDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBMPLUS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	// 객체 초기화
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

	// MFC 공간 범위 설정
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

	// 데이터베이스를 조회해서 객체의 값을 설정한다.

	// 소켓 클라이언트를 실행해 서버와 연결한다.
	// InitSocket();

	// 객체를 통해 그리기 초기설정
	InitPaint();
	MyPaint();				// 객체값을 받아 그린다

	// 객체를 통해 리스트 컨트롤(그리드)를 넣는다
	SetListView();

	// Full HD 크기 조정
	SetWindowPos(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);				//ShowWindow(SW_MAXIMIZE)

	// 투명화를 위해 자기자신 다이얼로그에 23000 메시지를 날린다
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
		// 출력한다.
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
	if (::MessageBox(m_hWnd, _T("프로그램을 종료하시겠습니까?"), _T("프로그램 종료"), MB_OKCANCEL) == IDOK)
	{
		CDialogEx::OnCancel();
	}
}


void CCBMPlusDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	if (m_client_socket.IsConnect() != 0) {
		m_client_socket.CloseSocket();				// 소켓 제거
	}
	//KillTimer(1)
	// 강제종료시 로그인 다이얼로그 메모리 해제
	CloseLoginDialog();
}

// 메시지 발생 시 호출되는 함수
LRESULT CCBMPlusDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	// 소켓 객체가 메시지를 처리할 수 있도록 함수를 호출(26001, 26002)
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
	m_dcp.FillSolidRect(m_title_rect.left, m_title_rect.top, m_title_rect.right, m_title_rect.bottom, RGB24(56, 56, 56));					// 타이틀 사각형
	m_dcp.Draw(_T("./res/bar_graph.png"), m_graph1.left, m_graph1.top, m_graph1.Width(), m_graph1.Height());
	m_dcp.Draw(_T("./res/line_graph.png"), m_graph2.left, m_graph2.top, m_graph2.Width(), m_graph2.Height());
	
	m_dcp.Draw(_T("./res/button27.png"), m_title_menu1.left, m_title_menu1.top, m_title_menu1.Width(), m_title_menu1.Height());
	m_dcp.Draw(_T("./res/button27.png"), m_title_menu2.left, m_title_menu2.top, m_title_menu2.Width(), m_title_menu2.Height());
	m_dcp.Draw(_T("./res/button27.png"), m_title_menu3.left, m_title_menu3.top, m_title_menu3.Width(), m_title_menu3.Height());

	m_dcp.GpTextOut(m_title_menu1.left+4, m_title_menu1.top+4, RGB24(255, 255, 255), _T("사용자 관리"), 6);
	m_dcp.GpTextOut(m_title_menu2.left+4, m_title_menu2.top+4, RGB24(255, 255, 255), _T("데이터 입력"), 6);
	m_dcp.GpTextOut(m_title_menu3.left+4, m_title_menu3.top+4, RGB24(210, 0, 0), _T("프로그램 종료"), 7);
	
	m_dcp.PieChart(10, 10, 200, 200, 0.7);
}


void CCBMPlusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1) {
		AfxMessageBox(_T("테스트 타이머"));
		m_timer_count++;
		if (m_timer_count > 3) {
			KillTimer(1);
			m_timer_count = 0;
		}
	}
	else if (nIDEvent == 1000) {
		// GUI 연결상태 전송
		if (m_client_socket.IsConnect() != 0) {
			int result = 0;
			TSTRING tstr = _T("sj");
			LONG64 data_size = tstr.length();
			if (data_size > 0) {
				result = m_client_socket.SendFrameData(1, const_cast<const TCHAR*>(tstr.c_str()), (data_size + 1) * sizeof(TCHAR));
				if (result == SOCKET_ERROR) {
					AfxMessageBox(_T("소켓 전송 실패"));
				}
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CCBMPlusDlg::Connect2Server()
{
	// 서버소켓에 연결
	if (m_client_socket.IsConnect() == 0) {
		m_client_socket.ConnectToServer(_T("127.0.0.1"), 65384, this, 26001, 26002);
	}
	else {
		AfxMessageBox(_T("서버에 이미 접속을 시도하고 있거나 접속된 상태입니다."));
	}
}

// 메인화면 배경이미지
void CCBMPlusDlg::SetMainBk()
{
	m_main_bk_image.Load(_T(".\\res\\fhd_bg.png"));
	SetBackgroundImage(m_main_bk_image);
}

void CCBMPlusDlg::InitPaint()
{
	GetClientRect(m_rect);
	m_dcp.CreateDCP(m_rect.Width(), m_rect.Height());				// 객체 생성
	//m_dcp.Clear(RGB24(255, 255, 255))								// 배경색 덮기
	SetMainBk();					// 배경이미지
	m_dcp.SelectObject(_T("굴림체"), 14);					// 글꼴
}

void CCBMPlusDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDown(nFlags, point);
}

// 로그인을 위한 메인화면 투명처리
BOOL CCBMPlusDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CDialogEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	// TODO:  Add your specialized creation code here
	// 로그인 하기 전까지 투명처리
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// 현재 윈도우의 속성을 가져옴
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style | WS_EX_LAYERED);					// LAYERED 속성을 추가함

	return TRUE;
}

// 로그인창 생성
void CCBMPlusDlg::DoLoginDialog(UINT message, WPARAM wParam, LPARAM lParam)
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
			// 아이디 비번 빈칸
			if ((_tcscmp(id, _T("")) == 0) && (_tcscmp(pw, _T("")) == 0)) {
				CloseLoginDialog();
				LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);									// 현재 윈도우의 속성을 가져옴
				::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style & ~WS_EX_LAYERED);					// LAYERED 속성만 제거함
				Invalidate(TRUE);
				m_listview1.Invalidate(1);
				m_listview1.UpdateWindow();
			}
			else {
				AfxMessageBox(_T("아이디 또는 비밀번호가 일치하지 않습니다."));
			}
		}
		else if ((int)wParam == 0) {
			// 로그인 취소버튼
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

// 객체를 받아 리스트 컨트롤 초기설정
void CCBMPlusDlg::SetListView()
{
	// 리스트컨트롤 Font
	mh_font = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_MODERN, _T("굴림"));
	m_listview1.SendMessage(WM_SETFONT, (WPARAM)mh_font, (LPARAM)TRUE);

	// 리스트 컨트롤 기본 설정
	LVITEM item;
	::ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	item.pszText = _T("LIG");
	item.iItem = 5;
	item.iImage = 1;
	item.state = LVIS_SELECTED | LVIS_FOCUSED;

	// 격자 모양 추가 후 컬럼 추가
	m_listview1.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);							// 격자 추가
	m_listview1.InsertColumn(0, _T("No"), LVCFMT_CENTER, 40);
	m_listview1.InsertColumn(1, _T("부대명"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(2, _T("재고번호"), LVCFMT_CENTER, 180);
	m_listview1.InsertColumn(3, _T("일련번호"), LVCFMT_CENTER, 180);
	m_listview1.InsertColumn(4, _T("탄종"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(5, _T("온도"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(6, _T("습도"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(7, _T("충격량"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(8, _T("지역"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(9, _T("이상여부"), LVCFMT_CENTER, 120);
	m_listview1.InsertColumn(10, _T("권장점검일"), LVCFMT_CENTER, 180);
	m_listview1.InsertColumn(11, _T("상태"), LVCFMT_CENTER, 60);
	m_listview1.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	// 항목 갯수 30개
	CString strItem = _T("");
	for (int i = 1; i <= 30; i++) {
		strItem.Format(_T("%d"), i);
		m_listview1.InsertItem(i, strItem, i);
	}

	// 각 항목들의 정보 입력 30row
	for (int i = 0; i < 30; i++) {
		m_listview1.SetItem(i, 1, LVIF_TEXT, _T("한국"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 2, LVIF_TEXT, _T("1430010876337"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 3, LVIF_TEXT, _T("JN17148851"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 4, LVIF_TEXT, _T("천궁"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 5, LVIF_TEXT, _T("-36.5℃"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 6, LVIF_TEXT, _T("20℃"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 7, LVIF_TEXT, _T("9.8"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 8, LVIF_TEXT, _T("서울"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 9, LVIF_TEXT, _T("이상"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 10, LVIF_TEXT, _T("2028-03-21"), NULL, NULL, NULL, NULL);
		m_listview1.SetItem(i, 11, LVIF_TEXT, _T("C"), NULL, NULL, NULL, NULL);
	}
}