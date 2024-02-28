
// CBMPlus.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "CBMPlus.h"
#include "CBMPlusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCBMPlusApp

BEGIN_MESSAGE_MAP(CCBMPlusApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCBMPlusApp construction

CCBMPlusApp::CCBMPlusApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCBMPlusApp object

CCBMPlusApp theApp;


// CCBMPlusApp initialization

BOOL CCBMPlusApp::InitInstance()
{
	CWinApp::InitInstance();
	CCBMPlusDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

