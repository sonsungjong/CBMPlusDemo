// LineGraphWnd.cpp: 구현 파일
#include "pch.h"
#include "CBMServer.h"
#include "TW_Direct2D.h"
#include "LineGraphWnd.h"

// LineGraphWnd
IMPLEMENT_DYNAMIC(LineGraphWnd, CWnd)

//// 그래프를 구성하는 실숫값을 제거하는 함수
void DeleteCurrentValueData(void *ap_data)
{
	delete[] (float *)ap_data;
}

LineGraphWnd::LineGraphWnd(int a_line_count) : m_data_list(DeleteCurrentValueData)
{
	m_line_count = a_line_count;
	mp_graph_geo = new ID2D1PathGeometry *[m_line_count];
	for (int i = 0; i < m_line_count; i++) *(mp_graph_geo + i) = 0;
}

LineGraphWnd::~LineGraphWnd()
{
	delete[] mp_graph_geo;

	for (int i = 0; i < m_line_count; i++) delete[] *(mp_pos_list + i);
	delete[] mp_pos_list;
}

BEGIN_MESSAGE_MAP(LineGraphWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// LineGraphWnd 메시지 처리기
int LineGraphWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	CRect r;
	GetClientRect(r);

	CreateD2D(m_hWnd, r);
	m_data_limit_count = r.Width() / X_INTERVAL + 1; // 수평으로 출력할 데이터의 개수를 계산
	m_half_cy = (float)(r.Height() / 2.0f); // 0 값의 위치를 출력할 y 좌표를 계산한다.

	mp_pos_list = new D2D1_POINT_2F * [m_line_count]; // 데이터를 저장할 메모리를 할당한다.
	for (int i = 0; i < m_line_count; i++) *(mp_pos_list + i) = new D2D1_POINT_2F[m_data_limit_count];

	float x = (float)(r.right - 5); // 그래프 시작 위치
	// x 좌표는 고정되기 때문에 한 번 계산되면 계속 사용할 수 있다
	for (int i = 0; i < m_data_limit_count; i++, x -= X_INTERVAL) {
		for (int sub_i = 0; sub_i < m_line_count; sub_i++) {
			mp_pos_list[sub_i][i].x = x;
		}
	}

	// for (int i = 1; i < m_line_count; i++) memcpy(mp_pos_list[i], mp_pos_list[0], sizeof(D2D1_POINT_2F) * m_data_limit_count);

	CreateGraphicObject(); // 프로그램에서 사용할 그래픽 객체를 만든다.	

	return 0;
}

void LineGraphWnd::OnDestroy()
{
	CWnd::OnDestroy();

	DestroyGraphicObject();  // 생성된 D2D 객체를 제거
}

void LineGraphWnd::OnDraw()
{
	mp_target->BeginDraw(); // 그림을 그릴 타겟을 얻는다.
	mp_target->Clear(ColorF(1.0000f, 1.0000f, 1.0000f)); // 배경색으로 채운다.

	// 0 값의 위치를 표시할 그리드 선을 출력한다.
	mp_target->DrawLine(Point2F(0, m_half_cy), Point2F((float)m_view_size.width, m_half_cy), mp_center_grid_brush, 3, mp_stroke_style);
	mp_target->DrawRectangle(m_rect, mp_center_grid_brush);  // 그래프의 테두리를 그린다.

	for (int i = 0; i < m_line_count; i++)
	{
		if (*(mp_graph_geo + i)) mp_target->DrawGeometry(*(mp_graph_geo+i), mp_line_brush[i], 3, mp_line_style); // 구성된 그래프를 화면에 출력한다.
	}


	int count = m_data_list.Count();
	for (int i = 0; i < m_line_count; i++) {
		D2D1_POINT_2F* p_pos = mp_pos_list[i], * p_limit = p_pos + count;
		D2D1_ELLIPSE p_data_rect = { {0.0f, 0.0f}, 4.0f, 4.0f };
		while (p_pos < p_limit) {
			p_data_rect.point = *p_pos++;
			mp_target->FillEllipse(p_data_rect, mp_ellipse_brush[i]); // 데이터 위치에 원을 출력한다.
		}
	}

	if (m_selected_index != -1) {
		mp_target->DrawLine(Point2F(mp_pos_list[0][count - m_selected_index].x, 0), Point2F(mp_pos_list[0][count - m_selected_index].x, m_rect.bottom), mp_max_data_brush, 8);
	}

	// 평균값, 최댓값, 최솟값의 위치를 선으로 표시한다.
	//mp_target->DrawLine(Point2F(0, m_avr_pos), Point2F(m_rect.right, m_avr_pos), mp_avr_data_brush, 3, mp_stroke_style);
	//mp_target->DrawLine(Point2F(0, m_max_pos), Point2F(m_rect.right, m_max_pos), mp_max_data_brush, 3, mp_stroke_style);
	//mp_target->DrawLine(Point2F(0, m_min_pos), Point2F(m_rect.right, m_min_pos), mp_min_data_brush, 3, mp_stroke_style);

	if (mp_target->EndDraw()) { // 타겟을 반납한다.
		DestroyGraphicObject(); // 사용하던 그래픽 객체를 제거한다.
		CreateGraphicObject();  // 프로그램에서 사용할 그래픽 객체를 만든다.
	}
}

