// StretchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DIPSystem.h"
#include "StretchDlg.h"
#include <cmath>
#include "Mainfrm.h"
#include "DIPSystemDoc.h"
#include "DIPSystemView.h"
#include "GrayOperator.h"

#pragma warning ( disable : 4244)


// CStretchDlg �Ի���

IMPLEMENT_DYNAMIC(CStretchDlg, CDialog)

CStretchDlg::CStretchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStretchDlg::IDD, pParent)
	, m_dPsX1(50)
	, m_dPsY1(50)
	, m_dPsX2(150)
	, m_dPsY2(150)
{
	m_bIsDraging = false;
	m_nPointType = PT_NULL;
	m_bModify = false;
	m_bInitialized = false;
}

CStretchDlg::~CStretchDlg()
{
	SAFE_DELETE (dlg);
}

void CStretchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POINTONE_X, m_dPsX1);
	DDX_Text(pDX, IDC_POINTONE_Y, m_dPsY1);
	DDX_Text(pDX, IDC_POINTTWO_X, m_dPsX2);
	DDX_Text(pDX, IDC_POINTTWO_Y, m_dPsY2);
	DDX_Control(pDX, IDC_PREV, m_stiPrev);
}


BEGIN_MESSAGE_MAP(CStretchDlg, CDialog)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_POINTONE_X, &CStretchDlg::OnEnChangePointoneX)
	ON_EN_CHANGE(IDC_POINTONE_Y, &CStretchDlg::OnEnChangePointoneY)
	ON_EN_CHANGE(IDC_POINTTWO_X, &CStretchDlg::OnEnChangePointtwoX)
	ON_EN_CHANGE(IDC_POINTTWO_Y, &CStretchDlg::OnEnChangePointtwoY)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_SAVE, &CStretchDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CStretchDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CStretchDlg ��Ϣ�������

void CStretchDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	Refresh();

	if (!m_bInitialized)
	{
		m_bInitialized = true;
		Invalidate();
	}
}

/******************************************************************************
*	����:		ˢ��Ԥ����
******************************************************************************/
void CStretchDlg::Refresh()
{
	UpdateData(TRUE);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	GrayStretch(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
				pView->m_nPicWidth, pView->m_nPicHeight, 
				m_dPsX1, m_dPsY1, m_dPsX2, m_dPsY2);

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

	// ת������,��(10, 280)ת��Ϊ����ԭ��
	Matrix matrix1;
	Matrix matrix2(0, 1, 1, 0, 0, 0);	// ת�þ���

	matrix1.Translate(10, 280);
	matrix1.Rotate(270);
	matrix1.Multiply(&matrix2);
	graph.SetTransform(&matrix1);

	// ���Ʒֶκ���
	graph.DrawLine(&Pen(Color::Black), 0.0f, 0.0f, m_dPsX1, m_dPsY1);
	graph.DrawLine(&Pen(Color::Blue), m_dPsX1, m_dPsY1, m_dPsX2, m_dPsY2);
	graph.DrawLine(&Pen(Color::Black), m_dPsX2, m_dPsY2, 255.0f, 255.0f);

	// �����������Ƶ�
	graph.FillEllipse(&SolidBrush(Color::Red), RectF(m_dPsX1 - 2.5f, m_dPsY1 - 2.5f, 5, 5));
	graph.FillEllipse(&SolidBrush(Color::Red), RectF(m_dPsX2 - 2.5f, m_dPsY2 - 2.5f, 5, 5));

	// �����ڴ滭������
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	m_stiPrev.ReleaseDC(pDC);
}

/******************************************************************************
*	����:		�Ի����ʼ������
******************************************************************************/
BOOL CStretchDlg::OnInitDialog()
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
*	����:		�ı���Ƶ�1��x����ʱ����Ӧ����
******************************************************************************/
void CStretchDlg::OnEnChangePointoneX()
{
	UpdateData(TRUE);
	
	if (m_dPsX1 < 0)
		m_dPsX1 = 0;
	if (m_dPsX1 > m_dPsX2)
		m_dPsX1 = m_dPsX2;

	UpdateData(FALSE);

	Refresh();
	m_bModify = true;
}

