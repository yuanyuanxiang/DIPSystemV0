// DIPSystemDoc.cpp : CDIPSystemDoc ���ʵ��
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


// CDIPSystemDoc ����/����

CDIPSystemDoc::CDIPSystemDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CDIPSystemDoc::~CDIPSystemDoc()
{
}

BOOL CDIPSystemDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CDIPSystemDoc ���л�

void CDIPSystemDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CDIPSystemDoc ���

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


// CDIPSystemDoc ����
