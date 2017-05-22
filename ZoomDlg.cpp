// ZoomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DIPSystem.h"
#include "ZoomDlg.h"
#include "GeoOperator.h"
#include "Mainfrm.h"
#include "DIPSystemDoc.h"
#include "DIPSystemView.h"


// CZoomDlg �Ի���

IMPLEMENT_DYNAMIC(CZoomDlg, CDialog)

CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_nType(0)
	, m_bIsPro(TRUE)
{
}

CZoomDlg::~CZoomDlg()
{
}

void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDX_CBIndex(pDX, IDC_MODE, m_nType);
	DDX_Check(pDX, IDC_PRO, m_bIsPro);
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
	ON_BN_CLICKED(IDC_PREV, &CZoomDlg::OnBnClickedPrev)
	ON_EN_CHANGE(IDC_WIDTH, &CZoomDlg::OnEnChangeWidth)
	ON_EN_CHANGE(IDC_HEIGHT, &CZoomDlg::OnEnChangeHeight)
	ON_BN_CLICKED(IDC_PRO, &CZoomDlg::OnBnClickedPro)
	ON_BN_CLICKED(IDC_SAVE, &CZoomDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CZoomDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CZoomDlg ��Ϣ�������

/******************************************************************************
*	����:		��Ԥ��Ч������ť����Ӧ����
******************************************************************************/
void CZoomDlg::OnBnClickedPrev()
{
	Refresh();

	CPreviewDlg dlg;
	dlg.m_nType = 1;
	dlg.DoModal();
}

/******************************************************************************
*	����:		���½���ͼ������
******************************************************************************/
void CZoomDlg::Refresh()
{
	UpdateData(TRUE);

	// ����Ԥ���Ի���
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();
	double fx = (double)m_nWidth / pView->m_nPicWidth;
	double fy = (double)m_nHeight / pView->m_nPicHeight;

	SAFE_DELETE_ARRAY (pView->m_pImageTempBuffer);

	if (m_nType == 0)
		ZoomNormal(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
				   pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nTempWidth, 
				   pView->m_nTempHeight, fx, fy);
	else
		ZoomInterpolation(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
						  pView->m_nPicWidth, pView->m_nPicHeight, 
						  pView->m_nTempWidth, pView->m_nTempHeight, 
						  fx, fy);
	m_bModify = true;
}

/******************************************************************************
*	����:		�Ի����ʼ��
******************************************************************************/
BOOL CZoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	m_nWidth = m_nSrcWidth = pView->m_nPicWidth;
	m_nHeight = m_nSrcHeight = pView->m_nPicHeight;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CZoomDlg::OnEnChangeWidth()
{
	UpdateData(TRUE);

	if (m_nWidth > 4000)
		m_nWidth = 4000;
	if (m_nWidth <= 0)
		m_nWidth = 1;

	UpdateData(FALSE);
	RefreshSize(true);
}

void CZoomDlg::OnEnChangeHeight()
{
	UpdateData(TRUE);

	if (m_nHeight > 4000)
		m_nHeight = 4000;
	if (m_nHeight <= 0)
		m_nHeight = 1;

	UpdateData(FALSE);
	RefreshSize(false);
}

void CZoomDlg::OnBnClickedPro()
{
	RefreshSize(true);
}

/******************************************************************************
*	����:		�Զ��������ͼ��Ĵ�С
*	����:		
*		width	�Ƿ��Կ������Ϊ��׼
******************************************************************************/
void CZoomDlg::RefreshSize(bool width)
{
	UpdateData(TRUE);

	if (!m_bIsPro)
		return;

	if (width)	// �Կ�����ű����޸ĸ߶�
	{
		double fx = (double)m_nWidth / m_nSrcWidth;
		m_nHeight = (int)(m_nSrcHeight * fx);
	}
	else		// �Ը߶����ű����޸Ŀ��
	{
		double fy = (double)m_nHeight / m_nSrcHeight;
		m_nWidth = (int)(m_nSrcWidth * fy);
	}

	UpdateData(FALSE);
}

void CZoomDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	Refresh();
	pView->OnFileSave();
	m_bModify = false;
}

void CZoomDlg::OnBnClickedOk()
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
