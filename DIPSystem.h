// DIPSystem.h : DIPSystem 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CDIPSystemApp:
// 有关此类的实现，请参阅 DIPSystem.cpp
//

class CDIPSystemApp : public CWinApp
{
public:
	CDIPSystemApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现
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
