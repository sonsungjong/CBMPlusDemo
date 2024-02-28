
#include "pch.h"
#include "CBMServer.h"
#include "VibGraphWnd.h"
#include <cmath>

IMPLEMENT_DYNAMIC(VibGraphWnd, CWnd)

// �׷����� �����ϴ� �Ǽ����� �����ϴ� �Լ�
extern void DeleteCurrentValueData(void* ap_data);

// ������
VibGraphWnd::VibGraphWnd() : m_vib_list(DeleteCurrentValueData)
{
}

VibGraphWnd::~VibGraphWnd()
{}

void VibGraphWnd::OnDraw()
{
	mp_target->BeginDraw(); // �׸��� �׸� Ÿ���� ��´�.
	mp_target->Clear(ColorF(0.3843f, 0.6078f, 0.6392f)); // �������� ä���.

	// 0 ���� ��ġ�� ǥ���� �׸��� ���� ����Ѵ�.
	mp_target->DrawLine(Point2F(0, m_half_cy), Point2F(m_rect.right, m_half_cy), mp_center_grid_brush, 3, mp_stroke_style);
	mp_target->DrawRectangle(m_rect, mp_center_grid_brush);  // �׷����� �׵θ��� �׸���.

	if (mp_graph_vib_geo) mp_target->DrawGeometry(mp_graph_vib_geo, mp_graph_brush, 3); // ������ �׷����� ȭ�鿡 ����Ѵ�.

	int count = m_vib_list.Count();
	D2D1_POINT_2F* p_pos = mp_pos_list, * p_limit = p_pos + count;
	D2D1_ELLIPSE p_data_rect = { {0.0f, 0.0f}, 4.0f, 4.0f };
	while (p_pos < p_limit) {
		p_data_rect.point = *p_pos++;
		mp_target->FillEllipse(p_data_rect, mp_data_brush); // ������ ��ġ�� ���� ����Ѵ�.
	}

	// ��հ�, �ִ�, �ּڰ��� ��ġ�� ������ ǥ���Ѵ�.
	mp_target->DrawLine(Point2F(0, m_avr_pos), Point2F(m_rect.right, m_avr_pos), mp_avr_data_brush, 3, mp_stroke_style);
	mp_target->DrawLine(Point2F(0, m_max_pos), Point2F(m_rect.right, m_max_pos), mp_max_data_brush, 3, mp_stroke_style);
	mp_target->DrawLine(Point2F(0, m_min_pos), Point2F(m_rect.right, m_min_pos), mp_min_data_brush, 3, mp_stroke_style);

	if (mp_target->EndDraw()) { // Ÿ���� �ݳ��Ѵ�.
		DestroyGraphicObject(); // ����ϴ� �׷��� ��ü�� �����Ѵ�.
		CreateGraphicObject();  // ���α׷����� ����� �׷��� ��ü�� �����.
	}
}

BEGIN_MESSAGE_MAP(VibGraphWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()


int VibGraphWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{ return -1; }

	// TODO:  Add your specialized creation code here

	// �׷��� ��� ���� ����
	GetClientRect(m_range_rect);
	m_rect.left = (float)(m_range_rect.left);
	m_rect.top = (float)(m_range_rect.top);
	m_rect.right = (float)(m_range_rect.right);
	m_rect.bottom = (float)(m_range_rect.bottom);

	m_data_limit_count = m_range_rect.Width() / X_INTERVAL + 1;					// ������ �� ��°���
	m_half_cy = (float)(m_range_rect.Height() / 2.0f);							// y�࿡�� 0��ġ ���
	mp_pos_list = new D2D1_POINT_2F[m_data_limit_count];				// ������ ������ �޸�

	// D2D�� �׸� ����
	ID2D1HwndRenderTarget* p_hwnd_target;
	mp_factory->CreateHwndRenderTarget(RenderTargetProperties(), HwndRenderTargetProperties(m_hWnd, SizeU(m_rect.right - m_rect.left, m_rect.bottom - m_rect.top)), &p_hwnd_target);
	mp_target = p_hwnd_target;

	float x = m_rect.left - 5;					// �׷��� ������ġ
	for(int i=0;i<m_data_limit_count;i++, x+= X_INTERVAL)
	{ mp_pos_list[i].x = x; }

	CreateGraphicObject();

	return 0;
}

