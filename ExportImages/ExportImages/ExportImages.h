
// ExportImages.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CExportImagesApp: 
// �йش����ʵ�֣������ ExportImages.cpp
//

class CExportImagesApp : public CWinApp
{
public:
	CExportImagesApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CExportImagesApp theApp;