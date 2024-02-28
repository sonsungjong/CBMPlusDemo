
#pragma once
//#include "DBFile.h"
#include "WinSocketServer.h"
#include "HeaderGrid.h"
#include "TW_Direct2D.h"
#include "SensorDataList.h"
#include "LineGraphWnd.h"
#include "VibGraphWnd.h"
#include "TW_Button.h"
#include "TW_ToolBar.h"
#include "MT_ListCtrl.h"

//#define MAX_USER_COUNT			2000		// 서버에 접속 가능한 최대 클라이언트 수
#define IP				L"127.0.0.1"
#define PORT				10005
#define SCREEN_WIDTH				1920
#define SCREEN_HEIGHT			1080

class MyServerSocket : public WinSocketServer
{
	// 오버라이딩
public:
	// 새로운 클라이언트가 접속되었을 때 호출
	virtual void AcceptUserProc(UserData* ap_user_data);
	// 메시지 수신 후 작업할 공간
	virtual int ProcessNetMessage();
	// 클라이언트 접속 해제
	virtual void CloseUserProc(UserData* ap_user_data, int a_error_flag);
};

// CCBMServerDlg dialog
class CCBMServerDlg : public CDialogEx
{
private:
	TWD_Core m_d2d_core;
	HeaderGrid m_header_grid;
	MyServerSocket m_server_socket;			// 서버의 소켓 객체 선언
	SensorDataListBox m_sensor_data_list;
	LineGraphWnd m_line_graph, m_vib_graph;
	// VibGraphWnd m_vib_graph;
	MT_ListCtrl m_data_list;

	CRect m_temp_graph_rect;
	CRect m_vib_graph_rect;

	TW_Button m_search_btn;
	TW_Button m_term_btn[3];
	unsigned char m_choice_term = 2;

	TW_ToolBar m_tool_bar;
	CFont m_font;

	int m_listctrl_flag = 0;

protected:
	const wchar_t* ipconfig = IP;		// 내 IP
	const int thisPort = PORT;										// 해당 port

// Construction
public:
	CCBMServerDlg(CWnd* pParent = nullptr);	// standard constructor

	template<class Interface> void TW_IRelease(Interface** ap_interface_object)
	{
		if (*ap_interface_object != NULL) {
			(*ap_interface_object)->Release();
			(*ap_interface_object) = NULL;
		}
	}

	// 리스트 박스에 이벤트 로그 추가
	void AddEventString(const wchar_t* ap_string)
	{
		int index = m_event_list.InsertString(-1, ap_string);
		m_event_list.SetCurSel(index);
	}

	void ProcessingSensorData(char* ap_data, unsigned short a_data_size);
	void ListCtrlSearch();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBMSERVER_DIALOG };
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



public:
	int ReceiveData(SOCKET ah_socket, char* ap_data, int a_size);
private:
	CListBox m_event_list;
public:
	afx_msg void OnBnClickedSearchBtn();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnBnClickedThreeMonthBtn();
	afx_msg void OnBnClickedSixMonthBtn();
	afx_msg void OnBnClickedYearBtn();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
