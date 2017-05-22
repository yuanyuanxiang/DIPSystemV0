// HistogramDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DIPSystem.h"
#include "HistogramDlg.h"
#include "Mainfrm.h"
#include "DIPSystemDoc.h"
#include "DIPSystemView.h"

// CHistogramDlg �Ի���

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialog)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistogramDlg::IDD, pParent)
	, m_nLimitLow(0)
	, m_nLimitUp(255)
	, m_nGray(0)
	, m_dPer(0)
{
	memset(m_lCounts, 0, sizeof(long) * 256);
	m_nIsDraging = DT_NULL;
	m_bInitialized = false;
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LIMIT_LOWER, m_nLimitLow);
	DDX_Text(pDX, IDC_LIMIT_UP, m_nLimitUp);
	DDX_Control(pDX, IDC_HISTOGRAM, m_stiHistogram);
	DDX_Text(pDX, IDC_STATIC_GRAY, m_nGray);
	DDX_Text(pDX, IDC_STATIC_PER, m_dPer);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialog)
	ON_EN_CHANGE(IDC_LIMIT_LOWER, &CHistogramDlg::OnEnChangeLimitLower)
	ON_EN_CHANGE(IDC_LIMIT_UP, &CHistogramDlg::OnEnChangeLimitUp)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CHistogramDlg ��Ϣ�������

/******************************************************************************
*	����:		�Ի����ʼ��ʱ��������Ҷ�����
******************************************************************************/
BOOL CHistogramDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	for (UINT i = 0; i < pView->m_nPicWidth * pView->m_nPicHeight; i++)
	{
		int value = pView->m_pImageTempBuffer[i * 4];

		m_lCounts[value]++;
	}

	// ���������ܸ���
	m_nPixelCount = pView->m_nPicWidth * pView->m_nPicHeight;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


/******************************************************************************
*	����:		�Ҷ����޸ı����Ӧ����
******************************************************************************/
void CHistogramDlg::OnEnChangeLimitLower()
{
	UpdateData(TRUE);

	// �޶�ȡֵ��Χ
	if (m_nLimitLow < 0)
		m_nLimitLow = 0;
	else if (m_nLimitLow > 255)
		m_nLimitLow = 255;

	// ������ޱ����޴��򻥻�
	if (m_nLimitLow > m_nLimitUp)
	{
		int nTemp = m_nLimitLow;
		m_nLimitLow = m_nLimitUp;
		m_nLimitUp = nTemp;
	}

	Refresh();
	UpdateData(FALSE);
}


/******************************************************************************
*	����:		�Ҷ����޸ı����Ӧ����
******************************************************************************/
void CHistogramDlg::OnEnChangeLimitUp()
{
	UpdateData(TRUE);

	// �޶�ȡֵ��Χ
	if (m_nLimitUp < 0)
		m_nLimitUp = 0;
	else if (m_nLimitUp > 255)
		m_nLimitUp = 255;

	if (m_nLimitLow > m_nLimitUp)
	{
		int nTemp = m_nLimitLow;
		m_nLimitLow = m_nLimitUp;
		m_nLimitUp = nTemp;
	}

	Refresh();
	UpdateData(FALSE);
}

/******************************************************************************
*	����:		���ƶԻ���
******************************************************************************/
void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	Refresh();

	// ǿ��ˢ��һ��
	if (!m_bInitialized)
	{
		m_bInitialized = true;
		Invalidate(TRUE);
	}
}

