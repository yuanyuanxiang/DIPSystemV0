// DIPSystemView.cpp : CDIPSystemView 类的实现
//

#include "stdafx.h"
#include "DIPSystem.h"

#include "DIPSystemDoc.h"
#include "DIPSystemView.h"

#include "DIPSystemDoc.h"
#include "mainfrm.h"

#include "EffectDisplay.h"
#include "HistogramDlg.h"

#include "PreviewDlg.h"
#include "LineTranDlg.h"
#include "StretchDlg.h"
#include "ThresholdDlg.h"
#include "GrayOperator.h"
#include "LogTranDlg.h"
#include "PowerTranDlg.h"
#include "ExpTranDlg.h"
#include "MoveTranDlg.h"
#include "MirTranDlg.h"
#include "ZoomDlg.h"
#include "RotateDlg.h"

#include "GeoOperator.h"
#include "improve.h"
#include "Filter.h"

#include <iostream>
#include <string>
using namespace std;

#pragma warning ( disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 图像显示类型枚举
enum ShowType
{
	ST_NORMAL		= 0,	// 原始大小
	ST_FIXWIDTH		= 1,	// 适合宽度
	ST_FIXHEIGHT	= 2,	// 适合高度
	ST_FIXSCREEN	= 3		// 适合屏幕
};

typedef struct s_BM_header {
	WORD BMP_id ; // 'B''M'
	DWORD size; // size in bytes of the BMP file
	DWORD zero_res; // 0
	DWORD offbits; // 54
	DWORD biSize; // 0x28
	DWORD Width;  // X
	DWORD Height;  // Y
	WORD  biPlanes; // 1
	WORD  biBitCount ; // 24
	DWORD biCompression; // 0 = BI_RGB
	DWORD biSizeImage; // 0
	DWORD biXPelsPerMeter; // 0xB40
	DWORD biYPelsPerMeter; // 0xB40
	DWORD biClrUsed; //0
	DWORD biClrImportant; //0
} BM_header;

typedef struct s_RGB {
	BYTE B;
	BYTE G;
	BYTE R;
} RGB;

// CDIPSystemView

IMPLEMENT_DYNCREATE(CDIPSystemView, CView)

