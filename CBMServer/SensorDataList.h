#ifndef SENSOR_DATA_LIST_BOX_H_
#define SENSOR_DATA_LIST_BOX_H_

#include "TW_ListBox.h"
#include "LineGraphWnd.h"
//#include "VibGraphWnd.h"
/*

�Ͻ� 8byte
�µ� float 4byte
���� char 1byte
��ݰ��ӵ� float 4byte
���� float 4byte
�浵 float 4byte
*/

struct MiData
{
	time_t date;
	float temp;
	int test;
};

struct SensorData
{
	time_t date;
	float temp;
	char humi;
	float acc;
	float lat;
	float lon;
	UINT8 str_date_len, str_temp_len, str_humi_len, str_acc_len, str_lat_len, str_lon_len;			// ��¿�(ȿ��)
	wchar_t str_date[20], str_temp[8], str_humi[8], str_acc[8], str_lat[8], str_lon[8];
};

typedef struct SensorDataFormat
{
	UINT8 type;  // 0: MiData, 1: SensorData
	void* p_data;

	//time_t date;
	//float temp;
	//char humi;
	//float acc;
	//float lat;
	//float lon;
	//UINT8 str_date_len, str_temp_len, str_humi_len, str_acc_len, str_lat_len, str_lon_len;			// ��¿�(ȿ��)
	//wchar_t str_date[20], str_temp[8], str_humi[8], str_acc[8], str_lat[8], str_lon[8];
} SDF;

class SensorDataListBox : public TW_ListBox
{
protected:
	// �۲�
	HFONT mh_font;
	LineGraphWnd* mp_graph;
	LineGraphWnd* mp_vib_graph;
	HPEN mh_grid_pen;
	int m_position[100] = { 6, 170, 240, 290, 360, 410, };

public:
	SensorDataListBox() { mh_font = NULL; }
	virtual void CreateUserObject();			// ������ ���
	virtual void DeleteUserObject();			// ���Ž� ���
	virtual void UserDraw(HDC ah_dc, RECT* ap_rect, void* ap_data);
	virtual void BaseDraw(HDC ah_dc, RECT* ap_rect);

	void ResetSensorData();
	void AddMiData(time_t a_time, float a_temp, int a_test);
	void AddSensorData(char* ap_data, unsigned short a_data_size, int a_set_cursor);
	void SetLineGraph(LineGraphWnd* ap_graph, LineGraphWnd* ap_vib_graph)
	{
		mp_graph = ap_graph;
		mp_vib_graph = ap_vib_graph;
	}

	// ����� ����Ʈ��Ʈ�ѷ� �ѱ� ��Ȳ(���X)
	virtual void DetailInfo(CPoint point) 
	{ 
		CRect r;
		GetClientRect(r);

		if ((r.right - 80) < point.x) {
			GetParent()->PostMessage(26000);				// ����Ʈ�ڽ� �󼼺��� Ŭ��
		}
	}


	//void SetLineGraph(VibGraphWnd* ap_vib_graph)
	//{
	//	mp_vib_graph = ap_vib_graph;
	//}
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif