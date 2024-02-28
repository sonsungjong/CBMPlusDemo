
#include "pch.h"
#include "CBMServer.h"
#include "VibGraphWnd.h"
#include <cmath>

IMPLEMENT_DYNAMIC(VibGraphWnd, CWnd)

// 그래프를 구성하는 실숫값을 제거하는 함수
extern void DeleteCurrentValueData(void* ap_data);

// 생성자
VibGraphWnd::VibGraphWnd() : m_vib_list(DeleteCurrentValueData)
{
}

VibGraphWnd::~VibGraphWnd()
{}

void VibGraphWnd::OnDraw()
{
	mp_target->BeginDraw(); // 그림을 그릴 타겟을 얻는다.
	mp_target->Clear(ColorF(0.3843f, 0.6078f, 0.6392f)); // 배경색으로 채운다.

	// 0 값의 위치를 표시할 그리드 선을 출력한다.
	mp_target->DrawLine(Point2F(0, m_half_cy), Point2F(m_rect.right, m_half_cy), mp_center_grid_brush, 3, mp_stroke_style);
	mp_target->DrawRectangle(m_rect, mp_center_grid_brush);  // 그래프의 테두리를 그린다.

	if (mp_graph_vib_geo) mp_target->DrawGeometry(mp_graph_vib_geo, mp_graph_brush, 3); // 구성된 그래프를 화면에 출력한다.

	int count = m_vib_list.Count();
	D2D1_POINT_2F* p_pos = mp_pos_list, * p_limit = p_pos + count;
	D2D1_ELLIPSE p_data_rect = { {0.0f, 0.0f}, 4.0f, 4.0f };
	while (p_pos < p_limit) {
		p_data_rect.point = *p_pos++;
		mp_target->FillEllipse(p_data_rect, mp_data_brush); // 데이터 위치에 원을 출력한다.
	}

	// 평균값, 최댓값, 최솟값의 위치를 선으로 표시한다.
	mp_target->DrawLine(Point2F(0, m_avr_pos), Point2F(m_rect.right, m_avr_pos), mp_avr_data_brush, 3, mp_stroke_style);
	mp_target->DrawLine(Point2F(0, m_max_pos), Point2F(m_rect.right, m_max_pos), mp_max_data_brush, 3, mp_stroke_style);
	mp_target->DrawLine(Point2F(0, m_min_pos), Point2F(m_rect.right, m_min_pos), mp_min_data_brush, 3, mp_stroke_style);

	if (mp_target->EndDraw()) { // 타겟을 반납한다.
		DestroyGraphicObject(); // 사용하던 그래픽 객체를 제거한다.
		CreateGraphicObject();  // 프로그램에서 사용할 그래픽 객체를 만든다.
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

	// 그래프 출력 영역 지정
	GetClientRect(m_range_rect);
	m_rect.left = (float)(m_range_rect.left);
	m_rect.top = (float)(m_range_rect.top);
	m_rect.right = (float)(m_range_rect.right);
	m_rect.bottom = (float)(m_range_rect.bottom);

	m_data_limit_count = m_range_rect.Width() / X_INTERVAL + 1;					// 가로축 총 출력갯수
	m_half_cy = (float)(m_range_rect.Height() / 2.0f);							// y축에서 0위치 계산
	mp_pos_list = new D2D1_POINT_2F[m_data_limit_count];				// 데이터 저장할 메모리

	// D2D로 그릴 영역
	ID2D1HwndRenderTarget* p_hwnd_target;
	mp_factory->CreateHwndRenderTarget(RenderTargetProperties(), HwndRenderTargetProperties(m_hWnd, SizeU(m_rect.right - m_rect.left, m_rect.bottom - m_rect.top)), &p_hwnd_target);
	mp_target = p_hwnd_target;

	float x = m_rect.left - 5;					// 그래프 시작위치
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
	DestroyGraphicObject();				// D2D 제거
	delete[] mp_pos_list;					// 데이터 좌표 메모리 해제
}


void VibGraphWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	::ValidateRect(m_hWnd, NULL);				// WM_PAINT 무효화
	OnDraw();					// 그래프 그리기
}

