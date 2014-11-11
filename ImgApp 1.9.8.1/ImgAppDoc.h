// ImgAppDoc.h : CImgAppDoc 類別的介面
//
#include "ImgHistory.h"
#include "Functions.h"

#pragma once


class CImgAppDoc : public CDocument
{
protected: // 僅從序列化建立
	CImgAppDoc();
	DECLARE_DYNCREATE(CImgAppDoc)

// 屬性
public:
	ImageHistory *m_pHistory;
	CImage *m_pImage;
	CString m_sFileName;

// 作業
public:
	int AddHistory();

// 覆寫
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 程式碼實作
public:
	virtual ~CImgAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
};


