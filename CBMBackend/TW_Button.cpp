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


// TW_Button �޽��� ó����
void TW_Button::DrawItem(LPDRAWITEMSTRUCT lpds)
{
    CRect r(lpds->rcItem);  // RECT ����ü�� CRect ��ü�� ��ȯ
    CDC* p_dc = CDC::FromHandle(lpds->hDC); // HDC �ڵ��� CDC ��ü�� ��ȯ

    p_dc->SelectStockObject(DC_BRUSH);  // ���� �����ؼ� ����ϴ� Brush ����
    p_dc->SelectStockObject(DC_PEN);   // ���� �����ؼ� ����ϴ� Pen ����

    if (lpds->itemState & ODS_FOCUS) {  // ��Ŀ�� ������ ���
        p_dc->SetDCBrushColor(m_focus_btn_color);  // Brush ���� ����
        p_dc->SetDCPenColor(m_focus_border_color);   // Pen ���� ����
    }
    else {  // ��Ŀ�� ���°� �ƴ� ���
        p_dc->SetDCBrushColor(m_btn_color);  // Brush ���� ����
        p_dc->SetDCPenColor(m_border_color);   // Pen ���� ����
    }
    p_dc->Rectangle(r);   // ������ ������ Pen�� Brush�� �簢���� �׸���.

    p_dc->SelectStockObject(NULL_BRUSH);  // Brush�� ��� �� ������ ����
    p_dc->SetDCPenColor(m_in_border_color);  // Pen ���� ����
    // ��ư�� ���� ������ �׸���.
    p_dc->Rectangle(r.left + 1, r.top + 1, r.right - 1, r.bottom - 1);

    // ��ư�� �������ٸ� ��ư�� �ؽ�Ʈ�� ��¦ �Ʒ������� �̵����Ѽ� ������ ������ �����Ѵ�.
    if (lpds->itemState & ODS_SELECTED) r += CPoint(1, 1);

    CString str;
    GetWindowText(str);  // ��ư�� �ؽ�Ʈ�� ��´�.
    p_dc->SetBkMode(TRANSPARENT);   // �ؽ�Ʈ�� ����� �����ϰ� ó���Ѵ�.
    p_dc->SetTextColor(m_text_color);   // �ؽ�Ʈ�� ������ �����Ѵ�.
    p_dc->DrawText(str, r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);  // �ؽ�Ʈ�� ����Ѵ�.
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
