// DIPSystemView.h : CDIPSystemView ��Ľӿ�
//

#pragma once

#define BYTE unsigned char //����1���ֽڣ�=8���أ�
#define WORD unsigned short int //����2���ֽ�
#define DWORD unsigned int //����4���ֽ�
#define SDWORD signed int //����4���ֽ�
#define SBYTE signed char //����1���ֽ�
#define SWORD signed short int //����2���ֽ�

class CDIPSystemView : public CView
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CDIPSystemView)

public:
	BYTE*	m_pImageBuffer;				// �༭ͼ��ԭʼ��������
	BYTE*	m_pImageTempBuffer;			// ��������������
	UINT	m_nPicWidth;				// ��ǰ�༭ͼ����
	UINT	m_nPicHeight;				// ��ǰ�༭ͼ��߶�
	UINT	m_nTempWidth;				// �����ͼ��Ŀ��
	UINT	m_nTempHeight;				// �����ͼ��ĸ߶�


	bool	m_bWidhtOut;				// ��ʾͼ���Ƿ񳬳���ͼ���
	bool	m_bHeightOut;				// ��ʾͼ���Ƿ񳬳���ͼ�߶�
	bool	m_bIsEditMode;				// ��ǰ�Ƿ�Ϊ�༭ģʽ
	int		m_nShowType;				// ��ʾ����

	virtual ~CDIPSystemView();

	CImage* GetImage()					// ��ȡͼ��ָ��
		{
			return m_pImage;
		}

	void SaveImage();					// ����ͼ��

	void ShowPicture(CDC* pDC, CImage& image, int nShowType);
										// ��ָ��ģʽ��ʾͼ��
	void SetShowType(int nType);		// ������ʾģʽ
	CSize GetShowPicSize(CImage& image, int nShowType);
										// ���ָ��ģʽ��ͼ��Ĵ�С

	BOOL OpenFile(const CString& strPath);	// ��ͼ���ļ�

	/******************************************************************************
	*	����:		���ͼ���ƫ������
	******************************************************************************/
	void ClearOffset(void)
	{
		m_nXX = m_nYY = m_nXXMax = m_nYYMax = 0;
	}

	void FixOffset(void);				// ����ͼ���ƫ������

	/***************************************************************************
	*	����:		���ָ�������ļ���ȫ·��
	***************************************************************************/
	CString GetFilePath(int nIndex)
	{
		ASSERT(nIndex >= 0 && nIndex < m_nPicNum);
		return m_strPath + L"\\" + m_FilesNameAry[nIndex];
	}

	void RefreshIndicator();			// ����״̬����ָʾ����Ϣ

	CDIPSystemDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);		// ��д�Ի��Ƹ���ͼ

	// ��Ϣ��Ӧ����
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
										// ����ƶ�����Ӧ����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
										// ��������������Ӧ����
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
										// ���������
	afx_msg void OnFileOpen();			// "��"��������Ӧ����
	afx_msg void OnFileSave();			// "����"��������Ӧ����
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);// ��ֹ�ػ汳��

	afx_msg void OnModeBrowse();		// ���ģʽ
	afx_msg void OnModeEdit();			// �༭ģʽ
	
	// "ģʽ"���º���
	afx_msg void OnUpdateModeBrowse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeEdit(CCmdUI *pCmdUI);

	// "��ʾģʽ"�˵���غ���
	afx_msg void OnModeNormal();
	afx_msg void OnModeWidth();
	afx_msg void OnModeHeight();
	afx_msg void OnModeScreen();
	afx_msg void OnUpdateModeNormal(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeWidth(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeHeight(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeScreen(CCmdUI *pCmdUI);
	// "��ͼ"��"ͼ��"�˵���غ���
	afx_msg void OnImagePrev();
	afx_msg void OnImageNext();
	afx_msg void OnImageFirst();
	afx_msg void OnImageLast();
	afx_msg void OnUpdateImagePrev(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageNext(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageFirst(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageLast(CCmdUI *pCmdUI);
	// "��ʾ��Ч"�˵���غ���
	afx_msg void OnEffectNone();
	afx_msg void OnEffectRand();
	afx_msg void OnEffectScan();
	afx_msg void OnEffectDscan();
	afx_msg void OnEffectMove();
	afx_msg void OnEffectVBlind();
	afx_msg void OnEffectHRaster();
	afx_msg void OnEffectMosaic();
	afx_msg void OnEffectHsmove();
	afx_msg void OnEffectSblind();
	afx_msg void OnEffectVraster();
	afx_msg void OnEffectRaindrop();
	afx_msg void OnUpdateEffectNone(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectRand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectScan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectDscan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectMove(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectBlind(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectRaster(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectMosaic(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectHsmove(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectSblind(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectVraster(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectRaindrop(CCmdUI *pCmdUI);
	// ������
	afx_msg void OnPointLinetran();
	afx_msg void OnPointStretch();
	afx_msg void OnPointEqualize();
	afx_msg void OnPointHistogram();
	afx_msg void OnPointThreshold();
	afx_msg void OnPointLog();
	afx_msg void OnPointPower();
	afx_msg void OnPointExp();
	// ���α任
	afx_msg void OnGeoMove();
	afx_msg void OnGeoMirror();
	afx_msg void OnGeoTranspose();
	afx_msg void OnGeoZoom();
	afx_msg void OnGeoRotate();
	// ͼ����ǿ
	afx_msg void OnImproveSmooth();
	afx_msg void OnImproveSmoothg();
	afx_msg void OnImproveMedian();
	afx_msg void OnImproveSharp();
	afx_msg void OnImproveSobel();

protected:
	CDIPSystemView();

	virtual void OnInitialUpdate();		// ���º���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	int				m_nEffectDisplayType;// ��ǰ��Ч����
	bool			m_bEffectDraw;		// �Ƿ�ʹ����Ч��ʾ
	CImage*			m_pImage;			// ˫�����ͼ
	int				m_nPicNum;			// ��ǰĿ¼���ļ�����
	CString			m_strPath;			// ��ǰ��Ŀ¼
	CStringArray	m_FilesNameAry;		// ���浱ǰĿ¼�������ļ���������
	int				m_nPos;				// ��ǰ�ļ��������е�����
	int				m_nShowPicHeight;	// ��ʾͼƬ�ĸ߶�
	int				m_nShowPicWidth;	// ��ʾͼƬ�Ŀ��

	CPoint			m_psMove;			// ��¼�ƶ�ǰ����λ��
	int				m_nXX;				// ͼƬX�����ƫ����
	int				m_nYY;				// ͼƬY�����ƫ����
	int				m_nXXMax;			// X�������ƫ����
	int				m_nYYMax;			// Y�������ƫ����

	void PreDrawImage(void);			// ���ڴ���Ԥ�Ȼ���ͼ��
	void EffectDisplayImage(CDC* pDC, CDC* pMemDC);
										// ʹ����Ч��ʾͼ��
	bool GetImageBuffer(CString& strPath);
										// ��ָ��·����ͼ����Ϣ��ȡ��������
	void FreeImageBuffer(void);			// �ͷ�ͼ����Ϣռ�õ��ڴ�

	void WriteBufferToBMP(BYTE *im_buffer, WORD X_bitmap, WORD Y_bitmap, const char* BMPname);
										// ��ͼ����Ϣд��bmp�ļ���
	void ResetImage();					// ����m_pImageTempBuffer����Ϣ

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#ifndef _DEBUG  // DIPSystemView.cpp �еĵ��԰汾
inline CDIPSystemDoc* CDIPSystemView::GetDocument() const
   { return reinterpret_cast<CDIPSystemDoc*>(m_pDocument); }
#endif

