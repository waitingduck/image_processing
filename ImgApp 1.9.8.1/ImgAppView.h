// ImgAppView.h : CImgAppView 類別的介面
//
#include "MainFrm.h"

#include "Functions.h"
#include "Dialog.h"

#pragma once


class CImgAppView : public CScrollView
{
protected: // 僅從序列化建立
	CImgAppView();
	DECLARE_DYNCREATE(CImgAppView)

// 屬性
public:
	CImgAppDoc* GetDocument() const;
	CImage	*m_pImageView;
	double	m_dScale;
	int		m_nEnableAddToHistory;

//陣列	
	double** m_WvltCoeff;
	/////////
	double** m_WvltCoeff_L;
	double** m_WvltCoeff_A;
	double** m_WvltCoeff_B;
	//bool 
	bool m_bOnce, m_bTwice, m_bTribl, m_bFour, m_bFilter ;

// 作業
public:
	void	ShowViewImage();

// 覆寫
public:
	virtual void OnDraw(CDC* pDC);  // 覆寫以描繪此檢視
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 建構後第一次呼叫
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 程式碼實作
public:
	virtual ~CImgAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditHistoryLast();
	afx_msg void OnEditHistoryNext();
	afx_msg void OnViewSizeSmaller();
	afx_msg void OnViewSizeLarger();
	afx_msg void OnViewSizeOriginal();
	afx_msg void OnViewSizeFill();
	afx_msg void OnViewSizeFillWidth();
	afx_msg void OnViewSizeFillHeight();
	afx_msg void OnViewAddtohistory();
	afx_msg void OnTestFunction0();
	afx_msg void OnTestFunction1();
	afx_msg void OnTestFunction2();
	afx_msg void OnTestFunction3();
	afx_msg void OnTestFunction4();
	afx_msg void OnTestFunction5();
	afx_msg void OnTestFunction6();
	afx_msg void OnTestFunction7();
	afx_msg void OnTestFunction8();
	afx_msg void OnTestFunction9();
	afx_msg void OnViewRgbR();
	afx_msg void OnViewRgbG();
	afx_msg void OnViewRgbB();
	afx_msg void OnFileSaveHistory();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void On32829();
	afx_msg void On32830();
	afx_msg void On32831();
	afx_msg void On32833();
	afx_msg void On32834();
	afx_msg void On32835();
	afx_msg void On32837();
	afx_msg void On32838();
	afx_msg void On32839();
	afx_msg void On32840();
	afx_msg void On32841();
};

#ifndef _DEBUG  // ImgAppView.cpp 中的偵錯版本
inline CImgAppDoc* CImgAppView::GetDocument() const
   { return reinterpret_cast<CImgAppDoc*>(m_pDocument); }
#endif

