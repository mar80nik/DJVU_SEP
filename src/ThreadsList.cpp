#include "stdafx.h"
#include "threadslist.h"
#include "graphicthread.h"
#include "testWMFDlg.h"
#include "Psapi.h"
#include "atlimage.h"

void GetRandomString(CString &T)
{
	T=""; CString T1; srand( (unsigned)time( NULL ) ); BYTE code, code1; double t, t1;
	for(int i=0;i<4;i++)
	{	
		t=rand()/((double)RAND_MAX); t1=rand()/((double)RAND_MAX);
		code=(BYTE)(97+25*t); code1=(BYTE)(65+25*t1);
		T1.Format("%c%c",code, code1); T+=T1;
	}		   
}

ThreadParams::ThreadParams(): Terminator(true,true) 
{
	Name="NoName"; StopWaiting=true; Parent=0;  PID=-1; Status=Preparing; 
	Time=TimeLeft=sec(-1);
};

CString ThreadParams::GetStatusText()
{
	CString T;
	switch(Status)
	{
	case Preparing: T="Preparing"; break;
	case Working: T="Working"; break;
	case Finished: T="Finished"; break;
	case Waiting: T="Waiting"; break;
	case Rename: T="Rename"; break;
	case Error: 
	default: T="Error"; break; 	
	}
    return T;
}

void ThreadParams::PostParentMessage(UINT Msg, StatMessage* msg)
{
	msg->ThreadPID=PID; 
	Parent->PostThreadMessage(Msg,(WPARAM)msg,0);	
}

void ThreadParams::FormatElement(int subItem, CString& T)
{
	switch (subItem)
	{
	case Name_COL: T=Name; break;
	case Start_COL: T.Format("%02d:%02d:%02d",
						StartTime.GetHour(),StartTime.GetMinute(), StartTime.GetSecond()); break;
	case Time_COL: T=ConvTimeToStr(Time); break;
	case Left_COL: T=ConvTimeToStr(TimeLeft); break;
	case Status_COL: T=GetStatusText(); break;
	default: T="???";
	}
}

void PrintThreadParams::FormatElement(int subItem, CString& T)
{
	switch (subItem)
	{
	case Pics_COL: T.Format("%d",PicsN); break;
	case Done_COL: T.Format("%d",Done); break;
	default: ThreadParams::FormatElement(subItem,T);
	}
}

void RenameThreadParams::FormatElement(int subItem, CString& T)
{
	switch (subItem)
	{	
	case Done_COL: 
		if(Renamed==0) T.Format("%d",Done); 
		else T.Format("%d (%d)",Done,Renamed); 
		break;
	default: PrintThreadParams::FormatElement(subItem,T);
	}
}

int ThreadParams::Update(StatMessage* msg)
{
	int t=0;
	if(msg->FLAGS & TIME_MSK) {Time=msg->Time; t++;}
	if(msg->FLAGS & TIME_LEFT_MSK) {TimeLeft=msg->TimeLeft; t++;}
	if(msg->FLAGS & STATUS_MSK) {Status=msg->Status; t++;}
	return t;
}

int PrintThreadParams::Update(StatMessage* msg)
{
	int t=0;
	if(msg->FLAGS & DONE_MSK) {Done=msg->Done; t++;}
	t+=ThreadParams::Update(msg);
	return t;
}

void ThreadParams::_Main()
{
	StatMessage* msg; 	
	Terminator.ResetEvent(); StopWaiting=false; Timer1.Start();

	msg=new StatMessage(); Timer1.Stop(); msg->Time=Timer1.GetValue(); StartTime=CTime::GetCurrentTime();	
	msg->Status=Working; msg->FLAGS=STATUS_MSK | TIME_MSK;
	PostParentMessage(UM_UPDATE_THREAD_LIST,msg);	

	Main();

	msg=new StatMessage(); Timer1.Stop(); msg->Time=Timer1.GetValue(); msg->TimeLeft=msg->Time;
	msg->Status=Finished; msg->FLAGS=STATUS_MSK | TIME_MSK | TIME_LEFT_MSK;
	PostParentMessage(UM_UPDATE_THREAD_LIST,msg);	

	Terminator.SetEvent();
}

