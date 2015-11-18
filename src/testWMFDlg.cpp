// testWMFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testWMF.h"
#include "testWMFDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "my_timer.h"
#include ".\testwmfdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

MyTimer Timer1;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestWMFDlg dialog

CTestWMFDlg::CTestWMFDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestWMFDlg::IDD, pParent)	
	, Errors(0)
	, LoadOriginal(FALSE)//, dlg1(false)		
{
	//{{AFX_DATA_INIT(CTestWMFDlg)
	threshold = 127;
	GBsigma = 3.0;
	GBn = 0;
	bckg = 1020;
	angle = 0.0;
	time = 0.0;
	interp = FALSE;
	edges = FALSE;
	edge_level = 50;
	canvasW = 1263;
	canvasH = 1977;
	DJVUOutFldr="g:\\!tmp\\";	
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SaveListFile="";
	InputFldr=CString(""); ThreadsPID=1;
}

void CTestWMFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestWMFDlg)
	DDX_Control(pDX, IDC_LIST5, FRGFileList);
	DDX_Control(pDX, IDC_LIST4, ThreadsList);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_EDIT10, DJVUOutFldr);
}

BEGIN_MESSAGE_MAP(CTestWMFDlg, CDialog)
	//{{AFX_MSG_MAP(CTestWMFDlg)	
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON8, OnSaveList)

	ON_COMMAND(ID_PRINT,OnButton1)	
	ON_COMMAND(ID_SELECT_ALL,OnSelectAll)	
	ON_COMMAND(ID_ORIGINAL,OnOriginal)
	//}}AFX_MSG_MAP	
	ON_BN_CLICKED(IDC_BUTTON13, OnBnClickedButton13)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_EN_KILLFOCUS(IDC_EDIT4, OnEnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT5, OnEnKillfocusEdit5)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_MESSAGE(UM_PRINT,OnPrint)
	ON_MESSAGE(UM_PRINT_DIALOG,OnPrintDialog)
	ON_MESSAGE(UM_WAITING,OnWaiting)	
	ON_MESSAGE(UM_PRINTERLOCK,OnPrinterLock)	
	ON_MESSAGE(UM_UPDATE_THREAD_LIST,OnThreadListUpdate)	
	
	ON_BN_CLICKED(IDC_BUTTON16, OnBnClickedButton16)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, OnLvnColumnclickList3)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestWMFDlg message handlers

BOOL CTestWMFDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	Graph->ParentHWND=GetSafeHwnd();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

//	BKGFileList.Parent=
	ThreadsList.Parent=this;
	FRGFileList.CreateColumns(); FRGFileList.CreateColumns();
	DWORD ExStyle = FRGFileList.GetExtendedStyle();
	ExStyle |= LVS_EX_FULLROWSELECT;
	FRGFileList.SetExtendedStyle(ExStyle);
	FRGFileList.DragAcceptFiles(true);

	ExStyle = ThreadsList.GetExtendedStyle();
	ExStyle |= LVS_EX_FULLROWSELECT;
	ThreadsList.SetExtendedStyle(ExStyle);
	ThreadsList.CreateColumns();
	ThreadsList.UpdateList();

	Graph->bckg=bckg;
	Graph->ResumeThread();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestWMFDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


UINT WorkThreadMain( LPVOID pParam )
{
	ThreadParams *Params=(ThreadParams *)pParam;
	Params->_Main();
	return 0;
}

LRESULT CTestWMFDlg::OnPrintDialog(WPARAM wParam, LPARAM lParam )
{
	FRGPicArray *Pics; FRGPicArray Pics1;

	FRGFileList.GetSelectedItems(Pics1);
	CString T; T.Format("Completed 0%%"); SetWindowText(T);
	PrintThreadParams* Params;

	for(int i=0;i<4;i++)
	{
		Params=new PrintThreadParams(); Pics=new FRGPicArray();
		Params->Name.Format("Print%d",i+1); Params->PID=ThreadsPID++; Params->Parent=AfxGetApp();
		Params->Path=&DJVUOutFldr; Params->Pics=Pics;
		for(int j=i;j<Pics1.GetSize();j+=4) Pics->Add(Pics1[j]);
		if(Pics->GetSize()!=0) Threads.Add(Params);
		else {delete Params; delete Pics;}
		int a=5;
	}

	ThreadsList.UpdateList();

	for(int i=0;i<Threads.GetSize();i++)
	{
		ThreadParams *Params=Threads[i];
		if(Params->Status==Preparing) AfxBeginThread(WorkThreadMain,Params,THREAD_PRIORITY_NORMAL);	
		Sleep(1000);
	}
    return 0;
}

void CTestWMFDlg::OnButton1() 
{
//	OnSaveList();	
	UpdateData(); 
	OnPrintDialog(0,0);
}


int CTestWMFDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	ShowWindow(SW_SHOWMAXIMIZED);
	
	return 0;
}