BEGIN_MESSAGE_MAP(CDIPSystemView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	//文件
	ON_COMMAND(ID_FILE_OPEN, &CDIPSystemView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CDIPSystemView::OnFileSave)
	//其他
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	//显示模式
	ON_COMMAND(ID_MODE_NORMAL, &CDIPSystemView::OnModeNormal)
	ON_COMMAND(ID_MODE_WIDTH, &CDIPSystemView::OnModeWidth)
	ON_COMMAND(ID_MODE_HEIGHT, &CDIPSystemView::OnModeHeight)
	ON_COMMAND(ID_MODE_SCREEN, &CDIPSystemView::OnModeScreen)
	ON_UPDATE_COMMAND_UI(ID_MODE_NORMAL, &CDIPSystemView::OnUpdateModeNormal)
	ON_UPDATE_COMMAND_UI(ID_MODE_WIDTH, &CDIPSystemView::OnUpdateModeWidth)
	ON_UPDATE_COMMAND_UI(ID_MODE_HEIGHT, &CDIPSystemView::OnUpdateModeHeight)
	ON_UPDATE_COMMAND_UI(ID_MODE_SCREEN, &CDIPSystemView::OnUpdateModeScreen)
	//视图->图像
	ON_COMMAND(ID_IMAGE_PREV, &CDIPSystemView::OnImagePrev)
	ON_COMMAND(ID_IMAGE_NEXT, &CDIPSystemView::OnImageNext)
	ON_COMMAND(ID_IMAGE_FIRST, &CDIPSystemView::OnImageFirst)
	ON_COMMAND(ID_IMAGE_LAST, &CDIPSystemView::OnImageLast)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_PREV, &CDIPSystemView::OnUpdateImagePrev)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_NEXT, &CDIPSystemView::OnUpdateImageNext)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_FIRST, &CDIPSystemView::OnUpdateImageFirst)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_LAST, &CDIPSystemView::OnUpdateImageLast)
	//显示特效
	ON_COMMAND(ID_EFFECT_NONE, &CDIPSystemView::OnEffectNone)
	ON_COMMAND(ID_EFFECT_RAND, &CDIPSystemView::OnEffectRand)
	ON_COMMAND(ID_EFFECT_SCANDOWN, &CDIPSystemView::OnEffectScan)
	ON_COMMAND(ID_EFFECT_VSCAN, &CDIPSystemView::OnEffectDscan)
	ON_COMMAND(ID_EFFECT_MOVERIGHT, &CDIPSystemView::OnEffectMove)
	ON_COMMAND(ID_EFFECT_VBLIND, &CDIPSystemView::OnEffectVBlind)
	ON_COMMAND(ID_EFFECT_HRASTER, &CDIPSystemView::OnEffectHRaster)
	ON_COMMAND(ID_EFFECT_MOSAIC, &CDIPSystemView::OnEffectMosaic)
	ON_COMMAND(ID_EFFECT_HSMOVE, &CDIPSystemView::OnEffectHsmove)
	ON_COMMAND(ID_EFFECT_SBLIND, &CDIPSystemView::OnEffectSblind)
	ON_COMMAND(ID_EFFECT_VRASTER, &CDIPSystemView::OnEffectVraster)
	ON_COMMAND(ID_EFFECT_RAINDROP, &CDIPSystemView::OnEffectRaindrop)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_NONE, &CDIPSystemView::OnUpdateEffectNone)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_RAND, &CDIPSystemView::OnUpdateEffectRand)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_SCANDOWN, &CDIPSystemView::OnUpdateEffectScan)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_VSCAN, &CDIPSystemView::OnUpdateEffectDscan)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_MOVERIGHT, &CDIPSystemView::OnUpdateEffectMove)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_VBLIND, &CDIPSystemView::OnUpdateEffectBlind)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_HRASTER, &CDIPSystemView::OnUpdateEffectRaster)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_MOSAIC, &CDIPSystemView::OnUpdateEffectMosaic)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_HSMOVE, &CDIPSystemView::OnUpdateEffectHsmove)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_SBLIND, &CDIPSystemView::OnUpdateEffectSblind)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_VRASTER, &CDIPSystemView::OnUpdateEffectVraster)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_RAINDROP, &CDIPSystemView::OnUpdateEffectRaindrop)
	//运行模式
	ON_COMMAND(ID_MODE_BROWSE, &CDIPSystemView::OnModeBrowse)
	ON_COMMAND(ID_MODE_EDIT, &CDIPSystemView::OnModeEdit)
	ON_UPDATE_COMMAND_UI(ID_MODE_BROWSE, &CDIPSystemView::OnUpdateModeBrowse)
	ON_UPDATE_COMMAND_UI(ID_MODE_EDIT, &CDIPSystemView::OnUpdateModeEdit)
	//点运算
	ON_COMMAND(ID_POINT_HISTOGRAM, &CDIPSystemView::OnPointHistogram)
	ON_COMMAND(ID_POINT_LINETRAN, &CDIPSystemView::OnPointLinetran)
	ON_COMMAND(ID_POINT_STRETCH, &CDIPSystemView::OnPointStretch)
	ON_COMMAND(ID_POINT_EQUALIZE, &CDIPSystemView::OnPointEqualize)
	ON_COMMAND(ID_POINT_THRESHOLD, &CDIPSystemView::OnPointThreshold)
	ON_COMMAND(ID_POINT_LOG, &CDIPSystemView::OnPointLog)
	ON_COMMAND(ID_POINT_POWER, &CDIPSystemView::OnPointPower)
	ON_COMMAND(ID_POINT_EXP, &CDIPSystemView::OnPointExp)
	//几何变换
	ON_COMMAND(ID_GEO_MOVE, &CDIPSystemView::OnGeoMove)
	ON_COMMAND(ID_GEO_MIRROR, &CDIPSystemView::OnGeoMirror)
	ON_COMMAND(ID_GEO_TRANSPOSE, &CDIPSystemView::OnGeoTranspose)
	ON_COMMAND(ID_GEO_ZOOM, &CDIPSystemView::OnGeoZoom)
	ON_COMMAND(ID_GEO_ROTATE, &CDIPSystemView::OnGeoRotate)
	//图像增强
	ON_COMMAND(ID_IMPROVE_SMOOTH, &CDIPSystemView::OnImproveSmooth)
	ON_COMMAND(ID_IMPROVE_SMOOTHG, &CDIPSystemView::OnImproveSmoothg)
	ON_COMMAND(ID_IMPROVE_MEDIAN, &CDIPSystemView::OnImproveMedian)
	ON_COMMAND(ID_IMPROVE_SHARP, &CDIPSystemView::OnImproveSharp)
	ON_COMMAND(ID_IMPROVE_SOBEL, &CDIPSystemView::OnImproveSobel)
END_MESSAGE_MAP()

// CDIPSystemView 构造/析构

CDIPSystemView::CDIPSystemView()
{
	m_bIsEditMode = true;
	m_nShowType = ST_NORMAL;
	m_bHeightOut = false;
	m_bWidhtOut = false;
	m_nXX = 0;
	m_nYY = 0;
	m_nXXMax = 0;
	m_nYYMax = 0;
	m_nPos = -1;

	m_pImage = NULL;
	m_bEffectDraw = false;
	m_nEffectDisplayType = EDT_RAND;

	m_pImageBuffer = NULL;
	m_pImageTempBuffer = NULL;
	m_nPicWidth = m_nPicHeight = 0;
}

