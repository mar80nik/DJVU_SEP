// MyJPEG.h: interface for the MyJPEG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYJPEG_H__21CA2E5B_78AF_4A77_81CC_E8766498B9F0__INCLUDED_)
#define AFX_MYJPEG_H__21CA2E5B_78AF_4A77_81CC_E8766498B9F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "bmpanvas.h"

class MyJPEG
{
public:
	BMPanvas *buffer;
	CString Name;	

	MyJPEG(CString&);
	virtual ~MyJPEG();
	static BMPINFO ReadJPGHeader(const CString&);

	int ReadImage();
//		int &width, // ������ ����������� ��������.
//		int &height, // ������ ����������� ��������.
//		int &bpp, // ���� (�� ���) �� �������.
///		const void *buffer, // ����� ������, � ������� �������� ������ jpeg-�� ������.
//		int sizebytes); // ������ ������ jpeg-�� ������.

	int Compress(int quality=75);//(const void *buffer, // ����� �������� � ������
//		int width, // ������ �������� � ��������
//		int height, // ������ �������� � ��������.
//		int bpp, // *����* �� ������� ����������� 1 ��� 3
///		int &len, // ������������ ����� ������ ������
//		int quality=75); // �������� �������� � ���������
};

#endif // !defined(AFX_MYJPEG_H__21CA2E5B_78AF_4A77_81CC_E8766498B9F0__INCLUDED_)