
// CBMReaderDlg.h : header file
//

#pragma once

#include "WinSocketClient.h"
#define IP						_T("127.0.0.1")
#define PORT					10005

class MyClientSocket : public WinSocketClient
{
public:
	virtual void ConnectedProcess();			// ������ ������ ����� ������ ȣ��Ǵ� �Լ�
	virtual int ProcessNetMessage();			// ProcessNetMessage() �Լ��� �������ؼ� ���ŵ� �����͸� ��� ó���� ������ �۾�
	virtual void ClosedProcess(int a_error_flag);			// ������ ������ ��쿡 ȣ��Ǵ� �Լ�
};

// CCBMReaderDlg dialog
class CCBMReaderDlg : public CDialogEx
{
protected:
	MyClientSocket m_client_socket;		// ������ ��ſ� ����� ���� ��ü
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
