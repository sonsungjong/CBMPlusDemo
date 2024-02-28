#include "pch.h"
#include "HeaderGrid.h"
#include "TW_Direct2D.h"
#include "SensorDataList.h"

// 리스트 박스가 생성될 때 호출되는 함수
void SensorDataListBox::CreateUserObject()
{
	// 펜 색 설정
	m_focus_pen_color = m_select_pen_color = RGB(112, 127, 154);
	mh_grid_pen = CreatePen(PS_SOLID, 1, RGB(92, 107, 134));

	// 폰트 생성 (중요!!)
	mh_font = ::CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("consolas"));
	SetItemHeight(0, 21);				// 리스트박스 항목의 높이를 설정 (중요!!)
}

// 리스트 박스에 등록된 바이너리 데이터 정보를 모두 제거
void SensorDataListBox::ResetSensorData()
{
	SDF* p_data;
	int count = GetCount();				// 등록된 항목 수를 얻는다
	for (int i = 0; i < count; i++) {
		p_data = (SDF*)GetItemDataPtr(i);				// i 번째 항목의 메모리 주소를 얻는다
		if (p_data->type == 1) {
			delete (SensorData *)p_data->p_data;
		} else if (p_data->type == 0) {
			delete (MiData*)p_data->p_data;
		}
		delete p_data;				// 해제한다
	}
	ResetContent();				// 항목 모두 제거
}

// 리스트 박스가 제거될 때 호출되는 함수
void SensorDataListBox::DeleteUserObject()
{
	ResetSensorData();					// 항목 모두 제거
	if (mh_font != NULL) { ::DeleteObject(mh_font); }				// 출력용 GDI Object 제거
	DeleteObject(mh_grid_pen);
}