// 프로그램에서 사용할 그래픽 객체를 생성하는 함수
void VibGraphWnd::CreateGraphicObject()
{
	// 지정된 색상으로 브러시를 생성한다.
	mp_target->CreateSolidColorBrush(ColorF(0.1922f, 0.3294f, 0.3608f), &mp_center_grid_brush); // 0 위치 선 색상
	mp_target->CreateSolidColorBrush(ColorF(0.5922f, 0.9216f, 0.8627f, 0.6f), &mp_graph_brush); // 그래프 선 색상
	mp_target->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 1.0f, 1.0f), &mp_data_brush);  // 데이터 위치 색상
	mp_target->CreateSolidColorBrush(ColorF(0.2f, 0.95f, 0.2f, 0.3f), &mp_avr_data_brush); // 평균값 위치 선 색상
	mp_target->CreateSolidColorBrush(ColorF(1.0f, 0.0f, 1.0f, 0.3f), &mp_max_data_brush); // 최댓값 위치 선 색상
	mp_target->CreateSolidColorBrush(ColorF(1.0f, 0.0f, 0.0f, 0.3f), &mp_min_data_brush); // 최솟값 위치 선 색상

	// Dot 속성으로 획(stroke) 객체를 생성한다.	
	mp_factory->CreateStrokeStyle(StrokeStyleProperties(D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER, 10.0f,
		D2D1_DASH_STYLE_DOT, 0.0f), NULL, 0, &mp_stroke_style);
}

// 프로그램에서 사용하던 그래픽 객체를 제거하는 함수
void VibGraphWnd::DestroyGraphicObject()
{
	// 선 색상을 위해 만들었던 브러시들을 제거한다.
	TW_IRelease(&mp_center_grid_brush);
	TW_IRelease(&mp_graph_brush);
	TW_IRelease(&mp_data_brush);
	TW_IRelease(&mp_avr_data_brush);
	TW_IRelease(&mp_max_data_brush);
	TW_IRelease(&mp_min_data_brush);

	TW_IRelease(&mp_stroke_style); // 점선 스타일을 위해 만들었던 스타일을 제거한다.
	TW_IRelease(&mp_graph_vib_geo); // 그래프 선을 출력하기 위해 만든 기하 도형 객체를 제거한다.

	TW_IRelease(&mp_target);
	TW_IRelease(&mp_factory);
}

// 그래프 데이터의 최솟값, 최댓값, 평균값을 구하는 함수
void VibGraphWnd::GetMinMaxAverage()
{
	node* p_node = m_vib_list.GetHead(); // 연결 리스트의 첫 노드를 얻는다.
	float total_value = *(float*)p_node->p_data, temp;
	m_max_value = m_min_value = total_value; // 첫 값은 최댓값이자 최솟값이다.
	p_node = p_node->p_next;

	while (p_node) {
		temp = *(float*)p_node->p_data;
		total_value += temp; // 총점을 구한다.
		if (temp > m_max_value) m_max_value = temp; // 최댓값인지 체크
		else if (temp < m_min_value) m_min_value = temp; // 최솟값인지 체크
		p_node = p_node->p_next; // 다음 데이터 위치로 이동한다.
	}
	m_avr_value = total_value / m_vib_list.Count(); // 평균값을 구한다.
}

