#pragma once
#include "TW_DoubleLinkedList.h"

#define X_INTERVAL				12					// 데이터간 X축 거리

class VibGraphWnd : public CWnd
{
	DECLARE_DYNAMIC(VibGraphWnd)
private:
	ID2D1Factory* mp_factory;
	ID2D1RenderTarget* mp_target;
	D2D1_RECT_F m_rect;				// 실제 그래프가 출력될 영역

	float m_half_cy = 0.0f;					// 그래프의 중간높이
	float m_avr_value = 0.0f;				// 평균값
	float m_max_value = 0.0f;				// 최대값
	float m_min_value = 0.0f;				// 최소값

	float m_avr_pos = 0.0f;					// 평균값 출력 위치
	float m_max_pos = 0.0f;				// 최대값 출력 위치
	float m_min_pos = 0.0f;					// 최소값 출력 위치

	int m_data_limit_count = 0;				// 수평으로 출력할 수 있는 데이터의 최대 개수
	int m_auto_scale_mode = 0;				// 자동 스케일 모드 여부 (0:고정, 1:자동)
	D2D1_POINT_2F* mp_pos_list = NULL;				// 데이터를 출력할 좌표 위치
	TW_DoubleLinkedList m_vib_list;					// 데이터값을 기억할 리스트 (진동가속도)

	// 그리드, 그래프 선 출력에 사용할 브러시
	ID2D1SolidColorBrush* mp_center_grid_brush = NULL, * mp_graph_brush = NULL;
	// 그래프 데이터, 최댓값 라인 출력에 사용할 브러시
	ID2D1SolidColorBrush* mp_data_brush = NULL, * mp_max_data_brush = NULL;
	// 최솟값 라인, 평균값 라인 출력에 사용할 브러시
	ID2D1SolidColorBrush* mp_min_data_brush = NULL, * mp_avr_data_brush = NULL;
	// 수평 그리드 선을 DOT 스타일로 출력할 때 사용할 스타일 객체
	ID2D1StrokeStyle* mp_stroke_style = NULL;
	// 선 그래프를 구성할 기하 도형 객체
	ID2D1PathGeometry* mp_graph_vib_geo = NULL;

	CRect m_range_rect;				// 그래프 출력 영역 지정용

public:
	VibGraphWnd();
	virtual ~VibGraphWnd();

	void SetFactory(ID2D1Factory* ap_factory);

	void OnDraw();
	void CreateGraphicObject();
	void DestroyGraphicObject(); // 프로그램에서 사용하던 그래픽 객체를 제거하는 함수
	//void SetD2dCore(TW_D2D_Core *ap_d2d_core); // Direct2D Core를 설정하는 함수 ??

	void AddData(float a_vib);					// 데이터 추가
	void ChangeAutoScale(int a_auto_flag);					// 자동 스케일 여부
	void GetMinMaxAverage();										// 그래프 데이터의 최소, 최대, 평균 구하기

	template<class Interface> void TW_IRelease(Interface** ap_interface_object)
	{
		if (*ap_interface_object != NULL) {
			(*ap_interface_object)->Release();
			(*ap_interface_object) = NULL;
		}
	}


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
};


