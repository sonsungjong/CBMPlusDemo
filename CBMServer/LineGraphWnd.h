#ifndef _LINE_GRAPH_WND_H_
#define _LINE_GRAPH_WND_H_

#include "TW_DoubleLinkedList.h"

#define X_INTERVAL    12  // 데이터 간의 x축 거리

// LineGraphWnd
class LineGraphWnd : public CWnd, public TWD_View
{
	DECLARE_DYNAMIC(LineGraphWnd)
private:
	//ID2D1Factory *mp_factory; 
	//ID2D1RenderTarget *mp_target;
	//D2D1_RECT_F m_rect;  // 그래프가 출력될 영역

	int m_line_count;
	float m_half_cy = 0.0f; // 그래프의 중간 높이를 기억하는 변수
	float m_avr_value = 0.0f, m_max_value = 0.0f, m_min_value = 0.0f; // 평균값, 최댓값, 최솟값
	float m_avr_pos = 0.0f, m_max_pos = 0.0f, m_min_pos = 0.0f; // 평균값, 최댓값, 최솟값의 출력 위치
	int m_data_limit_count = 0; // 수평으로 출력할 수 있는 데이터의 최대 개수
	int m_auto_scale_mode = 0; // 자동 스케일 모드 여부 (0:고정, 1:자동)
	int m_selected_index = -1, m_is_clicked = 0;


	TW_DoubleLinkedList m_data_list; // 데이터 값을 기억할 리스트 (온도)

	D2D1_POINT_2F** mp_pos_list = NULL; // 데이터를 출력할 좌표의 위치
	// 그리드, 그래프 선 출력에 사용할 브러시
	TBRUSH*mp_line_brush, *mp_ellipse_brush;
	TBRUSH mp_center_grid_brush = NULL, mp_graph_brush = NULL;
	// 그래프 데이터, 최댓값 라인 출력에 사용할 브러시
	TBRUSH mp_data_brush = NULL, mp_max_data_brush = NULL;
	// 최솟값 라인, 평균값 라인 출력에 사용할 브러시
	TBRUSH mp_min_data_brush = NULL, mp_avr_data_brush = NULL;
	// 수평 그리드 선을 DOT 스타일로 출력할 때 사용할 스타일 객체
	ID2D1StrokeStyle *mp_stroke_style = NULL, *mp_line_style = NULL; 
	// 선 그래프를 구성할 기하 도형 객체
	ID2D1PathGeometry **mp_graph_geo = NULL;
	

public:
	LineGraphWnd(int a_line_count);
	virtual ~LineGraphWnd();
	virtual void OnDraw(); // 선 그래프를 출력하는 함수
	virtual void CreateGraphicObject();  // 프로그램에서 사용할 그래픽 객체를 생성하는 함수
	virtual void DestroyGraphicObject(); // 프로그램에서 사용하던 그래픽 객체를 제거하는 함수
	//void SetD2dCore(TW_D2D_Core *ap_d2d_core); // Direct2D Core를 설정하는 함수
	ID2D1PathGeometry*MakeLinePath(int a_index);

	void AddData(float *ap_data);  // 선 그래프를 구성할 데이터를 추가하는 함수 (온도, 습도)
	void ChangeAutoScale(int a_auto_flag); // 자동 스케일 여부를 설정하는 함수
	void GetMinMaxAverage();  // 그래프 데이터의 최솟값, 최댓값, 평균값을 구하는 함수

	//template<class Interface> void TW_IRelease(Interface **ap_interface_object)
	//{
	//	if (*ap_interface_object != NULL) {
	//		(*ap_interface_object)->Release();
	//		(*ap_interface_object) = NULL;
	//	}
	//}

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#endif

