// MirTranDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DIPSystem.h"
#include "MirTranDlg.h"
#include "Mainfrm.h"
#include "DIPSystemDoc.h"
#include "DIPSystemView.h"
#include "GeoOperator.h"


// CMirTranDlg �Ի���

IMPLEMENT_DYNAMIC(CMirTranDlg, CDialog)

CMirTranDlg::CMirTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMirTranDlg::IDD, pParent)
	, m_nType(0)
{

}

CMirTranDlg::~CMirTranDlg()
{
	SAFE_DELETE (dlg);
}

void CMirTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_MIRROR_H, m_nType);
}


BEGIN_MESSAGE_MAP(CMirTranDlg, CDialog)
	ON_BN_CLICKED(IDC_MIRROR_H, &CMirTranDlg::OnBnClickedMirrorH)
	ON_BN_CLICKED(IDC_MIRROR_V, &CMirTranDlg::OnBnClickedMirrorV)
	ON_BN_CLICKED(IDC_SAVE, &CMirTranDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CMirTranDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMirTranDlg ��Ϣ�������

BOOL CMirTranDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	dlg = new CPreviewDlg(this);

	dlg->m_nType = 1;
	dlg->m_bEnableClose = false;
	dlg->Create(IDD_PREVIEW);
	dlg->ShowWindow(SW_SHOW);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/******************************************************************************
*	����:		���½��о���任��ˢ��Ԥ������
******************************************************************************/
void CMirTranDlg::Refresh()
{
	UpdateData(TRUE);

	// ����Ԥ���Ի���
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	SAFE_DELETE_ARRAY (pView->m_pImageTempBuffer);

	if (m_nType == 0)
		HMirrorTrans(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
		pView->m_nPicWidth, pView->m_nPicHeight);
	else
		VMirrorTrans(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
		pView->m_nPicWidth, pView->m_nPicHeight);

	dlg->Refresh();
}

/******************************************************************************
*	����:		ѡ��ˮƽ���񡱵�������Ӧ����
******************************************************************************/
void CMirTranDlg::OnBnClickedMirrorH()
{
	Refresh();
}

/******************************************************************************
*	����:		ѡ�񡰴�ֱ���񡱵�������Ӧ����
******************************************************************************/
void CMirTranDlg::OnBnClickedMirrorV()
{
	Refresh();
}

void CMirTranDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	pView->OnFileSave();
	m_bModify = false;
}

void CMirTranDlg::OnBnClickedOk()
{
	if (m_bModify)
	{
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();
		pView->SaveImage();
		pView->Invalidate(TRUE);
	}

	CDialog::OnOK();
}
