// DIPSystemDoc.cpp : CDIPSystemDoc 类的实现
//

#include "stdafx.h"
#include "DIPSystem.h"

#include "DIPSystemDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDIPSystemDoc

IMPLEMENT_DYNCREATE(CDIPSystemDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIPSystemDoc, CDocument)
END_MESSAGE_MAP()


// CDIPSystemDoc 构造/析构

CDIPSystemDoc::CDIPSystemDoc()
{
	// TODO: 在此添加一次性构造代码

}

CDIPSystemDoc::~CDIPSystemDoc()
{
}

BOOL CDIPSystemDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDIPSystemDoc 序列化

void CDIPSystemDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CDIPSystemDoc 诊断

#ifdef _DEBUG
void CDIPSystemDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIPSystemDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDIPSystemDoc 命令
