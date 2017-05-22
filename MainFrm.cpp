// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "DIPSystem.h"

#include "MainFrm.h"

#include "DIPSystemDoc.h"
#include "DIPSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_MYTREECTRL, OnItemexpandedTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MYTREECTRL, OnSelchangedTree)
	ON_COMMAND(ID_VIEW_CONTROL, &CMainFrame::OnViewControl)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CONTROL, &CMainFrame::OnUpdateViewControl)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_INDICATOR_PICNUM,	// ͼƬ����
	ID_INDICATOR_PICNAME,	// ��ǰ��ʾ��ͼƬ�ļ���
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CreateCtrlBar();

	return 0;
}

/******************************************************************************
*	����:		�������ƴ���
*	��ע:		���ƴ��ڰ����ļ�Ŀ¼��λͼ�����������,ͨ��
******************************************************************************/
BOOL CMainFrame::CreateCtrlBar(void)
{
	if ( !m_wndCtrlBar.Create(L"���ƴ���", this, 100) ) 
	{ 
		TRACE0("δ�ܴ������ƴ���\n"); 
		return -1; 
	}

	m_wndCtrlBar.SetBarStyle(m_wndCtrlBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC); 

	m_wndCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndCtrlBar, AFX_IDW_DOCKBAR_LEFT);	// ͣ�������

	// ����һ��ѡ��ؼ�
	m_TabCtrl.Create(TCS_DOWN | WS_CHILD | WS_VISIBLE, CRect(0,0,100,100), &m_wndCtrlBar, 125);

	// �������Ϳؼ�
	if (!m_TreeCtrl.Create(WS_CHILD|WS_VISIBLE|
		TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_TabCtrl, IDC_MYTREECTRL))
	{
		TRACE0("����ʧ�ܣ�\n");
		return -1;
	}

	m_TreeCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	DWORD dwStyle = GetWindowLong(m_TreeCtrl.m_hWnd, GWL_STYLE);

	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLong(m_TreeCtrl.m_hWnd, GWL_STYLE, dwStyle);

	m_hRoot = m_TreeCtrl.InsertItem(L"�ҵĵ���");
	InsertLogicalDrives(m_hRoot);
	InsertDriveDir(m_hRoot);
	m_TreeCtrl.Expand(m_hRoot,TVE_EXPAND);

	// �����Ϳؼ����뵽TabCtrl��
	m_TabCtrl.AddPage(&m_TreeCtrl, L"�ļ�Ŀ¼", IDI_DIR);
	m_TabCtrl.AddPage(RUNTIME_CLASS(CPrevDlg), IDD_PREV, L"ͼ�����", IDI_DIR);
	m_TabCtrl.UpdateWindow();

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style &= ~FWS_ADDTOTITLE;	// ȡ��MFC�Զ���ӱ���ķ��

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������

/******************************************************************************
*	����:		��ָ�����ڵ��²���ϵͳ�������ڵ�
******************************************************************************/
void CMainFrame::InsertLogicalDrives(HTREEITEM hParent)
{
	DWORD	szAllDriveStrings = GetLogicalDriveStrings(0,NULL);
	LPWSTR	pDriveStrings = new WCHAR[szAllDriveStrings + sizeof(_T(""))];
	LPWSTR	pDel = pDriveStrings;
	GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
	size_t szDriveString = wcslen(pDriveStrings);
	while(szDriveString > 0)
	{
		m_TreeCtrl.InsertItem(pDriveStrings, hParent);
		pDriveStrings += szDriveString + 1;
		szDriveString = wcslen(pDriveStrings);
	}

	SAFE_DELETE_ARRAY (pDel);
}

/******************************************************************************
*	����:		��ָ�����ڵ��²����������µ���������
******************************************************************************/
void CMainFrame::InsertDriveDir(HTREEITEM hParent)
{
	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hParent);
	while(hChild)
	{
		CString strText = m_TreeCtrl.GetItemText(hChild);
		if(strText.Right(1) != L"\\")
			strText += L"\\";
		strText += L"*.*";
		CFileFind file;
		BOOL bContinue = file.FindFile(strText);
		while(bContinue)
		{
			bContinue = file.FindNextFile();
			if(!file.IsDots())
				m_TreeCtrl.InsertItem(file.GetFileName(), hChild);
		}
		InsertDriveDir(hChild);
		hChild = m_TreeCtrl.GetNextItem(hChild, TVGN_NEXT);
		file.Close();
	}	
}

