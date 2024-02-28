#pragma once
#define SCREEN_WIDTH			1920
#define SCREEN_HEIGHT		1080
#define SELECT_LISTCTRL_ROW				22001

#include "EnvironmentModel.h"
#include "DBFile.h"
#include "WinSocketServer.h"
#include "TW_ToolBar.h"
#include "TW_Button.h"
#include "TW_ListBox.h"
#include "MT_ListCtrl.h"

class MyServerSocket : public WinSocketServer
{
	// �������̵�
public:
	// ���ο� Ŭ���̾�Ʈ�� ���ӵǾ��� �� ȣ��
	virtual void AcceptUserProc(UserData* ap_user_data);
	// �޽��� ���� �� �۾��� ����
	virtual int ProcessNetMessage();
	// Ŭ���̾�Ʈ ���� ����
	virtual void CloseUserProc(UserData* ap_user_data, int a_error_flag);
};

// CCBMBackendDlg dialog
class CCBMBackendDlg : public CDialogEx
{
private:
	EnvironmentModel* mp_environment;
	DBFile m_dbfile;

	MyServerSocket m_server_socket;			// ���� ����
	MT_ListCtrl m_data_listctrl;							// ����Ʈ ��Ʈ��

	CRect m_temp_humi_graph;				// �½��� �׷��� ����
	CRect m_vib_graph;							// �������ӵ� �׷��� ����
	int m_move = 0;

	TW_ToolBar m_tool_bar;
	TW_Button m_search_btn;
	TW_Button m_term_btn[3];
	unsigned char m_choice_term = 2;			// �⺻���� 1��
	CFont m_font;

	int m_listctrl_flag = 0;
	const wchar_t* ipconfig = L"127.0.0.1";
	const int thisPort = 10005;

// Construction
public:
	CCBMBackendDlg(CWnd* pParent = nullptr);	// standard constructor

	void ListCtrlSearch();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBMBACKEND_DIALOG };
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
	afx_msg void OnBnClickedSearchBtn();

public:
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
