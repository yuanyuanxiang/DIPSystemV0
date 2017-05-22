#pragma once
#include "afxwin.h"

#include "PreviewDlg.h"

// CExpTranDlg �Ի���

class CExpTranDlg : public CDialog
{
	DECLARE_DYNAMIC(CExpTranDlg)

public:
	CExpTranDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExpTranDlg();

// �Ի�������
	enum { IDD = IDD_EXPTRAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void Refresh();

	int		m_nPosA;
	int		m_nA;
	int		m_nPosB;
	int		m_nB;
	int		m_nPosC;
	int		m_nC;
	bool	m_bInitialized;
	CStatic m_stiPrev;

	CPreviewDlg* dlg;
	bool	m_bModify;
	afx_msg void OnEnChangeEdita();
	afx_msg void OnEnChangeEditb();
	afx_msg void OnEnChangeEditc();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedOk();
};
