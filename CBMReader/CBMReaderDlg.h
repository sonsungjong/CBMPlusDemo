
// CBMReaderDlg.h : header file
//

#pragma once

#include "WinSocketClient.h"
#define IP						_T("127.0.0.1")
#define PORT					10005

class MyClientSocket : public WinSocketClient
{
public:
	virtual void ConnectedProcess();			// 서버에 접속한 결과가 나오면 호출되는 함수
	virtual int ProcessNetMessage();			// ProcessNetMessage() 함수를 재정의해서 수신된 데이터를 어떻게 처리할 것인지 작업
	virtual void ClosedProcess(int a_error_flag);			// 접속이 해제된 경우에 호출되는 함수
};

// CCBMReaderDlg dialog
class CCBMReaderDlg : public CDialogEx
{
protected:
	MyClientSocket m_client_socket;		// 서버와 통신에 사용할 소켓 객체
// Construction
public:
	CCBMReaderDlg(CWnd* pParent = nullptr);	// standard constructor
	void AddEventString(const wchar_t* ap_string);


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBMREADER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	CListBox m_event_list;
public:
	afx_msg void OnBnClickedConnectBtn();
	afx_msg void OnBnClickedDisconnectBtn();
	afx_msg void OnBnClickedGenDataBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
