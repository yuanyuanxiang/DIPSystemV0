#pragma once
#include "afxwin.h"
#include "resource.h"


// CPrevDlg �Ի���

class CPrevDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CPrevDlg)

public:
	// �ؼ�����
	CString		m_strNum;				// ͼ��������Ϣ�ı�
	CScrollBar	m_ScrollBar;			// ˮƽ������
	CStatic		m_staPrev;				// ͼ��Ԥ����

	CPrevDlg(CWnd* pParent = NULL);		// ��׼���캯��
	virtual ~CPrevDlg();

	void Redraw(void);					// Ԥ�����ػ溯��

	// ��Ϣ��Ӧ����
	afx_msg void OnPaint();				// ���ƶԻ���
	afx_msg void OnBnClickedOpen();		// ���򿪡���ť��Ӧ����
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
										// ˮƽ��������Ӧ����

	enum { IDD = IDD_PREV };			// �Ի�������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	int				m_nPicNum;			// ��ǰĿ¼���ļ�����
	CString			m_strPath;			// ��ǰ��Ŀ¼
	CStringArray	m_FilesNameAry;		// ���浱ǰĿ¼�������ļ���������
	int				m_nPos;				// ��ǰ�ļ��������е�����
};
