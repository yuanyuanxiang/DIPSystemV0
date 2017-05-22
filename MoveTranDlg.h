#pragma once

// CMoveTranDlg �Ի���

#include "PreviewDlg.h"

class CMoveTranDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CMoveTranDlg)

public:
	int		m_nType;		// ƽ�Ʊ任����
	int		m_nX;			// ˮƽƫ����
	int		m_nY;			// ��ֱƫ����

	CMoveTranDlg(CWnd* pParent = NULL); 
	virtual ~CMoveTranDlg();

	void Refresh();						// ���½���ƽ�Ʊ任��ˢ��Ԥ������
	virtual BOOL OnInitDialog();		// ��ʼ���Ի���

	// ��Ϣ��Ӧ����
	afx_msg void OnBnClickedRadioNormal();
	afx_msg void OnBnClickedRadioSize();
	afx_msg void OnEnChangeEditX();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnBnClickedSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	CPreviewDlg*	dlg;		// Ԥ���Ի���
	bool			m_bModify;	// ͼ���Ƿ��޸�

	// �Ի�������
	enum { IDD = IDD_MOVETRAN };
public:
	afx_msg void OnBnClickedOk();
};
