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



// TW_ToolBar 메시지 처리기
void TW_ToolBar::OnPaint()
{
    CPaintDC dc(this);

    dc.SelectStockObject(DC_BRUSH);  // 색상만 변경해서 사용하는 Brush 설정
    dc.SelectStockObject(DC_PEN);   // 색상만 변경해서 사용하는 Pen 설정

    dc.SetDCBrushColor(m_bk_color);  // 툴바의 배경색 설정
    dc.SetDCPenColor(m_bk_color);   // 툴바의 테두리 색상 설정

    dc.Rectangle(m_rect);  // 툴바 전체 영역을 사각형으로 그린다.

    dc.SelectStockObject(NULL_BRUSH);  // Brush는 사용 안 함으로 설정
    dc.SetDCPenColor(m_border_color);  // 툴바의 내부 테두리 색상 설정
    // 툴바의 내부 테두리를 그린다.
    dc.Rectangle(m_rect.left + 1, m_rect.top + 1, m_rect.right - 1, m_rect.bottom - 1);

    // 툴바에 등록된 첫 번째 버튼의 정보를 가리킨다.
    TipsCommandData* p_btn = m_btn_list;
    CRect r;

    dc.SelectObject(&m_tool_font);  // 글꼴 설정
    dc.SetBkMode(TRANSPARENT);  // 문자열의 배경은 투명하게 처리한다.

    for (int i = 0; i < m_btn_count; i++, p_btn++) {
        // 툴바에 등록된 버튼의 위치를 사각형 영역으로 구성한다.
        r.SetRect(p_btn->x, 3, p_btn->x + p_btn->width, m_rect.bottom - 8);
        if (p_btn->p_image != NULL) {  // 이미지가 로드된 상태라면
            p_btn->p_image->Draw(dc, p_btn->x + p_btn->width / 2 - 16, 10);
        }
        // 밝은 색으로 버튼의 문자열을 출력한다.
        dc.SetTextColor(m_text_color);
        dc.DrawText(p_btn->p_name, r, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
    }
}

void TW_ToolBar::CreateToolBar(int a_rect_id, CWnd* ap_parent, int a_ctrl_id)
{
    CRect r;
    // 대화상자에 추가된 a_rect_id 아이디의 컨트롤 위치를 계산한다.
    ap_parent->GetDlgItem(a_rect_id)->GetWindowRect(r);
    ap_parent->ScreenToClient(r);
    // 오른쪽으로 약간의 여백이 생기기 때문에 오른쪽으로 조금더 크게 보정한다.
    r.right += 2;
    // 툴바로 사용할 윈도우를 생성한다.
    Create(NULL, NULL, WS_CHILD | WS_VISIBLE, r, ap_parent, a_ctrl_id);
}

int TW_ToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

    // 툴바 윈도우 클라이언트 전체 영역의 좌표를 얻는다.
    GetClientRect(m_rect);
    // 툴바에서 사용할 폰트를 생성한다.
    m_tool_font.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"맑은 고딕");
    // 등록된 버튼 정보를 가지고 각 버튼의 위치 정보를 구성한다.
    UpdateButtonInfo();
    return 0;
}

void TW_ToolBar::UpdateButtonInfo()
{
    CClientDC dc(this);

    // 툴바에 등록된 첫 번째 버튼의 정보를 가리킨다.
    TipsCommandData* p_btn = m_btn_list;
    dc.SelectObject(&m_tool_font);  // 글꼴 설정
    int pos = 5, i;
    for (i = 0; i < m_btn_count; i++, p_btn++) {
        p_btn->x = pos;  // 시작 위치를 대입
        // 버튼 이름이 출력되었을 때 차지할 폭을 계산하여 버튼의 크기를 결정한다.
        //CSize temp_size = dc.GetTextExtent(p_btn->p_name, p_btn->name_len);
        //p_btn->width = temp_size.cx + 20;
        p_btn->width = dc.GetTextExtent(p_btn->p_name, p_btn->name_len).cx + 20;
        pos += p_btn->width;  // 다음 버튼을 위해 위치 값을 변경한다.
    }
}

void TW_ToolBar::AddButton(const wchar_t* ap_name, int a_command_id, const wchar_t* ap_png_path)
{
    if (m_btn_count < MAX_COMMAND_COUNT) {
        int len = wcslen(ap_name);  // 전달된 이름의 길이를 구한다.
        m_btn_list[m_btn_count].name_len = len++;  // len은 NULL 문자 포함한 크기로 변경
        m_btn_list[m_btn_count].p_name = new wchar_t[len];  // 이름을 저장할 메모리를 할당한다.
        memcpy(m_btn_list[m_btn_count].p_name, ap_name, len << 1);  // 이름을 복사한다.
        // 버튼을 눌렀을 때 사용할 메시지 ID를 저장한다.
        m_btn_list[m_btn_count].command_id = a_command_id;
        // 리소스에 저장된 비트맵을 읽기 위한 객체를 만든다.
        m_btn_list[m_btn_count].p_image= new CImage;
        m_btn_list[m_btn_count].p_image->Load(ap_png_path);
        m_btn_count++;  // 등록된 버튼의 수를 증가시킨다.
    }
}

