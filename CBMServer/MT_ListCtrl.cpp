// MT_ListCtrl.cpp : implementation file
//

#include "pch.h"
#include "CBMServerDlg.h"
#include "MT_ListCtrl.h"


// MT_ListCtrl

IMPLEMENT_DYNAMIC(MT_ListCtrl, CListCtrl)

MT_ListCtrl::MT_ListCtrl()
{
	m_font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_MODERN, _T("���� ���"));

}

MT_ListCtrl::~MT_ListCtrl()
{
}

void MT_ListCtrl::SetData()
{
	SendMessage(WM_SETFONT, (WPARAM)m_font, (LPARAM)TRUE);

	LVITEM item;
	::ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	item.pszText = _T("TEST ITEM");
	item.iItem = 5;
	item.iImage = 1;
	item.state = LVIS_SELECTED | LVIS_FOCUSED;

	// ���� ��� �߰� �� �÷� �߰�
	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);							// ���� �߰�
	InsertColumn(0, _T("No"), LVCFMT_CENTER, 40);
	InsertColumn(1, _T("Name"), LVCFMT_CENTER, 100);
	InsertColumn(2, _T("Kor"), LVCFMT_CENTER, 60);
	InsertColumn(3, _T("Eng"), LVCFMT_CENTER, 60);
	InsertColumn(4, _T("Math"), LVCFMT_CENTER, 60);
	InsertColumn(5, _T("Sum"), LVCFMT_CENTER, 100);
	InsertColumn(6, _T("Avg"), LVCFMT_CENTER, 60);
	InsertColumn(7, _T("�� ����"), LVCFMT_CENTER, 100);
	ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	CString strItem;
	// �׸� ����
	for (int i = 0; i < 30; i++) {
		strItem.Format(_T("%d"), i + 1);
		InsertItem(i, strItem);
		SetItem(i, 1, LVIF_TEXT, _T("item"), NULL, NULL, NULL, NULL);
		SetItem(i, 2, LVIF_TEXT, _T("33"), NULL, NULL, NULL, NULL);
		SetItem(i, 3, LVIF_TEXT, _T("44"), NULL, NULL, NULL, NULL);
		SetItem(i, 4, LVIF_TEXT, _T("55"), NULL, NULL, NULL, NULL);
		SetItem(i, 5, LVIF_TEXT, _T("22"), NULL, NULL, NULL, NULL);
		SetItem(i, 6, LVIF_TEXT, _T("10.1"), NULL, NULL, NULL, NULL);
		// SetBtn 7
	}
}

void MT_ListCtrl::SetData2()
{
	SendMessage(WM_SETFONT, (WPARAM)m_font, (LPARAM)TRUE);

	LVITEM item;
	::ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	item.pszText = _T("LIG");
	item.iItem = 5;
	item.iImage = 1;
	item.state = LVIS_SELECTED | LVIS_FOCUSED;

	// ���� ��� �߰� �� �÷� �߰�
	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);							// ���� �߰�
	InsertColumn(0, _T("No"), LVCFMT_LEFT, 40);
	InsertColumn(1, _T("�δ��"), LVCFMT_CENTER, 100);
	InsertColumn(2, _T("����ȣ"), LVCFMT_CENTER, 160);
	InsertColumn(3, _T("�Ϸù�ȣ"), LVCFMT_CENTER, 160);
	InsertColumn(4, _T("ź��"), LVCFMT_CENTER, 100);
	InsertColumn(5, _T("�µ�"), LVCFMT_CENTER, 100);
	InsertColumn(6, _T("����"), LVCFMT_CENTER, 100);
	InsertColumn(7, _T("��ݷ�"), LVCFMT_CENTER, 100);
	InsertColumn(8, _T("����"), LVCFMT_CENTER, 100);
	InsertColumn(9, _T("�̻󿩺�"), LVCFMT_CENTER, 100);
	InsertColumn(10, _T("����������"), LVCFMT_CENTER, 160);
	InsertColumn(11, _T("����"), LVCFMT_CENTER, 60);
	InsertColumn(12, _T("�� ����"), LVCFMT_CENTER, 100);
	ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	CString strItem;
	// �׸� ���� 30��
	for (int i = 0; i < 30; i++) {
		strItem.Format(_T("%d"), i + 1);
		InsertItem(i, strItem);
		SetItem(i, 1, LVIF_TEXT, _T("�ѱ�"), NULL, NULL, NULL, NULL);
		SetItem(i, 2, LVIF_TEXT, _T("1430010876337"), NULL, NULL, NULL, NULL);
		SetItem(i, 3, LVIF_TEXT, _T("JN17148851"), NULL, NULL, NULL, NULL);
		SetItem(i, 4, LVIF_TEXT, _T("õ��"), NULL, NULL, NULL, NULL);
		SetItem(i, 5, LVIF_TEXT, _T("-36.5��"), NULL, NULL, NULL, NULL);
		SetItem(i, 6, LVIF_TEXT, _T("20��"), NULL, NULL, NULL, NULL);
		SetItem(i, 7, LVIF_TEXT, _T("9.8"), NULL, NULL, NULL, NULL);
		SetItem(i, 8, LVIF_TEXT, _T("����"), NULL, NULL, NULL, NULL);
		SetItem(i, 9, LVIF_TEXT, _T("�̻�"), NULL, NULL, NULL, NULL);
		SetItem(i, 10, LVIF_TEXT, _T("2028-03-21"), NULL, NULL, NULL, NULL);
		SetItem(i, 11, LVIF_TEXT, _T("C"), NULL, NULL, NULL, NULL);
		// SetBtn 12
	}
}

