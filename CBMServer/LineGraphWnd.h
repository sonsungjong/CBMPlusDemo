#ifndef _LINE_GRAPH_WND_H_
#define _LINE_GRAPH_WND_H_

#include "TW_DoubleLinkedList.h"

#define X_INTERVAL    12  // ������ ���� x�� �Ÿ�

// LineGraphWnd
class LineGraphWnd : public CWnd, public TWD_View
{
	DECLARE_DYNAMIC(LineGraphWnd)
private:
	//ID2D1Factory *mp_factory; 
	//ID2D1RenderTarget *mp_target;
	//D2D1_RECT_F m_rect;  // �׷����� ��µ� ����

	int m_line_count;
	float m_half_cy = 0.0f; // �׷����� �߰� ���̸� ����ϴ� ����
	float m_avr_value = 0.0f, m_max_value = 0.0f, m_min_value = 0.0f; // ��հ�, �ִ�, �ּڰ�
	float m_avr_pos = 0.0f, m_max_pos = 0.0f, m_min_pos = 0.0f; // ��հ�, �ִ�, �ּڰ��� ��� ��ġ
	int m_data_limit_count = 0; // �������� ����� �� �ִ� �������� �ִ� ����
	int m_auto_scale_mode = 0; // �ڵ� ������ ��� ���� (0:����, 1:�ڵ�)
	int m_selected_index = -1, m_is_clicked = 0;


	TW_DoubleLinkedList m_data_list; // ������ ���� ����� ����Ʈ (�µ�)

	D2D1_POINT_2F** mp_pos_list = NULL; // �����͸� ����� ��ǥ�� ��ġ
	// �׸���, �׷��� �� ��¿� ����� �귯��
	TBRUSH*mp_line_brush, *mp_ellipse_brush;
	TBRUSH mp_center_grid_brush = NULL, mp_graph_brush = NULL;
	// �׷��� ������, �ִ� ���� ��¿� ����� �귯��
	TBRUSH mp_data_brush = NULL, mp_max_data_brush = NULL;
	// �ּڰ� ����, ��հ� ���� ��¿� ����� �귯��
	TBRUSH mp_min_data_brush = NULL, mp_avr_data_brush = NULL;
	// ���� �׸��� ���� DOT ��Ÿ�Ϸ� ����� �� ����� ��Ÿ�� ��ü
	ID2D1StrokeStyle *mp_stroke_style = NULL, *mp_line_style = NULL; 
	// �� �׷����� ������ ���� ���� ��ü
	ID2D1PathGeometry **mp_graph_geo = NULL;
	

public:
	LineGraphWnd(int a_line_count);
	virtual ~LineGraphWnd();
	virtual void OnDraw(); // �� �׷����� ����ϴ� �Լ�
	virtual void CreateGraphicObject();  // ���α׷����� ����� �׷��� ��ü�� �����ϴ� �Լ�
	virtual void DestroyGraphicObject(); // ���α׷����� ����ϴ� �׷��� ��ü�� �����ϴ� �Լ�
	//void SetD2dCore(TW_D2D_Core *ap_d2d_core); // Direct2D Core�� �����ϴ� �Լ�
	ID2D1PathGeometry*MakeLinePath(int a_index);

	void AddData(float *ap_data);  // �� �׷����� ������ �����͸� �߰��ϴ� �Լ� (�µ�, ����)
	void ChangeAutoScale(int a_auto_flag); // �ڵ� ������ ���θ� �����ϴ� �Լ�
	void GetMinMaxAverage();  // �׷��� �������� �ּڰ�, �ִ�, ��հ��� ���ϴ� �Լ�

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

