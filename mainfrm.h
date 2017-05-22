// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

#include "CoolTabCtrl.h"
#include "PrevDlg.h"

class CMainFrame : public CFrameWnd
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CMainFrame)

// ��д
public:
	CMainFrame();
	virtual ~CMainFrame();

	// ���ָ��Ŀ¼�����е�ͼ���ļ���
	static void GetAllFileNames(const CString& path, CStringArray& ary);
													
	/**************************************************************************
	*	����:			����״̬����ָʾ��
	*	����:		
	*		nID			ָʾ����ID
	*		strShow		ָʾ����ʾ���ַ���
	**************************************************************************/
	void UpdateIndicator(int nID, const CString& strShow)
	{
		int nIndex = m_wndStatusBar.CommandToIndex(nID);
		m_wndStatusBar.SetPaneText(nIndex, strShow);

		int nLength = strShow.GetLength();		// �����ı��ı�ָʾ����С
		m_wndStatusBar.SetPaneInfo(nIndex, nID, SBPS_NORMAL, nLength * 8);
	}

	// ��Ϣ��Ӧ����
	afx_msg void OnViewControl();				// ��ʾ/���ء����ƴ��ڡ�
	afx_msg void OnUpdateViewControl(CCmdUI *pCmdUI);
												// �����ƴ��ڡ��˵����������º���
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult);
												// չ������ͼ��ĳһ�����Ӧ����
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
												// ����ͼѡ��ı����Ӧ����
	afx_msg void OnSize(UINT nType, int cx, int cy);
												// ���ڴ�С�ı����Ӧ����
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar  m_wndStatusBar;					// ״̬��
	CToolBar    m_wndToolBar;					// ������

private:
	CCoolBar		m_wndCtrlBar;				// �������ƴ���
	CCoolTabCtrl	m_TabCtrl;					// ѡ��ؼ�, ���ڹ����Ӵ���
	CTreeCtrl		m_TreeCtrl;					// ���Ϳؼ�,������ʾĿ¼��
	HTREEITEM		m_hRoot;					// ����һ�����ͽڵ�

	BOOL CreateCtrlBar(void);					// �������ƴ���
	void AddSubDir(HTREEITEM hParent);			// ����Ŀ¼�µ���������
	CString GetFullPath(HTREEITEM hCurrent);	// ��ȡָ���ڵ��ȫ·��
	void InsertDriveDir(HTREEITEM hParent);		// �����������µ���������
	void InsertLogicalDrives(HTREEITEM hParen);	// ����ϵͳ�������ڵ�
};


