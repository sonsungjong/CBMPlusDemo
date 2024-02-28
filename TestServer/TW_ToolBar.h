#pragma once

#define MAX_COMMAND_COUNT         24

// 툴바에 사용된 버튼별 정보를 정의한 구조체
struct TipsCommandData {
    wchar_t* p_name;  // 툴바 버튼에 사용될 이름
    int name_len;  // 이름 문자열의 길이 (NULL 문자 제외 길이)
    // 툴바 버튼을 발생했을때 발생한 명령번호
    // ( WM_COMMAND 메시지 사용, WPARAM 에 command_id 가 저장되어 전송됨)
    int command_id;
    int x;  // 버튼의 시작 위치
    int width;  // 버튼의 폭
    CImage* p_image;
};

class TW_ToolBar : public CWnd
{
	DECLARE_DYNAMIC(TW_ToolBar)
private:
    CRect m_rect;  // 툴바의 영역 좌표
    CFont m_tool_font;  // 툴바에서 사용할 글꼴
    TipsCommandData m_btn_list[MAX_COMMAND_COUNT];   // 툴바의 버튼 정보
    // 툴바에 추가된 버튼의 갯수와 선택된 버튼의 위치
    int m_btn_count = 0, m_select_index = -1;
    // 마우스 클릭 여부 (0:클릭안됨, 1:클릭됨, 2:클릭되었지만 버튼 밖에 있음)
    char m_clicked_flag = 0;
    CRect m_select_rect;  // 클릭된 버튼의 영역 정보
    // 툴바의 배경색, 테두리 색상, 내부 테두리 색상, 눌러진 버튼의 색상, 버튼 제목 색상
    COLORREF m_bk_color, m_border_color, m_in_border_color, m_push_btn_color, m_text_color;

public:
	TW_ToolBar();
	virtual ~TW_ToolBar();
    // 툴바를 생성하는 함수
    void CreateToolBar(int a_rect_id, CWnd* ap_parent, int a_ctrl_id);
    // 툴바에 버튼을 추가하는 함수 (CreateToolBar 함수를 호출하기 전에 사용해야 함!!)
    void AddButton(const wchar_t* ap_name, int a_command_id, const wchar_t *ap_png_path);
    // 등록된 버튼 정보를 사용해서 버튼이 위치할 정보를 구성한다.
    void UpdateButtonInfo();
    // 마우스가 위치한 곳에 버튼을 표시하는 함수
    void CheckButtonInToolBar(CPoint point);
    // 버튼이 눌러진 상태를 출력할 함수
    void DrawPushButton(TipsCommandData* ap_btn);
    // 버튼이 눌러졌다가 해제된 상태를 출력할 함수
    void DrawPopButton(TipsCommandData* ap_btn);
    

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    
};


