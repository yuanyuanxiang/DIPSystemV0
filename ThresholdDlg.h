#pragma once

#include "PreviewDlg.h"
#include "afxcmn.h"

// CThresholdDlg �Ի���

class CThresholdDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CThresholdDlg)

public:
	CSliderCtrl m_SliderNum;	// ��ֵ�Ļ������
	int			m_nNum;			// ��ֵ
	int			m_nPos;			// ��������������

	// ��׼�������������
	CThresholdDlg(CWnd* pParent = NULL);
	virtual ~CThresholdDlg();

	void Refresh();						// ˢ��Ԥ������

	virtual BOOL OnInitDialog();		// ��ʼ���Ի���
	afx_msg void OnEnChangeNum();		// ��ֵ���޸�ʱ����Ӧ����
	afx_msg void OnBnClickedSave();		// �����桱��ť��Ӧ���� 
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
										// ��������Ӧ����

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	CPreviewDlg*	dlg;				// Ԥ���Ի���
	bool			m_bModify;			// ͼ���Ƿ��޸�

	// �Ի�������
	enum { IDD = IDD_THRESHOLD };
public:
	afx_msg void OnBnClickedOk();
};
