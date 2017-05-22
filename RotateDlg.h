#pragma once

#include "PreviewDlg.h"

// CRotateDlg �Ի���

class CRotateDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CRotateDlg)

public:
	int m_nAngle;		// ��ת�Ƕ�
	int m_nPos;			// ��ת�ǶȻ������
	int m_nType;		// ��תģʽ

	CRotateDlg(CWnd* pParent = NULL); 
	virtual ~CRotateDlg();

	void Refresh();
	virtual BOOL OnInitDialog();

	// ��Ϣ��Ӧ����
	afx_msg void OnBnClickedPrev();
	afx_msg void OnEnChangeAngle();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	bool	m_bModify;	// ͼ���Ƿ��޸�

	// �Ի�������
	enum { IDD = IDD_ROTATE };
public:
	afx_msg void OnBnClickedOk();
};