void LineGraphWnd::OnPaint()
{
	// CPaintDC dc(this); // device context for painting

	::ValidateRect(m_hWnd, NULL);  // WM_PAINT 메시지 플래그를 무효화 시킨다.
	OnDraw();  // 그래프를 그린다.

}

// 프로그램에서 사용할 그래픽 객체를 생성하는 함수
void LineGraphWnd::CreateGraphicObject()
{
	mp_line_brush = new ID2D1SolidColorBrush * [m_line_count];
	mp_ellipse_brush = new ID2D1SolidColorBrush * [m_line_count];

	D2D1_COLOR_F line_color_table[3] = { ColorF(0.1922f, 0.3294f, 0.3608f), ColorF(0.6922f, 0.3294f, 0.3608f), ColorF(0.3922f, 0.3294f, 0.9608f) };
	D2D1_COLOR_F ellipse_color_table[3] = { ColorF(0.1922f, 0.8294f, 0.3608f), ColorF(0.6922f, 0.8294f, 0.3608f), ColorF(0.3922f, 0.8294f, 0.9608f) };

	for (int i = 0; i < m_line_count; i++) {
		mp_line_brush[i] = CreateSolidBrush(line_color_table[i]);
		mp_ellipse_brush[i] = CreateSolidBrush(ellipse_color_table[i]);

		// mp_target->CreateSolidColorBrush(line_color_table[i], &mp_line_brush[i]);
		// mp_target->CreateSolidColorBrush(ellipse_color_table[i], &mp_ellipse_brush[i]);
	}

	// 지정된 색상으로 브러시를 생성한다.
	mp_center_grid_brush = CreateSolidBrush(TW_RGBA(64, 128, 128, 200)); // 0 위치 선 색상
	// mp_target->CreateSolidColorBrush(TW_RGBA(64, 128, 128, 200), &mp_center_grid_brush); // 0 위치 선 색상
	mp_target->CreateSolidColorBrush(ColorF(0.5922f, 0.9216f, 0.8627f, 0.6f), &mp_graph_brush); // 그래프 선 색상
	mp_target->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 1.0f, 1.0f), &mp_data_brush);  // 데이터 위치 색상
	mp_target->CreateSolidColorBrush(ColorF(0.2f, 0.95f, 0.2f, 0.3f), &mp_avr_data_brush); // 평균값 위치 선 색상
	mp_target->CreateSolidColorBrush(ColorF(0.5843f, 0.8078f, 0.8392f, 0.4f), &mp_max_data_brush); // 최댓값 위치 선 색상
	mp_target->CreateSolidColorBrush(ColorF(1.0f, 0.0f, 0.0f, 0.3f), &mp_min_data_brush); // 최솟값 위치 선 색상
	
	// Dot 속성으로 획(stroke) 객체를 생성한다.	
	mp_stroke_style = CreateDotLine();
	mp_line_style = CreateMultiLine();
}

