#ifndef CBM_PLUS_H_
#define CBM_PLUS_H_

#include "DCP.h"
#include "SocketClass.h"
#include "LoginDlg.h"

#define SCREEN_WIDTH					1920
#define SCREEN_HEIGHT				1080

class CCBMPlusDlg : public CDialogEx
{
private:
	// 객체화
	DCP m_dcp;
	SocketClass m_client_socket;
	LoginDlg* mp_login_dlg = NULL;
	CImage m_main_bk_image;

	// 메시지박스 출력을 위한 임시 객체
	CString m_msg_content;
	CString m_msg_caption;
	INT m_timer_count = 0;

	// 범위
	CRect m_rect;				// 전체화면 크기
	CRect m_title_rect;
	CRect m_title_menu1;
	CRect m_title_menu2;
	CRect m_title_menu3;

	CRect m_text_rect;
	CRect m_graph1;
	CRect m_graph2;
	HFONT mh_font;

	// GUI
	DOUBLE m_pi = 3.141592;
	CHAR m_is_clicked = 0;				// 마우스 클릭 여부

	// TCP 통신

// Construction
public:
	CCBMPlusDlg(CWnd* pParent = nullptr);	// standard constructor
	void MyPaint();
	void InitPaint();
	void Connect2Server();
	void SetMainBk();

	// 로그인 및 투명처리
	void DoLoginDialog(UINT message, WPARAM wParam, LPARAM lParam);
	void CloseLoginDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBMPLUS_DIALOG };
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	void SetListView();
private:
	CListCtrl m_listview1;
};

#endif