#pragma once


typedef struct ListData
{
	UINT8 type;  // 0: 군수, 1: 환경센서, 2: 분석, 3: 기타...
	void* p_data;
} LD;

class MT_ListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(MT_ListCtrl)

public:
	MT_ListCtrl();
	virtual ~MT_ListCtrl();

	void SetData(EnvironmentModel* ap);
	void SetData2();

	void DrawCtrl(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DetailInfo(CPoint point);
	int m_prev_index = -2;

protected:
	HFONT m_font;
	CRect m_last_column;
	CPoint m_last_point;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


