// TW_ToolBar.cpp : implementation file
//

#include "pch.h"
#include "TestServer.h"
#include "TW_ToolBar.h"


// TW_ToolBar

IMPLEMENT_DYNAMIC(TW_ToolBar, CWnd)

TW_ToolBar::TW_ToolBar()
{
    m_bk_color = RGB(39, 43, 47);
    m_border_color = RGB(59, 63, 67);
    m_in_border_color = RGB(109, 113, 117);
    m_push_btn_color = RGB(59, 63, 67);
    m_text_color = RGB(255, 255, 255);
    
}

TW_ToolBar::~TW_ToolBar()
{
}


BEGIN_MESSAGE_MAP(TW_ToolBar, CWnd)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    
END_MESSAGE_MAP()



// TW_ToolBar �޽��� ó����
void TW_ToolBar::OnPaint()
{
    CPaintDC dc(this);

    dc.SelectStockObject(DC_BRUSH);  // ���� �����ؼ� ����ϴ� Brush ����
    dc.SelectStockObject(DC_PEN);   // ���� �����ؼ� ����ϴ� Pen ����

    dc.SetDCBrushColor(m_bk_color);  // ������ ���� ����
    dc.SetDCPenColor(m_bk_color);   // ������ �׵θ� ���� ����

    dc.Rectangle(m_rect);  // ���� ��ü ������ �簢������ �׸���.

    dc.SelectStockObject(NULL_BRUSH);  // Brush�� ��� �� ������ ����
    dc.SetDCPenColor(m_border_color);  // ������ ���� �׵θ� ���� ����
    // ������ ���� �׵θ��� �׸���.
    dc.Rectangle(m_rect.left + 1, m_rect.top + 1, m_rect.right - 1, m_rect.bottom - 1);

    // ���ٿ� ��ϵ� ù ��° ��ư�� ������ ����Ų��.
    TipsCommandData* p_btn = m_btn_list;
    CRect r;

    dc.SelectObject(&m_tool_font);  // �۲� ����
    dc.SetBkMode(TRANSPARENT);  // ���ڿ��� ����� �����ϰ� ó���Ѵ�.

    for (int i = 0; i < m_btn_count; i++, p_btn++) {
        // ���ٿ� ��ϵ� ��ư�� ��ġ�� �簢�� �������� �����Ѵ�.
        r.SetRect(p_btn->x, 3, p_btn->x + p_btn->width, m_rect.bottom - 8);
        if (p_btn->p_image != NULL) {  // �̹����� �ε�� ���¶��
            p_btn->p_image->Draw(dc, p_btn->x + p_btn->width / 2 - 16, 10);
        }
        // ���� ������ ��ư�� ���ڿ��� ����Ѵ�.
        dc.SetTextColor(m_text_color);
        dc.DrawText(p_btn->p_name, r, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
    }
}

void TW_ToolBar::CreateToolBar(int a_rect_id, CWnd* ap_parent, int a_ctrl_id)
{
    CRect r;
    // ��ȭ���ڿ� �߰��� a_rect_id ���̵��� ��Ʈ�� ��ġ�� ����Ѵ�.
    ap_parent->GetDlgItem(a_rect_id)->GetWindowRect(r);
    ap_parent->ScreenToClient(r);
    // ���������� �ణ�� ������ ����� ������ ���������� ���ݴ� ũ�� �����Ѵ�.
    r.right += 2;
    // ���ٷ� ����� �����츦 �����Ѵ�.
    Create(NULL, NULL, WS_CHILD | WS_VISIBLE, r, ap_parent, a_ctrl_id);
}

int TW_ToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

    // ���� ������ Ŭ���̾�Ʈ ��ü ������ ��ǥ�� ��´�.
    GetClientRect(m_rect);
    // ���ٿ��� ����� ��Ʈ�� �����Ѵ�.
    m_tool_font.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"���� ���");
    // ��ϵ� ��ư ������ ������ �� ��ư�� ��ġ ������ �����Ѵ�.
    UpdateButtonInfo();
    return 0;
}

void TW_ToolBar::UpdateButtonInfo()
{
    CClientDC dc(this);

    // ���ٿ� ��ϵ� ù ��° ��ư�� ������ ����Ų��.
    TipsCommandData* p_btn = m_btn_list;
    dc.SelectObject(&m_tool_font);  // �۲� ����
    int pos = 5, i;
    for (i = 0; i < m_btn_count; i++, p_btn++) {
        p_btn->x = pos;  // ���� ��ġ�� ����
        // ��ư �̸��� ��µǾ��� �� ������ ���� ����Ͽ� ��ư�� ũ�⸦ �����Ѵ�.
        //CSize temp_size = dc.GetTextExtent(p_btn->p_name, p_btn->name_len);
        //p_btn->width = temp_size.cx + 20;
        p_btn->width = dc.GetTextExtent(p_btn->p_name, p_btn->name_len).cx + 20;
        pos += p_btn->width;  // ���� ��ư�� ���� ��ġ ���� �����Ѵ�.
    }
}

void TW_ToolBar::AddButton(const wchar_t* ap_name, int a_command_id, const wchar_t* ap_png_path)
{
    if (m_btn_count < MAX_COMMAND_COUNT) {
        int len = wcslen(ap_name);  // ���޵� �̸��� ���̸� ���Ѵ�.
        m_btn_list[m_btn_count].name_len = len++;  // len�� NULL ���� ������ ũ��� ����
        m_btn_list[m_btn_count].p_name = new wchar_t[len];  // �̸��� ������ �޸𸮸� �Ҵ��Ѵ�.
        memcpy(m_btn_list[m_btn_count].p_name, ap_name, len << 1);  // �̸��� �����Ѵ�.
        // ��ư�� ������ �� ����� �޽��� ID�� �����Ѵ�.
        m_btn_list[m_btn_count].command_id = a_command_id;
        // ���ҽ��� ����� ��Ʈ���� �б� ���� ��ü�� �����.
        m_btn_list[m_btn_count].p_image= new CImage;
        m_btn_list[m_btn_count].p_image->Load(ap_png_path);
        m_btn_count++;  // ��ϵ� ��ư�� ���� ������Ų��.
    }
}

void TW_ToolBar::OnDestroy()
{
    // ��ư �̸��� �����ϱ� ���� �Ҵ��ߴ� �޸𸮸� �����Ѵ�.
    for (int i = 0; i < m_btn_count; i++) {
        delete[] m_btn_list[i].p_name;   // ��ư �̸��� �����ϴ� �޸� ����
        if (m_btn_list[i].p_image != NULL) {  // ��ư�� ��Ʈ���� ����ϴ��� üũ�Ѵ�.
            m_btn_list[i].p_image->Destroy();
            delete m_btn_list[i].p_image;
        }
    }

    m_tool_font.DeleteObject();  // ����ϴ� �۲��� �����Ѵ�.
    CWnd::OnDestroy();
}

void TW_ToolBar::DrawPushButton(TipsCommandData* ap_btn)
{
    CClientDC dc(this);

    dc.SelectObject(&m_tool_font);  // �۲� ����
    dc.SetBkMode(TRANSPARENT);  // ���ڿ��� ����� �����ϰ� ó���Ѵ�.

    dc.SelectStockObject(DC_BRUSH);  // ���� �����ؼ� ����ϴ� Brush ����
    dc.SelectStockObject(DC_PEN);   // ���� �����ؼ� ����ϴ� Pen ����

    dc.SetDCBrushColor(m_push_btn_color);  // ��ư�� ���� ����
    dc.SetDCPenColor(m_in_border_color);   // ��ư�� �׵θ� ���� ����

    dc.Rectangle(m_select_rect);  // ��ư�� �׸���.

    CRect r = m_select_rect;
    r.bottom -= 4;   // ��ġ ����!
    if (ap_btn->p_image != NULL) {  // �̹����� �ε�� ���¶��
        ap_btn->p_image->Draw(dc, ap_btn->x + ap_btn->width / 2 - 16 + 1, 11);
    }

    dc.SetTextColor(m_text_color);  // ���ڿ��� ������� ���
    // ��ư�� �׸��鼭 ��ư�� �̸��� ���������� ��ư�� �̸��� ����Ѵ�.
    dc.DrawText(ap_btn->p_name, r + CPoint(1, 1), DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
}

void TW_ToolBar::DrawPopButton(TipsCommandData* ap_btn)
{
    CClientDC dc(this);

    dc.SelectObject(&m_tool_font);  // �۲� ����
    dc.SetBkMode(TRANSPARENT);  // ���ڿ��� ����� �����ϰ� ó���Ѵ�.

    dc.SelectStockObject(DC_BRUSH);  // ���� �����ؼ� ����ϴ� Brush ����
    dc.SelectStockObject(DC_PEN);   // ���� �����ؼ� ����ϴ� Pen ����

    dc.SetDCBrushColor(m_bk_color);  // ������ ���� ����
    dc.SetDCPenColor(m_in_border_color);   // ��ư�� �׵θ� ���� ����

    dc.Rectangle(m_select_rect);  // �⺻ ��ư�� ����Ѵ�.

    CRect r = m_select_rect;
    r.bottom -= 4;  // ��ġ ����
    if (ap_btn->p_image != NULL) {  // �̹����� �ε�� ���¶��
        ap_btn->p_image->Draw(dc, ap_btn->x + ap_btn->width / 2 - 16, 10);
    }

    // ������� ��ư�� ���ڿ��� ����Ѵ�.
    dc.SetTextColor(m_text_color);
    dc.DrawText(ap_btn->p_name, r, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
}

void TW_ToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_clicked_flag && m_select_index != -1) {  // ���콺�� Ŭ���� ���°� �ƴ� ��쿡�� ó��!
        m_clicked_flag = 1;  // ���콺�� Ŭ����
        TipsCommandData* p_btn = m_btn_list + m_select_index;  // ���õ� ��ư�� ����
        // ���õ� ��ư�� ���� ������ �����Ѵ�.
        m_select_rect.SetRect(p_btn->x + 4, 4, p_btn->x + p_btn->width - 4, m_rect.bottom - 4);
        // ��ư�� ������ ����� ����Ѵ�.
        DrawPushButton(p_btn);
        // ���콺�� ���� ������ ������� ���콺 �޽����� �޵��� �����Ѵ�.
        SetCapture();
    }
    CWnd::OnLButtonDown(nFlags, point);
}