void MT_ListCtrl::DrawCtrl(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	int index = lpDrawItemStruct->itemID;

	if (index >= 0 && index < GetItemCount())
	{
		CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);
		CRect r = lpDrawItemStruct->rcItem;

		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			// ���� ���ο� ���� ���ڻ��� ������
			dc->SetTextColor(RGB(30, 30, 250));
			dc->FillSolidRect(r, RGB(188, 231, 241));
		}
		else
		{
			// ������ �ȵ� ���
			dc->SetTextColor(RGB(0, 0, 0));
			dc->FillSolidRect(r, RGB(255, 255, 255));
		}

		LV_COLUMN column_data;
		column_data.mask = LVCF_WIDTH;

		CString str;
		// �� �÷��� ���� 5��ŭ ������ ���� ���ڿ��� ����Ѵ�
		// ����Ʈ ��Ʈ���� ���� �ִ� ��� ��Ʈ���� �����͸� �� �÷����� ���Ѵ�
		int blank = 10;
		int header_count = GetHeaderCtrl()->GetItemCount();

		// �ݺ����� ���� TextOut���� �׸��� (���� ������ �÷��� �ٸ� ��Ʈ�ѷ� �ְ� ������ -1)
		for (int i = 0; i < header_count; i++)
		{
			CRect rcSubItem;
			GetSubItemRect(index, i, LVIR_BOUNDS, rcSubItem);

			// �����͸� ��´�
			str = GetItemText(index, i);
			if (i == 0) {
				dc->TextOut(r.left + blank, r.top + blank, str);
			}
			else if (i == header_count - 1)				// ������ �÷�
			{
				COLORREF old_color = dc->SetTextColor(RGB(255, 255, 255));
				dc->FillSolidRect(&rcSubItem, RGB(0, 100, 255));
				dc->DrawText(_T("�� ����"), &rcSubItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				dc->SetTextColor(old_color);
				m_last_column = rcSubItem;
			}
			else {
				dc->DrawText(str, &rcSubItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}
		}
	}
}

// Ư�� ������ Ŭ���ϸ� �̺�Ʈ �߻�
void MT_ListCtrl::DetailInfo(CPoint point)
{
	if (m_last_column.left < point.x && m_last_column.right > point.x && m_last_column.bottom > point.y) {
		GetParent()->PostMessage(26001);			// �θ� ��ȭ���ڿ� �޽��� ������
	}
}


BEGIN_MESSAGE_MAP(MT_ListCtrl, CListCtrl)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void MT_ListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//DetailInfo(point);
	CListCtrl::OnLButtonUp(nFlags, point);
}


void MT_ListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Add your code to draw the specified item
	DrawCtrl(lpDrawItemStruct);
}


void MT_ListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	UINT uFlags;
	int nItem = HitTest(point, &uFlags);				// ���õ� �׸��� �ε����� ��´�

	if ((nItem != -1) && (uFlags & LVHT_ONITEM))				// 
	{
		// �ش� row�� ���õǾ����� Ȯ��
		bool bSelected = (GetItemState(nItem, LVIS_SELECTED) & LVIS_SELECTED) != 0;					// �׸��� ���¸� ����

		if (bSelected)
		{
			// ���õ� row�� ���� ���� ����
			DetailInfo(point);
		}
	}
	
	CListCtrl::OnLButtonDown(nFlags, point);
}


void MT_ListCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnTimer(nIDEvent);
	
}