void TW_ToolBar::OnDestroy()
{
    // 버튼 이름을 저장하기 위해 할당했던 메모리를 제거한다.
    for (int i = 0; i < m_btn_count; i++) {
        delete[] m_btn_list[i].p_name;   // 버튼 이름을 저장하던 메모리 해제
        if (m_btn_list[i].p_image != NULL) {  // 버튼에 비트맵을 사용하는지 체크한다.
            m_btn_list[i].p_image->Destroy();
            delete m_btn_list[i].p_image;
        }
    }

    m_tool_font.DeleteObject();  // 사용하던 글꼴을 제거한다.
    CWnd::OnDestroy();
}

void TW_ToolBar::DrawPushButton(TipsCommandData* ap_btn)
{
    CClientDC dc(this);

    dc.SelectObject(&m_tool_font);  // 글꼴 설정
    dc.SetBkMode(TRANSPARENT);  // 문자열의 배경은 투명하게 처리한다.

    dc.SelectStockObject(DC_BRUSH);  // 색상만 변경해서 사용하는 Brush 설정
    dc.SelectStockObject(DC_PEN);   // 색상만 변경해서 사용하는 Pen 설정

    dc.SetDCBrushColor(m_push_btn_color);  // 버튼의 배경색 설정
    dc.SetDCPenColor(m_in_border_color);   // 버튼의 테두리 색상 설정

    dc.Rectangle(m_select_rect);  // 버튼을 그린다.

    CRect r = m_select_rect;
    r.bottom -= 4;   // 위치 보정!
    if (ap_btn->p_image != NULL) {  // 이미지가 로드된 상태라면
        ap_btn->p_image->Draw(dc, ap_btn->x + ap_btn->width / 2 - 16 + 1, 11);
    }

    dc.SetTextColor(m_text_color);  // 문자열은 흰색으로 출력
    // 버튼을 그리면서 버튼의 이름이 지웠졌으니 버튼의 이름을 출력한다.
    dc.DrawText(ap_btn->p_name, r + CPoint(1, 1), DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
}

void TW_ToolBar::DrawPopButton(TipsCommandData* ap_btn)
{
    CClientDC dc(this);

    dc.SelectObject(&m_tool_font);  // 글꼴 설정
    dc.SetBkMode(TRANSPARENT);  // 문자열의 배경은 투명하게 처리한다.

    dc.SelectStockObject(DC_BRUSH);  // 색상만 변경해서 사용하는 Brush 설정
    dc.SelectStockObject(DC_PEN);   // 색상만 변경해서 사용하는 Pen 설정

    dc.SetDCBrushColor(m_bk_color);  // 툴바의 배경색 설정
    dc.SetDCPenColor(m_in_border_color);   // 버튼의 테두리 색상 설정

    dc.Rectangle(m_select_rect);  // 기본 버튼을 출력한다.

    CRect r = m_select_rect;
    r.bottom -= 4;  // 위치 보정
    if (ap_btn->p_image != NULL) {  // 이미지가 로드된 상태라면
        ap_btn->p_image->Draw(dc, ap_btn->x + ap_btn->width / 2 - 16, 10);
    }

    // 흰색으로 버튼의 문자열을 출력한다.
    dc.SetTextColor(m_text_color);
    dc.DrawText(ap_btn->p_name, r, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
}

void TW_ToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_clicked_flag && m_select_index != -1) {  // 마우스가 클릭된 상태가 아닌 경우에만 처리!
        m_clicked_flag = 1;  // 마우스가 클릭됨
        TipsCommandData* p_btn = m_btn_list + m_select_index;  // 선택된 버튼의 정보
        // 선택된 버튼의 영역 정보를 구성한다.
        m_select_rect.SetRect(p_btn->x + 4, 4, p_btn->x + p_btn->width - 4, m_rect.bottom - 4);
        // 버튼이 눌러진 모습을 출력한다.
        DrawPushButton(p_btn);
        // 마우스가 툴바 영역을 벗어나더라도 마우스 메시지를 받도록 설정한다.
        SetCapture();
    }
    CWnd::OnLButtonDown(nFlags, point);
}

void TW_ToolBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_clicked_flag) {  // 마우스가 클릭된 상태라면 처리한다.
        m_clicked_flag = 0;  // 마우스 클릭이 해제되었다고 설정
        ReleaseCapture();  // SetCapture 상태를 해제한다.

        TipsCommandData* p_btn = m_btn_list + m_select_index;  // 선택된 버튼의 정보
        DrawPopButton(p_btn);  // 버튼이 눌러졌던 형태를 기존 형태로 복구한다.

        if (m_select_rect.PtInRect(point)) {  // 버튼 영역 내에서 마우스가 해제된 경우!
            // 부모 윈도우로 이 버튼이 눌러졌음을 WM_COMMAND 메시지로 알린다.
            GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(p_btn->command_id, 0), 0);
        }
    }
    CWnd::OnLButtonUp(nFlags, point);
}

void TW_ToolBar::CheckButtonInToolBar(CPoint point)
{
    // 툴바에 등록된 첫 번째 버튼의 정보를 가리킨다.
    TipsCommandData* p_btn = m_btn_list;
    CRect r;
    int i, old_index = m_select_index;
    // 마우스의 현재 위치를 이용해서 툴바의 어떤 버튼 영역에 위치했는지 체크한다.
    for (i = 0; i < m_btn_count; i++, p_btn++) {
        // 버튼 정보를 사용해서 현재 버튼의 위치 정보를 구성한다.
        r.SetRect(p_btn->x + 3, 3, p_btn->x + p_btn->width - 3, m_rect.bottom - 3);
        if (r.PtInRect(point)) {  // point 좌표가 버튼 영역에 위치한 경우
            m_select_index = i;  // 현재 버튼을 마우스가 위치한 버튼으로 설정한다.
            break;
        }
    }

    // i 값이 m_btn_count 값과 동일하다는 뜻은 툴바에 등록된 버튼 위에
    // 마우스가 위치하지 않았다는 뜻이다.
    if (i == m_btn_count) m_select_index = -1;

    if (old_index != m_select_index) { // 툴바의 선택 상태가 변경된 경우
        CClientDC dc(this);
        dc.SelectStockObject(DC_PEN);   // 색상만 변경해서 사용하는 Pen 설정
        dc.SelectStockObject(NULL_BRUSH);  // Brush는 사용 안 함으로 설정

        if (old_index != -1) {
            // 이전에 선택되어 있던 버튼의 상태를 지운다.
            p_btn = m_btn_list + old_index;
            dc.SetDCPenColor(m_bk_color);   // Pen 색상 설정
            // 선택 사각형이 두 줄로 되어있기 때문에 두 줄을 모두 지운다.
            dc.Rectangle(p_btn->x + 3, 3, p_btn->x + p_btn->width - 3, m_rect.bottom - 3);
            dc.Rectangle(p_btn->x + 4, 4, p_btn->x + p_btn->width - 4, m_rect.bottom - 4);
        }

        if (m_select_index != -1) {
            // 새로 선택된 버튼 위치에 선택 상태를 그린다.
            p_btn = m_btn_list + m_select_index;
            dc.SetDCPenColor(m_border_color);   // Pen 색상 설정
            dc.Rectangle(p_btn->x + 3, 3, p_btn->x + p_btn->width - 3, m_rect.bottom - 3);
            dc.SetDCPenColor(m_in_border_color);   // Pen 색상 설정
            dc.Rectangle(p_btn->x + 4, 4, p_btn->x + p_btn->width - 4, m_rect.bottom - 4);
        }
    }
}

void TW_ToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_clicked_flag) {  // 마우스가 클릭이 안된 상태
        CheckButtonInToolBar(point);  // 버튼의 위치만 표시한다.
    }
    else {  // 마우스가 클릭된 상태
        if (m_select_rect.PtInRect(point)) {  // 마우스가 버튼 영역내에 있는 경우!
            if (m_clicked_flag == 2) {  // 버튼 영역 밖에 있다가 영역으로 들어온 경우!
                m_clicked_flag = 1;  // 버튼 내부에 있음을 설정
                DrawPushButton(m_btn_list + m_select_index);  // 버튼을 눌러진 상태로 그린다.
            }
        }
        else {  // 마우스가 버튼 영역 밖에 있는 경우
            if (m_clicked_flag == 1) {  // 버튼 안에 있다가 버튼 밖으로 나온 경우!
                m_clicked_flag = 2;  // 마우스가 버튼 외부에 있음을 설정
                DrawPopButton(m_btn_list + m_select_index);  // 버튼을 기본 형태로 다시 그림
            }
        }
    }
    CWnd::OnMouseMove(nFlags, point);
}