/******************************************************************************
*	����:		�ı���Ƶ�1��y����ʱ����Ӧ����
******************************************************************************/
void CStretchDlg::OnEnChangePointoneY()
{
	UpdateData(TRUE);
	
	if (m_dPsY1 < 0)
		m_dPsY1 = 0;
	if (m_dPsY1 > m_dPsY2)
		m_dPsY1 = m_dPsY2;

	UpdateData(FALSE);

	Refresh();
	m_bModify = true;
}

/******************************************************************************
*	����:		�ı���Ƶ�2��x����ʱ����Ӧ����
******************************************************************************/
void CStretchDlg::OnEnChangePointtwoX()
{
	UpdateData(TRUE);

	if (m_dPsX2 < m_dPsX1)
		m_dPsX2 = m_dPsX1;
	if (m_dPsX2 > 255)
		m_dPsX2 = 255;

	UpdateData(FALSE);

	Refresh();
	m_bModify = true;
}

/******************************************************************************
*	����:		�ı���Ƶ�2��y����ʱ����Ӧ����
******************************************************************************/
void CStretchDlg::OnEnChangePointtwoY()
{
	UpdateData(TRUE);

	if (m_dPsY2 < m_dPsY1)
		m_dPsY2 = m_dPsY1;
	if (m_dPsY2 > 255)
		m_dPsY2 = 255;

	UpdateData(FALSE);

	Refresh();
	m_bModify = true;
}

/******************************************************************************
*	����:		����ƶ�ʱ��Ӧ�϶�����
******************************************************************************/
void CStretchDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( (nFlags & MK_LBUTTON) && m_bIsDraging )
	{
		CRect rect;
		ClientToScreen(&point);
		m_stiPrev.GetWindowRect(rect);

		// ��ȡ��ǰ���Ƶ��������ֵ
		int x = point.x - 10 - rect.left;
		int y = 280 - point.y + rect.top;

		int xMax, xMin, yMax, yMin;

		// ���ݵ�ǰ����ȷ�����Ƶ�ȡֵ��Χ
		switch (m_nPointType)
		{
		case PT_POINT_ONE:
			xMin = 0;
			xMax = m_dPsX2;
			yMin = 0;
			yMax = m_dPsY2;
			break;

		case PT_POINT_TWO:
			xMin = m_dPsX1;
			xMax = 255;
			yMin = m_dPsY1;
			yMax = 255;
			break;
		default:;
		}

		// �жϵ�ǰ���Ƿ񳬽�
		if (x < xMin)
			x = xMin;
		else if (x > xMax)
			x = xMax;
		if (y < yMin)
			y = yMin;
		else if (y > yMax)
			y = yMax;

		// ��������
		if (m_nPointType == PT_POINT_ONE)
		{
			m_dPsX1 = x;
			m_dPsY1 = y;
		}
		else if (m_nPointType == PT_POINT_TWO)
		{
			m_dPsX2 = x;
			m_dPsY2 = y;
		}

		UpdateData(FALSE);
		Refresh();
		m_bModify = true;
	}
}

/******************************************************************************
*	����:		��������������Ӧ����
******************************************************************************/
void CStretchDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nPointType != PT_NULL)
		m_bIsDraging = true;
}

/******************************************************************************
*	����:		�ͷ�����������Ӧ����
******************************************************************************/
void CStretchDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bIsDraging = false;
	m_nPointType = PT_NULL;
}


/******************************************************************************
*	����:		���������
******************************************************************************/
BOOL CStretchDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	UpdateData(TRUE);

	CPoint point;
	CRect rect;

	GetCursorPos(&point);
	m_stiPrev.GetWindowRect(rect);

	// �Ƿ���Ҫ���������
	CRect rect1(CPoint(rect.left + 10 + m_dPsX1 - 2.5f, rect.top + 280 - m_dPsY1 - 2.5f), CSize(5, 5));
	if (rect1.PtInRect(point))
	{
		m_nPointType = PT_POINT_ONE;
		SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		return TRUE;
	}

	CRect rect2(CPoint(rect.left + 10 + m_dPsX2 - 2.5f, rect.top + 280 - m_dPsY2 - 2.5f), CSize(5, 5));
	if (rect2.PtInRect(point))
	{
		m_nPointType = PT_POINT_TWO;
		SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
		return TRUE;
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

/******************************************************************************
*	����:		�����桱��ť��Ӧ����
******************************************************************************/
void CStretchDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	pView->OnFileSave();
	m_bModify = false;
}


void CStretchDlg::OnBnClickedOk()
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