void TW_ToolBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_clicked_flag) {  // ���콺�� Ŭ���� ���¶�� ó���Ѵ�.
        m_clicked_flag = 0;  // ���콺 Ŭ���� �����Ǿ��ٰ� ����
        ReleaseCapture();  // SetCapture ���¸� �����Ѵ�.

        TipsCommandData* p_btn = m_btn_list + m_select_index;  // ���õ� ��ư�� ����
        DrawPopButton(p_btn);  // ��ư�� �������� ���¸� ���� ���·� �����Ѵ�.

        if (m_select_rect.PtInRect(point)) {  // ��ư ���� ������ ���콺�� ������ ���!
            // �θ� ������� �� ��ư�� ���������� WM_COMMAND �޽����� �˸���.
            GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(p_btn->command_id, 0), 0);
        }
    }
    CWnd::OnLButtonUp(nFlags, point);
}

void TW_ToolBar::CheckButtonInToolBar(CPoint point)
{
    // ���ٿ� ��ϵ� ù ��° ��ư�� ������ ����Ų��.
    TipsCommandData* p_btn = m_btn_list;
    CRect r;
    int i, old_index = m_select_index;
    // ���콺�� ���� ��ġ�� �̿��ؼ� ������ � ��ư ������ ��ġ�ߴ��� üũ�Ѵ�.
    for (i = 0; i < m_btn_count; i++, p_btn++) {
        // ��ư ������ ����ؼ� ���� ��ư�� ��ġ ������ �����Ѵ�.
        r.SetRect(p_btn->x + 3, 3, p_btn->x + p_btn->width - 3, m_rect.bottom - 3);
        if (r.PtInRect(point)) {  // point ��ǥ�� ��ư ������ ��ġ�� ���
            m_select_index = i;  // ���� ��ư�� ���콺�� ��ġ�� ��ư���� �����Ѵ�.
            break;
        }
    }

    // i ���� m_btn_count ���� �����ϴٴ� ���� ���ٿ� ��ϵ� ��ư ����
    // ���콺�� ��ġ���� �ʾҴٴ� ���̴�.
    if (i == m_btn_count) m_select_index = -1;

    if (old_index != m_select_index) { // ������ ���� ���°� ����� ���
        CClientDC dc(this);
        dc.SelectStockObject(DC_PEN);   // ���� �����ؼ� ����ϴ� Pen ����
        dc.SelectStockObject(NULL_BRUSH);  // Brush�� ��� �� ������ ����

        if (old_index != -1) {
            // ������ ���õǾ� �ִ� ��ư�� ���¸� �����.
            p_btn = m_btn_list + old_index;
            dc.SetDCPenColor(m_bk_color);   // Pen ���� ����
            // ���� �簢���� �� �ٷ� �Ǿ��ֱ� ������ �� ���� ��� �����.
            dc.Rectangle(p_btn->x + 3, 3, p_btn->x + p_btn->width - 3, m_rect.bottom - 3);
            dc.Rectangle(p_btn->x + 4, 4, p_btn->x + p_btn->width - 4, m_rect.bottom - 4);
        }

        if (m_select_index != -1) {
            // ���� ���õ� ��ư ��ġ�� ���� ���¸� �׸���.
            p_btn = m_btn_list + m_select_index;
            dc.SetDCPenColor(m_border_color);   // Pen ���� ����
            dc.Rectangle(p_btn->x + 3, 3, p_btn->x + p_btn->width - 3, m_rect.bottom - 3);
            dc.SetDCPenColor(m_in_border_color);   // Pen ���� ����
            dc.Rectangle(p_btn->x + 4, 4, p_btn->x + p_btn->width - 4, m_rect.bottom - 4);
        }
    }
}

void TW_ToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_clicked_flag) {  // ���콺�� Ŭ���� �ȵ� ����
        CheckButtonInToolBar(point);  // ��ư�� ��ġ�� ǥ���Ѵ�.
    }
    else {  // ���콺�� Ŭ���� ����
        if (m_select_rect.PtInRect(point)) {  // ���콺�� ��ư �������� �ִ� ���!
            if (m_clicked_flag == 2) {  // ��ư ���� �ۿ� �ִٰ� �������� ���� ���!
                m_clicked_flag = 1;  // ��ư ���ο� ������ ����
                DrawPushButton(m_btn_list + m_select_index);  // ��ư�� ������ ���·� �׸���.
            }
        }
        else {  // ���콺�� ��ư ���� �ۿ� �ִ� ���
            if (m_clicked_flag == 1) {  // ��ư �ȿ� �ִٰ� ��ư ������ ���� ���!
                m_clicked_flag = 2;  // ���콺�� ��ư �ܺο� ������ ����
                DrawPopButton(m_btn_list + m_select_index);  // ��ư�� �⺻ ���·� �ٽ� �׸�
            }
        }
    }
    CWnd::OnMouseMove(nFlags, point);
}