CDIPSystemView::~CDIPSystemView()
{
	::delete m_pImage;

	FreeImageBuffer();
}

/******************************************************************************
*	作用:		视图重绘函数
******************************************************************************/
void CDIPSystemView::OnDraw(CDC* pDC)
{
	CDIPSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(rect);

	CDC memDC;
	CBitmap MemBitmap;

	// 设备描述表初始化
	memDC.CreateCompatibleDC(NULL);

	// 建立与屏幕显示兼容的内存显示设备
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	// 选取空白位图
	memDC.SelectObject(MemBitmap);
	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));

	if (m_nPos >= 0)
		ShowPicture(&memDC, *m_pImage, m_nShowType);

	if (m_bEffectDraw && m_nEffectDisplayType != EDT_NONE)
	{
		int nHeight = rect.Height() - m_nShowPicHeight;
		int nWidth = rect.Width() - m_nShowPicWidth;

		if (nHeight < 0)	
			nHeight = 0;
		if (nWidth < 0)	
			nWidth = 0;

		pDC->FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));

		EffectDisplay::s_nOffsetX = nWidth / 2;
		EffectDisplay::s_nOffsetY = nHeight / 2;
		EffectDisplay::s_nPicWidth = m_nShowPicWidth;
		EffectDisplay::s_nPicHeight = m_nShowPicHeight;
		EffectDisplay::s_nCDCWidth = rect.Width();
		EffectDisplay::s_nCDCHeight = rect.Height();

		EffectDisplayImage(pDC, &memDC);

		m_bEffectDraw = false;
	}
	else
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	MemBitmap.DeleteObject();
	memDC.DeleteDC();
}

/******************************************************************************
*	作用:		打开指定路径的文件
******************************************************************************/
BOOL CDIPSystemView::OpenFile(const CString& strPath)
{	
	FreeImageBuffer();
	m_FilesNameAry.RemoveAll();

	int nIndex = strPath.ReverseFind('\\');
	m_strPath = strPath.Left(nIndex);
	CString strFileName = strPath.Right(strPath.GetLength() - nIndex - 1);

	CMainFrame::GetAllFileNames(m_strPath, m_FilesNameAry);
	m_nPicNum = (int)m_FilesNameAry.GetCount();

	m_nPos = 0;

	while( m_nPos < m_nPicNum && m_FilesNameAry[m_nPos] != strFileName)
		m_nPos++;

	PreDrawImage();
	m_bEffectDraw = true;
	Invalidate();

	if (m_bIsEditMode)
	{
		CString strPath = GetFilePath(m_nPos);
		GetImageBuffer(strPath);
	}
	return TRUE;
}

void CDIPSystemView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

// CDIPSystemView 打印

BOOL CDIPSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDIPSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDIPSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CDIPSystemView 诊断

#ifdef _DEBUG
void CDIPSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CDIPSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDIPSystemDoc* CDIPSystemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPSystemDoc)));
	return (CDIPSystemDoc*)m_pDocument;
}
#endif //_DEBUG

/******************************************************************************
*	作用:		"打开"的命令响应函数
******************************************************************************/
void CDIPSystemView::OnFileOpen()
{
	CFileDialog dlg(true, NULL, L"",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		L"所有图像文件(*.bmp;*.jpg;*.png;*.tif;*.gif)|*.bmp;*.jpg;*.png;*.tif;*.gif|所有文件(*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		CString sPathName = dlg.GetPathName();
		OpenFile(sPathName);
		FixOffset();
		RefreshIndicator();
		Invalidate();
		// 添加到最近打开
		GetDocument()->SetPathName(sPathName);
	}
}

/******************************************************************************
*	作用:		禁止重绘背景
******************************************************************************/
BOOL CDIPSystemView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

/******************************************************************************
*	作用:			以指定模式显示图像
*	参数:		
*		pDC			目标设备上下文指针
*		image		显示图像
*		nShowType	显示模式
******************************************************************************/
void CDIPSystemView::ShowPicture(CDC* pDC, CImage& image, int nShowType)
{
	CRect winRect;
	GetClientRect(winRect);

	REAL x = 0.0;
	REAL y = 0.0;
	CSize showPicSize = GetShowPicSize(image, m_nShowType);
	int width  = (int)showPicSize.cx;
	int height = (int)showPicSize.cy;

	// 使图像居中
	if (winRect.Width() > width)
		x = (winRect.Width() - width) / 2.0f;

	if ( winRect.Height() > height )
		y = (winRect.Height() - height) / 2.0f;

	if (m_bWidhtOut)
		x += m_nXX;

	if (m_bHeightOut)
		y += m_nYY;

	m_nShowPicHeight = (int)height;
	m_nShowPicWidth = (int)width;

	// 宽或高之一为0便不绘制
	if(width * height == 0) return;

	image.Draw(pDC->m_hDC, (int)x, (int)y, (int)width, (int)height);
}

/******************************************************************************
*	作用:		设置显示模式
*	参数:		
*		nType	新的显示模式
******************************************************************************/
void CDIPSystemView::SetShowType(int nType)
{
	m_nShowType = nType;
}

/******************************************************************************
*	作用:			获得指定模式下图像的大小
*	参数:		
*		image		图像对象
*		nShowType	显示模式
*	返回值:			在指定模式下图像的显示大小
******************************************************************************/
CSize CDIPSystemView::GetShowPicSize(CImage& image, int nShowType)
{
	REAL width = (REAL)image.GetWidth();
	REAL height = (REAL)image.GetHeight();
	REAL c = 0.0f;

	CRect winRect;
	GetClientRect(winRect);

	switch (nShowType)
	{
	case ST_FIXWIDTH:	// 适合宽度模式
		width = (REAL)winRect.Width();
		c = width / image.GetWidth();
		height = c * image.GetHeight();
		break;

	case ST_FIXHEIGHT:	// 适合高度模式
		height = (REAL)winRect.Height();
		c = height / image.GetHeight();
		width = c * image.GetWidth();

		break;

	case ST_FIXSCREEN:	// 适合大小模式

		// 按适合宽度模式处理
		width = (REAL)winRect.Width();
		c = width / image.GetWidth();

		// 如果没有超界
		if (c * image.GetHeight() < winRect.Height())
			height = c * image.GetHeight();
		// 超界则采用适合高度模式
		else
		{
			height = (REAL)winRect.Height();
			c = height / image.GetHeight();
			width = c * image.GetWidth();
		}

		break;
	}

	return CSize((int)width, (int)height);
}


/******************************************************************************
*	作用:		鼠标按下的响应函数
******************************************************************************/
void CDIPSystemView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_psMove = point;

	CView::OnLButtonDown(nFlags, point);
}

/******************************************************************************
*	作用:		修正图像的偏移坐标等信息
*	备注:		以显示模式为依据
******************************************************************************/
void CDIPSystemView::FixOffset()
{
	if (m_nPos < 0 || m_nPos >= m_nPicNum)
		return;

	CImage image;
	image.Load(GetFilePath(m_nPos));

	if ( image.IsNull() )
		return;

	m_bHeightOut = false;
	m_bWidhtOut = false;

	CSize imgSize = GetShowPicSize(image, m_nShowType);
	CRect rect;

	GetClientRect(rect);

	switch (m_nShowType)
	{
	case ST_NORMAL:
		if (imgSize.cx > rect.Width())
		{
			m_nXXMax = imgSize.cx - rect.Width();
			m_bWidhtOut = true;
		}

		if (imgSize.cy > rect.Height())
		{
			m_nYYMax = imgSize.cy - rect.Height();
			m_bHeightOut = true;
		}
		break;

	case ST_FIXWIDTH:
		if (imgSize.cy > rect.Height())
		{
			m_nYYMax = imgSize.cy - rect.Height();
			m_bHeightOut = true;
		}
		break;

	case ST_FIXHEIGHT:
		if (imgSize.cx > rect.Width())
		{
			m_nXXMax = imgSize.cx - rect.Width();
			m_bWidhtOut = true;
		}
		break;

	case ST_FIXSCREEN:
	default:;
	}
}

/******************************************************************************
*	作用:		鼠标移动的响应函数
*	备注:		如果图像为拖动状态,则自动修正图像的偏移坐标
******************************************************************************/
void CDIPSystemView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		if (m_bWidhtOut)
		{
			m_nXX += point.x - m_psMove.x;
			if (m_nXX < 0 && abs(m_nXX) > m_nXXMax)
				m_nXX = -m_nXXMax;
			if (m_nXX > 0)
				m_nXX = 0;
		}

		if (m_bHeightOut)
		{
			m_nYY += point.y - m_psMove.y;
			if (m_nYY < 0 && abs(m_nYY) > m_nYYMax)
				m_nYY = -m_nYYMax;
			if (m_nYY > 0)
				m_nYY = 0;
		}

		m_psMove = point;

		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}

/******************************************************************************
*	作用:		根据情况改变鼠标光标
******************************************************************************/
BOOL CDIPSystemView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bHeightOut || m_bWidhtOut)
	{
		SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		return TRUE;
	}
	else
		return CView::OnSetCursor(pWnd, nHitTest, message);
}

/******************************************************************************
*	作用:		在内存中预先绘制图像
******************************************************************************/
void CDIPSystemView::PreDrawImage(void)
{
	SAFE_DELETE(m_pImage);
	m_pImage = new CImage;
	CString sPath = GetFilePath(m_nPos);
	m_pImage->Load(GetFilePath(m_nPos));
}

