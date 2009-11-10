// UCamDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CUCamDemoApp:
// See UCamDemo.cpp for the implementation of this class
//

class CUCamDemoApp : public CWinApp
{
public:
	CUCamDemoApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CUCamDemoApp theApp;