// 선 그래프를 구성할 데이터를 추가하는 함수
void VibGraphWnd::AddData(float a_data)
{
	float* p_data;
	int count = m_vib_list.Count();

	// 데이터의 개수가 m_data_limit_count보다 작을 때는 메모리를 할당해서 데이터를 추가하고
	// m_data_limit_count와 동일해지면 가장 오래된 데이터가 저장된 메모리를 재활용한다.
	if (count < m_data_limit_count) {
		p_data = new float;
		count++;
	}
	else {
		node* p = m_vib_list.GetHead(); // 시작 노드를 얻는다.
		p_data = (float*)p->p_data;  // 시작 노드에 저장된 메모리의 주소를 얻는다.
		m_vib_list.DeleteNode(p, 0); // 시작 노드를 지운다. (추가된 메모리는 지우지 않는다.)
	}
	*p_data = a_data; // 전달된 값을 메모리에 저장한다.

	m_vib_list.AddNode(p_data); // 노드를 추가한다.

	TW_IRelease(&mp_graph_vib_geo); // 생성되어 있는 그래프의 선 패턴을 지운다.

	if (count >= 2) { // 데이터가 2개 이상이면 그래프를 그린다.
		//ID2D1Factory1 *p_factory = (ID2D1Factory1 *)mp_d2d_core->GetFactory();
		if (SUCCEEDED(mp_factory->CreatePathGeometry(&mp_graph_vib_geo))) {
			ID2D1GeometrySink* p_sink = NULL;
			// 도형을 추가하기 위해 입력을 받을 sink 자원을 연다.
			if (SUCCEEDED(mp_graph_vib_geo->Open(&p_sink))) {
				GetMinMaxAverage(); // 그래프 데이터의 최솟값, 최댓값, 평균값을 구한다.

				float scale = 1.0f;  // 기본 스케일을 지정한다.
				if (m_auto_scale_mode) { // 자동 스케일이 적용된 경우
					float a = abs(m_max_value), b = abs(m_min_value);
					scale = (a > b) ? a : b; // 최댓값과 최솟값의 절댓값중에 더 큰값을 구한다.
					scale = (m_half_cy - 10) / scale; // 그래프 위쪽 또는 아래쪽에 여백으로 10 지정
				}

				// 온도 그리기
				node* p_node = m_vib_list.GetHead(); // 연결 리스트의 시작 노드를 얻는다.
				D2D1_POINT_2F* p_pos = mp_pos_list;
				// 첫 번째 데이터의 위치를 선 그리기 시작 위치로 지정한다. (m_scroll_index번째 데이터 기준)
				p_pos->y = m_half_cy - *(float*)p_node->p_data * scale;
				// 기하 도형의 시작 점을 지정한다.
				p_sink->BeginFigure(*p_pos++, D2D1_FIGURE_BEGIN_FILLED);
				p_node = p_node->p_next;
				while (p_node) {
					// 다음 위치의 y 좌표를 계산한다.
					p_pos->y = m_half_cy - *(float*)p_node->p_data * scale;
					p_sink->AddLine(*p_pos++); // 계산된 좌표를 기하 도형에 추가한다.
					p_node = p_node->p_next; // 다음 데이터 위치로 이동한다.
				}

				p_sink->EndFigure(D2D1_FIGURE_END_OPEN); // 패스를 닫는다.
				p_sink->Close();
				TW_IRelease(&p_sink);  // 도형 추가를 위해 사용하던 sink 자원 반납

				m_avr_pos = m_half_cy - m_avr_value * scale; // 평균선의 위치
				m_max_pos = m_half_cy - m_max_value * scale; // 최댓값 선의 위치
				m_min_pos = m_half_cy - m_min_value * scale; // 최솟값 선의 위치
			}

			
		}
		Invalidate(0);  // 윈도우를 갱신하여 구성된 그래프를 화면에 출력한다.
	}
}

// 자동 스케일 여부를 설정하는 함수
void VibGraphWnd::ChangeAutoScale(int a_auto_flag)
{
	m_auto_scale_mode = a_auto_flag;
}

//// Direct2D Core를 설정하는 함수
//void VibGraphWnd::SetD2dCore(TW_D2D_Core *ap_d2d_core)
//{
//	mp_d2d_core = ap_d2d_core;
//}