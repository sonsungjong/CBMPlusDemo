
// TestServerDlg.h : header file
//

#pragma once
#include "DBFile.h"
#include "LoginDlg.h"
#include "TW_ToolBar.h"
#include "TW_Button.h"
#include "MT_ListCtrl.h"

typedef std::basic_string<TCHAR> TSTRING;
typedef std::basic_stringstream<TCHAR> TSTRINGSTREAM;

#define IP					L"127.0.0.1"
#define PORT				10005
#define SCREEN_WIDTH				1920
#define SCREEN_HEIGHT			1080

// CTestServerDlg dialog
class CTestServerDlg : public CDialogEx
{
// Construction
public:
	CTestServerDlg(CWnd* pParent = nullptr);	// standard constructor
	void DoLoginDialog(UINT message, WPARAM wParam, LPARAM lParam);
	void CloseLoginDialog();



private:
	// 포함
	LoginDlg* mp_login_dlg = NULL;				// 로그인창
	TW_ToolBar m_tool_bar;
	MT_ListCtrl m_data_list;
	DBFile m_dbfile;

	// 다이얼로그
	CFont m_font;
	CRect m_temp_rect;
	CRect m_vib_rect;

	const wchar_t* m_ipconfig = IP;
	const int m_port = PORT;
	int m_list_flag = 0;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTSERVER_DIALOG };
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedSeachBtn();
};
