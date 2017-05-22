#pragma once

#include "PreviewDlg.h"
#include "afxwin.h"

// CLogTranDlg �Ի���

class CLogTranDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CLogTranDlg)

public:
	int		m_nPosA;		// ����A��������
	int		m_nPosB;		// ����B��������
	int		m_nA;			// ����a
	int		m_nB;			// ����b
	CStatic m_stiPrev;		// �任����Ԥ����

	// ��׼������������
	CLogTranDlg(CWnd* pParent = NULL); 
	virtual ~CLogTranDlg();

	void Refresh();						// ˢ��Ԥ����
	virtual BOOL OnInitDialog();		// �Ի����ʼ��ʱ��ʾԤ���Ի���

	afx_msg void OnEnChangeEdita();		// �ı����Aʱ����Ӧ����
	afx_msg void OnEnChangeEditb();		// �ı����Bʱ����Ӧ����
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
										// ���黬������Ӧ����
	afx_msg void OnPaint();				// �Ի����ػ溯��
	afx_msg void OnBnClickedSave();		// �����桱��ť��Ӧ����

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	CPreviewDlg*	dlg;		// Ԥ���Ի���
	bool			m_bModify;	// ͼ���Ƿ��޸�

	enum { IDD = IDD_LOGTRAN };	// �Ի�������
public:
	afx_msg void OnBnClickedOk();
};
