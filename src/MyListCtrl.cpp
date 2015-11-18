// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "testWMF.h"
#include "MyListCtrl.h"
#include "testwmfdlg.h"
#include ".\mylistctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "Windows.h"
/////////////////////////////////////////////////////////////////////////////
// MyListCtrl

MyListCtrl::MyListCtrl()
{
	LastN=0; SortStatus=1; SortMask=1;
}

MyListCtrl::~MyListCtrl()
{
}


BEGIN_MESSAGE_MAP(MyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(MyListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DROPFILES()	
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
//	ON_COMMAND(ID_GETCROPZONE,OnGetCropZone)
//	ON_COMMAND(ID_CLEARCROPZONE,OnClearCropZone)
//	ON_COMMAND(ID_GETCROPMAXW,OnGetCropMaxW)
//	ON_COMMAND(ID_GETCROPMAXH,OnGetCropMaxH)
//	ON_COMMAND(ID_GETCROPMAXWH,OnGetCropMaxWH)
//	ON_COMMAND(ID_SAVENAMESLIST,OnSaveNamesList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyListCtrl message handlers
char Compare(int v1,int v2, char inv)
{
	char ret;
	if(v1==v2) ret=0;
	else ret=inv*(v1>v2 ? -1:1);	
	return ret;
}

int CALLBACK CompareFunc1(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int ret;		
	MyListCtrl* pListCtrl = (MyListCtrl*) lParamSort;
	FRGPic& Pic1=pListCtrl->Pics[lParam1];
	FRGPic& Pic2=pListCtrl->Pics[lParam2];	
	int& mask=pListCtrl->SortMask; int &Status=pListCtrl->SortStatus;
	int n=1,v1,v2; while(mask>>n) n++; n--; 
	char inv=(Status&mask ? -1:1);

	switch(n)
	{
	case 0:
		v1=lParam1; v2=lParam2; ret=Compare(v1,v2,inv); break;
	case 1:
		ret=inv*strcmp(Pic1.Name,Pic2.Name); break;
/*
	case 2: 
		v1=Pic1->PicDim.cx; v2=Pic2->PicDim.cx; 
		ret=Compare(v1,v2,inv); break;
	case 3: 			
		v1=Pic1->PicDim.cy; v2=Pic2->PicDim.cy; ret=Compare(v1,v2,inv); break;
	case 4: 
		v1=Pic1->GetZones()->GetSize(); v2=Pic2->GetZones()->GetSize(); 
		ret=Compare(v1,v2,inv); break;
	case 6: 
		v1=Pic1->CheckDjvu(); v2=Pic2->CheckDjvu(); 
		ret=Compare(v1,v2,inv); break;
	case 8:
		v1=Pic1->CropZone.Rect.Width(); v2=Pic2->CropZone.Rect.Width(); ret=Compare(v1,v2,inv); break;
	case 9:
		v1=Pic1->CropZone.Rect.Height(); v2=Pic2->CropZone.Rect.Height(); ret=Compare(v1,v2,inv); break;
*/
	default: ret=0;
	}
	return ret;
}

void MyListCtrl::UpdateList()
{		
	LV_ITEM lvi; CString T;
	POSITION pos=GetFirstSelectedItemPosition();
	int n=HitItem, n1=Pics.GetSize();
	if(n>=n1) n=n-1;

	if(LastN!=n1) 
	{
		DeleteAllItems(); 
		for(int i=0;i<Pics.GetSize();i++)
		{
//			DjvuPic &tt=*(Parent->Pics[i]); 
			lvi.mask = LVIF_PARAM; lvi.iItem = i;lvi.iSubItem = 0; lvi.lParam=i;			
			InsertItem(&lvi);
		}
	}
    
	for(int i=0;i<GetItemCount();i++)
	{
		FRGPic& Pic=Pics[GetItemData(i)];
	
		lvi.mask = LVIF_TEXT; lvi.iItem = i; lvi.iSubItem=0;
		T.Format("%d",GetItemData(i)+1); SetItemText(i,lvi.iSubItem++,(char*)LPCTSTR(T));
		//T=Pic.Name;
		T.Format("%s",Pic.Name+Pic.Ext); 
		SetItemText(i,lvi.iSubItem++,(char*)LPCTSTR(T));
			
		T.Format("%d",Pic.w); SetItemText(i,lvi.iSubItem++,(char*)LPCTSTR(T));
		T.Format("%d",Pic.h); SetItemText(i,lvi.iSubItem++,(char*)LPCTSTR(T)); 		

		lvi.mask=LVIF_STATE;		
		lvi.state = 0; lvi.stateMask = LVIS_SELECTED | LVIS_FOCUSED;		
		SetItemState(i,&lvi);		
	}
	LastN=Pics.GetSize();

	if(n>=0)
	{	
		_ms_lvi.mask=LVIF_STATE;
		_ms_lvi.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		_ms_lvi.state = LVIS_SELECTED | LVIS_FOCUSED;			
		
		LV_ITEM FAR * temp=&_ms_lvi;

		this->PostMessage(LVM_SETITEMSTATE ,(WPARAM)(n), (LPARAM)(temp));
		EnsureVisible(n, false);	
	}	
	SortItems(CompareFunc1,(LPARAM) (this));
}



int MyListCtrl::HitTest(CPoint point)
{
	LVHITTESTINFO hittest;
	hittest.flags=LVHT_ONITEM;
	hittest.pt=point;
	CListCtrl::HitTest(&hittest);
	if(point.x==0xFFFF && point.y==0xFFFF) return HitItem;
	else return hittest.iItem;
}


void MyListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
/*
	OutputWnd &OWnd1=Graph->OWnd1;
	CArray<DjvuPic*,DjvuPic*> &pics=Parent->Pics;		
	if(nFlags!=1234) HitItem=HitTest(point);

	if(HitItem>=0)
	{
		int t=HitItem,err=0; 
		CTestWMFDlg* parent=(CTestWMFDlg* )GetParent();
		parent->UpdateData();
		
		DjvuPic *CurPic=pics[GetItemData(t)];
	
		CurPic->GBparam.sigma=parent->GBsigma;
		CurPic->GBparam.KernelSize=parent->GBn;
		CurPic->CenterRotate.Q=RotateFilterParam::LOW;

		if(CurPic->IsBckgFile)
		{
			parent->LoadOriginal=false;
			parent->OriginalCheck.EnableWindow();	
		}
		else
		{
			parent->LoadOriginal=true;
			parent->OriginalCheck.EnableWindow(false);		
		}

		Graph->PostThreadMessage(UM_LOAD_PIC,(WPARAM)CurPic,0);
	
		UpdateList();		
		parent->OnSaveList();
	}	
*/
}

void MyListCtrl::SetState(int num, int state)
{
	LV_ITEM lvi;	
	lvi.mask = LVIF_STATE;
	lvi.iItem = num;
	lvi.iSubItem = 0;		
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.state = INDEXTOSTATEIMAGEMASK(state);
	SetItemState(num,&lvi);	
}

int MyListCtrl::GetState(int num)
{
	UINT state=GetItemState(num,LVIS_STATEIMAGEMASK);
	return state>>12; 
}

void MyListCtrl::OnDropFiles(HDROP hDropInfo) 
{	
	CString T;	char buf[1000]; 
	int num=DragQueryFile(hDropInfo,0xFFFFFFFF,buf,1000), i;
	for(i=0;i<num;i++)
	{
		DragQueryFile(hDropInfo,i,buf,1000); T=CString(buf);
		Pics.Add(FRGPic(T));
	}
	if(num) UpdateList();
	CListCtrl::OnDropFiles(hDropInfo);
}

void MyListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{	
	CListCtrl::OnMouseMove(nFlags, point);
}

BOOL MyListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==LVM_SETITEMSTATE)	
	{
		LV_ITEM FAR* temp=(LV_ITEM FAR*)pMsg->lParam;
		int a=5;	
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

void MyListCtrl::SelectAll(void)
{
	LV_ITEM lvi;
	for(int i=0;i<GetItemCount();i++)
	{
		lvi.mask=LVIF_STATE;// lvi.iSubItem=0; lvi.iItem=i;
		lvi.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		lvi.state = LVIS_SELECTED | LVIS_FOCUSED;
		SetItemState(i,&lvi);
	}
	SetFocus();
}


void MyListCtrl::GetSelectedItems( FRGPicArray& List )
{
	POSITION pos=GetFirstSelectedItemPosition(); int n;
	while(pos)
	{
		n=GetNextSelectedItem(pos); 
		n=GetItemData(n);
		List.Add(Pics[n]); 
	}
}

void MyListCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu1; menu1.LoadMenu(IDR_MENU2);
	CMenu *menu2=menu1.GetSubMenu(0);
	menu2->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,point.x,point.y,pWnd);	
}


void MyListCtrl::CreateColumns(void)
{
	int i=0;
	InsertColumn(i++,"¹",LVCFMT_CENTER ,20);
	InsertColumn(i++,"Name",LVCFMT_LEFT ,370);
	InsertColumn(i++,"Width",LVCFMT_CENTER ,40);
	InsertColumn(i++,"Height",LVCFMT_CENTER ,60);
}

FRGPic::FRGPic(CString name)
{
	FullName=name;
	ProcessFullName();
	CImage t; t.Load(FullName);
	w=t.GetWidth(); h=t.GetHeight();
}

int FRGPic::ProcessFullName()
{
	int t;
	if((t=FullName.ReverseFind('\\'))<0)
	{
		Name=FullName; Path="";	
	}
	else
	{
		Path=FullName.Left(++t);	
		Name=FullName.Right(FullName.GetLength()-t);
	}
	if((t=Name.ReverseFind('.'))<0)
	{
		Ext="";	
	}
	else
	{
		Ext=Name.Right(Name.GetLength()-(t));
		Name=Name.Left(t);			
	}

	return 0;
}
