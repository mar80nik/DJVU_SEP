; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTestWMFDlg
LastTemplate=CPrintDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "testWMF.h"

ClassCount=7
Class1=CTestWMFApp
Class2=CTestWMFDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TESTWMF_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Class4=OutputWnd
Class5=MyListBox
Class6=MyListCtrl
Class7=MyPrintDlg
Resource5=IDD_TESTWMF_DIALOG (English (U.S.))
Resource6=IDR_ACCELERATOR1

[CLS:CTestWMFApp]
Type=0
HeaderFile=testWMF.h
ImplementationFile=testWMF.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CTestWMFApp

[CLS:CTestWMFDlg]
Type=0
HeaderFile=testWMFDlg.h
ImplementationFile=testWMFDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CTestWMFDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=testWMFDlg.h
ImplementationFile=testWMFDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_TESTWMF_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CTestWMFDlg

[DLG:IDD_TESTWMF_DIALOG (English (U.S.))]
Type=1
Class=CTestWMFDlg
ControlCount=33
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_BUTTON2,button,1342242816
Control3=IDC_EDIT3,edit,1350631552
Control4=IDC_BUTTON4,button,1342242816
Control5=IDC_BUTTON3,button,1342242816
Control6=IDC_BUTTON5,button,1342242816
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,button,1342177287
Control9=IDC_EDIT4,edit,1350631552
Control10=IDC_EDIT5,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT6,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_BUTTON6,button,1342242816
Control16=IDC_EDIT7,edit,1350631552
Control17=IDC_EDIT8,edit,1350631552
Control18=IDC_EDIT9,edit,1350631552
Control19=IDC_CHECK1,button,1342242819
Control20=IDC_STATIC,static,1342308352
Control21=IDC_BUTTON7,button,1342242816
Control22=IDC_BUTTON8,button,1342242816
Control23=IDC_BUTTON9,button,1342242816
Control24=IDC_BUTTON10,button,1342242816
Control25=IDC_CHECK2,button,1342242819
Control26=IDC_EDIT1,edit,1350631552
Control27=IDC_STATIC,button,1342177287
Control28=IDC_STATIC,static,1342308352
Control29=IDC_EDIT2,edit,1350631552
Control30=IDC_BUTTON11,button,1342242816
Control31=IDC_LIST2,listbox,1353779457
Control32=IDC_LIST3,SysListView32,1350631425
Control33=IDC_STATIC,button,1342177287

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:OutputWnd]
Type=0
HeaderFile=OutputWnd.h
ImplementationFile=OutputWnd.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=OutputWnd

[CLS:MyListBox]
Type=0
HeaderFile=MyListBox.h
ImplementationFile=MyListBox.cpp
BaseClass=CListBox
Filter=W
VirtualFilter=bWC

[CLS:MyListCtrl]
Type=0
HeaderFile=MyListCtrl.h
ImplementationFile=MyListCtrl.cpp
BaseClass=CListCtrl
Filter=W
LastObject=ID_NEXT123
VirtualFilter=FWC

[CLS:MyPrintDlg]
Type=0
HeaderFile=MyPrintDlg.h
ImplementationFile=MyPrintDlg.cpp
BaseClass=CPrintDialog
Filter=D
VirtualFilter=dWC

[ACL:IDR_ACCELERATOR1]
Type=1
Class=?
Command1=ID_PREV123
Command2=ID_NEXT123
CommandCount=2