/******************************************************************************
*	作用:		更新状态栏的指示器信息
******************************************************************************/
void CDIPSystemView::RefreshIndicator()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	CString strTemp;
	strTemp.Format(L"%d/%d", m_nPos + 1, m_nPicNum);
	pMain->UpdateIndicator(ID_INDICATOR_PICNUM, strTemp);
	pMain->UpdateIndicator(ID_INDICATOR_PICNAME, m_FilesNameAry[m_nPos]);
}

/******************************************************************************
*	作用:		使用特效显示图像
*	参数:
*		pDC		目标设备上下文
*		pMemDC	内存设备上下文
*	备注:	将内存设备上下文中已经绘制的图像以特效形式显示在目标设备上下文中
******************************************************************************/
void CDIPSystemView::EffectDisplayImage(CDC* pDC, CDC* pMemDC)
{
	int nType = m_nEffectDisplayType;

	if (nType == EDT_RAND)
	{
		LARGE_INTEGER seed;
		QueryPerformanceFrequency(&seed);
		QueryPerformanceCounter(&seed);

		//初始化一个以微秒为单位的时间种子
		srand((int)seed.QuadPart);

		nType = rand() % 10;
	}

	switch (nType)
	{
	case EDT_SCANDOWN:
		EffectDisplay::ScanDownDisplay(pDC, pMemDC);
		break;

	case EDT_VSCAN:
		EffectDisplay::VSScanDisplay(pDC, pMemDC);
		break;

	case EDT_MOVERIGHT:
		EffectDisplay::MoveRightDisplay(pDC, pMemDC);
		break;

	case EDT_HSMOVE:
		EffectDisplay::HSMoveDisplay(pDC, pMemDC);
		break;

	case EDT_VBLIND:
		EffectDisplay::VBlindDisplay(pDC, pMemDC);
		break;

	case EDT_HBLIND:
		EffectDisplay::HBlindDisplay(pDC, pMemDC);
		break;

	case EDT_VRASTER:
		EffectDisplay::VRasterDisplay(pDC, pMemDC);
		break;

	case EDT_HRASTER:
		EffectDisplay::HRasterDisplay(pDC, pMemDC);
		break;

	case EDT_MOSAIC:
		EffectDisplay::MosaicDisplay(pDC, pMemDC);
		break;

	default:
		EffectDisplay::RaindropDisplay(pDC, pMemDC);
	}
}

void CDIPSystemView::OnModeNormal()
{
	if (m_nShowType != ST_NORMAL)
	{
		m_nShowType = ST_NORMAL;
		FixOffset();
		Invalidate();
	}
}

void CDIPSystemView::OnModeWidth()
{
	if (m_nShowType != ST_FIXWIDTH)
	{
		m_nShowType = ST_FIXWIDTH;
		FixOffset();
		Invalidate();
	}
}

void CDIPSystemView::OnModeHeight()
{
	if (m_nShowType != ST_FIXHEIGHT)
	{
		m_nShowType = ST_FIXHEIGHT;
		FixOffset();
		Invalidate();
	}
}

void CDIPSystemView::OnModeScreen()
{
	if (m_nShowType != ST_FIXSCREEN)
	{
		m_nShowType = ST_FIXSCREEN;
		FixOffset();
		Invalidate();
	}
}

void CDIPSystemView::OnUpdateModeNormal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_NORMAL);
}

void CDIPSystemView::OnUpdateModeWidth(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXWIDTH);
}

void CDIPSystemView::OnUpdateModeHeight(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXHEIGHT);
}

void CDIPSystemView::OnUpdateModeScreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXSCREEN);
}

