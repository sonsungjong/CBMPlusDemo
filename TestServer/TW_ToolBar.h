#pragma once

#define MAX_COMMAND_COUNT         24

// ���ٿ� ���� ��ư�� ������ ������ ����ü
struct TipsCommandData {
    wchar_t* p_name;  // ���� ��ư�� ���� �̸�
    int name_len;  // �̸� ���ڿ��� ���� (NULL ���� ���� ����)
    // ���� ��ư�� �߻������� �߻��� ��ɹ�ȣ
    // ( WM_COMMAND �޽��� ���, WPARAM �� command_id �� ����Ǿ� ���۵�)
    int command_id;
    int x;  // ��ư�� ���� ��ġ
    int width;  // ��ư�� ��
    CImage* p_image;
};

class TW_ToolBar : public CWnd
{
	DECLARE_DYNAMIC(TW_ToolBar)
private:
    CRect m_rect;  // ������ ���� ��ǥ
    CFont m_tool_font;  // ���ٿ��� ����� �۲�
    TipsCommandData m_btn_list[MAX_COMMAND_COUNT];   // ������ ��ư ����
    // ���ٿ� �߰��� ��ư�� ������ ���õ� ��ư�� ��ġ
    int m_btn_count = 0, m_select_index = -1;
    // ���콺 Ŭ�� ���� (0:Ŭ���ȵ�, 1:Ŭ����, 2:Ŭ���Ǿ����� ��ư �ۿ� ����)
    char m_clicked_flag = 0;
    CRect m_select_rect;  // Ŭ���� ��ư�� ���� ����
    // ������ ����, �׵θ� ����, ���� �׵θ� ����, ������ ��ư�� ����, ��ư ���� ����
    COLORREF m_bk_color, m_border_color, m_in_border_color, m_push_btn_color, m_text_color;

public:
	TW_ToolBar();
	virtual ~TW_ToolBar();
    // ���ٸ� �����ϴ� �Լ�
    void CreateToolBar(int a_rect_id, CWnd* ap_parent, int a_ctrl_id);
    // ���ٿ� ��ư�� �߰��ϴ� �Լ� (CreateToolBar �Լ��� ȣ���ϱ� ���� ����ؾ� ��!!)
    void AddButton(const wchar_t* ap_name, int a_command_id, const wchar_t *ap_png_path);
    // ��ϵ� ��ư ������ ����ؼ� ��ư�� ��ġ�� ������ �����Ѵ�.
    void UpdateButtonInfo();
    // ���콺�� ��ġ�� ���� ��ư�� ǥ���ϴ� �Լ�
    void CheckButtonInToolBar(CPoint point);
    // ��ư�� ������ ���¸� ����� �Լ�
    void DrawPushButton(TipsCommandData* ap_btn);
    // ��ư�� �������ٰ� ������ ���¸� ����� �Լ�
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