void CTestWMFDlg::PostNcDestroy() 
{
	for(int i=0;i<Threads.GetSize();i++)
	{
		Threads[i]->StopWaiting=true;
		WaitForSingleObject(Threads[i]->Terminator,1000);
		delete Threads[i];
	}	
	
	CDialog::PostNcDestroy();
}
/*
void CTestWMFDlg::OnButton3() 
{
	for(int i=0;i<Pics.GetSize();i++)
	{
		Pics[i]->Destroy();
		delete Pics[i];	
	}
	Pics.RemoveAll();
	BKGFileList.UpdateList();
	SaveListFile="";
}
*/
/*
void CTestWMFDlg::OnCommandNext()
{
	//CArray<DjvuPic*,DjvuPic*> &pics=*Pics;

	int &n=BKGFileList.HitItem;
	if(n>=0 && n<Pics.GetSize()-1) 
	{
		n++;			
		BKGFileList.SendMessage(WM_LBUTTONDBLCLK,1234,0);
	}

	//return true;
}
*/
/*
void CTestWMFDlg::OnCommandPrev()
{
	int &n=BKGFileList.HitItem;
	if(n>0) 
	{
		n--;		
		BKGFileList.SendMessage(WM_LBUTTONDBLCLK,1234,0);
	}
	//return true;
}
*/

void CTestWMFDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CTestWMFDlg::OnSaveList() 
{
	CFile f; CString ret="SaveList Error"; ;
	CString fltr="Dat Files (*.dat)|*.dat|All Files (*.*)|*.*||";
	CFileDialog dlg1(false,"dat","list",OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,fltr);
	if(SaveListFile=="")
	{
		if(dlg1.DoModal()==IDOK) SaveListFile=dlg1.GetFileName();
		else SaveListFile="";
	}
	else
	{
		if(f.Open(SaveListFile,CFile::modeCreate | CFile::modeWrite))
		{
			{
			CArchive ar(&f,CArchive::store);
			ret="SaveList OK"; 
			}
			f.Close();	
		}			
	}	
	InfoList.AddString(ret);
}

void CTestWMFDlg::OnBnClickedButton13()
{
}

void CTestWMFDlg::OnBnClickedCheck3()
{
	UpdateData(); 
	Graph->PostThreadMessage(UM_ORIGINAL,(WPARAM)LoadOriginal,0);
}


void CTestWMFDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}



void CTestWMFDlg::OnEnKillfocusEdit4()
{
	UpdateData();
}

void CTestWMFDlg::OnEnKillfocusEdit5()
{
	UpdateData();
}
/*
void CTestWMFDlg::OnBnClickedButton15()
{
	UpdateData();
	FRGPicArray PicList;
	BKGFileList.GetSelectedItems(PicList);
	for(int i=0;i<PicList.GetSize();i++) 
	{
//		PicList[i]->GBparam.KernelSize=GBn;
//		PicList[i]->GBparam.sigma=GBsigma;
	}
	BKGFileList.UpdateList();
}
*/
LRESULT CTestWMFDlg::OnWaiting(WPARAM wParam, LPARAM lParam )
{
	CString T; T.Format("Waiting for files to rename");
	InfoList.AddString(T);    
	return 0;
}

LRESULT CTestWMFDlg::OnPrint(WPARAM wParam, LPARAM lParam )
{	
	Beep(2000,1000);
	CString T; T.Format("Waiting Completed");
	InfoList.AddString(T);    
	ThreadsList.UpdateList();
	return 0;
}

LRESULT CTestWMFDlg::OnPrinterLock(WPARAM wParam, LPARAM lParam )
{
	CString T; 
	if(lParam)
	{
		T.Format("Locking for LtiVpd.exe");	
	}
	else
	{
		if(wParam) T.Format("LtiVpd.exe set IDLE Ok");
		else T.Format("LtiVpd.exe set priority ERROR");		
	}
	InfoList.AddString(T);    
	return 0;
}

void CTestWMFDlg::OnBnClickedButton4()
{
/*
	UpdateData();
	DjvuPicArray PicList;
	BKGFileList.GetSelectedItems(&PicList);
	for(int i=0;i<PicList.GetSize();i++) PicList[i]->CenterRotate.angle=angle;
	BKGFileList.UpdateList();
	*/
}

void CTestWMFDlg::OnSelectAll()
{
//	CString T; T="Select All"; InfoList.AddString(T);
	FRGFileList.SelectAll();
}


void CTestWMFDlg::OnOriginal()
{	
	LoadOriginal=(LoadOriginal ? false:true);
	UpdateData(false);
	OnBnClickedCheck3();	
}

void CTestWMFDlg::OnBnClickedButton16()
{
	for(int i=0;i<Threads.GetSize();i++)
	{
		Threads[i]->StopWaiting=true;
		WaitForSingleObject(Threads[i]->Terminator,1500);
	}	
	ThreadsList.UpdateList();
}