void CDIPSystemView::OnImagePrev()
{
	if (m_nPos <= 0)
		return;

	m_nPos--;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CDIPSystemView::OnImageNext()
{
	if (m_nPos >= m_nPicNum - 1)
		return;

	m_nPos++;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CDIPSystemView::OnImageFirst()
{
	m_nPos = 0;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CDIPSystemView::OnImageLast()
{
	m_nPos = m_nPicNum - 1;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CDIPSystemView::OnUpdateImagePrev(CCmdUI *pCmdUI)
{
	if (m_nPos <= 0 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CDIPSystemView::OnUpdateImageNext(CCmdUI *pCmdUI)
{
	if ((m_nPos < 0) || m_nPos >= m_nPicNum - 1 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CDIPSystemView::OnUpdateImageFirst(CCmdUI *pCmdUI)
{
	if (m_nPos < 0 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CDIPSystemView::OnUpdateImageLast(CCmdUI *pCmdUI)
{
	if (m_nPos < 0 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CDIPSystemView::OnEffectNone()
{
	m_nEffectDisplayType = EDT_NONE;
}

void CDIPSystemView::OnEffectRand()
{
	m_nEffectDisplayType = EDT_RAND;
}

void CDIPSystemView::OnEffectScan()
{
	m_nEffectDisplayType = EDT_SCANDOWN;
}

void CDIPSystemView::OnEffectDscan()
{
	m_nEffectDisplayType = EDT_VSCAN;
}

void CDIPSystemView::OnEffectMove()
{
	m_nEffectDisplayType = EDT_MOVERIGHT;
}

void CDIPSystemView::OnEffectHsmove()
{
	m_nEffectDisplayType = EDT_HSMOVE;
}

void CDIPSystemView::OnEffectVBlind()
{
	m_nEffectDisplayType = EDT_VBLIND;
}

void CDIPSystemView::OnEffectSblind()
{
	m_nEffectDisplayType = EDT_HBLIND;
}

void CDIPSystemView::OnEffectVraster()
{
	m_nEffectDisplayType = EDT_VRASTER;
}

void CDIPSystemView::OnEffectHRaster()
{
	m_nEffectDisplayType = EDT_HRASTER;
}

void CDIPSystemView::OnEffectMosaic()
{
	m_nEffectDisplayType = EDT_MOSAIC;
}

void CDIPSystemView::OnEffectRaindrop()
{
	m_nEffectDisplayType = EDT_RAINDROP;
}

void CDIPSystemView::OnUpdateEffectNone(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_NONE);
}

void CDIPSystemView::OnUpdateEffectRand(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_RAND);
}

void CDIPSystemView::OnUpdateEffectScan(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_SCANDOWN);
}

void CDIPSystemView::OnUpdateEffectDscan(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_VSCAN);
}

void CDIPSystemView::OnUpdateEffectMove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_MOVERIGHT);
}

void CDIPSystemView::OnUpdateEffectBlind(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_VBLIND);
}

void CDIPSystemView::OnUpdateEffectRaster(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_HRASTER);
}

void CDIPSystemView::OnUpdateEffectMosaic(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_MOSAIC);
}

void CDIPSystemView::OnUpdateEffectHsmove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_HSMOVE);
}

void CDIPSystemView::OnUpdateEffectSblind(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_HBLIND);
}

void CDIPSystemView::OnUpdateEffectVraster(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_VRASTER);
}

void CDIPSystemView::OnUpdateEffectRaindrop(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_RAINDROP);
}

void CDIPSystemView::OnModeBrowse()
{
	if (!m_bIsEditMode)
		return;

	FreeImageBuffer();
	m_bIsEditMode = false;
}

void CDIPSystemView::OnModeEdit()
{
	if (m_bIsEditMode)
		return;

	m_bIsEditMode = true;

	if (m_nPos != -1)
	{
		CString strPath = GetFilePath(m_nPos);
		GetImageBuffer(strPath);
	}
}

/******************************************************************************
*	作用:			将指定路径的图像信息提取到数组中
*	参数:			
*		strPath		图像文件地址
******************************************************************************/
bool CDIPSystemView::GetImageBuffer(CString& strPath)
{
	m_nTempWidth = m_nPicWidth = m_pImage->GetWidth();
	m_nTempHeight = m_nPicHeight = m_pImage->GetHeight();
	
	long nRowLen = abs(m_pImage->GetPitch());
	long nLength = 4 * m_nPicWidth * m_nPicHeight;
	int nChannel = m_pImage->GetBPP() / 8;
	BYTE* pBits = (BYTE*)m_pImage->GetBits() + (m_pImage->GetPitch() * (m_nPicHeight - 1));
	m_pImageBuffer = new BYTE[nLength];
	m_pImageTempBuffer = new BYTE[nLength];

	UINT i, j;
	long nNewRowLen = WIDTHBYTES(32 * m_nPicWidth);
	BYTE *pDst, *pSrc;
	pDst = m_pImageBuffer;
	switch (nChannel)
	{
	case 1://8位色
		for (i = 0; i < m_nPicHeight; i++)
		{
			for (j = 0; j < m_nPicWidth; j++)
			{
				pSrc = pBits + j + i * nRowLen;
				pDst = m_pImageBuffer + j * 4 + i * nNewRowLen;
				*pDst = *(pDst + 1) = *(pDst + 2) = *pSrc;
				*(pDst + 3) = 0;
			}
		}
		break;
	case 3://24位色
		for (i = 0; i < m_nPicHeight; i++)
		{
			for (j = 0; j < m_nPicWidth; j++)
			{
				pSrc = pBits + j * 3 + i * nRowLen;
				pDst = m_pImageBuffer + j * 4 + i * nNewRowLen;
				memcpy(pDst, pSrc, 3);
				*(pDst + 3) = 255;
			}
		}
		break;
	case 4://32位色
		memcpy(m_pImageBuffer, pBits, nLength);
		break;
	default:
		return false;
	}
	memcpy(m_pImageTempBuffer, m_pImageBuffer, nLength);
	return true;
}

