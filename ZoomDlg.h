#pragma once


// CZoomDlg �Ի���

#include "PreviewDlg.h"

class CZoomDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CZoomDlg)

public:
	int		m_nWidth;		// ���ͼ��Ŀ��
	int		m_nHeight;		// ���ͼ��ĸ߶�
	BOOL	m_bIsPro;		// �Ƿ��޶��������
	int		m_nType;		// ����ģʽ

	CZoomDlg(CWnd* pParent = NULL); 
	virtual ~CZoomDlg();

	virtual BOOL OnInitDialog();

	// ��Ϣ��Ӧ����
	afx_msg void OnBnClickedPrev();
	afx_msg void OnEnChangeWidth();
	afx_msg void OnEnChangeHeight();
	afx_msg void OnBnClickedPro();
	afx_msg void OnBnClickedSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	int		m_nSrcWidth;			// ԭʼͼ��Ŀ��
	int		m_nSrcHeight;			// ԭʼͼ��ĸ߶�

	void Refresh();					// ���½���ͼ�����Ų�ˢ��Ԥ������
	void RefreshSize(bool width);	// �Զ��������ͼ��Ĵ�С

private:
	bool			m_bModify;		// ͼ���Ƿ��޸�

	// �Ի�������
	enum { IDD = IDD_ZOOM };
public:
	afx_msg void OnBnClickedOk();
};