/******************************************************************************
*	����:		ˢ��ֱ��ͼ
*	��ע:		˫������Ʒ���
******************************************************************************/
void CHistogramDlg::Refresh()
{
	CDC*	pDC = m_stiHistogram.GetDC();
	CRect	rect;
	CDC		memDC;
	CBitmap MemBitmap;

	// ��ȡ��ͼ����
	m_stiHistogram.GetClientRect(rect);

	// �豸�������ʼ��
	memDC.CreateCompatibleDC(NULL);

	// ��������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	// ѡȡ�հ�λͼ
	memDC.SelectObject(MemBitmap);
	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));

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

	// ���Ƶ�ǰ�Ҷ�����
	Pen pen(Color::Blue);

	pen.SetDashStyle(DashStyleDash);

	graph.DrawLine(&pen, 10 + m_nLimitLow, 280, 10 + m_nLimitLow, 10);
	graph.DrawLine(&pen, 10 + m_nLimitUp, 280, 10 + m_nLimitUp, 10);

	long lMax = 0;
	REAL dHeight = 0.0;

	// �������ֵ
	for (int i = m_nLimitLow; i <= m_nLimitUp; i++)
		lMax = max(lMax, m_lCounts[i]);

	// y��̶�
	strNum.Format(L"%d", lMax);
	graph.DrawString(strNum, -1, &font,
		PointF(10, 25), &SolidBrush(Color::Black));

	// ������״ͼ
	for (int i = m_nLimitLow; i <= m_nLimitUp; i++)
	{
		dHeight = (REAL)(m_lCounts[i]) / lMax * 250;
		graph.DrawLine(&Pen(Color::Gray), i + 10.0f, 280.0f, i + 10.0f, 280 - dHeight);
	}

	// �����ڴ滭������
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	m_stiHistogram.ReleaseDC(pDC);
}

/******************************************************************************
*	����:		����ƶ�ʱ��Ӧ��ק����
******************************************************************************/
void CHistogramDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;

	m_stiHistogram.GetWindowRect(rect);

	if ( (nFlags & MK_LBUTTON) && m_nIsDraging )
	{
		// ��קƫ����
		int offset = point.x - m_psMove.x;

		// �����ק��������
		if (m_nIsDraging == DT_UP)
		{
			// ���û�г���
			if ( (offset + m_nLimitUp) <= 255 )
			{
				if ( (offset + m_nLimitUp) >= m_nLimitLow )
					m_nLimitUp += offset;
				else
					m_nLimitUp = m_nLimitLow;
			}
			else
				m_nLimitUp = 255;
		}
		else
		{
			// ���û�г���
			if ( (offset + m_nLimitLow) >= 0 )
			{
				if ( (offset + m_nLimitLow) <= m_nLimitUp )
					m_nLimitLow += offset;
				else
					m_nLimitLow = m_nLimitUp;
			}
			else
				m_nLimitLow = 0;
		}

		UpdateData(FALSE);
		Refresh();

		m_psMove = point;
	}
	else
		m_nIsDraging = DT_NULL;

	ClientToScreen(&point);

	// ��굱ǰ���ڻҶ�λ��,�������0��255֮�����ʾ��겻��ָ��������
	int x = point.x - rect.left - 10;

	if (abs(x - m_nLimitUp) > 3 && abs(x - m_nLimitLow) > 3)
		m_nIsDraging = DT_NULL;

	// ��������ֱ��ͼ������
	if (rect.PtInRect(point))
	{
		if (x >= m_nLimitLow && x <= m_nLimitUp)
		{
			m_nGray = x;
			m_dPer = float(m_lCounts[x]) / m_nPixelCount * 100;
		}

		UpdateData(FALSE);
	}
}

/******************************************************************************
*	����:		��קʱ�ı������
******************************************************************************/
BOOL CHistogramDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CRect rect;
	CPoint point;

	GetCursorPos(&point);
	m_stiHistogram.GetWindowRect(rect);
	if (rect.PtInRect(point))
	{
		int x = point.x - rect.left - 10;

		if (abs(x - m_nLimitUp) <= 3 || abs(x - m_nLimitLow) <= 3)
		{
			SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			return TRUE;
		}
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

/******************************************************************************
*	����:		��갴��ʱ�ж��Ƿ��ڻҶ�������ֱ�߷�Χ��
******************************************************************************/
void CHistogramDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	CPoint oldPoint = point;

	m_stiHistogram.GetWindowRect(rect);
	ClientToScreen(&point);
	int x = point.x - rect.left - 10;

	if (abs(x - m_nLimitUp) <= 3)
	{
		m_psMove = oldPoint;
		m_nIsDraging = DT_UP;
	}
	else if (abs(x - m_nLimitLow) <= 3)
	{
		m_psMove = oldPoint;
		m_nIsDraging = DT_LOW;
	}
}

/******************************************************************************
*	����:		�ͷ�������Ӧ����
******************************************************************************/
void CHistogramDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_nIsDraging = DT_NULL;
}