void VibGraphWnd::SetFactory(ID2D1Factory* ap_factory)
{
	mp_factory = ap_factory;
}


void VibGraphWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
	DestroyGraphicObject();				// D2D ����
	delete[] mp_pos_list;					// ������ ��ǥ �޸� ����
}


void VibGraphWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	::ValidateRect(m_hWnd, NULL);				// WM_PAINT ��ȿȭ
	OnDraw();					// �׷��� �׸���
}

// ���α׷����� ����� �׷��� ��ü�� �����ϴ� �Լ�
void VibGraphWnd::CreateGraphicObject()
{
	// ������ �������� �귯�ø� �����Ѵ�.
	mp_target->CreateSolidColorBrush(ColorF(0.1922f, 0.3294f, 0.3608f), &mp_center_grid_brush); // 0 ��ġ �� ����
	mp_target->CreateSolidColorBrush(ColorF(0.5922f, 0.9216f, 0.8627f, 0.6f), &mp_graph_brush); // �׷��� �� ����
	mp_target->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 1.0f, 1.0f), &mp_data_brush);  // ������ ��ġ ����
	mp_target->CreateSolidColorBrush(ColorF(0.2f, 0.95f, 0.2f, 0.3f), &mp_avr_data_brush); // ��հ� ��ġ �� ����
	mp_target->CreateSolidColorBrush(ColorF(1.0f, 0.0f, 1.0f, 0.3f), &mp_max_data_brush); // �ִ� ��ġ �� ����
	mp_target->CreateSolidColorBrush(ColorF(1.0f, 0.0f, 0.0f, 0.3f), &mp_min_data_brush); // �ּڰ� ��ġ �� ����

	// Dot �Ӽ����� ȹ(stroke) ��ü�� �����Ѵ�.	
	mp_factory->CreateStrokeStyle(StrokeStyleProperties(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER, 10.0f,
		D2D1_DASH_STYLE_DOT, 0.0f), NULL, 0, &mp_stroke_style);
}

// ���α׷����� ����ϴ� �׷��� ��ü�� �����ϴ� �Լ�
void VibGraphWnd::DestroyGraphicObject()
{
	// �� ������ ���� ������� �귯�õ��� �����Ѵ�.
	TW_IRelease(&mp_center_grid_brush);
	TW_IRelease(&mp_graph_brush);
	TW_IRelease(&mp_data_brush);
	TW_IRelease(&mp_avr_data_brush);
	TW_IRelease(&mp_max_data_brush);
	TW_IRelease(&mp_min_data_brush);

	TW_IRelease(&mp_stroke_style); // ���� ��Ÿ���� ���� ������� ��Ÿ���� �����Ѵ�.
	TW_IRelease(&mp_graph_vib_geo); // �׷��� ���� ����ϱ� ���� ���� ���� ���� ��ü�� �����Ѵ�.

	TW_IRelease(&mp_target);
	TW_IRelease(&mp_factory);
}

// �׷��� �������� �ּڰ�, �ִ�, ��հ��� ���ϴ� �Լ�
void VibGraphWnd::GetMinMaxAverage()
{
	node* p_node = m_vib_list.GetHead(); // ���� ����Ʈ�� ù ��带 ��´�.
	float total_value = *(float*)p_node->p_data, temp;
	m_max_value = m_min_value = total_value; // ù ���� �ִ����� �ּڰ��̴�.
	p_node = p_node->p_next;

	while (p_node) {
		temp = *(float*)p_node->p_data;
		total_value += temp; // ������ ���Ѵ�.
		if (temp > m_max_value) m_max_value = temp; // �ִ����� üũ
		else if (temp < m_min_value) m_min_value = temp; // �ּڰ����� üũ
		p_node = p_node->p_next; // ���� ������ ��ġ�� �̵��Ѵ�.
	}
	m_avr_value = total_value / m_vib_list.Count(); // ��հ��� ���Ѵ�.
}

