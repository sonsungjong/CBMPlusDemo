#include "pch.h"
#include "TW_Direct2D.h"

TWD_View::TWD_View()
{
	mh_wnd = NULL;
	mp_target = NULL;
	m_view_size.width = m_view_size.height = 0;
	// 기본으로 생성되는 글꼴의 이름과 크기를 지정한다.
	memcpy(m_def_font_name, _T("맑은 고딕"), 12);
	m_def_font_size = 16.0f;
}

TWD_View::~TWD_View()
{
	TWD_IRelease(&mp_target);
	TWD_IRelease(&mp_user_brush);
	TWD_IRelease(&mp_text_format);
}

int TWD_View::CreateD2D(HWND ah_wnd, RECT* ap_rect)
{
	mh_wnd = ah_wnd;
	m_rect.left = (float)ap_rect->left;
	m_rect.top = (float)ap_rect->top;
	m_rect.right = (float)ap_rect->right;
	m_rect.bottom = (float)ap_rect->bottom;

	m_view_size.width = ap_rect->right - ap_rect->left;
	m_view_size.height = ap_rect->bottom - ap_rect->top;

	// 문자열 출력 형식을 지정할 자원 생성
	if (S_OK == gp_tw_d2d->GetWriteFactory()->CreateTextFormat(m_def_font_name, NULL,
		DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		m_def_font_size, _T("ko-kr"), &mp_text_format)) {
		// 수평정렬 (가운데)
		mp_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		// 수직정렬 (가운데)
		mp_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	// 윈도우 렌더타겟 생성에서 주소를 가져올 포인터
	ID2D1HwndRenderTarget* p_hwnd_target;
	// 지정한 윈도우 클라이언트 영역에 그림을 그리는 RenderTarget 생성
	gp_tw_d2d->GetFactory()->CreateHwndRenderTarget(RenderTargetProperties(),
		HwndRenderTargetProperties(ah_wnd, m_view_size), &p_hwnd_target);

	// 부모 인터페이스로 주소를 보관
	mp_target = p_hwnd_target;

	// 선과 텍스트 출력에 사용할 브러시 생성
	mp_target->CreateSolidColorBrush(m_def_color, &mp_user_brush);

	return 1;
}

// WM_PAINT에서 호출해야하는 함수
void TWD_View::OnUserPaint()
{
	// WM_PAINT 플래그를 무효화
	::ValidateRect(mh_wnd, NULL);
	UpdateView();
}

// 뷰를 강제로 갱신하는 함수
void TWD_View::UpdateView()
{
	mp_target->BeginDraw();
	// 화면을 검은색으로
	mp_target->Clear(m_bk_color);

	OnDraw();
	mp_target->EndDraw();
}

ID2D1PathGeometry* TWD_View::CreatePathGeometry(D2D1_POINT_2F* ap_pos_list, int a_count)
{
	ID2D1PathGeometry* p_geo = NULL;
	if (SUCCEEDED(gp_tw_d2d->GetFactory()->CreatePathGeometry(&p_geo))) {
		ID2D1GeometrySink* p_sink = NULL;
		// 도형을 추가하기 위해 입력을 받을 sink 자원을 연다.
		if (SUCCEEDED(p_geo->Open(&p_sink))) {
			D2D1_POINT_2F* p_pos_list_limit = ap_pos_list + a_count;

			// 기하 도형의 시작 점을 지정한다.
			p_sink->BeginFigure(*ap_pos_list++, D2D1_FIGURE_BEGIN_FILLED);
			while (ap_pos_list < p_pos_list_limit) {
				p_sink->AddLine(*ap_pos_list++); // 계산된 좌표를 기하 도형에 추가한다.
			}
			p_sink->EndFigure(D2D1_FIGURE_END_OPEN); // 패스를 닫는다.
			p_sink->Close();
			TWD_IRelease(&p_sink);  // 도형 추가를 위해 사용하던 sink 자원 반납
		}
	}
	return p_geo;
}

ID2D1StrokeStyle* TWD_View::CreateDotLine()
{
	ID2D1StrokeStyle* p_pen = NULL;
	gp_tw_d2d->GetFactory()->CreateStrokeStyle(StrokeStyleProperties(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER, 10.0f,
		D2D1_DASH_STYLE_DOT, 0.0f), NULL, 0, &p_pen);
	return p_pen;
}

ID2D1StrokeStyle* TWD_View::CreateMultiLine()
{
	ID2D1StrokeStyle* p_pen = NULL;

	gp_tw_d2d->GetFactory()->CreateStrokeStyle(StrokeStyleProperties(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_ROUND, 10.0f,
		D2D1_DASH_STYLE_SOLID, 0.0f), NULL, 0, &p_pen);

	return p_pen;
}

ID2D1SolidColorBrush* TWD_View::CreateSolidBrush(D2D1_COLOR_F a_color)
{
	ID2D1SolidColorBrush* temp;
	mp_target->CreateSolidColorBrush(a_color, &temp); // 0 위치 선 색상
	return temp;
}