// 保存图像
void CDIPSystemView::SaveImage()
{
	SAFE_DELETE(m_pImage);
	m_pImage = new CImage;
	m_pImage->Create(m_nTempWidth, m_nTempHeight, 32);
	BYTE* pDst = (BYTE*)m_pImage->GetBits() + m_pImage->GetPitch() * (m_nTempHeight - 1);
	memcpy(pDst, m_pImageTempBuffer, m_nTempWidth * m_nTempHeight * 4);
	if (m_nPicWidth != m_nTempWidth || m_nPicHeight != m_nTempHeight)
	{
		SAFE_DELETE(m_pImageBuffer);
		m_pImageBuffer = new BYTE[m_nTempWidth * m_nTempHeight * 4];
		m_nPicWidth = m_nTempWidth;
		m_nPicHeight = m_nTempHeight;
	}
	memcpy(m_pImageBuffer, m_pImageTempBuffer, m_nTempWidth * m_nTempHeight * 4);
}

/******************************************************************************
*	作用:		释放图像信息占用的内存
******************************************************************************/
void CDIPSystemView::FreeImageBuffer(void)
{
	if (m_pImageBuffer != NULL)
	{
		free(m_pImageBuffer);
		m_pImageBuffer = NULL;
	}

	if (m_pImageTempBuffer != NULL)
	{
		free(m_pImageTempBuffer);
		m_pImageTempBuffer = NULL;
	}
}

/******************************************************************************
*	作用:		"保存"的命令相应函数
******************************************************************************/
void CDIPSystemView::OnFileSave()
{
	if (m_nPos == -1) 
	{
		MessageBox(L"工作区中未加载图片文件!", L"DIPSystem", MB_ICONWARNING);
		return;
	}
	if (m_pImageTempBuffer == NULL) 
	{
		MessageBox(L"没有可以保存的文件!", L"DIPSystem", MB_ICONWARNING);
		return;
	}
	CFileDialog SaveDlg( FALSE, L"bmp", L"未命名", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, L"图像文件格式(*.bmp) |*.bmp||", NULL);
	if(SaveDlg.DoModal() == IDOK)
	{
		CString tempname = SaveDlg.GetPathName();
		string savePath = (LPCSTR)CStringA(tempname);
		WriteBufferToBMP(m_pImageTempBuffer, m_nTempWidth, m_nTempHeight, savePath.c_str());
		tempname.ReleaseBuffer();
		MessageBox(L"文件已成功保存!");
	}
}

/******************************************************************************
*	作用:			将图像信息写入bmp文件中
*	参数:		
*		im_buffer	图像信息
*		X_bitmap	宽度
*		Y_bitmap	高度
*		BMPname		保存bmp文件路径
******************************************************************************/
void CDIPSystemView::WriteBufferToBMP(BYTE *im_buffer, WORD X_bitmap, WORD Y_bitmap, const char* BMPname)
{
	SWORD x,y;
	RGB *pixel;
	BM_header BH;
	FILE *fp_bitmap;
	DWORD im_loc_bytes;
	BYTE nr_fillingbytes, i;
	BYTE zero_byte=0;

	fp_bitmap=fopen(BMPname,"wb");
	if (fp_bitmap==NULL) 
	{
			AfxMessageBox(L"错误! 文件无法创建.", MB_ICONWARNING);
			return;
	}

	if (X_bitmap%4!=0) nr_fillingbytes=4-((X_bitmap*3L)%4);
	else nr_fillingbytes=0;

	BH.BMP_id = 'M'*256+'B';     fwrite(&BH.BMP_id,2,1,fp_bitmap);
	BH.size=54+Y_bitmap*(X_bitmap*3+nr_fillingbytes);fwrite(&BH.size,4,1,fp_bitmap);
	BH.zero_res = 0;             fwrite(&BH.zero_res,4,1,fp_bitmap);
	BH.offbits = 54;             fwrite(&BH.offbits,4,1,fp_bitmap);
	BH.biSize = 0x28;            fwrite(&BH.biSize,4,1,fp_bitmap);
	BH.Width = X_bitmap;	      fwrite(&BH.Width,4,1,fp_bitmap);
	BH.Height = Y_bitmap;	      fwrite(&BH.Height,4,1,fp_bitmap);
	BH.biPlanes = 1;             fwrite(&BH.biPlanes,2,1,fp_bitmap);
	BH.biBitCount = 24;          fwrite(&BH.biBitCount,2,1,fp_bitmap);
	BH.biCompression = 0;        fwrite(&BH.biCompression,4,1,fp_bitmap);
	BH.biSizeImage = 0;          fwrite(&BH.biSizeImage,4,1,fp_bitmap);
	BH.biXPelsPerMeter = 0xB40;  fwrite(&BH.biXPelsPerMeter,4,1,fp_bitmap);
	BH.biYPelsPerMeter = 0xB40;  fwrite(&BH.biYPelsPerMeter,4,1,fp_bitmap);
	BH.biClrUsed = 0;	          fwrite(&BH.biClrUsed,4,1,fp_bitmap);
	BH.biClrImportant = 0;	      fwrite(&BH.biClrImportant,4,1,fp_bitmap);

	im_loc_bytes=(DWORD)im_buffer+((DWORD)Y_bitmap-1)*X_bitmap*4;

	for (y=0;y<Y_bitmap;y++)
	{
		for (x=0;x<X_bitmap;x++)
		{
			pixel=(RGB *)im_loc_bytes;
			fwrite(pixel, 3, 1, fp_bitmap);
			im_loc_bytes+=4;
		}
		for (i=0;i<nr_fillingbytes;i++)
			fwrite(&zero_byte,1,1,fp_bitmap);
		im_loc_bytes-=2L*X_bitmap*4;
	}
	fclose(fp_bitmap);
}
void CDIPSystemView::OnUpdateModeBrowse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bIsEditMode);
}

