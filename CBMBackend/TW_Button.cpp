// TW_Button.cpp : implementation file
//

#include "pch.h"
#include "CBMBackendDlg.h"
#include "TW_Button.h"


// TW_Button

IMPLEMENT_DYNAMIC(TW_Button, CButton)

TW_Button::TW_Button()
{
    m_focus_btn_color = RGB(59, 63, 67);
    m_focus_border_color = RGB(109, 113, 117);
    m_btn_color = RGB(39, 43, 47);
    m_border_color = RGB(59, 63, 67);
    m_in_border_color = RGB(157, 171, 196);
    m_text_color = RGB(232, 248, 248);
    
}

TW_Button::~TW_Button()
{
}


BEGIN_MESSAGE_MAP(TW_Button, CButton)
END_MESSAGE_MAP()



// TW_Button message handlers


// TW_Button 메시지 처리기
void TW_Button::DrawItem(LPDRAWITEMSTRUCT lpds)
{
    CRect r(lpds->rcItem);  // RECT 구조체를 CRect 객체로 변환
    CDC* p_dc = CDC::FromHandle(lpds->hDC); // HDC 핸들을 CDC 객체로 변환

    p_dc->SelectStockObject(DC_BRUSH);  // 색상만 변경해서 사용하는 Brush 설정
    p_dc->SelectStockObject(DC_PEN);   // 색상만 변경해서 사용하는 Pen 설정

    if (lpds->itemState & ODS_FOCUS) {  // 포커스 상태인 경우
        p_dc->SetDCBrushColor(m_focus_btn_color);  // Brush 색상 설정
        p_dc->SetDCPenColor(m_focus_border_color);   // Pen 색상 설정
    }
    else {  // 포커스 상태가 아닌 경우
        p_dc->SetDCBrushColor(m_btn_color);  // Brush 색상 설정
        p_dc->SetDCPenColor(m_border_color);   // Pen 색상 설정
    }
    p_dc->Rectangle(r);   // 위에서 설정한 Pen과 Brush로 사각형을 그린다.

    p_dc->SelectStockObject(NULL_BRUSH);  // Brush는 사용 안 함으로 설정
    p_dc->SetDCPenColor(m_in_border_color);  // Pen 색상 설정
    // 버튼의 내부 색상을 그린다.
    p_dc->Rectangle(r.left + 1, r.top + 1, r.right - 1, r.bottom - 1);

    // 버튼이 눌러졌다면 버튼의 텍스트를 살짝 아래쪽으로 이동시켜서 눌러진 느낌이 나게한다.
    if (lpds->itemState & ODS_SELECTED) r += CPoint(1, 1);

    CString str;
    GetWindowText(str);  // 버튼의 텍스트를 얻는다.
    p_dc->SetBkMode(TRANSPARENT);   // 텍스트의 배경은 투명하게 처리한다.
    p_dc->SetTextColor(m_text_color);   // 텍스트의 색상을 설정한다.
    p_dc->DrawText(str, r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);  // 텍스트를 출력한다.
}

void TW_Button::ChangeColor(COLORREF a_focus_btn_color, COLORREF a_focus_border_color,
    COLORREF a_btn_color, COLORREF a_border_color,
    COLORREF a_in_border_color, COLORREF a_text_color)
{
    m_focus_btn_color = a_focus_btn_color;
    m_focus_border_color = a_focus_border_color;
    m_btn_color = a_btn_color;
    m_border_color = a_border_color;
    m_in_border_color = a_in_border_color;
    m_text_color = a_text_color;
}
