#pragma once


// TW_Button

class TW_Button : public CButton
{
	DECLARE_DYNAMIC(TW_Button)
private:
	// ��Ŀ���� ��ư��, ��Ŀ���� �׵θ� ��, �Ϲ� ��ư��, �Ϲ� �׵θ� ��
	COLORREF m_focus_btn_color, m_focus_border_color, m_btn_color, m_border_color;
	// ��ư ���� �׵θ���, ��ư �ؽ�Ʈ ����
	COLORREF m_in_border_color, m_text_color;
public:
	TW_Button();
	virtual ~TW_Button();

	// ��ư�� ������ �����ϴ� �Լ�
	void ChangeColor(COLORREF a_focus_btn_color, COLORREF a_focus_border_color,
		COLORREF a_btn_color, COLORREF a_border_color,
		COLORREF a_in_border_color, COLORREF a_text_color);
	
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpds);
};


