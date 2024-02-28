#pragma once
#include "TW_DoubleLinkedList.h"

#define X_INTERVAL				12					// �����Ͱ� X�� �Ÿ�

class VibGraphWnd : public CWnd
{
	DECLARE_DYNAMIC(VibGraphWnd)
private:
	ID2D1Factory* mp_factory;
	ID2D1RenderTarget* mp_target;
	D2D1_RECT_F m_rect;				// ���� �׷����� ��µ� ����

	float m_half_cy = 0.0f;					// �׷����� �߰�����
	float m_avr_value = 0.0f;				// ��հ�
	float m_max_value = 0.0f;				// �ִ밪
	float m_min_value = 0.0f;				// �ּҰ�

	float m_avr_pos = 0.0f;					// ��հ� ��� ��ġ
	float m_max_pos = 0.0f;				// �ִ밪 ��� ��ġ
	float m_min_pos = 0.0f;					// �ּҰ� ��� ��ġ

	int m_data_limit_count = 0;				// �������� ����� �� �ִ� �������� �ִ� ����
	int m_auto_scale_mode = 0;				// �ڵ� ������ ��� ���� (0:����, 1:�ڵ�)
	D2D1_POINT_2F* mp_pos_list = NULL;				// �����͸� ����� ��ǥ ��ġ
	TW_DoubleLinkedList m_vib_list;					// �����Ͱ��� ����� ����Ʈ (�������ӵ�)

	// �׸���, �׷��� �� ��¿� ����� �귯��
	ID2D1SolidColorBrush* mp_center_grid_brush = NULL, * mp_graph_brush = NULL;
	// �׷��� ������, �ִ� ���� ��¿� ����� �귯��
	ID2D1SolidColorBrush* mp_data_brush = NULL, * mp_max_data_brush = NULL;
	// �ּڰ� ����, ��հ� ���� ��¿� ����� �귯��
	ID2D1SolidColorBrush* mp_min_data_brush = NULL, * mp_avr_data_brush = NULL;
	// ���� �׸��� ���� DOT ��Ÿ�Ϸ� ����� �� ����� ��Ÿ�� ��ü
	ID2D1StrokeStyle* mp_stroke_style = NULL;
	// �� �׷����� ������ ���� ���� ��ü
	ID2D1PathGeometry* mp_graph_vib_geo = NULL;

	CRect m_range_rect;				// �׷��� ��� ���� ������

public:
	VibGraphWnd();
	virtual ~VibGraphWnd();

	void SetFactory(ID2D1Factory* ap_factory);

	void OnDraw();
	void CreateGraphicObject();
	void DestroyGraphicObject(); // ���α׷����� ����ϴ� �׷��� ��ü�� �����ϴ� �Լ�
	//void SetD2dCore(TW_D2D_Core *ap_d2d_core); // Direct2D Core�� �����ϴ� �Լ� ??

	void AddData(float a_vib);					// ������ �߰�
	void ChangeAutoScale(int a_auto_flag);					// �ڵ� ������ ����
	void GetMinMaxAverage();										// �׷��� �������� �ּ�, �ִ�, ��� ���ϱ�

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


