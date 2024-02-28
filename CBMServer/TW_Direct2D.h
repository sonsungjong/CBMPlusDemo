#ifndef _TW_DIRECT2D_H_
#define _TW_DIRECT2D_H

// ���ӽ����̽�
using namespace D2D1;

#define TW_RGBA(r, g, b, a) ColorF((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, (float)a/255.0f )
typedef ID2D1SolidColorBrush* TBRUSH;

class TWD_View
{
protected:
	// D2D���� ������ Ŭ���̾�Ʈ ������ �׸��� �׸� ��ü
	ID2D1RenderTarget* mp_target;
	// ���� �ؽ�Ʈ ��¿� ����� �귯�� �ڿ��� �ּҸ� ������ ����
	ID2D1SolidColorBrush* mp_user_brush;
	// ���ڿ� ��½� ����� �۲� ����
	IDWriteTextFormat* mp_text_format;
	// ���� ũ�� ����
	D2D1_SIZE_U m_view_size;
	D2D1_RECT_F m_rect;  // �׷����� ��µ� ����
	// View�� ����� ������ �ڵ� ��
	HWND mh_wnd;
	// �⺻ �귯�� ����
	D2D1_COLOR_F m_def_color = { 1.0f, 1.0f, 1.0f, 1.0f };							// RGBA
	D2D1_COLOR_F m_bk_color = { 0.0f, 0.0f, 0.0f, 1.0f };							// RGBA
	// �⺻���� �����Ǵ� �۲��� �̸� (���� ����)
	TCHAR m_def_font_name[32];
	// �⺻���� �����Ǵ� �۲��� ũ��
	FLOAT m_def_font_size;

public:
	TWD_View();
	virtual ~TWD_View();

	// View�� �����ϴ� �Լ�
	int CreateD2D(HWND ah_wnd, RECT* ap_rect);
	ID2D1PathGeometry* CreatePathGeometry(D2D1_POINT_2F *ap_pos_list, int a_count);
	ID2D1StrokeStyle* CreateDotLine();
	ID2D1StrokeStyle* CreateMultiLine();
	ID2D1SolidColorBrush* CreateSolidBrush(D2D1_COLOR_F a_color);

	// WM_PAINT �޽��� ó���⿡�� ȣ���ؾ��� �Լ�
	virtual void OnUserPaint();
	// View �׸��� �۾��� ������ �Լ�
	virtual void OnDraw() {};
	virtual void CreateGraphicObject() {}   // ���α׷����� ����� �׷��� ��ü�� �����ϴ� �Լ�
	virtual void DestroyGraphicObject() {}  // ���α׷����� ����ϴ� �׷��� ��ü�� �����ϴ� �Լ�
	// �並 ������ �����ϴ� �Լ�
	void UpdateView();

	// COM���� ������ ��ü�� �����ϴ� �Լ�
	template<class Interface> void TWD_IRelease(Interface** ap_interface_object)
	{
		if (*ap_interface_object) {
			(*ap_interface_object)->Release();
			(*ap_interface_object) = 0;
		}
	}

	// ������ ��ư ������ ���콺 ��ǥ�� ���ԵǾ� �ִ��� üũ�ϴ� �Լ�
	__inline int TW_PtInRectF(D2D1_RECT_F* ap_rect, CPoint* ap_pos) {
		return ap_rect->left <= ap_pos->x && ap_rect->top <= ap_pos->y &&
			ap_rect->right >= ap_pos->x && ap_rect->bottom >= ap_pos->y;
	}


};

class TWD_Core
{
protected:
	// D2D�� �����ϴ� ���� ��ü�� �����ϴ� Factory
	ID2D1Factory* mp_factory;
	// ���ڿ� ��°� ���õ� Factory
	IDWriteFactory* mp_write_factory;

public:
	TWD_Core();
	virtual ~TWD_Core();

	ID2D1Factory* GetFactory();
	IDWriteFactory* GetWriteFactory();
	// COM���� ������ ��ü�� �����ϴ� �Լ�
	template<class Interface> void TWD_IRelease(Interface** ap_interface_object)
	{
		if (*ap_interface_object) {
			(*ap_interface_object)->Release();
			(*ap_interface_object) = 0;
		}
	}
};

extern TWD_Core* gp_tw_d2d;

#endif