void CTestWMFDlg::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int& mask=FRGFileList.SortMask; int &Status=FRGFileList.SortStatus;

	mask=1<<pNMLV->iSubItem; 
	if(Status&mask) Status&=(~mask);
	else Status|=mask;	

	FRGFileList.SortItems(CompareFunc1,(LPARAM) (&FRGFileList));


	*pResult = 0;
}

LRESULT CTestWMFDlg::OnThreadListUpdate(WPARAM wParam, LPARAM lParam )
{	
	StatMessage *msg=(StatMessage *)wParam; int ArrayN; int t=0;
	ThreadParams* Thrd=Threads.FindElement(msg->ThreadPID,ArrayN);
	if(Thrd) 
	{
		if(Thrd->Update(msg)) 
			ThreadsList.UpdateElement(ThreadsList.FindElement(msg->ThreadPID),ArrayN);
	}
	delete msg;
	return 0;
}

/*
		if(msg->FLAGS & DONE_MSK) {Thrd->Done=msg->Done; t++;}
		if(msg->FLAGS & TIME_MSK) {Thrd->Time=msg->Time; t++;}
		if(msg->FLAGS & TIME_LEFT_MSK) {Thrd->TimeLeft=msg->TimeLeft; t++;}
		if(msg->FLAGS & STATUS_MSK) {Thrd->Status=msg->Status; t++;}

*/

/*
void CTestWMFDlg::OnBnClickedButton7()
{
	CString Name,TmpPBM,TmpRLE,TmpPPM,TmpSEP,T; 

	Name=CString("p0005"); 
	TmpPBM.Format("%s_PBM.pbm",Name); TmpRLE.Format("%_RLE.rle",Name);
	TmpPPM.Format("%_PPM.ppm",Name); TmpSEP.Format("%_SEP.sep",Name);


	T.Format("i_view32 %s.tif /convert=%s",Name,TmpPBM);
	STARTUPINFO si;  GetStartupInfo(&si); PROCESS_INFORMATION pi1;	

	if(CreateProcess(NULL, (char *)LPCSTR(T), NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED | IDLE_PRIORITY_CLASS, NULL, NULL, &si, &pi1)) 
	{
		ResumeThread(pi1.hThread);
		WaitForSingleObject(pi1.hThread,INFINITE);
	}

	T.Format("i_view32 %s.jpg /convert=%s",Name,TmpPPM);
	if(CreateProcess(NULL, (char *)LPCSTR(T), NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED | IDLE_PRIORITY_CLASS, NULL, NULL, &si, &pi1)) 
	{
		ResumeThread(pi1.hThread);
		WaitForSingleObject(pi1.hThread,INFINITE);
	}

	T.Format("pnmtodjvurle.exe"); 
	SECURITY_ATTRIBUTES sa;
	sa.nLength=sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor=NULL;
	sa.bInheritHandle=TRUE;

	HANDLE in=CreateFile(TmpPBM, GENERIC_READ, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE out=CreateFile(TmpRLE, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	si.hStdInput=in;
	si.hStdOutput=out;
	si.dwFlags|=STARTF_USESTDHANDLES;

	if(CreateProcess(NULL, (char *)LPCSTR(T), NULL, NULL, TRUE, 
		DETACHED_PROCESS | CREATE_SUSPENDED | IDLE_PRIORITY_CLASS, NULL, NULL, &si, &pi1)) 
	{
		ResumeThread(pi1.hThread);
		WaitForSingleObject(pi1.hThread,INFINITE);
	}
	CloseHandle(in); CloseHandle(out); DeleteFile(TmpPBM);


	HANDLE sep=CreateFile(TmpSEP, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE frg=CreateFile(TmpRLE, GENERIC_READ, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE bkg=CreateFile(TmpPPM, GENERIC_READ, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	
	int size; DWORD done; BYTE *arr=NULL;
	size=GetFileSize(frg,0); arr=new BYTE[size];
	ReadFile(frg,arr,size,&done,0);	WriteFile(sep,arr,size,&done,0);
	delete[] arr;
	SetEndOfFile(sep);
	size=GetFileSize(bkg,0); arr=new BYTE[size];
	ReadFile(bkg,arr,size,&done,0);	WriteFile(sep,arr,size,&done,0);
	delete[] arr;
	CloseHandle(sep); CloseHandle(frg); CloseHandle(bkg);
	DeleteFile(TmpRLE); DeleteFile(TmpPPM);

	GetStartupInfo(&si);
	T.Format("csepdjvu %s %s.djvu",TmpSEP,Name);
	if(CreateProcess(NULL, (char *)LPCSTR(T), NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED | IDLE_PRIORITY_CLASS, NULL, NULL, &si, &pi1)) 
	{
		ResumeThread(pi1.hThread);
		WaitForSingleObject(pi1.hThread,INFINITE);
	}
	DeleteFile(TmpSEP);	
}
*/
void CTestWMFDlg::OnBnClickedButton3()
{
	FRGFileList.Pics.RemoveAll();
	FRGFileList.UpdateList();
}
