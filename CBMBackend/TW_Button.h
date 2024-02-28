#pragma once


// TW_Button

class TW_Button : public CButton
{
	DECLARE_DYNAMIC(TW_Button)
private:
	// 포커스된 버튼색, 포커스된 테두리 색, 일반 버튼색, 일반 테두리 색
	COLORREF m_focus_btn_color, m_focus_border_color, m_btn_color, m_border_color;
	// 버튼 내부 테두리색, 버튼 텍스트 색상
	COLORREF m_in_border_color, m_text_color;
public:
	TW_Button();
	virtual ~TW_Button();

	// 버튼의 색상을 변경하는 함수
	void ChangeColor(COLORREF a_focus_btn_color, COLORREF a_focus_border_color,
		COLORREF a_btn_color, COLORREF a_border_color,
		COLORREF a_in_border_color, COLORREF a_text_color);
	
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpds);
};


