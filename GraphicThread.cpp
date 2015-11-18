#include "stdafx.h"
#include ".\graphicthread.h"
#include "resource.h"
#include "testWMFDlg.h"
#include "rotatefilter.h"

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


int GraphicThread::Main(WPARAM wParam, LPARAM lParam)
{
	int ret=ERR_OK;
    return ret;
}
//------------------------------------------------
void GraphicThread::DrawShape(AbstractGraphics *shape,BMPanvas* dest,double zoom)
{
	if(!(shape->Draw(dest,zoom)))
	{		
		shape->zOrder=Shapes.Add(shape)+1;								
		Eraser.Add(shape->Rgn);
	}	
}
void GraphicThread::EraseShape(AbstractGraphics *shape,BMPanvas* dest,CRect rect)
{	
	shape->Erase(dest,rect); 
	if(!shape->ActiveMask) delete shape;								
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