#pragma once

// Visible(ǥ��) : True, Center(��� ����) : True

class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)
private:
	TCHAR m_id[256];
	TCHAR m_pw[256];

public:
	LoginDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~LoginDlg();
	TCHAR* GetID();
	TCHAR* GetPW();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