/******************************************************************************
*	����:		��ָ�����ڵ��²���Ŀ¼�µ���������
******************************************************************************/
void CMainFrame::AddSubDir(HTREEITEM hParent)
{
	CString strPath = GetFullPath(hParent);
	if(strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += L"*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);
	while(bContinue)
	{
		bContinue = file.FindNextFile();
		if( !file.IsDots())
			m_TreeCtrl.InsertItem(file.GetFileName(), hParent);
	}
}

/******************************************************************************
*	����:		��ȡָ���ڵ��ȫ·��
******************************************************************************/
CString CMainFrame::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = L"";
	while(hCurrent != m_hRoot)
	{
		strTemp = m_TreeCtrl.GetItemText(hCurrent);
		if(strTemp.Right(1) != "\\")
			strTemp += L"\\";
		strReturn = strTemp  + strReturn;
		hCurrent = m_TreeCtrl.GetParentItem(hCurrent);
	}

	return strReturn;
}

/******************************************************************************
*	����:		չ������ͼ��ĳһ�����Ӧ����
******************************************************************************/
void CMainFrame::OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)
		return;
	HTREEITEM hChild = m_TreeCtrl.GetChildItem(item.hItem);
	while(hChild)
	{
		AddSubDir(hChild);
		hChild = m_TreeCtrl.GetNextItem(hChild,TVGN_NEXT);
	}
	*pResult = 0;
}

/******************************************************************************
*	����:		���ָ��Ŀ¼�����е�ͼ���ļ���
*	����:		
*		path	ָ��Ŀ¼·��
*		ary		����ͼ���ļ������ַ�������
******************************************************************************/
void CMainFrame::GetAllFileNames(const CString& path, CStringArray& ary)
{
	ary.RemoveAll();

	CFileFind fileFind;
	CString filesPath = path + L"\\*.*";

	if (fileFind.FindFile(filesPath))
	{

		CString strPicPath;
		CString strExt;

		// �����Ŀ¼�����ļ�
		while (fileFind.FindNextFile())
		{
			strPicPath = fileFind.GetFileName();
			// ��ȡ��׺��
			strExt = strPicPath.Right(3);
			strExt.MakeLower();

			// �����׺��Ϊbmp,jpg���ж���ͼƬ�ļ�
			// �˴��������书��
			if (strExt == "bmp" || strExt == "jpg" || strExt == "png" || strExt == "tif" || strExt == "gif")
				ary.Add(strPicPath);
		}

		strPicPath = fileFind.GetFileName();
		// ��ȡ��׺��
		strExt = strPicPath.Right(3);
		strExt.MakeLower();

		// �����׺��Ϊbmp,jpg���ж���ͼƬ�ļ�
			// �˴��������书��
		if (strExt == "bmp" || strExt == "jpg")
			ary.Add(strPicPath);
	}

	fileFind.Close();
}

/******************************************************************************
*	����:		����ͼѡ��ı����Ӧ����
******************************************************************************/
void CMainFrame::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)
		return;
	CString str = GetFullPath(item.hItem);
	str = str.Left(str.GetLength() - 1);
	CFileFind fileFile;

	if( fileFile.FindFile(str) )
	{
		CString strExt;

		fileFile.FindNextFile();
		if(!fileFile.IsDirectory() && !fileFile.IsDots())
		{
			strExt = str.Right(3);
			strExt.MakeLower();
		
			// �����׺��Ϊbmp,jpg���ж���ͼƬ�ļ�
			// �˴��������书��
			if (strExt == "bmp" || strExt == "jpg")
			{
				CDIPSystemView* pView = (CDIPSystemView*)GetActiveView();

				pView->ClearOffset();
				pView->OpenFile(str);
				pView->FixOffset();
				pView->RefreshIndicator();
			}
		}
	}
	fileFile.Close();

	*pResult = 0;
}

/******************************************************************************
*	����:		���ڴ�С�ı����Ӧ����
******************************************************************************/
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	CDIPSystemView* pView = (CDIPSystemView*)GetActiveView();

	if (pView != NULL)
	{
		pView->ClearOffset();
		pView->FixOffset();
	}
}


/******************************************************************************
*	����:		��ʾ/���ء����ƴ��ڡ�
******************************************************************************/
void CMainFrame::OnViewControl()
{
	BOOL bVisible = ((m_wndCtrlBar.IsWindowVisible()) != 0);

	ShowControlBar(&m_wndCtrlBar, !bVisible, FALSE);
	RecalcLayout();
}

/******************************************************************************
*	����:		�����ƴ��ڡ��˵����������º���
******************************************************************************/
void CMainFrame::OnUpdateViewControl(CCmdUI *pCmdUI)
{
	BOOL bVisible = ((m_wndCtrlBar.IsWindowVisible()) != 0);
	pCmdUI->SetCheck(bVisible);
}
