#include "pch.h"
#include "TW_Direct2D.h"

TWD_View::TWD_View()
{
	mh_wnd = NULL;
	mp_target = NULL;
	m_view_size.width = m_view_size.height = 0;
	// �⺻���� �����Ǵ� �۲��� �̸��� ũ�⸦ �����Ѵ�.
	memcpy(m_def_font_name, _T("���� ���"), 12);
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

	// ���ڿ� ��� ������ ������ �ڿ� ����
	if (S_OK == gp_tw_d2d->GetWriteFactory()->CreateTextFormat(m_def_font_name, NULL,
		DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		m_def_font_size, _T("ko-kr"), &mp_text_format)) {
		// �������� (���)
		mp_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		// �������� (���)
		mp_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	// ������ ����Ÿ�� �������� �ּҸ� ������ ������
	ID2D1HwndRenderTarget* p_hwnd_target;
	// ������ ������ Ŭ���̾�Ʈ ������ �׸��� �׸��� RenderTarget ����
	gp_tw_d2d->GetFactory()->CreateHwndRenderTarget(RenderTargetProperties(),
		HwndRenderTargetProperties(ah_wnd, m_view_size), &p_hwnd_target);

	// �θ� �������̽��� �ּҸ� ����
	mp_target = p_hwnd_target;

	// ���� �ؽ�Ʈ ��¿� ����� �귯�� ����
	mp_target->CreateSolidColorBrush(m_def_color, &mp_user_brush);

	return 1;
}

// WM_PAINT���� ȣ���ؾ��ϴ� �Լ�
void TWD_View::OnUserPaint()
{
	// WM_PAINT �÷��׸� ��ȿȭ
	::ValidateRect(mh_wnd, NULL);
	UpdateView();
}

// �並 ������ �����ϴ� �Լ�
void TWD_View::UpdateView()
{
	mp_target->BeginDraw();
	// ȭ���� ����������
	mp_target->Clear(m_bk_color);

	OnDraw();
	mp_target->EndDraw();
}

ID2D1PathGeometry* TWD_View::CreatePathGeometry(D2D1_POINT_2F* ap_pos_list, int a_count)
{
	ID2D1PathGeometry* p_geo = NULL;
	if (SUCCEEDED(gp_tw_d2d->GetFactory()->CreatePathGeometry(&p_geo))) {
		ID2D1GeometrySink* p_sink = NULL;
		// ������ �߰��ϱ� ���� �Է��� ���� sink �ڿ��� ����.
		if (SUCCEEDED(p_geo->Open(&p_sink))) {
			D2D1_POINT_2F* p_pos_list_limit = ap_pos_list + a_count;

			// ���� ������ ���� ���� �����Ѵ�.
			p_sink->BeginFigure(*ap_pos_list++, D2D1_FIGURE_BEGIN_FILLED);
			while (ap_pos_list < p_pos_list_limit) {
				p_sink->AddLine(*ap_pos_list++); // ���� ��ǥ�� ���� ������ �߰��Ѵ�.
			}
			p_sink->EndFigure(D2D1_FIGURE_END_OPEN); // �н��� �ݴ´�.
			p_sink->Close();
			TWD_IRelease(&p_sink);  // ���� �߰��� ���� ����ϴ� sink �ڿ� �ݳ�
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
	mp_target->CreateSolidColorBrush(a_color, &temp); // 0 ��ġ �� ����
	return temp;
}