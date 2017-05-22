#pragma once
#include "afxwin.h"

// CHistogramDlg �Ի���

class CHistogramDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CStatic m_stiHistogram;		// ֱ��ͼ��ʾ����
	int		m_nLimitLow;		// ��ʾ�Ҷȵ�����
	int		m_nLimitUp;			// ��ʾ�Ҷȵ�����
	long	m_lCounts[256];		// �����Ҷȳ��ֵĸ���
	long	m_nPixelCount;		// ͼ����������
	CPoint	m_psMove;			// ��¼��קʱ�����λ��
	int		m_nIsDraging;		// ����Ƿ�������ק
	int		m_nGray;			// ��ǰ���λ�õĻҶȼ���
	float	m_dPer;				// ���ָ���
	bool	m_bInitialized;     // ˢ��һ��


	CHistogramDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CHistogramDlg();

	afx_msg void OnEnChangeLimitLower();	// �Ҷ����޸ı����Ӧ����
	afx_msg void OnEnChangeLimitUp();		// �Ҷ����޸ı����Ӧ����
	afx_msg void OnPaint();					// ���ƶԻ���
	virtual BOOL OnInitDialog();			// �Ի����ʼ��ʱ��������Ҷ�����
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
											// ����ƶ�ʱ��Ӧ��ק����
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
											// ��קʱ�ı������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
											// ��갴��ʱ�ж��Ƿ��ڻҶ�������ֱ�߷�Χ��
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
											// �ͷ�������Ӧ����

	// �Ի�������
	enum { IDD = IDD_HISTOGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	void Refresh(void);	// ˢ��ֱ��ͼ

	// �϶�ö��
	enum DragingType
	{
		DT_NULL,		// ���϶�
		DT_LOW,			// �϶�����
		DT_UP			// �϶�����
	};
};
