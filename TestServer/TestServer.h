
// TestServer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestServerApp:
// See TestServer.cpp for the implementation of this class
//
#define TESTER				1

class CTestServerApp : public CWinApp
{
public:
	CTestServerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTestServerApp theApp;
