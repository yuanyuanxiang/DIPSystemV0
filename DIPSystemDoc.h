// DIPSystemDoc.h : CDIPSystemDoc ��Ľӿ�
//


#pragma once


class CDIPSystemDoc : public CDocument
{
protected: // �������л�����
	CDIPSystemDoc();
	DECLARE_DYNCREATE(CDIPSystemDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CDIPSystemDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


