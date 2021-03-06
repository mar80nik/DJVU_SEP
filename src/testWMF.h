#pragma once

#include "resource.h"		// main symbols
#include "MessageInspector.h"

#define MAINAPP (((CTestWMFApp*)AfxGetApp()))

/////////////////////////////////////////////////////////////////////////////
// CTestWMFApp:
// See testWMF.cpp for the implementation of this class
//
#include "graphicthread.h"

#define GraphID 345
extern CEvent Terminator1;
extern GraphicThread *Graph;

class CTestWMFApp : public CWinApp
{
public:
	CTestWMFApp();

	HACCEL accel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestWMFApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestWMFApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
//	afx_msg  void OnUpdate(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnNextPic(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnPrevPic(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnInit(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnPrint(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnWaiting(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnGetPath(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnCommand(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnUpdate1(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnPrinterLock(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnThreadListUpdate(WPARAM wParam, LPARAM lParam );
	afx_msg  void OnPrinterDialog(WPARAM wParam, LPARAM lParam );	


	afx_msg void OnSelectAll();	
	DECLARE_MESSAGE_MAP()
};

