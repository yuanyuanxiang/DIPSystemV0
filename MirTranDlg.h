#pragma once

// CMirTranDlg �Ի���

#include "PreviewDlg.h"

class CMirTranDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CMirTranDlg)

public:
	int m_nType;			// ����任����

	CMirTranDlg(CWnd* pParent = NULL);
	virtual ~CMirTranDlg();

	void Refresh();					// ���½��о���任��ˢ��Ԥ������
	virtual BOOL OnInitDialog();	// ��ʼ���Ի���

	// ��Ϣ��Ӧ����
	afx_msg void OnBnClickedMirrorH();
	afx_msg void OnBnClickedMirrorV();
	afx_msg void OnBnClickedSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);// DDX/DDV ֧��

private:
	CPreviewDlg*	dlg;		// Ԥ���Ի���
	bool			m_bModify;	// ͼ���Ƿ��޸�

	// �Ի�������
	enum { IDD = IDD_MIRROR };
public:
	afx_msg void OnBnClickedOk();
};
