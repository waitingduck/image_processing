// ImgAppDoc.h : CImgAppDoc ���O������
//
#include "ImgHistory.h"
#include "Functions.h"

#pragma once


class CImgAppDoc : public CDocument
{
protected: // �ȱq�ǦC�ƫإ�
	CImgAppDoc();
	DECLARE_DYNCREATE(CImgAppDoc)

// �ݩ�
public:
	ImageHistory *m_pHistory;
	CImage *m_pImage;
	CString m_sFileName;

// �@�~
public:
	int AddHistory();

// �мg
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �{���X��@
public:
	virtual ~CImgAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ͪ��T�������禡
protected:
	DECLARE_MESSAGE_MAP()
};


