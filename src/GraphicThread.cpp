#include "stdafx.h"
#include ".\graphicthread.h"
#include "resource.h"
#include "testWMFDlg.h"

IMPLEMENT_DYNCREATE(GraphicThread, MyThread)

BEGIN_MESSAGE_MAP(GraphicThread, MyThread)
	//{{AFX_MSG_MAP(VoltmeterThread)
	ON_THREAD_MESSAGE(UM_PRINT,OnPrint)	
	//}}AFX_MSG_MAP
	// Standard file based document commands	
END_MESSAGE_MAP()


GraphicThread::GraphicThread(void)
{
    bckg=0;	
}

GraphicThread::~GraphicThread(void)
{
}


BOOL GraphicThread::InitInstance()
{	
	ParentWnd=CWnd::FromHandle(ParentHWND);
	accel=LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));
	return MyThread::InitInstance();
}

int GraphicThread::ExitInstance()
{	
	return MyThread::ExitInstance();
}


void GraphicThread::Main(WPARAM wParam, LPARAM lParam)
{
}

int GraphicThread::MainRotLine(Message*)
{
	int ret=0;
    return ret;
}


void GraphicThread::OnPrint(WPARAM wParam, LPARAM lParam )
{	

}

int GraphicThread::TerminateThread()
{
    StopWaiting=true;
	return MyThread::TerminateThread();
}