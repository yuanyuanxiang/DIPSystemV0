// LineTranDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DIPSystem.h"
#include "LineTranDlg.h"
#include "Mainfrm.h"
#include "DIPSystemDoc.h"
#include "DIPSystemView.h"
#include <cmath>

#include "GrayOperator.h"


// CLineTranDlg �Ի���

IMPLEMENT_DYNAMIC(CLineTranDlg, CDialog)

CLineTranDlg::CLineTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineTranDlg::IDD, pParent)
	, m_dSlope(1)
	, m_dIntercept(0)
{
	dlg = NULL;
	m_bModify = false;
	m_bInitialized = false;
}

CLineTranDlg::~CLineTranDlg()
{
	SAFE_DELETE (dlg);
}

void CLineTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SLOPE, m_dSlope);
	DDX_Text(pDX, IDC_INTERCEPT, m_dIntercept);
	DDX_Control(pDX, IDC_PREV, m_stiPrev);
}

BEGIN_MESSAGE_MAP(CLineTranDlg, CDialog)
	ON_EN_CHANGE(IDC_SLOPE, &CLineTranDlg::OnEnChangeSlope)
	ON_EN_CHANGE(IDC_INTERCEPT, &CLineTranDlg::OnEnChangeIntercept)
	ON_BN_CLICKED(IDC_SAVE, &CLineTranDlg::OnBnClickedSave)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CLineTranDlg::OnBnClickedOk)
END_MESSAGE_MAP()


/******************************************************************************
*	����:		�Ի����ʼ��ʱ��ʾԤ���Ի���
******************************************************************************/
BOOL CLineTranDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	dlg = new CPreviewDlg(this);

	dlg->m_nType = 1;
	dlg->m_bEnableClose = false;
	dlg->Create(IDD_PREVIEW);
	dlg->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/******************************************************************************
*	����:		�Ի����ػ溯��
******************************************************************************/
void CLineTranDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	Refresh();

	if (!m_bInitialized)
	{
		m_bInitialized = true;
		Invalidate(TRUE);
	}
}

/******************************************************************************
*	����:		ˢ��Ԥ����
******************************************************************************/
void CLineTranDlg::Refresh()
{
	UpdateData(TRUE);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	LineTrans(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
		pView->m_nPicWidth, pView->m_nPicHeight, m_dSlope, m_dIntercept);

	dlg->Refresh();

	CDC*	pDC = m_stiPrev.GetDC();
	CRect	rect;
	CDC		memDC;
	CBitmap MemBitmap;

	// ��ȡ��ͼ����
	m_stiPrev.GetClientRect(rect);

	// �豸�������ʼ��
	memDC.CreateCompatibleDC(NULL);

	// ��������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	// ѡȡ�հ�λͼ
	memDC.SelectObject(MemBitmap);
	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));

	Graphics graph(memDC.GetSafeHdc());

	// ʹ�ð�ɫ����
	graph.FillRectangles(&SolidBrush(Color::White), 
		&Rect(0, 0, rect.Width(), rect.Height()), 
		1);

	// ����y��
	graph.DrawLine(&Pen(Color::Black), 10, 10, 10, 280);
	graph.DrawLine(&Pen(Color::Black), 10, 10, 5, 15);
	graph.DrawLine(&Pen(Color::Black), 10, 10, 15, 15);

	// ����x��
	graph.DrawLine(&Pen(Color::Black), 10, 280, 290, 280);
	graph.DrawLine(&Pen(Color::Black), 290, 280, 285, 285);
	graph.DrawLine(&Pen(Color::Black), 290, 280, 285, 275);

	// ��������ԭ��
	CString strNum;
	Font font(L"����", 10);
	strNum = L"0";
	graph.DrawString(strNum, -1, &font, 
		PointF(8, 290), &SolidBrush(Color::Black));

	for (int i = 0; i < 256; i += 5)
	{
		if (i % 50 == 0)
			graph.DrawLine(&Pen(Color::Black), 10 + i, 280, 10 + i, 286);
		else if (i % 10 == 0)
			graph.DrawLine(&Pen(Color::Black), 10 + i, 280, 10 + i, 283);
	}

	// ����x��̶�
	strNum = L"50";
	graph.DrawString(strNum, -1, &font, 
		PointF(53, 290), &SolidBrush(Color::Black));
	strNum = L"100";
	graph.DrawString(strNum, -1, &font, 
		PointF(100, 290), &SolidBrush(Color::Black));
	strNum = L"150";
	graph.DrawString(strNum, -1, &font, 
		PointF(150, 290), &SolidBrush(Color::Black));
	strNum = L"200";
	graph.DrawString(strNum, -1, &font, 
		PointF(200, 290), &SolidBrush(Color::Black));
	strNum = L"255";
	graph.DrawString(strNum, -1, &font, 
		PointF(255, 290), &SolidBrush(Color::Black));
	// y��
	graph.DrawString(strNum, -1, &font, 
		PointF(10, 25), &SolidBrush(Color::Black));

	// ���Ʊ任����ͼ��
	PointF startPs;
	PointF endPs;

	if ( m_dIntercept >= 0.0 || fabs(m_dSlope-0.0) < 0.0001)
	{
		startPs.X = 0;
		startPs.Y = (REAL)m_dIntercept;
	}
	else
	{
		startPs.X = (REAL)(-m_dIntercept / m_dSlope);
		startPs.Y = 0;
	}

	endPs.X = 255;
	endPs.Y = (REAL)(m_dSlope * 255 + m_dIntercept);

	if (endPs.Y < 255 && fabs(m_dSlope-0.0) < 0.000)
	{
		endPs.Y = 255;
		endPs.X = (REAL)(255 - m_dIntercept / m_dSlope);
	}

	// ת������,��(10, 280)ת��Ϊ����ԭ��
	Matrix matrix1;
	Matrix matrix2(0, 1, 1, 0, 0, 0);	// ת�þ���

	matrix1.Translate(10, 280);
	matrix1.Rotate(270);
	matrix1.Multiply(&matrix2);
	graph.SetTransform(&matrix1);

	graph.DrawLine(&Pen(Color::Black), startPs.X, startPs.Y, endPs.X, endPs.Y);

	// �����ڴ滭������
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	m_stiPrev.ReleaseDC(pDC);
}

/******************************************************************************
*	����:		�ı�б��ʱ����Ӧ����
******************************************************************************/
void CLineTranDlg::OnEnChangeSlope()
{
	UpdateData(TRUE);

	Refresh();
	m_bModify = true;
}


/******************************************************************************
*	����:		�ı�ؾ�ʱ����Ӧ����
******************************************************************************/
void CLineTranDlg::OnEnChangeIntercept()
{
	UpdateData(TRUE);

	Refresh();
	m_bModify = true;
}

/******************************************************************************
*	����:		�����桱��ť��Ӧ����
******************************************************************************/
void CLineTranDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	pView->OnFileSave();
	m_bModify = false;
}

void CLineTranDlg::OnBnClickedOk()
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