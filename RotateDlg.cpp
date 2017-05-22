// RotateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DIPSystem.h"
#include "RotateDlg.h"
#include "GeoOperator.h"
#include "Mainfrm.h"
#include "DIPSystemDoc.h"
#include "DIPSystemView.h"


// CRotateDlg �Ի���

#define PI 3.1415926535

IMPLEMENT_DYNAMIC(CRotateDlg, CDialog)

CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRotateDlg::IDD, pParent)
	, m_nAngle(0)
	, m_nPos(0)
	, m_nType(0)
{
}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ANGLE, m_nAngle);
	DDX_Slider(pDX, IDC_SLIDER, m_nPos);
	DDX_CBIndex(pDX, IDC_MODE, m_nType);
}

BEGIN_MESSAGE_MAP(CRotateDlg, CDialog)
	ON_BN_CLICKED(IDC_PREV, &CRotateDlg::OnBnClickedPrev)
	ON_EN_CHANGE(IDC_ANGLE, &CRotateDlg::OnEnChangeAngle)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SAVE, &CRotateDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CRotateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRotateDlg ��Ϣ�������

/******************************************************************************
*	����:		�Ի����ʼ��
******************************************************************************/
BOOL CRotateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �޸ġ���ת�Ƕȡ��ķ�Χ
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetRange(0, 360);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/******************************************************************************
*	����:		��Ԥ��Ч������ť����Ӧ����
******************************************************************************/
void CRotateDlg::OnBnClickedPrev()
{
	Refresh();

	CPreviewDlg dlg;
	dlg.m_nType = 1;
	dlg.DoModal();
}

/******************************************************************************
*	����:		���½���ͼ����ת
******************************************************************************/
void CRotateDlg::Refresh()
{
	UpdateData(TRUE);

	// ����Ԥ���Ի���
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	SAFE_DELETE_ARRAY (pView->m_pImageTempBuffer);

	if (m_nType == 0)
		RotateNormal(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
					 pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nTempWidth, 
					 pView->m_nTempHeight, m_nAngle / 180.0 * PI);
	else
		RotateInterpolation(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
					 pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nTempWidth, 
					 pView->m_nTempHeight, m_nAngle / 180.0 * PI);
}

/******************************************************************************
*	����:		���Ƕȡ��ı�ʱ����Ӧ����
******************************************************************************/
void CRotateDlg::OnEnChangeAngle()
{
	UpdateData(TRUE);

	if (m_nAngle < 0)
		m_nAngle = 0;
	if (m_nAngle > 360)
		m_nAngle = 360;

	m_nPos = m_nAngle;

	UpdateData(FALSE);
}

/******************************************************************************
*	����:		ˮƽ�������Ӧ����
******************************************************************************/
void CRotateDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);
	m_nAngle = m_nPos;
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRotateDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	Refresh();
	pView->OnFileSave();
	m_bModify = false;
}

void CRotateDlg::OnBnClickedOk()
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