// 출력 정보를 그리는 함수
void SensorDataListBox::UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data)
{
	// 생성한 글꼴 적용
	::SelectObject(ah_dc, mh_font);
	// 글자 흰색
	::SetTextColor(ah_dc, RGB(200, 228, 255));
	// 글자 배경색 투명
	::SetBkMode(ah_dc, TRANSPARENT);

	SelectObject(ah_dc, mh_grid_pen);

	SDF* p_data = (SDF*)ap_data;
	if (p_data->type == 1) {
		SensorData* p_sensor = (SensorData*)p_data->p_data;

		::TextOut(ah_dc, ap_rect->left + m_position[0], ap_rect->top + 1, p_sensor->str_date, p_sensor->str_date_len);
		::TextOut(ah_dc, ap_rect->left + m_position[1], ap_rect->top + 1, p_sensor->str_temp, p_sensor->str_temp_len);
		::TextOut(ah_dc, ap_rect->left + m_position[2], ap_rect->top + 1, p_sensor->str_humi, p_sensor->str_humi_len);
		::TextOut(ah_dc, ap_rect->left + m_position[3], ap_rect->top + 1, p_sensor->str_acc, p_sensor->str_acc_len);
		::TextOut(ah_dc, ap_rect->left + m_position[4] + 5, ap_rect->top + 1, p_sensor->str_lat, p_sensor->str_lat_len);
		::TextOut(ah_dc, ap_rect->left + m_position[5] + 5, ap_rect->top + 1, p_sensor->str_lon, p_sensor->str_lon_len);
	}
	else 	if (p_data->type == 0) {
		MiData* p_mi = (MiData*)p_data->p_data;
		::TextOut(ah_dc, ap_rect->left + 6, ap_rect->top + 1, L"군수 데이터", 6);
	}

	SelectObject(ah_dc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(ah_dc, RGB(0, 0, 190));
	Rectangle(ah_dc, ap_rect->right - 85, ap_rect->top + 1, ap_rect->right - 5, ap_rect->bottom - 2);
	::SetTextColor(ah_dc, RGB(255, 255, 0));
	::TextOut(ah_dc, ap_rect->right - 80, ap_rect->top + 1, L"상세 정보", 5);

	MoveToEx(ah_dc, 0, ap_rect->bottom - 2, NULL);
	LineTo(ah_dc, ap_rect->right, ap_rect->bottom -2);

	MoveToEx(ah_dc, ap_rect->left + m_position[0], ap_rect->top + 1, NULL);
	LineTo(ah_dc, ap_rect->left + m_position[0], ap_rect->bottom - 2);

	MoveToEx(ah_dc, ap_rect->left + m_position[1], ap_rect->top + 1, NULL);
	LineTo(ah_dc, ap_rect->left + m_position[1], ap_rect->bottom - 2);

	MoveToEx(ah_dc, ap_rect->left + m_position[2], ap_rect->top + 1, NULL);
	LineTo(ah_dc, ap_rect->left + m_position[2], ap_rect->bottom - 2);

	MoveToEx(ah_dc, ap_rect->left + m_position[3], ap_rect->top + 1, NULL);
	LineTo(ah_dc, ap_rect->left + m_position[3], ap_rect->bottom - 2);

	MoveToEx(ah_dc, ap_rect->left + m_position[4], ap_rect->top + 1, NULL);
	LineTo(ah_dc, ap_rect->left + m_position[4], ap_rect->bottom - 2);

	MoveToEx(ah_dc, ap_rect->left + m_position[5], ap_rect->top + 1, NULL);
	LineTo(ah_dc, ap_rect->left + m_position[5], ap_rect->bottom - 2);

	MoveToEx(ah_dc, ap_rect->right - 90, ap_rect->top + 1, NULL);
	LineTo(ah_dc, ap_rect->right - 90, ap_rect->bottom - 2);
}

// 배경을 그릴 함수
void SensorDataListBox::BaseDraw(HDC ah_dc, RECT* ap_rect)
{
	// 전체 갱신, 포커스 갱신, 선택 갱신
	if (mp_last_lpds->itemAction & 0x07)				// 0x07 == ODA_DRAWENTIRE|ODA_FOCUS|ODA_SELECT
	{
		::SelectObject(ah_dc, ::GetStockObject(DC_BRUSH));
		::SelectObject(ah_dc, ::GetStockObject(DC_PEN));
		// 선택된 상태인지 체크
		if (mp_last_lpds->itemState & ODS_SELECTED)
		{
			// 선택상태 펜으로 변경
			::SetDCPenColor(ah_dc, m_select_pen_color);
		}
		else {
			// 선택된 상태가 아니면 배경색과 동일한 펜색
			::SetDCPenColor(ah_dc, m_bk_pen_color);
		}
		::SetDCBrushColor(ah_dc, m_bk_brush_color);
		// 배경그리기
		::Rectangle(ah_dc, ap_rect->left, ap_rect->top, ap_rect->right, ap_rect->bottom);
	}
}

void SensorDataListBox::AddMiData(time_t a_time, float a_temp, int a_test)
{
	SDF* p = new SDF;
	p->type = 0;
	MiData* p_mi = new MiData;
	p->p_data = p_mi;

	p_mi->date = a_time;
	p_mi->temp = a_temp;
	p_mi->test = a_test;

	int index = InsertString(-1, _T(""));
	SetItemDataPtr(index, p);

}

// 한 줄 추가
void SensorDataListBox::AddSensorData(char* ap_data, unsigned short a_data_size, int a_set_cursor)
{
	FILE* p_file = NULL;

	if (0 == fopen_s(&p_file, ".\\data\\cbm.dat", "a+b")) {  
		

		fwrite(ap_data, 1, a_data_size, p_file);          // 할당된 메모리에 파일 내용을 복사한다.
		fclose(p_file);  // 파일을 닫는다!
	}
	int count = *ap_data++;
	int index = 0;
	SensorData* p_sensor;
	for (int i = 0; i < count; i++) {
		SDF* p = new SDF;
		p->type = 1;
		p_sensor = new SensorData;
		p->p_data = p_sensor;

		p_sensor->date = *(time_t*)ap_data;
		ap_data += sizeof(time_t);

		p_sensor->temp = *(float*)ap_data;
		ap_data += sizeof(float);

		p_sensor->humi = *ap_data++;

		p_sensor->acc = *(float*)ap_data;
		ap_data += sizeof(float);

		p_sensor->lat = *(float*)ap_data;
		ap_data += sizeof(float);

		p_sensor->lon = *(float*)ap_data;
		ap_data += sizeof(float);

		tm tm_time;
		// 서비스 시작 일을 날짜 형식으로 값을 얻는다.
		localtime_s(&tm_time, &p_sensor->date);

		p_sensor->str_date_len = swprintf_s(p_sensor->str_date, 20, L"%04d-%02d-%02d %02d:%02d", tm_time.tm_year + 1900, tm_time.tm_mon + 1,
			tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min);

		//int len = swprintf_s(str, 128, L"%.1f, %d%%, %.1f, %.1f, %.1f", p_data->temp, p_data->humi, p_data->acc, p_data->lat, p_data->lon);
		//::TextOut(ah_dc, ap_rect->left + 180, ap_rect->top + 1, str, len);
		p_sensor->str_temp_len = swprintf_s(p_sensor->str_temp, 8, L"%5.1f", p_sensor->temp);
		p_sensor->str_humi_len = swprintf_s(p_sensor->str_humi, 8, L"%3d%%", p_sensor->humi);
		p_sensor->str_acc_len = swprintf_s(p_sensor->str_acc, 8, L"%5.1f", p_sensor->acc);
		p_sensor->str_lat_len = swprintf_s(p_sensor->str_lat, 8, L"%.1f", p_sensor->lat);
		p_sensor->str_lon_len = swprintf_s(p_sensor->str_lon, 8, L"%.1f", p_sensor->lon);

		index = InsertString(-1, _T(""));
		SetItemDataPtr(index, p);

		float temp[2] = { p_sensor->temp, (float)p_sensor->humi };
		mp_graph->AddData(temp);				// 온습도 그래프
		temp[0] = p_sensor->acc;
		mp_vib_graph->AddData(temp);									// 진동 그래프
	}
	if(a_set_cursor) SetCurSel(index);
}


LRESULT SensorDataListBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == 32001) {
		HID* p = (HID*)lParam;
		int i, position = 0;
		for (i = 0; i < wParam; i++) {
			m_position[i] = position;
			position += p->width;
			p++;
		}
		m_position[i] = position;
		Invalidate();
	}

	return TW_ListBox::WindowProc(message, wParam, lParam);
}
