
// CBMReaderDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CBMReader.h"
#include "CBMReaderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCBMReaderDlg dialog

// ������ ������ ����� ������ ȣ��Ǵ� �Լ�
void MyClientSocket::ConnectedProcess() {
	if (m_is_connected) ((CCBMReaderDlg*)mp_notify_wnd)->AddEventString(L"������ �����߽��ϴ�.");
	else ((CCBMReaderDlg*)mp_notify_wnd)->AddEventString(L"������ �������� ���߽��ϴ�.");
}

// ���ŵ� �����͸� ��� ó���� ������ �����ϴ� �Լ�
int MyClientSocket::ProcessNetMessage()
{
	// ���⿡ ���ŵ� �����͸� ó��
	// m_net_msg_id == 1�̸� ä������. ����Ʈ �ڽ��� ���ڿ� �߰�ó��
	if (m_net_msg_id == 1) {
		// ���ŵ� ���ڿ��� ����Ʈ �ڽ��� ���
		((CCBMReaderDlg*)mp_notify_wnd)->AddEventString((TCHAR*)mp_net_body_data);
	}

	return 1;
}

// ������ ������ ��쿡 ȣ��Ǵ� �Լ�
void MyClientSocket::ClosedProcess(int a_error_flag) {
	if (!a_error_flag) ((CCBMReaderDlg*)mp_notify_wnd)->AddEventString(L"�������� ������ �����߽��ϴ�.");
}

CCBMReaderDlg::CCBMReaderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBMREADER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCBMReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_LIST, m_event_list);
}

BEGIN_MESSAGE_MAP(CCBMReaderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CCBMReaderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCBMReaderDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CONNECT_BTN, &CCBMReaderDlg::OnBnClickedConnectBtn)
	ON_BN_CLICKED(IDC_DISCONNECT_BTN, &CCBMReaderDlg::OnBnClickedDisconnectBtn)
	ON_BN_CLICKED(IDC_GEN_DATA_BTN, &CCBMReaderDlg::OnBnClickedGenDataBtn)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCBMReaderDlg message handlers

BOOL CCBMReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	srand((UINT32)GetTickCount64());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCBMReaderDlg::OnPaint()
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
HCURSOR CCBMReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCBMReaderDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}


void CCBMReaderDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

void CCBMReaderDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

}

void CCBMReaderDlg::AddEventString(const wchar_t* ap_string)
{
	int index = m_event_list.InsertString(-1, ap_string);
	m_event_list.SetCurSel(index);
}

// �޽��� �߻� �� ȣ��Ǵ� �Լ�
LRESULT CCBMReaderDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	// ���� ��ü�� �޽����� ó���� �� �ֵ��� �Լ��� ȣ��(26001, 26002)
	m_client_socket.MessageProc(message, wParam, lParam);

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CCBMReaderDlg::OnBnClickedConnectBtn()
{
	// TODO: Add your control notification handler code here
	if (!m_client_socket.IsConnect()) {			// ���� ���� ���� üũ
		//  ������ ip�� port�� ����ؼ� ������ ���� �õ�
		m_client_socket.ConnectToServer(IP, PORT, this, 26001, 26002);
		AddEventString(L"������ ������ �õ��մϴ�.");
	}
	else {
		// ������ �õ����̰ų� ������ �� ���¶�� ���� �����ش�
		AddEventString(L"������ ������ �õ��ϰ� �ְų� ���ӵ� �����Դϴ�.");
	}
}


void CCBMReaderDlg::OnBnClickedDisconnectBtn()
{
	// TODO: Add your control notification handler code here
		// ������ ������� ���� �� ������ �����ϴ� �۾�
	if (m_client_socket.IsConnect()) {
		m_client_socket.CloseSocket();
		AddEventString(L"������ ������ �����߽��ϴ�.");
	}
	else AddEventString(L"������ ����Ǿ� ���� �ʽ��ϴ�.");
}


void CCBMReaderDlg::OnBnClickedGenDataBtn()
{
	// TODO: Add your control notification handler code here
	SetTimer(1, 1000, NULL);

}



void CCBMReaderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1) {
		char sensor_data[1000], *p = sensor_data;
		*p++ = 10;			// 10��
		for (int i = 0; i < 10; i++) {
			time((time_t*)p);
			p += sizeof(time_t);

			*(float*)p = (float)((rand() % 2000 - 1000) / 10.0);
			p += sizeof(float);

			*p++ = rand() % 101;
		
			*(float*)p = (float)((rand() % 200) / 10.0);			// ��ݰ��ӵ�
			p += sizeof(float);

			*(float*)p = (float)37.56667;			// ����
			p += sizeof(float);

			*(float*)p = (float)126.97806;			// �浵
			p += sizeof(float);
		}
		if (m_client_socket.IsConnect()) {
			m_client_socket.SendFrameData(1, sensor_data, p - sensor_data);

		}
	}



	CDialogEx::OnTimer(nIDEvent);
}
