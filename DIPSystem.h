// DIPSystem.h : DIPSystem Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDIPSystemApp:
// �йش����ʵ�֣������ DIPSystem.cpp
//

class CDIPSystemApp : public CWinApp
{
public:
	CDIPSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	GdiplusStartupInput m_GdiplusStartupInput;
	ULONG_PTR			m_GdiplusToken;
public:
	virtual int ExitInstance();
	afx_msg BOOL OnOpenRecentFile(UINT nID);
};

extern CDIPSystemApp theApp;
