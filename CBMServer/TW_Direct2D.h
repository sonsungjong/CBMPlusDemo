#ifndef _TW_DIRECT2D_H_
#define _TW_DIRECT2D_H

// 네임스페이스
using namespace D2D1;

#define TW_RGBA(r, g, b, a) ColorF((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, (float)a/255.0f )
typedef ID2D1SolidColorBrush* TBRUSH;

class TWD_View
{
protected:
	// D2D에서 윈도우 클라이언트 영역에 그림을 그릴 객체
	ID2D1RenderTarget* mp_target;
	// 선과 텍스트 출력에 사용할 브러시 자원의 주소를 저장할 변수
	ID2D1SolidColorBrush* mp_user_brush;
	// 문자열 출력시 사용할 글꼴 정보
	IDWriteTextFormat* mp_text_format;
	// 뷰의 크기 정보
	D2D1_SIZE_U m_view_size;
	D2D1_RECT_F m_rect;  // 그래프가 출력될 영역
	// View와 연결된 윈도우 핸들 값
	HWND mh_wnd;
	// 기본 브러시 색상
	D2D1_COLOR_F m_def_color = { 1.0f, 1.0f, 1.0f, 1.0f };							// RGBA
	D2D1_COLOR_F m_bk_color = { 0.0f, 0.0f, 0.0f, 1.0f };							// RGBA
	// 기본으로 생성되는 글꼴의 이름 (맑은 고딕)
	TCHAR m_def_font_name[32];
	// 기본으로 생성되는 글꼴의 크기
	FLOAT m_def_font_size;

public:
	TWD_View();
	virtual ~TWD_View();

	// View를 생성하는 함수
	int CreateD2D(HWND ah_wnd, RECT* ap_rect);
	ID2D1PathGeometry* CreatePathGeometry(D2D1_POINT_2F *ap_pos_list, int a_count);
	ID2D1StrokeStyle* CreateDotLine();
	ID2D1StrokeStyle* CreateMultiLine();
	ID2D1SolidColorBrush* CreateSolidBrush(D2D1_COLOR_F a_color);

	// WM_PAINT 메시지 처리기에서 호출해야할 함수
	virtual void OnUserPaint();
	// View 그리기 작업을 전담할 함수
	virtual void OnDraw() {};
	virtual void CreateGraphicObject() {}   // 프로그램에서 사용할 그래픽 객체를 생성하는 함수
	virtual void DestroyGraphicObject() {}  // 프로그램에서 사용하던 그래픽 객체를 제거하는 함수
	// 뷰를 강제로 갱신하는 함수
	void UpdateView();

	// COM으로 생성된 객체를 해제하는 함수
	template<class Interface> void TWD_IRelease(Interface** ap_interface_object)
	{
		if (*ap_interface_object) {
			(*ap_interface_object)->Release();
			(*ap_interface_object) = 0;
		}
	}

	// 지정한 버튼 영역에 마우스 좌표가 포함되어 있는지 체크하는 함수
	__inline int TW_PtInRectF(D2D1_RECT_F* ap_rect, CPoint* ap_pos) {
		return ap_rect->left <= ap_pos->x && ap_rect->top <= ap_pos->y &&
			ap_rect->right >= ap_pos->x && ap_rect->bottom >= ap_pos->y;
	}


};

class TWD_Core
{
protected:
	// D2D를 구성하는 각종 객체를 생성하는 Factory
	ID2D1Factory* mp_factory;
	// 문자열 출력과 관련된 Factory
	IDWriteFactory* mp_write_factory;

public:
	TWD_Core();
	virtual ~TWD_Core();

	ID2D1Factory* GetFactory();
	IDWriteFactory* GetWriteFactory();
	// COM으로 생성된 객체를 해제하는 함수
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