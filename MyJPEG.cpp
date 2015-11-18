// MyJPEG.cpp: implementation of the MyJPEG class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyJPEG.h"
#include "ijl.h" // заголовочный файл Интелевской библиотеки jpeg.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyJPEG::MyJPEG(CString& n)
{
    Name=n; buffer=0;
	buffer=new BMPanvas((CDC*)0);
}

MyJPEG::~MyJPEG()
{
    if(buffer) delete buffer;
}


BMPINFO MyJPEG::ReadJPGHeader(const CString& name)
{
	JPEG_CORE_PROPERTIES jcprops;
	BMPINFO temp; temp.bmiHeader.biWidth=temp.bmiHeader.biHeight=0;

	if ( ijlInit(&jcprops) == IJL_OK ) 
	{
		jcprops.JPGFile=name;
		if ( ijlRead(&jcprops,IJL_JFILE_READPARAMS) == IJL_OK )
		{			
			temp.bmiHeader.biWidth=jcprops.JPGWidth;
			temp.bmiHeader.biHeight=jcprops.JPGHeight;
			temp.bmiHeader.biBitCount=jcprops.JPGChannels*8;
		}
	}
	ijlFree(&jcprops);
	return temp;
}

// читаем картинку в этот буфер.
int MyJPEG::ReadImage()
{
	JPEG_CORE_PROPERTIES jcprops;
	if ( ijlInit(&jcprops) != IJL_OK ) {ijlFree(&jcprops); return 1;}

	jcprops.JPGFile=Name;

	if ( ijlRead(&jcprops,IJL_JFILE_READPARAMS) != IJL_OK ) {ijlFree(&jcprops); return 1;}
	
	BMPINFO temp=buffer->INFO;
	temp.bmiHeader.biWidth=jcprops.JPGWidth;
	temp.bmiHeader.biHeight=jcprops.JPGHeight;
	temp.bmiHeader.biBitCount=jcprops.JPGChannels*8;
	buffer->Create(&temp);
	buffer->CreateGrayPallete();
	buffer->LoadBitmapArray();

	jcprops.DIBWidth=jcprops.JPGWidth;
	jcprops.DIBHeight=-jcprops.JPGHeight;
	jcprops.DIBChannels=jcprops.JPGChannels;	
	jcprops.DIBPadBytes=IJL_DIB_PAD_BYTES(jcprops.JPGWidth,jcprops.JPGChannels);	
	jcprops.DIBBytes=buffer->arr;

	if ( jcprops.JPGChannels == 3 )
	{
		jcprops.DIBColor = IJL_BGR;
		jcprops.JPGColor = IJL_YCBCR;
	}
	else
	{
		jcprops.DIBColor = IJL_G;
		jcprops.JPGColor = IJL_G;
	}

	if ( ijlRead(&jcprops, IJL_JFILE_READWHOLEIMAGE) != IJL_OK ) 
	{
		buffer->UnloadBitmapArray();	
		ijlFree(&jcprops);return 1;
	}
	buffer->SetBitmapArray();
	buffer->UnloadBitmapArray();	

	if ( ijlFree(&jcprops) != IJL_OK ) return 1;
	return 0;
}


int MyJPEG::Compress(int quality)
{
	JPEG_CORE_PROPERTIES jcprops;
	
	if ( ijlInit(&jcprops) != IJL_OK ) {ijlFree(&jcprops); return 1;}

	buffer->LoadBitmapArray();	

	jcprops.JPGFile=Name;
	jcprops.DIBWidth = buffer->w;
	jcprops.DIBHeight = -buffer->h;
	jcprops.JPGWidth = buffer->w;
	jcprops.JPGHeight = buffer->h;
	jcprops.DIBBytes = buffer->arr;	
	jcprops.DIBChannels = buffer->INFO.bmiHeader.biBitCount/8;
	jcprops.JPGChannels = jcprops.DIBChannels;
	jcprops.DIBPadBytes = IJL_DIB_PAD_BYTES(buffer->w,jcprops.DIBChannels);
	jcprops.jquality = quality;

	if ( jcprops.DIBChannels == 3 )
	{
		jcprops.DIBColor = IJL_BGR;
		jcprops.JPGColor = IJL_YCBCR;
	}
	else
	{
		jcprops.DIBColor = IJL_G;
		jcprops.JPGColor = IJL_G;
	}

	if ( ijlWrite(&jcprops,IJL_JFILE_WRITEWHOLEIMAGE) != IJL_OK )
	{
		buffer->UnloadBitmapArray();	
		ijlFree(&jcprops); return 1;
	}
	
	buffer->UnloadBitmapArray();	
	
	if ( ijlFree(&jcprops) != IJL_OK ) {return 1;}	
	return 0;
}