void PrintThreadParams::Main()
{
	int j,num=0; int *ind=0; 	StatMessage* msg; 

	CString TmpName,TmpPPM,TmpSEP,TmpDJVU,T,name; 

	FRGPicArray &Pics1=*(Pics); PicsN=Pics->GetSize();
	CImage Img;
	CString RandomText; GetRandomString(RandomText);	
	CWinThread* thrd=AfxGetThread();
//*****************PRINTING************************	
	for(j=0;j<Pics1.GetSize() && !StopWaiting;j++)
	{
		msg=new StatMessage();	msg->Done=j+1; msg->Status=Working; 
		msg->FLAGS=TIME_MSK | DONE_MSK | STATUS_MSK;
		PostParentMessage(UM_UPDATE_THREAD_LIST,msg); 

		FRGPic &CurPic=Pics1[j];
		
		T=CurPic.Name; 

		T.Replace(' ','_'); TmpName=*Path+T;		
		TmpPPM=TmpName+".ppm"; 
		TmpSEP=TmpName+".sep";TmpDJVU=TmpName+".djvu";
// ******FOREGROUND**********
		T.Format("i_view32 %s /convert=%s",CurPic.FullName,TmpPPM);
		STARTUPINFO si;  GetStartupInfo(&si); PROCESS_INFORMATION pi1;	
		if(CreateProcess(NULL, (char *)LPCSTR(T), NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi1)) 
		{
			ResumeThread(pi1.hThread);
			WaitForSingleObject(pi1.hThread,INFINITE);
		}
		T.Format("pnmtodjvurle.exe"); 
		SECURITY_ATTRIBUTES sa; sa.nLength=sizeof(SECURITY_ATTRIBUTES); sa.lpSecurityDescriptor=NULL; sa.bInheritHandle=TRUE;
		HANDLE in=CreateFile(TmpPPM, GENERIC_READ, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		HANDLE out=CreateFile(TmpSEP, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		si.hStdInput=in; si.hStdOutput=out; si.dwFlags|=STARTF_USESTDHANDLES;
		if(CreateProcess(NULL, (char *)LPCSTR(T), NULL, NULL, TRUE, DETACHED_PROCESS | CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi1)) 
		{
			ResumeThread(pi1.hThread);
			WaitForSingleObject(pi1.hThread,INFINITE);
		}
		CloseHandle(in); CloseHandle(out); DeleteFile(TmpPPM);
		GetStartupInfo(&si);
		T.Format("csepdjvu %s %s",TmpSEP,TmpDJVU);

		if(CreateProcess(NULL, (char *)LPCSTR(T), NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi1)) 
		{
			ResumeThread(pi1.hThread);
			WaitForSingleObject(pi1.hThread,INFINITE);
		}
		DeleteFile(TmpSEP);	
		MoveFile(TmpDJVU,CurPic.Path+CurPic.Name+".djvu");
// **************************
		
		msg=new StatMessage();	Timer1.Stop(); msg->Time=Timer1.GetValue(); msg->Done=j+1;
		msg->TimeLeft=msg->Time*PicsN/(j+1); msg->FLAGS=TIME_MSK | DONE_MSK | TIME_LEFT_MSK;
		PostParentMessage(UM_UPDATE_THREAD_LIST,msg); Printed++;
	}
	delete Pics; Pics=0;
}

void PriorityThreadParams::Main()
{
	DWORD procs[1000]; DWORD num,len; int t,ret=0; BYTE buf[1000];
	do
	{
		if(EnumProcesses(procs,sizeof(DWORD)*1000,&num))
		{
			HANDLE p; CString ExeName; 
			for(DWORD i=0;i<num;i++)
			{
				if((p=OpenProcess(PROCESS_ALL_ACCESS,false,procs[i])))
				{
					len=GetProcessImageFileName(p,(LPSTR)buf,1000);					
					CString FullName(buf),Name;
					if(len>0)
					{					
						if((t=FullName.ReverseFind('\\'))<0) Name=FullName;
						else Name=FullName.Right(FullName.GetLength()-t-1);
						if(Name=="LtiVpd.exe") ret=SetPriorityClass(p,IDLE_PRIORITY_CLASS);
					}
					CloseHandle(p);
				}
			}
		}
		if(!ret) Sleep(1000);
	}
	while(!StopWaiting && ret==0);
}

void PrintDialogThreadParams::Main()
{
	if(PrintDlg->DoModal()==IDOK)
	{
		Parent->PostThreadMessage(UM_PRINT_DIALOG, 0,0);
	}
}

void RenameThreadParams::Main()
{
	Limit1->terminator.SetEvent();
	Parent->PostThreadMessage(UM_PRINT,0,0);	
}

PrintThreadParams::PrintThreadParams()
{
	Path=0; Done=0; PicsN=0; Pics=0;  Printed=0;
}

ThreadParams* ThreadsArray::FindElement(int pid,int &n)
{
	ThreadParams* ret=0,*t; n=-1;    
	for(int i=0;i<GetSize() && ret==0; i++) 
	{
		t=GetAt(i); if(t->PID==pid) {ret=t;	n=i;}
	}
    return ret;
}
//************************************************
BEGIN_MESSAGE_MAP(ThreadsList, CListCtrl)
END_MESSAGE_MAP()

ThreadsList::ThreadsList(void)
{
	LastN=-1;
}

ThreadsList::~ThreadsList(void)
{
}

void ThreadsList::CreateColumns(void)
{
	int i=0;
	InsertColumn(i++,"�",LVCFMT_CENTER ,20);
	InsertColumn(i++,"Name",LVCFMT_CENTER ,80);
	InsertColumn(i++,"Pics",LVCFMT_LEFT ,40);
	InsertColumn(i++,"Done",LVCFMT_LEFT ,40);
	InsertColumn(i++,"Start",LVCFMT_CENTER ,75);
	InsertColumn(i++,"Time",LVCFMT_CENTER ,75);
	InsertColumn(i++,"Total",LVCFMT_CENTER ,75);
	InsertColumn(i++,"Status",LVCFMT_CENTER ,70);
}

void ThreadsList::UpdateList()
{
	LV_ITEM lvi; ThreadsArray& Threads=Parent->Threads;
	int n1=Threads.GetSize(); CString T; int i;

	DeleteAllItems(); 
	for(i=0;i<Threads.GetSize();i++)
	{
		ThreadParams &tt=*(Threads[i]); 
		lvi.mask = LVIF_PARAM; lvi.iItem = i;lvi.iSubItem = 0; lvi.lParam=tt.PID;			
		InsertItem(&lvi);
	} 

	for(i=0;i<GetItemCount();i++) 
	{
		UpdateElement(i,i);
	}

	EnsureVisible(i-1, false);	
}

void ThreadsList::UpdateElement(int ListN,int ArrayN)
{
	int i=ListN; CString T; LV_ITEM lvi;
	ThreadsArray& Threads=Parent->Threads;
	ThreadParams* Thrd;

	if(ListN<0 || ListN>=GetItemCount()) return;
	if(ArrayN<0 || ArrayN>=Threads.GetSize()) Thrd=Threads.FindElement(GetItemData(ListN),ArrayN);
	if(ArrayN<0) return;
	Thrd=Threads[ArrayN];
	
	lvi.mask = LVIF_TEXT; lvi.iItem = i; lvi.iSubItem=0;
	T.Format("%d",i+1); SetItemText(i,lvi.iSubItem++,(char*)LPCTSTR(T));
	for(;lvi.iSubItem<=8;lvi.iSubItem++)
	{
		Thrd->FormatElement(lvi.iSubItem,T); 
		SetItemText(i,lvi.iSubItem,(char*)LPCTSTR(T));			
	}
}

int ThreadsList::FindElement(int pid)
{
	int ret=-1;
	for(int i=0;i<GetItemCount() && ret<0; i++) if(GetItemData(i)==pid) ret=i;	
    return ret;
}
