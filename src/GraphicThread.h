#pragma once
#include "mythread.h"

struct GraphModes 
{
	enum {NONE=0, FRG=1, BCKG=1<<1, BW=1<<2, BLUR=1<<3, ROTLINE=1<<4};
};

class Message: public CSingleLock
{
public:	
    int m1,m2,m3;	
	//CMutex* obj;

	Message(CMutex* _obj): CSingleLock(_obj)  
	{
		m1=m2=m3=0; //obj=_obj;		
	};	
};

enum GraphicMessages
{	
	UM_LOAD_PIC=WM_APP  + 176, UM_UPDATE,UM_UPDATE1, UM_NEXT_PIC, UM_PREV_PIC, UM_ORIGINAL,
	UM_UNLOAD_PIC, UM_BLUR, UM_ROTATE, UM_ON_INIT, UM_PRINT, UM_SELECT_ALL, UM_WAITING, UM_GET_PATH, UM_BW, 
	UM_PRINTERLOCK, UM_UPDATE_THREAD_LIST, UM_PRINT_DIALOG
};

class CTestWMFDlg;

class GraphicThread :	public MyThread
{
	friend class OutputWnd;
	DECLARE_DYNCREATE(GraphicThread)	
private:
	int MainRotLine(Message*);
protected:
	CArray<CRect,CRect> Eraser;

	virtual void Main(WPARAM wParam, LPARAM lParam);
	virtual ~GraphicThread(void);
	virtual BOOL InitInstance();
	virtual int ExitInstance();	
public:
	CWnd* ParentWnd;
	HWND ParentHWND;
	int bckg;
	HACCEL accel; bool StopWaiting; CString DJVUOutFldr;

	virtual int TerminateThread();
	GraphicThread(void);		

	afx_msg void OnPrint(WPARAM wParam, LPARAM lParam );	


	DECLARE_MESSAGE_MAP()
};
