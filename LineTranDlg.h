#pragma once


// CLineTranDlg �Ի���

#include "PreviewDlg.h"
#include "afxwin.h"

class CLineTranDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CLineTranDlg)

public:
	CStatic m_stiPrev;			// �任����Ԥ����
	double	m_dSlope;			// б��
	double	m_dIntercept;		// �ؾ�
	bool	m_bInitialized;     // ���Ϊ��ʼ��

	// ��׼������������
	CLineTranDlg(CWnd* pParent = NULL); 
	virtual ~CLineTranDlg();

	void Refresh();					// ˢ��Ԥ����
	virtual BOOL OnInitDialog();	// �Ի����ʼ��ʱ��ʾԤ���Ի���

	afx_msg void OnEnChangeSlope();	// �ı�б��ʱ����Ӧ����
	afx_msg void OnEnChangeIntercept();
									// �ı�ؾ�ʱ����Ӧ����
	afx_msg void OnBnClickedSave();	// �����桱��ť��Ӧ����
	afx_msg void OnPaint();			// �Ի����ػ溯��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	CPreviewDlg*	dlg;		// Ԥ���Ի���
	bool			m_bModify;	// ͼ���Ƿ��޸�

	// �Ի�������
	enum { IDD = IDD_LINETRAN };
public:
	afx_msg void OnBnClickedOk();
};