// 프로그램에서 사용하던 그래픽 객체를 제거하는 함수
void LineGraphWnd::DestroyGraphicObject()
{
	for (int i = 0; i < m_line_count; i++) {
		TWD_IRelease(&mp_line_brush[i]);
		TWD_IRelease(&mp_ellipse_brush[i]);
	}
	delete[] mp_line_brush;
	delete[] mp_ellipse_brush;


	// 선 색상을 위해 만들었던 브러시들을 제거한다.
	TWD_IRelease(&mp_center_grid_brush);
	TWD_IRelease(&mp_graph_brush);
	TWD_IRelease(&mp_data_brush);
	TWD_IRelease(&mp_avr_data_brush);
	TWD_IRelease(&mp_max_data_brush);
	TWD_IRelease(&mp_min_data_brush);

	TWD_IRelease(&mp_stroke_style); // 점선 스타일을 위해 만들었던 스타일을 제거한다.
	TWD_IRelease(&mp_line_style); // 점선 스타일을 위해 만들었던 스타일을 제거한다.

	for (int i = 0; i < m_line_count; i++)
	{
		TWD_IRelease(&*(mp_graph_geo+i)); // 그래프 선을 출력하기 위해 만든 기하 도형 객체를 제거한다.
	}

	TWD_IRelease(&mp_target);
}

// 그래프 데이터의 최솟값, 최댓값, 평균값을 구하는 함수
void LineGraphWnd::GetMinMaxAverage()
{
	node *p_node = m_data_list.GetHead(); // 연결 리스트의 첫 노드를 얻는다.
	float total_value = *(float *)p_node->p_data, temp;
	m_max_value = m_min_value = total_value; // 첫 값은 최댓값이자 최솟값이다.
	p_node = p_node->p_next;

	while (p_node) {
		temp = *(float *)p_node->p_data;
		total_value += temp; // 총점을 구한다.
		if (temp > m_max_value) m_max_value = temp; // 최댓값인지 체크
		else if (temp < m_min_value) m_min_value = temp; // 최솟값인지 체크
		p_node = p_node->p_next; // 다음 데이터 위치로 이동한다.
	}
	m_avr_value = total_value / m_data_list.Count(); // 평균값을 구한다.
}

ID2D1PathGeometry* LineGraphWnd::MakeLinePath(int a_index)
{
	node* p_node = m_data_list.GetHead(); // 연결 리스트의 시작 노드를 얻는다.
	D2D1_POINT_2F* p_pos = mp_pos_list[a_index];
	// 첫 번째 데이터의 위치를 선 그리기 시작 위치로 지정한다. (m_scroll_index번째 데이터 기준)
	p_pos->y = m_half_cy - *((float*)p_node->p_data + a_index);
	p_node = p_node->p_next;
	int count = 1;
	p_pos++;
	while (p_node) {
		// 다음 위치의 y 좌표를 계산한다.
		p_pos->y = m_half_cy - *((float*)p_node->p_data + a_index);
		p_pos++;
		p_node = p_node->p_next; // 다음 데이터 위치로 이동한다.
	}

	return CreatePathGeometry(mp_pos_list[a_index], m_data_list.Count());

	/*
	ID2D1PathGeometry* p_geo = NULL;
	if (SUCCEEDED(gp_tw_d2d->GetFactory()->CreatePathGeometry(&p_geo))) {
		ID2D1GeometrySink* p_sink = NULL;
		// 도형을 추가하기 위해 입력을 받을 sink 자원을 연다.
		if (SUCCEEDED(p_geo->Open(&p_sink))) {
			//GetMinMaxAverage(); // 그래프 데이터의 최솟값, 최댓값, 평균값을 구한다.

			float scale = 1.0f;  // 기본 스케일을 지정한다.
			//if (m_auto_scale_mode) { // 자동 스케일이 적용된 경우
			//	float a = abs(m_max_value), b = abs(m_min_value);
			//	scale = (a > b) ? a : b; // 최댓값과 최솟값의 절댓값중에 더 큰값을 구한다.
			//	scale = (m_half_cy - 10) / scale; // 그래프 위쪽 또는 아래쪽에 여백으로 10 지정
			//}

			// 온도 그리기
			node* p_node = m_data_list.GetHead(); // 연결 리스트의 시작 노드를 얻는다.
			D2D1_POINT_2F* p_pos = mp_pos_list[a_index];
			// 첫 번째 데이터의 위치를 선 그리기 시작 위치로 지정한다. (m_scroll_index번째 데이터 기준)
			p_pos->y = m_half_cy - *((float*)p_node->p_data + a_index)* scale;
			// 기하 도형의 시작 점을 지정한다.
			p_sink->BeginFigure(*p_pos++, D2D1_FIGURE_BEGIN_FILLED);
			p_node = p_node->p_next;
			while (p_node) {
				// 다음 위치의 y 좌표를 계산한다.
				p_pos->y = m_half_cy - *((float*)p_node->p_data + a_index) * scale;
				p_sink->AddLine(*p_pos++); // 계산된 좌표를 기하 도형에 추가한다.
				p_node = p_node->p_next; // 다음 데이터 위치로 이동한다.
			}

			p_sink->EndFigure(D2D1_FIGURE_END_OPEN); // 패스를 닫는다.
			p_sink->Close();
			TWD_IRelease(&p_sink);  // 도형 추가를 위해 사용하던 sink 자원 반납

			//m_avr_pos = m_half_cy - m_avr_value * scale; // 평균선의 위치
			//m_max_pos = m_half_cy - m_max_value * scale; // 최댓값 선의 위치
			//m_min_pos = m_half_cy - m_min_value * scale; // 최솟값 선의 위치
		}
	}
	*/
//	return p_geo;
}



// 선 그래프를 구성할 데이터를 추가하는 함수
void LineGraphWnd::AddData(float* ap_data)
{
	float *p_data;
	int count = m_data_list.Count();

	// 데이터의 개수가 m_data_limit_count보다 작을 때는 메모리를 할당해서 데이터를 추가하고
	// m_data_limit_count와 동일해지면 가장 오래된 데이터가 저장된 메모리를 재활용한다.
	if (count < m_data_limit_count) {
		p_data = new float[m_line_count];
		count++;
	} else {
		node *p = m_data_list.GetHead(); // 시작 노드를 얻는다.
		p_data = (float *)p->p_data;  // 시작 노드에 저장된 메모리의 주소를 얻는다.
		m_data_list.DeleteNode(p, 0); // 시작 노드를 지운다. (추가된 메모리는 지우지 않는다.)
	}

	memcpy(p_data, ap_data, sizeof(float) * m_line_count);
	m_data_list.AddNode(p_data); // 노드를 추가한다.

	for (int i = 0; i < m_line_count; i++)
	{
		TWD_IRelease(&*(mp_graph_geo + i)); // 그래프 선을 출력하기 위해 만든 기하 도형 객체를 제거한다.
	}

	if (count >= 2) { // 데이터가 2개 이상이면 그래프를 그린다.
		//ID2D1Factory1 *p_factory = (ID2D1Factory1 *)mp_d2d_core->GetFactory();
		for (int i = 0; i < m_line_count; i++) *(mp_graph_geo + i) = MakeLinePath(i);
		Invalidate(0);  // 윈도우를 갱신하여 구성된 그래프를 화면에 출력한다.
	}
}

// 자동 스케일 여부를 설정하는 함수
void LineGraphWnd::ChangeAutoScale(int a_auto_flag)
{
	m_auto_scale_mode = a_auto_flag;
}

//// Direct2D Core를 설정하는 함수
//void LineGraphWnd::SetD2dCore(TW_D2D_Core *ap_d2d_core)
//{
//	mp_d2d_core = ap_d2d_core;
//}

void LineGraphWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonDblClk(nFlags, point);
}


void LineGraphWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_is_clicked) {
		m_is_clicked = 1;
		SetCapture();
		int start_x = (int)(m_rect.right - 5) - X_INTERVAL * m_data_limit_count; // 그래프 시작 위치
		m_selected_index = (point.x - start_x + X_INTERVAL / 2) / X_INTERVAL;
		Invalidate(0);
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void LineGraphWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_is_clicked) {
		m_selected_index = -1;
		m_is_clicked = 0;
		ReleaseCapture();
		Invalidate(0);
	}

	CWnd::OnLButtonUp(nFlags, point);
}


void LineGraphWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_is_clicked) {
		int start_x = (int)(m_rect.right - 5) - X_INTERVAL * m_data_limit_count; // 그래프 시작 위치
		m_selected_index = (point.x - start_x + X_INTERVAL / 2) / X_INTERVAL;
		Invalidate(0);
	}

	CWnd::OnMouseMove(nFlags, point);
}
