// testWMFDlg.h : header file
//

#if !defined(AFX_TESTWMFDLG_H__E0D60B11_97B8_4CD4_A3E6_343FAC31B540__INCLUDED_)
#define AFX_TESTWMFDLG_H__E0D60B11_97B8_4CD4_A3E6_343FAC31B540__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mylistbox.h"
#include "mylistctrl.h"
#include "mylistbox.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ThreadsList.h"

/////////////////////////////////////////////////////////////////////////////
// CTestWMFDlg dialog

class CTestWMFDlg : public CDialog
{	
	int ThreadsPID;
// Construction
public:
	CString SaveListFile;
	CTestWMFDlg(CWnd* pParent = NULL);	// standard constructor	
	ThreadsArray Threads;
	CString InputFldr;
	ThreadsList ThreadsList;
	CString DJVUOutFldr;
	int Errors;
	BOOL LoadOriginal;
	CButton OriginalCheck;

	
// Dialog Data
	//{{AFX_DATA(CTestWMFDlg)
	enum { IDD = IDD_TESTWMF_DIALOG };
//	MyListCtrl	BKGFileList;
	MyListCtrl FRGFileList;
	MyListBox	InfoList;
	CButton	Zone2;
	CButton	Zone3;
	CButton	Zone1;
	int		value1;
	int		value2;
	int		threshold;
	double	GBsigma;
	int		GBn;
	int		bckg;
	double	angle;
	double	time;
	BOOL	interp;
	BOOL	edges;
	int		edge_level;
	int		errors;
	int		canvasW;
	int		canvasH;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestWMFDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PostNcDestroy();	
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestWMFDlg)	
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	afx_msg  LRESULT OnPrint(WPARAM wParam, LPARAM lParam );
	afx_msg  LRESULT OnWaiting(WPARAM wParam, LPARAM lParam );	
	afx_msg  LRESULT OnPrinterLock(WPARAM wParam, LPARAM lParam );	
	afx_msg  LRESULT OnPrintDialog(WPARAM wParam, LPARAM lParam );	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSaveList();
	afx_msg  void OnOriginal();
	afx_msg void OnSelectAll();	
	afx_msg void OnButton1();

	afx_msg void OnEnChangeEdit10();

	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedCheck3();

	afx_msg void OnBnClickedCheck1();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnEnKillfocusEdit5();
	afx_msg void OnBnClickedButton4();
	
	
	afx_msg void OnBnClickedButton16();
	
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg  LRESULT OnThreadListUpdate(WPARAM wParam, LPARAM lParam );
//	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton3();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTWMFDLG_H__E0D60B11_97B8_4CD4_A3E6_343FAC31B540__INCLUDED_)