// �� �׷����� ������ �����͸� �߰��ϴ� �Լ�
void VibGraphWnd::AddData(float a_data)
{
	float* p_data;
	int count = m_vib_list.Count();

	// �������� ������ m_data_limit_count���� ���� ���� �޸𸮸� �Ҵ��ؼ� �����͸� �߰��ϰ�
	// m_data_limit_count�� ���������� ���� ������ �����Ͱ� ����� �޸𸮸� ��Ȱ���Ѵ�.
	if (count < m_data_limit_count) {
		p_data = new float;
		count++;
	}
	else {
		node* p = m_vib_list.GetHead(); // ���� ��带 ��´�.
		p_data = (float*)p->p_data;  // ���� ��忡 ����� �޸��� �ּҸ� ��´�.
		m_vib_list.DeleteNode(p, 0); // ���� ��带 �����. (�߰��� �޸𸮴� ������ �ʴ´�.)
	}
	*p_data = a_data; // ���޵� ���� �޸𸮿� �����Ѵ�.

	m_vib_list.AddNode(p_data); // ��带 �߰��Ѵ�.

	TW_IRelease(&mp_graph_vib_geo); // �����Ǿ� �ִ� �׷����� �� ������ �����.

	if (count >= 2) { // �����Ͱ� 2�� �̻��̸� �׷����� �׸���.
		//ID2D1Factory1 *p_factory = (ID2D1Factory1 *)mp_d2d_core->GetFactory();
		if (SUCCEEDED(mp_factory->CreatePathGeometry(&mp_graph_vib_geo))) {
			ID2D1GeometrySink* p_sink = NULL;
			// ������ �߰��ϱ� ���� �Է��� ���� sink �ڿ��� ����.
			if (SUCCEEDED(mp_graph_vib_geo->Open(&p_sink))) {
				GetMinMaxAverage(); // �׷��� �������� �ּڰ�, �ִ�, ��հ��� ���Ѵ�.

				float scale = 1.0f;  // �⺻ �������� �����Ѵ�.
				if (m_auto_scale_mode) { // �ڵ� �������� ����� ���
					float a = abs(m_max_value), b = abs(m_min_value);
					scale = (a > b) ? a : b; // �ִ񰪰� �ּڰ��� �����߿� �� ū���� ���Ѵ�.
					scale = (m_half_cy - 10) / scale; // �׷��� ���� �Ǵ� �Ʒ��ʿ� �������� 10 ����
				}

				// �µ� �׸���
				node* p_node = m_vib_list.GetHead(); // ���� ����Ʈ�� ���� ��带 ��´�.
				D2D1_POINT_2F* p_pos = mp_pos_list;
				// ù ��° �������� ��ġ�� �� �׸��� ���� ��ġ�� �����Ѵ�. (m_scroll_index��° ������ ����)
				p_pos->y = m_half_cy - *(float*)p_node->p_data * scale;
				// ���� ������ ���� ���� �����Ѵ�.
				p_sink->BeginFigure(*p_pos++, D2D1_FIGURE_BEGIN_FILLED);
				p_node = p_node->p_next;
				while (p_node) {
					// ���� ��ġ�� y ��ǥ�� ����Ѵ�.
					p_pos->y = m_half_cy - *(float*)p_node->p_data * scale;
					p_sink->AddLine(*p_pos++); // ���� ��ǥ�� ���� ������ �߰��Ѵ�.
					p_node = p_node->p_next; // ���� ������ ��ġ�� �̵��Ѵ�.
				}

				p_sink->EndFigure(D2D1_FIGURE_END_OPEN); // �н��� �ݴ´�.
				p_sink->Close();
				TW_IRelease(&p_sink);  // ���� �߰��� ���� ����ϴ� sink �ڿ� �ݳ�

				m_avr_pos = m_half_cy - m_avr_value * scale; // ��ռ��� ��ġ
				m_max_pos = m_half_cy - m_max_value * scale; // �ִ� ���� ��ġ
				m_min_pos = m_half_cy - m_min_value * scale; // �ּڰ� ���� ��ġ
			}

			
		}
		Invalidate(0);  // �����츦 �����Ͽ� ������ �׷����� ȭ�鿡 ����Ѵ�.
	}
}

// �ڵ� ������ ���θ� �����ϴ� �Լ�
void VibGraphWnd::ChangeAutoScale(int a_auto_flag)
{
	m_auto_scale_mode = a_auto_flag;
}

//// Direct2D Core�� �����ϴ� �Լ�
//void VibGraphWnd::SetD2dCore(TW_D2D_Core *ap_d2d_core)
//{
//	mp_d2d_core = ap_d2d_core;
//}