// PreviewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PreviewDlg.h"
#include "MainFrm.h"
#include "DIPSystemDoc.h"
#include "DIPSystemView.h"
#include "GeoOperator.h"

SCROLLINFO scroll;

static BOOL scrollSign;

static int yCurrentScroll;//��ֱ�����ϣ��������ĵ�ǰλ��
static int yScrollMax;//��ֱ�����ϣ��������������ϱ߽�
static int yScrollMin;//��ֱ�����ϣ��������������±߽�

static int xCurrentScroll;//ˮƽ�����ϣ��������ĵ�ǰλ��
static int xScrollMax;//ˮƽ�����ϣ��������������ұ߽�
static int xScrollMin;//ˮƽ�����ϣ���������������߽�

// CPreViewDlg �Ի���

IMPLEMENT_DYNAMIC(CPreviewDlg, CDialog)

CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPreviewDlg::IDD, pParent)
{
	m_nType = 0;
	m_bEnableClose = true;
}

CPreviewDlg::~CPreviewDlg()
{
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CPreviewDlg ��Ϣ�������
//�Ի���ĳ�ʼ������
BOOL CPreviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	scrollSign = FALSE;

	CRect rect;
	GetClientRect(&rect);

	yCurrentScroll = 0;
	yScrollMax = 0;
	yScrollMin = 0;

	xCurrentScroll = 0;
	xScrollMax = 0;
	xScrollMin = 0;

	xScrollMax = max(pView->m_nTempWidth - rect.Width(), 0); 
	scroll.cbSize = sizeof(scroll); 
	scroll.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
	scroll.nMin   = xScrollMin; 
	scroll.nMax   = pView->m_nTempWidth;
	scroll.nPage  = rect.Width(); 
	scroll.nPos   = 0; 
	SetScrollInfo(SB_HORZ, &scroll, TRUE); 

	yScrollMax = max(pView->m_nTempHeight - rect.Height(), 0); 
	scroll.cbSize = sizeof(scroll); 
	scroll.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
	scroll.nMin   = yScrollMin; 
	scroll.nMax   = pView->m_nTempHeight; 
	scroll.nPage  = rect.Height(); 
	scroll.nPos   = 0; 
	SetScrollInfo(SB_VERT, &scroll, TRUE); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CPreviewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();

	int width = pView->m_nTempWidth;
	int height = pView->m_nTempHeight;

	// ��ת
	BYTE* temp = NULL;
	VMirrorTrans(pView->m_pImageTempBuffer, temp, width, height);
	CBitmap bitmap;
	bitmap.CreateBitmap(width, height, 1, 32, temp);
	delete [] temp;

	// ����
	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(bitmap);
	BitBlt(dc.m_hDC, -xCurrentScroll, -yCurrentScroll, width, height, MemDC, 0, 0, SRCCOPY);

	// ���ù�����״̬
	if(!scrollSign)
	{
		CRect rect;
		GetClientRect(&rect);

		xScrollMax = max(pView->m_nTempWidth - rect.Width(), 0); 
		scroll.cbSize = sizeof(scroll); 
		scroll.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
		scroll.nMin   = xScrollMin; 
		scroll.nMax   = pView->m_nTempWidth; 
		scroll.nPage  = rect.Width(); 
		scroll.nPos   = 0; 
		SetScrollInfo(SB_HORZ, &scroll, TRUE); 

		yScrollMax = max(pView->m_nTempHeight - rect.Height(), 0); 
		scroll.cbSize = sizeof(scroll); 
		scroll.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
		scroll.nMin   = yScrollMin; 
		scroll.nMax   = pView->m_nTempHeight; 
		scroll.nPage  = rect.Height(); 
		scroll.nPos   = 0; 
		SetScrollInfo(SB_VERT, &scroll, TRUE);
	}
}

/******************************************************************************   
*ˮƽ������������
******************************************************************************/
void CPreviewDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int xDelta;
	int yDelta = 0;

	xCurrentScroll = GetScrollPos(SB_HORZ);

	switch(nSBCode)
	{
	case SB_PAGEUP:
		xCurrentScroll -= 20;
		break;
	case SB_PAGEDOWN:
		xCurrentScroll += 20;
		break;
	case SB_LINELEFT:
		xCurrentScroll -= 5;
		break;
	case SB_LINERIGHT:
		xCurrentScroll += 5;
		break;
	case SB_THUMBPOSITION:
		xCurrentScroll = nPos;
		break;
	case SB_THUMBTRACK:
		xCurrentScroll = nPos;
		break;
	default:;
	}

	xCurrentScroll = max(0, xCurrentScroll);
	xCurrentScroll = min(xScrollMax, xCurrentScroll);

	if(xCurrentScroll == GetScrollPos(SB_HORZ))return;

	scrollSign = TRUE;
	xDelta = xCurrentScroll - GetScrollPos(SB_HORZ);

	ScrollWindowEx(-xDelta, -yDelta, NULL, NULL, NULL, (LPRECT)NULL, SW_INVALIDATE);	
	UpdateWindow();

	SetScrollPos(SB_HORZ, xCurrentScroll, TRUE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/******************************************************************************   
*��ֱ��������������ʵ�ַ���ͬˮƽ������������
******************************************************************************/
void CPreviewDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int xDelta = 0;
	int yDelta;
	int yNewPos;

	yCurrentScroll = GetScrollPos(SB_VERT);

	switch(nSBCode)
	{
	case SB_PAGEUP:
		yNewPos = yCurrentScroll - 20;
		break;
	case SB_PAGEDOWN:
		yNewPos = yCurrentScroll + 20;
		break;
	case SB_LINEUP:
		yNewPos = yCurrentScroll - 5;
		break;
	case SB_LINEDOWN:
		yNewPos = yCurrentScroll + 5;
		break;
	case SB_THUMBPOSITION:
		yNewPos = nPos;
		break;
	case SB_THUMBTRACK:
		yNewPos = nPos;
		break;
	default:
		yNewPos = yCurrentScroll;
	}

	yNewPos = max(0, yNewPos);
	yNewPos = min(yScrollMax, yNewPos);

	if(yNewPos == yCurrentScroll)
		return;

	scrollSign = TRUE;
	yDelta = yNewPos - yCurrentScroll;
	yCurrentScroll = yNewPos;

	ScrollWindowEx(-xDelta, -yDelta, NULL, NULL, NULL, (LPRECT)NULL, SW_INVALIDATE);
	UpdateWindow();

	SetScrollPos(SB_VERT, yCurrentScroll, TRUE);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPreviewDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_nType == 0)
		delete this;
}

BOOL CPreviewDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CPreviewDlg::OnCancel()
{
	if (m_bEnableClose)
	{
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		CDIPSystemView* pView = (CDIPSystemView*)pMain->GetActiveView();
		if (m_nType == 2)
			if (MessageBox(L"ͼ���Ѹ��ģ��Ƿ񱣴棿", L"��ʾ", MB_YESNO) == IDYES)
				pView->OnFileSave();

		CDialog::OnCancel();
	}
}