void CDIPSystemView::OnUpdateModeEdit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bIsEditMode);
}

void CDIPSystemView::ResetImage()
{
	m_nTempWidth = m_nPicWidth;
	m_nTempHeight = m_nPicHeight;

	int sizeofbuffer = m_nTempWidth * m_nTempHeight * 4;
	SAFE_DELETE (m_pImageTempBuffer);

	m_pImageTempBuffer = new BYTE[sizeofbuffer];
	memcpy(m_pImageTempBuffer, m_pImageBuffer, sizeofbuffer);
}

void CDIPSystemView::OnPointLinetran()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	CLineTranDlg dlg;

	ResetImage();
	dlg.DoModal();
}

void CDIPSystemView::OnPointHistogram()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	CHistogramDlg dlg;

	ResetImage();	// 该函数用于清空m_pImageTempBuffer数组
	dlg.DoModal();
}

void CDIPSystemView::OnPointStretch()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}
	
	CStretchDlg dlg;

	ResetImage();
	dlg.DoModal();
}

void CDIPSystemView::OnPointEqualize()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	GrayEqualize(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
	ResetImage();
}

void CDIPSystemView::OnPointThreshold()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	CThresholdDlg dlg;

	ResetImage();
	dlg.DoModal();
}

void CDIPSystemView::OnPointLog()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	CLogTranDlg dlg;
	
	ResetImage();
	dlg.DoModal();
}

void CDIPSystemView::OnPointPower()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	CPowerTranDlg dlg;

	ResetImage();
	dlg.DoModal();
}

void CDIPSystemView::OnPointExp()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	CExpTranDlg dlg;

	ResetImage();
	dlg.DoModal();
}

void CDIPSystemView::OnGeoMove()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	CMoveTranDlg dlg;
	ResetImage();
	dlg.DoModal();
}

void CDIPSystemView::OnGeoMirror()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	CMirTranDlg dlg;
	ResetImage();
	dlg.DoModal();
}

void CDIPSystemView::OnGeoTranspose()
{
	// 删除处理后的像素数组,Transpose函数会自动申请空间
	SAFE_DELETE (m_pImageTempBuffer);

	Transpose(m_pImageBuffer, m_pImageTempBuffer, 
			  m_nPicWidth, m_nPicHeight, m_nTempWidth, m_nTempHeight);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
	ResetImage();
}

void CDIPSystemView::OnGeoZoom()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	CZoomDlg dlg;
	dlg.DoModal();
}

void CDIPSystemView::OnGeoRotate()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}
	CRotateDlg dlg;
	dlg.DoModal();
}

void CDIPSystemView::OnImproveSmooth()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	ResetImage();
	SmoothCl(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight, 10);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}
void CDIPSystemView::OnImproveSmoothg()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	ResetImage();
	SmoothGaussCl(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}

void CDIPSystemView::OnImproveMedian()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	ResetImage();

	BeginWaitCursor();
	SmoothMedianCl(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight, 7);
	EndWaitCursor();

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}

void CDIPSystemView::OnImproveSharp()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	ResetImage();
	SharpLaplacianCl(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}

void CDIPSystemView::OnImproveSobel()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	ResetImage();

	int w = m_nPicWidth;
	int h = m_nPicHeight;

	BYTE* imageS1 = new BYTE[w*h*4];
	BYTE* imageS2 = new BYTE[w*h*4];
	SideSobel(m_pImageBuffer, imageS1, w, h, true);
	SideSobel(imageS1, imageS2, w, h, true);
	Subtract(imageS1, imageS2, w, h);
	ToTwoValue(imageS1, m_pImageTempBuffer, w, h, 100);
	delete[] imageS1;
	delete[] imageS2;

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}