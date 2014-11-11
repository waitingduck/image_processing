// ImgAppView.cpp : CImgAppView 類別的實作
//

#include "stdafx.h"
#include "ImgApp.h"

#include "ImgAppDoc.h"
#include "ImgAppView.h"

#include <math.h>
#include "Dialog.h"

#include <queue>

#include "ColorSpace.h" 
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int neighborgetp(int,int,int,int);
double iweight(double,double,double,double);

// CImgAppView

IMPLEMENT_DYNCREATE(CImgAppView, CScrollView)

BEGIN_MESSAGE_MAP(CImgAppView, CScrollView)
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CImgAppView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CImgAppView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CImgAppView::OnFileSaveAs)
	ON_COMMAND(ID_EDIT_HISTORY_LAST, &CImgAppView::OnEditHistoryLast)
	ON_COMMAND(ID_EDIT_HISTORY_NEXT, &CImgAppView::OnEditHistoryNext)
	ON_COMMAND(ID_VIEW_SIZE_SMALLER, &CImgAppView::OnViewSizeSmaller)
	ON_COMMAND(ID_VIEW_SIZE_LARGER, &CImgAppView::OnViewSizeLarger)
	ON_COMMAND(ID_VIEW_SIZE_ORIGINAL, &CImgAppView::OnViewSizeOriginal)
	ON_COMMAND(ID_VIEW_SIZE_FILL, &CImgAppView::OnViewSizeFill)
	ON_COMMAND(ID_VIEW_SIZE_FILL_WIDTH, &CImgAppView::OnViewSizeFillWidth)
	ON_COMMAND(ID_VIEW_SIZE_FILL_HEIGHT, &CImgAppView::OnViewSizeFillHeight)
	ON_COMMAND(ID_VIEW_ADDTOHISTORY, &CImgAppView::OnViewAddtohistory)
	ON_COMMAND(ID_FILE_SAVE_HISTORY, &CImgAppView::OnFileSaveHistory)
	ON_COMMAND(ID_TEST_FUNCTION0, &CImgAppView::OnTestFunction0)
	ON_COMMAND(ID_TEST_FUNCTION1, &CImgAppView::OnTestFunction1)
	ON_COMMAND(ID_TEST_FUNCTION2, &CImgAppView::OnTestFunction2)
	ON_COMMAND(ID_TEST_FUNCTION3, &CImgAppView::OnTestFunction3)
	ON_COMMAND(ID_TEST_FUNCTION4, &CImgAppView::OnTestFunction4)
	ON_COMMAND(ID_TEST_FUNCTION5, &CImgAppView::OnTestFunction5)
	ON_COMMAND(ID_TEST_FUNCTION6, &CImgAppView::OnTestFunction6)
	ON_COMMAND(ID_TEST_FUNCTION7, &CImgAppView::OnTestFunction7)
	ON_COMMAND(ID_TEST_FUNCTION8, &CImgAppView::OnTestFunction8)
	ON_COMMAND(ID_TEST_FUNCTION9, &CImgAppView::OnTestFunction9)
	ON_COMMAND(ID_VIEW_RGB_R, &CImgAppView::OnViewRgbR)
	ON_COMMAND(ID_VIEW_RGB_G, &CImgAppView::OnViewRgbG)
	ON_COMMAND(ID_VIEW_RGB_B, &CImgAppView::OnViewRgbB)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_32829, &CImgAppView::On32829)
	ON_COMMAND(ID_32830, &CImgAppView::On32830)
	ON_COMMAND(ID_32831, &CImgAppView::On32831)
	ON_COMMAND(ID_32833, &CImgAppView::On32833)
	ON_COMMAND(ID_32834, &CImgAppView::On32834)
	ON_COMMAND(ID_32835, &CImgAppView::On32835)
	ON_COMMAND(ID_32837, &CImgAppView::On32837)
	ON_COMMAND(ID_32838, &CImgAppView::On32838)
	ON_COMMAND(ID_32839, &CImgAppView::On32839)
	ON_COMMAND(ID_32840, &CImgAppView::On32840)
	ON_COMMAND(ID_32841, &CImgAppView::On32841)
END_MESSAGE_MAP()

// CImgAppView 建構/解構

CImgAppView::CImgAppView()
{
	m_pImageView = new CImage;
	m_dScale = 1.0;
	m_nEnableAddToHistory = 0;

	m_WvltCoeff = NULL;
	
	m_WvltCoeff_L = NULL;
	m_WvltCoeff_A = NULL;
	m_WvltCoeff_B = NULL;
	
	m_bTribl = FALSE;	m_bOnce = FALSE;	m_bTwice = FALSE;	m_bFour = FALSE;	m_bFilter = FALSE;
}

CImgAppView::~CImgAppView()
{
}

BOOL CImgAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CScrollView::PreCreateWindow(cs);
}

// CImgAppView 描繪

void CImgAppView::OnDraw(CDC* pDC)
{
	CImgAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SetStretchBltMode(*pDC, COLORONCOLOR); // 解決縮小時所產生的怪異線條

	int w = (int)(m_dScale * pDoc->m_pImage->GetWidth());
	int h = (int)(m_dScale * pDoc->m_pImage->GetHeight());
	pDoc->m_pImage->StretchBlt(*pDC, 0, 0, w, h, SRCCOPY);
}

void CImgAppView::OnInitialUpdate()
{
	CSize sizeTotal;

	CImgAppDoc* pDoc = GetDocument();
	sizeTotal.cx = (int)(pDoc->m_pImage->GetWidth() * m_dScale);
	sizeTotal.cy = (int)(pDoc->m_pImage->GetHeight() * m_dScale);

	SetScrollSizes(MM_TEXT, sizeTotal);
	CScrollView::OnInitialUpdate();
	m_nEnableAddToHistory++;
}


// CImgAppView 列印

BOOL CImgAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}

void CImgAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}

void CImgAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}


// CImgAppView 診斷

#ifdef _DEBUG
void CImgAppView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImgAppView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImgAppDoc* CImgAppView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImgAppDoc)));
	return (CImgAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CImgAppView 訊息處理常式

void CImgAppView::OnFileOpen()
{
	CImgAppDoc* pDoc = GetDocument();
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;
	CImage *pImgTemp = new CImage;

	hResult = pImgTemp->GetImporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if(FAILED(hResult)) return; // 副檔名讀取失敗

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	hResult = (int)dlg.DoModal();
	if(FAILED(hResult)) return; // 對話方塊執行失敗

	pDoc->m_sFileName = dlg.GetFileName();
	hResult = pImgTemp->Load(pDoc->m_sFileName);
	if(FAILED(hResult)) return; // 圖片讀取失敗

	pDoc->AddHistory();
	pDoc->m_pHistory->m_pInstantHistory->m_pImage = pImgTemp;
	pDoc->m_pImage = pImgTemp;

	fnCheckCImageIs24Bits(pImgTemp);

	m_dScale = 1.0;

	OnInitialUpdate();
}

void CImgAppView::OnFileSave()
{
	CImgAppDoc* pDoc = GetDocument();
	pDoc->m_pImage->Save(pDoc->m_sFileName);
}

void CImgAppView::OnFileSaveAs()
{
	CString strFilter, pathstring;
	CSimpleArray<GUID> aguidFileTypes;	
	CImgAppDoc* pDoc = GetDocument();
	HRESULT hResult;

	hResult = pDoc->m_pImage->GetExporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if(FAILED(hResult)) return; // 副檔名讀取失敗

	DWORD Flags = OFN_EXPLORER|OFN_EXTENSIONDIFFERENT|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
	CFileDialog dlg(FALSE, _T("BMP"), pDoc->m_sFileName, Flags, strFilter);
	hResult = (int)dlg.DoModal();
	if(FAILED(hResult)) return; // 對話方塊執行失敗

	hResult = pDoc->m_pImage->Save(dlg.GetPathName());
	if(FAILED(hResult)) return; // 圖片讀取失敗
}

void CImgAppView::ShowViewImage()
{
	m_nEnableAddToHistory = -1;
	OnInitialUpdate();
}

void CImgAppView::OnEditHistoryLast()
{
	CImgAppDoc* pDoc = GetDocument();
	pDoc->m_pHistory->m_pInstantHistory = pDoc->m_pHistory->m_pInstantHistory->m_pLast;
	pDoc->m_pImage = pDoc->m_pHistory->m_pInstantHistory->m_pImage;
	OnInitialUpdate();
}

void CImgAppView::OnEditHistoryNext()
{
	CImgAppDoc* pDoc = GetDocument();
	pDoc->m_pHistory->m_pInstantHistory = pDoc->m_pHistory->m_pInstantHistory->m_pNext;
	pDoc->m_pImage = pDoc->m_pHistory->m_pInstantHistory->m_pImage;
	OnInitialUpdate();
}

void CImgAppView::OnViewSizeSmaller()
{
	m_dScale *= 0.8;
	OnInitialUpdate();
	m_nEnableAddToHistory--;
}

void CImgAppView::OnViewSizeLarger()
{
	m_dScale *= 1.25;
	OnInitialUpdate();
	m_nEnableAddToHistory--;
}

void CImgAppView::OnViewSizeOriginal()
{
	m_dScale = 1.0;
	OnInitialUpdate();
	m_nEnableAddToHistory--;
}

void CImgAppView::OnViewSizeFill()
{
	CImgAppDoc* pDoc = GetDocument();

	static int count = 0;
	if(++count % 2 == 1) OnViewSizeFill();

	CRect rctWindowSize;
	GetClientRect(rctWindowSize);

	if((double) pDoc->m_pImage->GetWidth() / pDoc->m_pImage->GetHeight() > (double) rctWindowSize.Width() / rctWindowSize.Height())
		m_dScale = (double) rctWindowSize.Width() / pDoc->m_pImage->GetWidth();
	else
		m_dScale = (double) rctWindowSize.Height() / pDoc->m_pImage->GetHeight();

	OnInitialUpdate();
	m_nEnableAddToHistory--;
}

void CImgAppView::OnViewSizeFillWidth()
{
	CImgAppDoc* pDoc = GetDocument();

	static int count = 0;
	if(++count % 2 == 1) OnViewSizeFillWidth();

	CRect rctWindowSize;
	GetClientRect(rctWindowSize);

	m_dScale = (double) rctWindowSize.Width() / pDoc->m_pImage->GetWidth();
	OnInitialUpdate();
	m_nEnableAddToHistory--;
}

void CImgAppView::OnViewSizeFillHeight()
{
	CImgAppDoc* pDoc = GetDocument();

	static int count = 0;
	if(++count % 2 == 1) OnViewSizeFillHeight();

	CRect rctWindowSize;
	GetClientRect(rctWindowSize);

	m_dScale = (double) rctWindowSize.Height() / pDoc->m_pImage->GetHeight();
	OnInitialUpdate();
	m_nEnableAddToHistory--;
}


void CImgAppView::OnViewAddtohistory()
{
	CImgAppDoc* pDoc = GetDocument();

	if(m_nEnableAddToHistory != 0 || fnIsEqualImage(m_pImageView, pDoc->m_pImage)){
		AfxMessageBox(_T("目前狀態無法加入歷程或已加入歷程!!"));
		return;
	}

	if(pDoc->AddHistory() != 0) AfxMessageBox(_T("Doc->AddHistory().....Error!!"));
	*(pDoc->m_pImage) = *m_pImageView;
}

void CImgAppView::OnFileSaveHistory()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CImgAppDoc* pDoc = GetDocument();

	int nCount = pDoc->m_pHistory->GetCount();
	CString *strNumber = new CString[nCount];
	for(int i = 0; i < nCount; i++){
		strNumber[i].Format(_T("%d"), i);
		while(strNumber[i].GetLength() < (int)log((double)(nCount - 1)))
			strNumber[i] = '0' + strNumber[i];
	}

	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;	
	HRESULT hResult;
	hResult = pDoc->m_pImage->GetExporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if(FAILED(hResult)) return; // 副檔名讀取失敗

	DWORD Flags = OFN_EXPLORER|OFN_EXTENSIONDIFFERENT|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
	CFileDialog dlg(FALSE, _T("BMP"), pDoc->m_sFileName, Flags, strFilter);
	hResult = (int)dlg.DoModal();
	if(FAILED(hResult)) return; // 對話方塊執行失敗

	ImageHistoryEntity *pPtr = pDoc->m_pHistory->m_pInstantHistory;
	while(pPtr->m_pLast != pPtr) pPtr = pPtr->m_pLast;

	for(int i = 0; i < nCount; i++){
		hResult = pPtr->m_pImage->Save(dlg.GetFileTitle() + strNumber[i] + '.' + dlg.GetFileExt());
		if(FAILED(hResult)) return;
		pPtr = pPtr->m_pNext;
	}
}

void CImgAppView::OnViewRgbR()
{
	CImgAppDoc* pDoc = GetDocument();
	fnGetRChannel(m_pImageView, pDoc->m_pImage);
	ShowViewImage();
}

void CImgAppView::OnViewRgbG()
{
	CImgAppDoc* pDoc = GetDocument();
	fnGetGChannel(m_pImageView, pDoc->m_pImage);
	ShowViewImage();
}

void CImgAppView::OnViewRgbB()
{
	CImgAppDoc* pDoc = GetDocument();
	fnGetBChannel(m_pImageView, pDoc->m_pImage);
	ShowViewImage();
}

void CImgAppView::OnMouseMove(UINT nFlags, CPoint point)
{
	CImgAppDoc* pDoc = GetDocument();
	int nX = (int)((double)(GetScrollPos(SB_HORZ) + point.x) / m_dScale); // 相對於原圖的x座標
	int nY = (int)((double)(GetScrollPos(SB_VERT) + point.y) / m_dScale); // 相對於原圖的y座標
	if(nX < pDoc->m_pImage->GetWidth() && nY < pDoc->m_pImage->GetHeight()){
		BYTE *ptr = (BYTE *)pDoc->m_pImage->GetPixelAddress(nX, nY);
		CString str;
		str.Format(_T("[%d, %d] = (%d, %d, %d)"), nX, nY, ptr[2], ptr[1], ptr[0]);
		((CMainFrame*)(::AfxGetMainWnd()))->m_wndStatusBar.SetPaneText(0, str, 1);
	}
	else
		((CMainFrame*)(::AfxGetMainWnd()))->m_wndStatusBar.SetPaneText(0, (CString)"就緒", 1);
}
class list
{
	public:
		BYTE B;
		BYTE G;
		BYTE R;
		list *next;
};

void CImgAppView::On32829()
{
	// TODO: 馬賽克
	CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	
	CImage *pImage = pDoc->m_pImage;

	

	int mW=10,mH=10;
	
	int countH=pImage->GetHeight()/mH+1;
	/*
	BYTE *pmi = new BYTE [countH*3];

	for(int i = 0; i < pImage->GetWidth(); i++)
	{
		for(int j = 0; j < pImage->GetHeight(); j++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			if( j%mH==0 && i%mW==0 )
			{
				pmi[j/mH*3+0]=ptr[0];
				pmi[j/mH*3+1]=ptr[1];
				pmi[j/mH*3+2]=ptr[2];
			}
			ptr[0] = pmi[j/mH*3+0]; // B
			ptr[1] = pmi[j/mH*3+1]; // G
			ptr[2] = pmi[j/mH*3+2]; // R
			
		}
	}
	delete [] pmi;
	*/
	BYTE **pmi = new BYTE* [countH];
	
	for (int a=0;a<countH;a++)
	{
		pmi[a]= new BYTE[3];
	}
	int recordW=pImage->GetWidth();
	for(int i = 0; i < pImage->GetWidth(); i++)
	{
		for(int j = 0; j < pImage->GetHeight(); j++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			if( j%mH==0 && i%mW==0 )
			{
				pmi[j/mH][0]=ptr[0];
				pmi[j/mH][1]=ptr[1];
				pmi[j/mH][2]=ptr[2];
			}
			ptr[0] = pmi[j/mH][0]; // B
			ptr[1] = pmi[j/mH][1]; // G
			ptr[2] = pmi[j/mH][2]; // R
			
		}
	}
	for (int a=0;a<countH;a++)
	{
		delete [] pmi[a];
	}
	delete [] pmi;
	OnInitialUpdate();
}

void CImgAppView::On32830()
{
	// TODO: 分水嶺
#define INIT -1                     
#define MASK -2
#define WSHED 0
#define FICTITIOUS -3
using namespace std;

int curlab=0;
int curdist;
queue <int> q;         //建造一個QUEUE
int pixel,neighbor;
int width,height;
int count=0;
int sum,tempsum,intensity;
int door=25;
float weight[5][5]={{1,2.31,3.63,2.31,1},
					{2.31,5.5,6.82,5.5,2.31},
					{3.63,6.82,10,6.82,3.63},
					{2.31,5.5,6.82,5.5,2.31},
					{1,2.31,3.63,2.31,1}};

CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	
	CImage *pImage = pDoc->m_pImage;
	width=pImage->GetWidth();
	height=pImage->GetHeight();

	

	BYTE **bmp = new BYTE* [width];
	
	for (int a=0;a<width;a++)
	{
		bmp[a]= new BYTE[height];
	}

	BYTE **bmp2 = new BYTE* [width];
	
	for (int a=0;a<width;a++)
	{
		bmp2[a]= new BYTE[height];
	}

	

	int **imp = new int* [width*height];
	
	for (int a=0;a<(width*height);a++)
	{
		imp[a]= new int[2];
	}

	
	for(int j = 0; j < height; j++)         //存入灰度，初始化label為0
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			BYTE gray = (ptr[0] + ptr[1] + ptr[2]) / 3;
			bmp[i][j]=gray;
			bmp2[i][j]=gray;
			imp[j*width+i][0]=INIT;
			imp[j*width+i][1]=0;
		}
	}
/*
	for(int e=0;e<2;e++){
		for(int j = 2; j < pImage->GetHeight()-2; j++)
		{
			for(int i = 2; i < pImage->GetWidth()-2; i++)
			{
				sum=0;
				tempsum=0;
				BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
				if(i==252 && j==220)
				{count =0;}
				for(int a=-2;a<3;a++)
				{
					for(int b=-2;b<3;b++)
					{
						if((bmp[i][j]>=bmp[i+a][j+b] && bmp[i][j]-bmp[i+a][j+b]<door) || (bmp[i][j]<bmp[i+a][j+b] && bmp[i+a][j+b]-bmp[i][j]<door))
						{
							
							intensity=(int)bmp[i+a][j+b];
							sum=sum+weight[a+2][b+2]*intensity;
						}
					}
				}
				bmp2[i][j]=sum/get25sum(i,j,bmp);
				}
		}
		for(int j = 2; j < pImage->GetHeight()-2; j++)
		{
			for(int i = 2; i < pImage->GetWidth()-2; i++)
			{
				sum=0;
				tempsum=0;
				BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
				for(int a=-2;a<3;a++)
				{
					for(int b=-2;b<3;b++)
					{
						if((bmp2[i][j]>=bmp2[i+a][j+b] && bmp2[i][j]-bmp2[i+a][j+b]<door) || (bmp2[i][j]<bmp2[i+a][j+b] && bmp2[i+a][j+b]-bmp2[i][j]<door))
						{
							intensity=(int)bmp2[i+a][j+b];
							sum=sum+weight[a+2][b+2]*intensity;
						}
					}
				}
				bmp[i][j]=sum/get25sum(i,j,bmp2);
			}
		}
		
	}
*/
	int gx1,gx2,gy1,gy2;
	for(int j = 1; j < height-1; j++)         //用一階微分做影像銳化
	{
		for(int i = 1; i < width-1; i++)
		{
			gx1=bmp[i-1][j-1]+2*bmp[i][j-1]+bmp[i+1][j-1];
			gx2=bmp[i-1][j+1]+2*bmp[i][j+1]+bmp[i+1][j+1];
			gy1=bmp[i-1][j-1]+2*bmp[i-1][j]+bmp[i-1][j+1];
			gy2=bmp[i+1][j-1]+2*bmp[i+1][j]+bmp[i+1][j+1];
			if(gx1>=gx2)
			{
				gx1=gx1-gx2;
			}
			else
			{
				gx1=gx2-gx1;
			}
			if(gy1>=gy2)
			{
				gy1=gy1-gy2;
			}
			else
			{
				gy1=gy2-gy1;
			}
			bmp2[i][j]=(gx1+gy1)/4;
			
		}
	}

		
		for(int j = 0; j < pImage->GetHeight(); j++)
		{
			for(int i = 0; i < pImage->GetWidth(); i++)
			{
				
				bmp[i][j]=bmp2[i][j];
				}
		}
	/*	for(int j = 2; j < pImage->GetHeight()-2; j++)
		{
			for(int i = 2; i < pImage->GetWidth()-2; i++)
			{
				sum=0;
				tempsum=0;
				BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
				for(int a=-2;a<3;a++)
				{
					for(int b=-2;b<3;b++)
					{
						if((bmp[i][j]>=bmp[i+a][j+b] && bmp[i][j]-bmp[i+a][j+b]<door) || (bmp[i][j]<bmp[i+a][j+b] && bmp[i+a][j+b]-bmp[i][j]<door))
						{
							intensity=(int)bmp[i+a][j+b];
							sum=sum+weight[a+2][b+2]*intensity;
						}
					}
				}
				bmp2[i][j]=sum/get25sum(i,j,bmp);
			}
		}
		
	}
*/

	//start flooding
	for(int wheigth=0;wheigth<=255;wheigth++)
	{
		for(int j = 0; j < pImage->GetHeight(); j++)         
		{
			for(int i = 0; i < pImage->GetWidth(); i++)
			{
				if((int)bmp[i][j]==wheigth)
				{
					imp[j*width+i][0]=MASK;
					for(int n=0;n<8;n++)
					{
						neighbor=neighborgetp(n,j*width+i,width,height);
						if(neighbor>=0)
						{
							if(imp[neighbor][0]>0 || imp[neighbor][0]==WSHED)    //if has a neighbor lab>0 or ==wshed
							{
								imp[j*width+i][1]=1;                           
								q.push(j*width+i);
								break;
							}
						}
					}
				}
			}
		}
		curdist=1;
		q.push(FICTITIOUS);
		while(1)
		{
			pixel=q.front();
			q.pop();
			if(pixel==FICTITIOUS)
			{
				if(q.empty())
				{
					break;
				}
				else
				{
					q.push(FICTITIOUS);
					curdist++;
					pixel=q.front();
					q.pop();
				}
			}
			for(int n=0;n<8;n++)
			{
				neighbor=neighborgetp(n,pixel,width,height);
				if(neighbor>=0)
				{
					if(imp[neighbor][1]<=curdist && (imp[neighbor][0]>0 || imp[neighbor][0]==WSHED))    //<curdist表示不屬於目前的作業範圍，
					{
						if(imp[neighbor][0]>0)
						{
							if(imp[pixel][0]==MASK || imp[pixel][0]==WSHED)
							{
								imp[pixel][0]=imp[neighbor][0];
							}
							else if(imp[pixel][0]!=imp[neighbor][0])
							{
								imp[pixel][0]=WSHED;
								break;
							}
						}
						else if(imp[pixel][0]==MASK)
						{
							imp[pixel][0]=WSHED;
							//break;
						}
					}
					else if(imp[neighbor][0]==MASK && imp[neighbor][1]==0)   //此聚水盆底部是平的，dist=0  表一開始時其周圍沒有已標記或為分水嶺的pixel
					{
						imp[neighbor][1]=curdist+1;
						q.push(neighbor);
					}
				}
			}
		}
		//detect and process new minima at level wheight
		for(int j = 0; j < pImage->GetHeight(); j++)         
		{
			for(int i = 0; i < pImage->GetWidth(); i++)
			{
				if((int)bmp[i][j]==wheigth)
				{
					imp[j*width+i][1]=0;
					if(imp[j*width+i][0]==MASK)
					{
						curlab=curlab+1;
						q.push(j*width+i);
						imp[j*width+i][0]=curlab;
						while(!q.empty())
						{
							pixel=q.front();
							q.pop();
							for(int n=0;n<8;n++)
							{
								neighbor=neighborgetp(n,pixel,width,height);
								if(neighbor>=0)
								{
									if(imp[neighbor][0]==MASK)
									{
										q.push(neighbor);
										imp[neighbor][0]=curlab;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	BYTE setcolor=255;
	for(int j = 0; j < height; j++)         
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			if(imp[j*width+i][0]==WSHED)
			{
				ptr[0] = setcolor; // B    //將有做記號的點輸出為想要的顏色
				ptr[1] = setcolor; // G
				ptr[2] = 0; // R
			}
			/*else
			{
				ptr[0] = 255; // B    //將有做記號的點輸出為想要的顏色
				ptr[1] = 255; // G
				ptr[2] = 255; // R
			}*/
			/*else
			{
				ptr[0] = bmp2[i][j]; // B    //將有做記號的點輸出為原本的顏色
				ptr[1] = bmp2[i][j]; // G
				ptr[2] = bmp2[i][j]; // R
			}*/
			/*else
			{
				ptr[0] = imp[j*width+i][0]; // B    //將有做記號的點輸出為lable
				ptr[1] = imp[j*width+i][0]; // G
				ptr[2] = imp[j*width+i][0]; // R
			}*/
			
		}
	}

	// destruct
	for (int a=0;a<width;a++)
	{
		delete [] bmp[a];
	}
	delete [] bmp;

	for (int a=0;a<width;a++)
	{
		delete [] bmp2[a];
	}
	delete [] bmp2;

	

	for (int a=0;a<width*height;a++)
	{
		delete [] imp[a];
	}
	delete [] imp;

	OnInitialUpdate();
}

int neighborgetp(int n,int p,int w,int h)
{
	int r;
	if(p>=0)
	{
		switch(n)
		{
			case 0:
				r=(p-w-1);
				if(p/w-1!=r/w || r>=w*h)
				{
					r=-1;
				}
				break;
			case 1:
				r=(p-w);
				if(p/w-1!=r/w || r>=w*h)
				{
					r=-1;
				}
			break;
			case 2:
				r=(p-w+1);
				if(p/w-1!=r/w || r>=w*h)
				{
					r=-1;
				}
				break;
			case 3:
				r=(p-1);
				if(p/w!=r/w || r>=w*h)
				{
					r=-1;
				}
				break;
			case 4:
				r=(p+1);
				if(p/w!=r/w || r>=w*h)
				{
					r=-1;
				}
				break;
			case 5:
				r=(p+w-1);
				if(p/w+1!=r/w || r>=w*h)
				{
					r=-1;
				}
				break;
			case 6:
				r=(p+w);
				if(p/w+1!=r/w || r>=w*h)
				{
					r=-1;
				}
				break;
			case 7:
				r=(p+w+1);
				if(p/w+1!=r/w || r>=w*h)
				{
					r=-1;
				}
				break;
		}
	}
	else
	{
		r=-1;
	}
	
	return r;
}


void CImgAppView::On32831()
{
	// TODO: gradient
	int width,height;

	CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	
	CImage *pImage = pDoc->m_pImage;
	width=pImage->GetWidth();
	height=pImage->GetHeight();

	BYTE **bmp = new BYTE* [width];
	
	for (int a=0;a<width;a++)
	{
		bmp[a]= new BYTE[height];
	}
	
	BYTE **bmp2 = new BYTE* [width];
	
	
	for (int a=0;a<width;a++)
	{
		bmp2[a]= new BYTE[height];
	}
	for(int j = 0; j < height; j++)         //存入灰度，初始化label為0
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			BYTE gray = (ptr[0] + ptr[1] + ptr[2]) / 3;
			bmp2[i][j]=gray;
		}
	}
	/*
	for(int j = 1; j < height-1; j++)         //用一階微分做影像銳化
	{
		for(int i = 1; i < width-1; i++)
		{
			bmp[i][j]=8*bmp2[i][j]-bmp2[i-1][j-1]-bmp2[i][j-1]-bmp2[i+1][j-1]-bmp2[i-1][j]-bmp2[i+1][j]-bmp2[i-1][j+1]-bmp2[i][j+1]-bmp2[i+1][j+1];
		}
	}
*/
	/*BYTE gx,gy;
	for(int j = 0; j < height-1; j++)         //用一階微分做影像銳化
	{
		for(int i = 0; i < width-1; i++)
		{
			if(bmp[i][j]>=bmp[i+1][j])
			{
				gx=bmp2[i][j]-bmp2[i+1][j];
			}
			else
			{
				gx=bmp2[i+1][j]-bmp2[i][j];
			}
			if(bmp[i][j]>=bmp[i][j+1])
			{
				gy=bmp2[i][j]-bmp2[i][j+1];
			}
			else
			{
				gy=bmp2[i][j+1]-bmp2[i][j];
			}
			bmp[i][j]=gx+gy;
			
		}
	}*/
int gx1,gx2,gy1,gy2;
	for(int j = 1; j < height-1; j++)         //用一階微分做影像銳化
	{
		for(int i = 1; i < width-1; i++)
		{
			gx1=bmp2[i-1][j-1]+2*bmp2[i][j-1]+bmp2[i+1][j-1];
			gx2=bmp2[i-1][j+1]+2*bmp2[i][j+1]+bmp2[i+1][j+1];
			gy1=bmp2[i-1][j-1]+2*bmp2[i-1][j]+bmp2[i-1][j+1];
			gy2=bmp2[i+1][j-1]+2*bmp2[i+1][j]+bmp2[i+1][j+1];
			if(gx1>=gx2)
			{
				gx1=gx1-gx2;
			}
			else
			{
				gx1=gx2-gx1;
			}
			if(gy1>=gy2)
			{
				gy1=gy1-gy2;
			}
			else
			{
				gy1=gy2-gy1;
			}
			bmp[i][j]=(gx1+gy1)/4;
			
		}
	}

	for(int j = 0; j < height; j++)         
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			{
				ptr[0] = bmp[i][j]; // B    //將有做記號的點輸出為想要的顏色
				ptr[1] = bmp[i][j]; // G
				ptr[2] = bmp[i][j]; // R
			}
			
		}
	}
	OnInitialUpdate();
	for (int a=0;a<width;a++)
	{
		delete [] bmp[a];
	}
	delete [] bmp;

	for (int a=0;a<width;a++)
	{
		delete [] bmp2[a];
	}
	delete [] bmp2;
}

void CImgAppView::On32833()
{
	// TODO: 3*3高斯模糊

int width,height;
int count=0;


	CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	
	CImage *pImage = pDoc->m_pImage;
	width=pImage->GetWidth();
	height=pImage->GetHeight();

	

	BYTE **bmp = new BYTE* [width];
	
	for (int a=0;a<width;a++)
	{
		bmp[a]= new BYTE[height];
	}

	BYTE **bmp2 = new BYTE* [width];
	
	for (int a=0;a<width;a++)
	{
		bmp2[a]= new BYTE[height];
	}

	for(int j = 0; j < height; j++)         //存入灰度
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			BYTE gray = (ptr[0] + ptr[1] + ptr[2]) / 3;
			bmp[i][j]=gray;
			bmp2[i][j]=gray;
		}
	}

	for(int z=0;z<10;z++)
	{
		for(int j = 1; j < pImage->GetHeight()-1; j++)
		{
			for(int i = 1; i < pImage->GetWidth()-1; i++)
			{
				BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
				bmp2[i][j]=(bmp[i-1][j-1]+bmp[i-1][j]+bmp[i-1][j+1]+bmp[i][j-1]+bmp[i][j]+bmp[i][j+1]+bmp[i+1][j-1]+bmp[i+1][j]+bmp[i+1][j+1])/9;
			}
		}
		for(int j = 1; j < pImage->GetHeight()-1; j++)
		{
			for(int i = 1; i < pImage->GetWidth()-1; i++)
			{
				BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
				bmp[i][j]=(bmp2[i-1][j-1]+bmp2[i-1][j]+bmp2[i-1][j+1]+bmp2[i][j-1]+bmp2[i][j]+bmp2[i][j+1]+bmp2[i+1][j-1]+bmp2[i+1][j]+bmp2[i+1][j+1])/9;
			}
		}
	}

	for(int j = 0; j < height; j++)         
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			{
				ptr[0] = bmp[i][j]; // B    //將有做記號的點輸出為想要的顏色
				ptr[1] = bmp[i][j]; // G
				ptr[2] = bmp[i][j]; // R
			}
			
		}
	}
	OnInitialUpdate();

	for (int a=0;a<width;a++)
	{
		delete [] bmp[a];
	}
	delete [] bmp;

	for (int a=0;a<width;a++)
	{
		delete [] bmp2[a];
	}
	delete [] bmp2;
}

void CImgAppView::On32834()
{
	// TODO: 5*5高斯模糊
	int width,height;
int count=0;


	CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	
	CImage *pImage = pDoc->m_pImage;
	width=pImage->GetWidth();
	height=pImage->GetHeight();

	double weight[5][5]={{  0.085265,  0.100728,  0.106483,  0.100728,  0.085265},
{  0.100728,  0.118996,  0.125794,  0.118996,  0.100728},
{  0.106483,  0.125794,  0.132981,  0.125794,  0.106483},
{  0.100728,  0.118996,  0.125794,  0.118996,  0.100728},
{  0.085265,  0.100728,  0.106483,  0.100728,  0.085265}
};

	BYTE **bmp = new BYTE* [width];
	
	for (int a=0;a<width;a++)
	{
		bmp[a]= new BYTE[height];
	}

	double **bmp2 = new double* [width];
	
	for (int a=0;a<width;a++)
	{
		bmp2[a]= new double[height];
	}

	for(int j = 0; j < height; j++)         //存入灰度
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			BYTE gray = (ptr[0] + ptr[1] + ptr[2]) / 3;
			bmp[i][j]=gray;
			bmp2[i][j]=gray;
		}
	}

	
		for(int j = 2; j < pImage->GetHeight()-2; j++)
		{
			for(int i = 2; i < pImage->GetWidth()-2; i++)
			{
				BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
				bmp[i][j]=(weight[0][0]*bmp2[i-2][j-2]+weight[0][1]*bmp2[i-1][j-2]+weight[0][2]*bmp2[i][j-2]+weight[0][3]*bmp2[i+1][j-2]+weight[0][4]*bmp2[i+2][j-2]
						  +weight[1][0]*bmp2[i-2][j-1]+weight[1][1]*bmp2[i-1][j-1]+weight[1][2]*bmp2[i][j-1]+weight[1][3]*bmp2[i+1][j-1]+weight[1][4]*bmp2[i+2][j-1]
						  +weight[2][0]*bmp2[i-2][j]+weight[2][1]*bmp2[i-1][j]+weight[2][2]*bmp2[i][j]+weight[2][3]*bmp2[i+1][j]+weight[2][4]*bmp2[i+2][j]
						  +weight[3][0]*bmp2[i-2][j+1]+weight[3][1]*bmp2[i-1][j+1]+weight[3][2]*bmp2[i][j+1]+weight[3][3]*bmp2[i+1][j+1]+weight[3][4]*bmp2[i+2][j+1]
					      +weight[4][0]*bmp2[i-2][j+2]+weight[4][1]*bmp2[i-1][j+2]+weight[4][2]*bmp2[i][j+2]+weight[4][3]*bmp2[i+1][j+2]+weight[4][4]*bmp2[i+2][j+2])/2.684960;
			}
		}
	

	for(int j = 0; j < height; j++)         
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			{
				ptr[0] = bmp[i][j]; // B    //將有做記號的點輸出為想要的顏色
				ptr[1] = bmp[i][j]; // G
				ptr[2] = bmp[i][j]; // R
			}
			
		}
	}
	OnInitialUpdate();

	for (int a=0;a<width;a++)
	{
		delete [] bmp[a];
	}
	delete [] bmp;

	for (int a=0;a<width;a++)
	{
		delete [] bmp2[a];
	}
	delete [] bmp2;
}

void CImgAppView::On32835()//Bilateral Filter
{
	// TODO: 
	int width,height;
	int count=0;
	double sum,tempsum,intensity,num,sigma=30,weightsum;
	num=1/(sigma*sqrt(2*3.1415926535897));
	BYTE door=25;
	double weight[15][15]={{  0.000575,  0.001183,  0.002179,  0.003593,  0.005301,  0.006999,  0.008268,  0.008741,  0.008268,  0.006999,  0.005301,  0.003593,  0.002179,  0.001183,  0.000575},
						   {  0.001183,  0.002436,  0.004488,  0.007399,  0.010916,  0.014411,  0.017024,  0.017997,  0.017024,  0.014411,  0.010916,  0.007399,  0.004488,  0.002436,  0.001183},
					       {  0.002179,  0.004488,  0.008268,  0.013632,  0.020112,  0.026552,  0.031367,  0.033159,  0.031367,  0.026552,  0.020112,  0.013632,  0.008268,  0.004488,  0.002179},
						   {  0.003593,  0.007399,  0.013632,  0.022476,  0.033159,  0.043776,  0.051716,  0.054670,  0.051716,  0.043776,  0.033159,  0.022476,  0.013632,  0.007399,  0.003593},
						   {  0.005301,  0.010916,  0.020112,  0.033159,  0.048921,  0.064585,  0.076298,  0.080657,  0.076298,  0.064585,  0.048921,  0.033159,  0.020112,  0.010916,  0.005301},
						   {  0.006999,  0.014411,  0.026552,  0.043776,  0.064585,  0.085265,  0.100728,  0.106483,  0.100728,  0.085265,  0.064585,  0.043776,  0.026552,  0.014411,  0.006999},
						   {  0.008268,  0.017024,  0.031367,  0.051716,  0.076298,  0.100728,  0.118996,  0.125794,  0.118996,  0.100728,  0.076298,  0.051716,  0.031367,  0.017024,  0.008268},
						   {  0.008741,  0.017997,  0.033159,  0.054670,  0.080657,  0.106483,  0.125794,  0.132981,  0.125794,  0.106483,  0.080657,  0.054670,  0.033159,  0.017997,  0.008741},
						   {  0.008268,  0.017024,  0.031367,  0.051716,  0.076298,  0.100728,  0.118996,  0.125794,  0.118996,  0.100728,  0.076298,  0.051716,  0.031367,  0.017024,  0.008268},
						   {  0.006999,  0.014411,  0.026552,  0.043776,  0.064585,  0.085265,  0.100728,  0.106483,  0.100728,  0.085265,  0.064585,  0.043776,  0.026552,  0.014411,  0.006999},
						   {  0.005301,  0.010916,  0.020112,  0.033159,  0.048921,  0.064585,  0.076298,  0.080657,  0.076298,  0.064585,  0.048921,  0.033159,  0.020112,  0.010916,  0.005301},
						   {  0.003593,  0.007399,  0.013632,  0.022476,  0.033159,  0.043776,  0.051716,  0.054670,  0.051716,  0.043776,  0.033159,  0.022476,  0.013632,  0.007399,  0.003593},
						   {  0.002179,  0.004488,  0.008268,  0.013632,  0.020112,  0.026552,  0.031367,  0.033159,  0.031367,  0.026552,  0.020112,  0.013632,  0.008268,  0.004488,  0.002179},
						   {  0.001183,  0.002436,  0.004488,  0.007399,  0.010916,  0.014411,  0.017024,  0.017997,  0.017024,  0.014411,  0.010916,  0.007399,  0.004488,  0.002436,  0.001183},
						   {  0.000575,  0.001183,  0.002179,  0.003593,  0.005301,  0.006999,  0.008268,  0.008741,  0.008268,  0.006999,  0.005301,  0.003593,  0.002179,  0.001183,  0.000575}
						 };

	CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	
	CImage *pImage = pDoc->m_pImage;
	width=pImage->GetWidth();
	height=pImage->GetHeight();

	

	BYTE **bmp = new BYTE* [width];
	
	for (int a=0;a<width;a++)
	{
		bmp[a]= new BYTE[height];
	}

	
	
	for(int j = 0; j < height; j++)         //存入灰度
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			BYTE gray = (ptr[0] + ptr[1] + ptr[2]) / 3;
			bmp[i][j]=gray;
		}
	}
	/*
      for(int j = 0; j < pImage->GetHeight(); j++)
		{
			for(int i = 0; i < pImage->GetWidth(); i++)
			{
				sum=0;
				tempsum=0;
				weightsum=0;
				for(int a=-7;a<=7;a++)
				{
					for(int b=-7;b<=7;b++)
					{
						if((i+a>=0 && j+b>=0) && (i+a<pImage->GetWidth() && j+b<pImage->GetHeight()))
						{
							if((bmp[i][j]>=bmp[i+a][j+b] && bmp[i][j]-bmp[i+a][j+b]<door) || (bmp[i][j]<bmp[i+a][j+b] && bmp[i+a][j+b]-bmp[i][j]<door))
							{
							   intensity=(double)bmp[i+a][j+b];
							   weightsum=weightsum+weight[a+7][b+7];
							   sum=sum+( intensity *weight[a+7][b+7]);
							}
						}					
					}
				}
				BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
				{ptr[0]=sum/weightsum;
				ptr[1]=sum/weightsum;
				ptr[2]=sum/weightsum;}
			}
		}
	
		*/
		for(int j = 0; j < pImage->GetHeight(); j++)
		{
			for(int i = 0; i < pImage->GetWidth(); i++)
			{
				sum=0;
				tempsum=0;
				weightsum=0;
				for(int a=-7;a<=7;a++)
				{
					for(int b=-7;b<=7;b++)
					{
						if((i+a>=0 && j+b>=0) && (i+a<pImage->GetWidth() && j+b<pImage->GetHeight()))
						{
							intensity=(double)bmp[i+a][j+b];
							tempsum=iweight(bmp[i][j],bmp[i+a][j+b],sigma,num);
							weightsum=weightsum+(weight[a+7][b+7] *tempsum);
							sum=sum+( intensity *weight[a+7][b+7] *tempsum);
						}					
					}
				}
				BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
				{ptr[0]=sum/weightsum;
				ptr[1]=sum/weightsum;
				ptr[2]=sum/weightsum;}
			}
		}
	
	
	OnInitialUpdate();

	for (int a=0;a<width;a++)
	{
		delete [] bmp[a];
	}
	delete [] bmp;

}


void IDWT(double **spData, double **spTransData0, double **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	double **spOriginData, **spTransData, **spWvltData;
	/*int iHeight = (int)nHeight /pow(2.0,layer-1), iWidth =(int)nWidth / pow(2.0,layer-1);
	int iHeight_H =(int) nHeight_H / pow(2.0,layer-1), iWidth_H = (int)nWidth_H/ pow(2.0,layer-1);*/
	int hodd[2][6]={{0},{0}},wodd[2][6]={{0},{0}};
	hodd[0][0]=nHeight;
	wodd[0][0]=nWidth;

	for(int i=0;i<5;i++)
	{
		if(hodd[0][i]%2!=0)
		{
			hodd[1][i]=1;
			hodd[0][i+1]=(hodd[0][i]-1)/2;
		}
		else 
		{
			hodd[1][i]=0;
			hodd[0][i+1]=hodd[0][i]/2;
		}
		if(wodd[0][i]%2!=0)
		{
			wodd[1][i]=1;
			wodd[0][i+1]=(wodd[0][i]-1)/2;
		}
		else 
		{
			wodd[1][i]=0;
			wodd[0][i+1]=wodd[0][i]/2;
		}
	}
	double sigma;
	//分配圖像复原所需的內存空間
	
	spOriginData = spData;
	spTransData = spTransData0;
	spWvltData = spTransData1;
	
	//完成圖像小波變換的逆變換
//	CWvltTrans *WTrans;
	/*for(int i = layer; i >= 1; i--)
	{
		if(i==layer)
		{
			sigma=3*varsigma(spWvltData, iHeight_H, iWidth_H);
			bi_filter(spWvltData, spTransData, spOriginData, iHeight, iHeight_H, iWidth, iWidth_H, sigma, 1.414);
				thresholding(spWvltData, iHeight_H, iWidth_H, sigma);
		}
		else
		{
			bi_filter(spWvltData, spTransData, spOriginData, iHeight, iHeight_H, iWidth, iWidth_H, sigma, 1.414);
			sigma=3*varsigma(spWvltData, iHeight_H, iWidth_H);
				thresholding(spWvltData, iHeight_H, iWidth_H, sigma);
		}
		DWTi_Once(spOriginData, spTransData, spWvltData, iHeight, iHeight_H, iWidth, iWidth_H, i, 1.414);
		iHeight <<= 1;		iWidth <<= 1;
		iHeight_H <<= 1;	iWidth_H <<= 1;
	}
	bi_filter(spOriginData, spTransData, spOriginData, iHeight, iHeight_H, iWidth, iWidth_H, sigma, 0);
	lastthresholding(spOriginData, iHeight_H, iWidth_H, sigma);*/
	for(int i = layer; i >= 1; i--)
	{
		if(i==layer)
		{
			sigma=3*varsigma(spWvltData, hodd[0][i], wodd[0][i]);
			bi_filter(spWvltData, spTransData, spOriginData, hodd[0][i]*2, hodd[0][i], wodd[0][i]*2, wodd[0][i], sigma, 1.414);
			thresholding(spWvltData, hodd[0][i], wodd[0][i], sigma);
		}
		else
		{
			bi_filter(spWvltData, spTransData, spOriginData, hodd[0][i]*2, hodd[0][i], wodd[0][i]*2, wodd[0][i], sigma, 1.414);
			sigma=3*varsigma(spWvltData, hodd[0][i], wodd[0][i]);
			thresholding(spWvltData, hodd[0][i], wodd[0][i], sigma);
		}
		DWTi_Once(spOriginData, spTransData, spWvltData, hodd[0][i]*2, hodd[0][i], wodd[0][i]*2, wodd[0][i], i, 1.414);
		
	}
	bi_filter(spOriginData, spTransData, spOriginData, hodd[0][0], hodd[0][0], wodd[0][0], wodd[0][0], sigma, 0);
}

void CImgAppView::On32837()//一階小波轉換
{
		// TODO: 在此加入您的命令處理常式程式碼
	  CImgAppDoc* pDoc = GetDocument();
      pDoc->AddHistory();

      CImage *img = pDoc->m_pImage;//所開的圖設成*img

	  CImage *pImageTemp = new CImage;//開一張新的畫布 pImageTemp
	  fnCopyCImage(pImageTemp, img);//先把img內的值暫存到plmgTemp中

	int x,y;
	unsigned char tempR, tempG, tempB;
	float fTempBufforDisp;//正規化差值
	double MaxPixVal,MinPixVal,Diff;//正規化用

	  double **spOriginData, **spTransData0, **spTransData1;//動態宣告
	
	  //分配圖像小波變換所用的數据空間	
	spOriginData = new double* [img->GetHeight()];
	spTransData0 = new double* [img->GetHeight()];
	spTransData1 = new double* [img->GetHeight()];
	m_WvltCoeff	= new double * [img->GetHeight()];

	for(int i = 0; i < img->GetHeight(); i ++)
	{
		spOriginData[i] = new double [ img->GetWidth()];
		spTransData0[i] = new double [ img->GetWidth()];
		spTransData1[i] = new double [ img->GetWidth()];
		m_WvltCoeff[i] = new double [ img->GetWidth()];//全域變數 因為spTransData1之後會作正規化 改變原本的值 所以先用m_WvltCoeff存起來 作反小波時才能使用

	}

		
	//從設備緩存中獲取原始圖像數据
	for(y=0; y< img->GetHeight(); y++)
	{
		for( x=0; x< img->GetWidth(); x++)
		{
			BYTE *ptr = (BYTE *)img->GetPixelAddress(x, img->GetHeight()-1-y);
     		tempB=ptr[0];
			tempG=ptr[1];
			tempR=ptr[2];
			spOriginData[img->GetHeight()-1-y][x]=(short)(0.3*tempR+0.59*tempG+0.11*tempB);  //no problem //彩色影像轉灰階
			
		}
	}
	//完成一次圖像小波變換
	DWT_Once(spOriginData,spTransData0,spTransData1,img->GetHeight(),img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,1,1.414);//輸入spOriginData 輸出 spTransData1

		//允許圖像复原操作標志
	m_bOnce = TRUE;
	m_bOnce = ~m_bFour & ~m_bTribl & ~m_bTwice & m_bOnce & ~m_bFilter;
	
	//正規化 //判斷最大最小值
	MaxPixVal=spTransData1[0][0];
	MinPixVal=spTransData1[0][0];
	
	for( y=0; y< img->GetHeight(); y++)
	{
		for( x=0; x< img->GetWidth(); x++)
		{
			if(MaxPixVal<spTransData1[y][x])
				MaxPixVal=spTransData1[y][x];
			if(MinPixVal>spTransData1[y][x])
				MinPixVal=spTransData1[y][x];
			m_WvltCoeff[y][x] = spTransData1[y][x];
		}
	}
	Diff=MaxPixVal-MinPixVal;
	
	for(y=0; y< img->GetHeight(); y++)
	{
		for(x=0; x< img->GetWidth(); x++)
		{
			//正規化
          BYTE *ptr1 = (BYTE *)pImageTemp->GetPixelAddress(x,img->GetHeight()-1-y);

		//因為小波變換后的小波系數有可能超過255甚至更多，那么就將
		//小波系數的範圍映射到0~255區間內，以后出現類似的處理，目的都是一樣的
			fTempBufforDisp=spTransData1[img->GetHeight()-1-y][x];//((x-min)/(max-min))*255
			fTempBufforDisp-=MinPixVal;
			fTempBufforDisp*=255;
			fTempBufforDisp/=Diff;
		
			ptr1[0]= (unsigned char)fTempBufforDisp;
			ptr1[1]= (unsigned char)fTempBufforDisp;
			ptr1[2]= (unsigned char)fTempBufforDisp;
		}
	}

	fnCopyCImage(img,pImageTemp);

    delete spOriginData;
	delete spTransData0;
	delete spTransData1;
	
	OnInitialUpdate();
  
 //::AfxMessageBox(_T("end``````"));
}

void CImgAppView::On32838()//二階小波轉換
{
	// TODO: 在此加入您的命令處理常式程式碼
	  CImgAppDoc* pDoc = GetDocument();
      pDoc->AddHistory();

      CImage *img = pDoc->m_pImage;//所開的圖設成*img

	  
	  CImage *pImageTemp = new CImage;//開一張新的畫布 pImageTemp
	  fnCopyCImage(pImageTemp, img);//先把img內的值暫存到plmgTemp中

	int x,y;
	unsigned char tempR, tempG, tempB;
	float fTempBufforDisp;//正規化差值
	double MaxPixVal,MinPixVal,Diff;//正規化用

	 double **spOriginData, **spTransData0, **spTransData1;//動態宣告
	
	  //分配圖像小波變換所用的數据空間	
	spOriginData = new double* [img->GetHeight()];
	spTransData0 = new double* [img->GetHeight()];
	spTransData1 = new double* [img->GetHeight()];
	m_WvltCoeff	= new double * [img->GetHeight()];

	for(int i = 0; i < img->GetHeight(); i ++)
	{
		spOriginData[i] = new double [ img->GetWidth()];
		spTransData0[i] = new double [ img->GetWidth()];
		spTransData1[i] = new double [ img->GetWidth()];
		m_WvltCoeff[i] = new double [ img->GetWidth()];
	}

		
	//從設備緩存中獲取原始圖像數据
	for(y=0; y< img->GetHeight(); y++)
	{
		for( x=0; x< img->GetWidth(); x++)
		{
			BYTE *ptr = (BYTE *)img->GetPixelAddress(x, img->GetHeight()-1-y);
     		tempB=ptr[0];
			tempG=ptr[1];
			tempR=ptr[2];
			spOriginData[img->GetHeight()-1-y][x]=(double)(0.3*tempR+0.59*tempG+0.11*tempB);  //no problem //彩色影像轉灰階
			
		}
	}
	//完成一次圖像小波變換
	DWT_Two(spOriginData,spTransData0,spTransData1,img->GetHeight(),img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,2,1.414);//輸入spOriginData 輸出 spTransData1

		//允許圖像复原操作標志
	m_bTwice = TRUE;
	m_bTwice = ~m_bFour & ~m_bTribl & m_bTwice & ~m_bOnce & ~m_bFilter;
	
	//正規化 //判斷最大最小值
	MaxPixVal=spTransData1[0][0];
	MinPixVal=spTransData1[0][0];
	
	for( y=0; y< img->GetHeight(); y++)
	{
		for( x=0; x< img->GetWidth(); x++)
		{
			if(MaxPixVal<spTransData1[y][x])
				MaxPixVal=spTransData1[y][x];
			if(MinPixVal>spTransData1[y][x])
				MinPixVal=spTransData1[y][x];
			m_WvltCoeff[y][x] = spTransData1[y][x];
		}
	}
	Diff=MaxPixVal-MinPixVal;
	
	for(y=0; y< img->GetHeight(); y++)
	{
		for(x=0; x< img->GetWidth(); x++)
		{
			//正規化
          BYTE *ptr1 = (BYTE *)pImageTemp->GetPixelAddress(x,img->GetHeight()-1-y);

		//因為小波變換后的小波系數有可能超過255甚至更多，那么就將
		//小波系數的範圍映射到0~255區間內，以后出現類似的處理，目的都是一樣的
			fTempBufforDisp=spTransData1[img->GetHeight()-1-y][x];//((x-min)/(max-min))*255
			fTempBufforDisp-=MinPixVal;
			fTempBufforDisp*=255;
			fTempBufforDisp/=Diff;
		
			ptr1[0]= (unsigned char)fTempBufforDisp;
			ptr1[1]= (unsigned char)fTempBufforDisp;
			ptr1[2]= (unsigned char)fTempBufforDisp;
		}
	}


	fnCopyCImage(img,pImageTemp);

    delete spOriginData;
	delete spTransData0;
	delete spTransData1;
	
	OnInitialUpdate();
  
 //::AfxMessageBox(_T("end``````"));
}

void CImgAppView::On32839()//三階小波轉換
{
	 CImgAppDoc* pDoc = GetDocument();
      pDoc->AddHistory();

      CImage *img = pDoc->m_pImage;//所開的圖設成*img

	  
	  CImage *pImageTemp = new CImage;//開一張新的畫布 pImageTemp
	  fnCopyCImage(pImageTemp, img);//先把img內的值暫存到plmgTemp中

	int x,y;
	unsigned char tempR, tempG, tempB;
	float fTempBufforDisp;//正規化差值
	double MaxPixVal,MinPixVal,Diff;//正規化用

	 double **spOriginData, **spTransData0, **spTransData1;//動態宣告
	
	  //分配圖像小波變換所用的數据空間	
	spOriginData = new double* [img->GetHeight()];
	spTransData0 = new double* [img->GetHeight()];
	spTransData1 = new double* [img->GetHeight()];
	m_WvltCoeff	= new double * [img->GetHeight()];

	for(int i = 0; i < img->GetHeight(); i ++)
	{
		spOriginData[i] = new double [ img->GetWidth()];
		spTransData0[i] = new double [ img->GetWidth()];
		spTransData1[i] = new double [ img->GetWidth()];
		m_WvltCoeff[i] = new double [ img->GetWidth()];
	}

		
	//從設備緩存中獲取原始圖像數据
	for(y=0; y< img->GetHeight(); y++)
	{
		for( x=0; x< img->GetWidth(); x++)
		{
			BYTE *ptr = (BYTE *)img->GetPixelAddress(x, img->GetHeight()-1-y);
     		tempB=ptr[0];
			tempG=ptr[1];
			tempR=ptr[2];
			spOriginData[img->GetHeight()-1-y][x]=(double)(0.3*tempR+0.59*tempG+0.11*tempB);  //no problem //彩色影像轉灰階
			
		}
	}
	//完成一次圖像小波變換
	DWT_Tri(spOriginData,spTransData0,spTransData1,img->GetHeight(),img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,3,1.414);//輸入spOriginData 輸出 spTransData1

		//允許圖像复原操作標志
	m_bTribl = TRUE;
	m_bTribl = ~m_bFour & m_bTribl & ~m_bTwice & ~m_bOnce & ~m_bFilter;
	
	//正規化 //判斷最大最小值
	MaxPixVal=spTransData1[0][0];
	MinPixVal=spTransData1[0][0];
	
	for( y=0; y< img->GetHeight(); y++)
	{
		for( x=0; x< img->GetWidth(); x++)
		{
			if(MaxPixVal<spTransData1[y][x])
				MaxPixVal=spTransData1[y][x];
			if(MinPixVal>spTransData1[y][x])
				MinPixVal=spTransData1[y][x];
			m_WvltCoeff[y][x] = spTransData1[y][x];
		}
	}
	Diff=MaxPixVal-MinPixVal;
	
	for(y=0; y< img->GetHeight(); y++)
	{
		for(x=0; x< img->GetWidth(); x++)
		{
			//正規化
          BYTE *ptr1 = (BYTE *)pImageTemp->GetPixelAddress(x,img->GetHeight()-1-y);

		//因為小波變換后的小波系數有可能超過255甚至更多，那么就將
		//小波系數的範圍映射到0~255區間內，以后出現類似的處理，目的都是一樣的
			fTempBufforDisp=spTransData1[img->GetHeight()-1-y][x];//((x-min)/(max-min))*255
			fTempBufforDisp-=MinPixVal;
			fTempBufforDisp*=255;
			fTempBufforDisp/=Diff;
		
			ptr1[0]= (unsigned char)fTempBufforDisp;
			ptr1[1]= (unsigned char)fTempBufforDisp;
			ptr1[2]= (unsigned char)fTempBufforDisp;
		}
	}

	fnCopyCImage(img,pImageTemp);

    delete spOriginData;
	delete spTransData0;
	delete spTransData1;
	
	OnInitialUpdate();
  
 //::AfxMessageBox(_T("end``````"));
}

void CImgAppView::On32840()//反小波轉換
{
 CImgAppDoc* pDoc = GetDocument();
      pDoc->AddHistory();

      CImage *img = pDoc->m_pImage;//所開的圖設成*img

	  
	  CImage *pImageTemp = new CImage;//開一張新的畫布 pImageTemp
	  fnCopyCImage(pImageTemp, img);//先把img內的值暫存到plmgTemp中

	int x,y;

     double **spOriginData, **spTransData0;//, **spTransData1;//動態宣告
	
	  //分配圖像小波變換所用的數据空間	
	spOriginData = new double* [img->GetHeight()];
	spTransData0 = new double* [img->GetHeight()];
	//spTransData1 = new short* [img->GetHeight()];
	
	for(int i = 0; i < img->GetHeight(); i ++)
	{
		spOriginData[i] = new double [ img->GetWidth()];
		spTransData0[i] = new double [ img->GetWidth()];
		//spTransData1[i] = new short [ img->GetWidth()];
		
	}

		
	//如果圖像只經過一次小波變換，則進行相應的逆變換
	if(m_bOnce)
	{
		IDWT(spOriginData, spTransData0, m_WvltCoeff,img->GetHeight(),img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,1,1.414);//輸入m_WvltCoeff 輸出spOriginData
		m_bOnce = FALSE;
		delete m_WvltCoeff;
	}
	//如果圖像只經過兩次小波變換，則進行相應的逆變換
	else if(m_bTwice)
	{
		IDWT(spOriginData, spTransData0, m_WvltCoeff,img->GetHeight(),img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,2,1.414);
		m_bTwice = FALSE;
		delete m_WvltCoeff;
	}
	//如果圖像只經過三次小波變換，則進行相應的逆變換
	else if(m_bTribl)
	{
		IDWT(spOriginData, spTransData0, m_WvltCoeff, img->GetHeight(), img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,3,1.414);
		m_bTribl = FALSE;
		delete m_WvltCoeff;
	}
	else if(m_bFour)
	{
		IDWT(spOriginData, spTransData0, m_WvltCoeff, img->GetHeight(), img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,4,1.414);
		m_bFour = FALSE;
		delete m_WvltCoeff;
	}
	//判斷是否有小波系數可以進行圖像的复原
	else if(m_bFilter)
	{
	//	MessageBoxA(NULL,_T("沒有找到可用小波系數，或者是未做小波變換"),_T("Message"),MB_ICONEXCLAMATION|MB_OK);
		m_bFilter = FALSE;
	}
	//將圖像數据放入顯示緩存中
	for(y=0; y<(int)img->GetHeight(); y++)
	{
		for(x=0; x<(int)img->GetWidth(); x++)
		{
			BYTE *ptr1 = (BYTE *)pImageTemp->GetPixelAddress(x,img->GetHeight()-1-y);

			//cur= y*biAlign+3*x;		//current pixel	
			ptr1[0]	= (unsigned char)spOriginData[img->GetHeight() - 1- y][x];
			ptr1[1]= (unsigned char)spOriginData[img->GetHeight() - 1 - y][x];
			ptr1[2]= (unsigned char)spOriginData[img->GetHeight() - 1 - y][x];
		}
	}
       fnCopyCImage(img,pImageTemp);

	//顯示圖像复原的結果
	//UpdateAllViews(NULL);
	//刪除臨時的數据內存空間
	delete spOriginData;
	delete spTransData0;
	//delete spTransData1;
		OnInitialUpdate();
  
 //::AfxMessageBox(_T("end``````"));
	 
}

void CImgAppView::On32841()//四階小波轉換
{
	// TODO: 在此加入您的命令處理常式程式碼
	  CImgAppDoc* pDoc = GetDocument();
      pDoc->AddHistory();

      CImage *img = pDoc->m_pImage;//所開的圖設成*img

	  
	  CImage *pImageTemp = new CImage;//開一張新的畫布 pImageTemp
	  fnCopyCImage(pImageTemp, img);//先把img內的值暫存到plmgTemp中

	int x,y;
	unsigned char tempR, tempG, tempB;
	float fTempBufforDisp;//正規化差值
	double MaxPixVal,MinPixVal,Diff;//正規化用

	 double **spOriginData, **spTransData0, **spTransData1;//動態宣告
	
	  //分配圖像小波變換所用的數据空間	
	spOriginData = new double* [img->GetHeight()];
	spTransData0 = new double* [img->GetHeight()];
	spTransData1 = new double* [img->GetHeight()];
	m_WvltCoeff	= new double * [img->GetHeight()];

	for(int i = 0; i < img->GetHeight(); i ++)
	{
		spOriginData[i] = new double [ img->GetWidth()];
		spTransData0[i] = new double [ img->GetWidth()];
		spTransData1[i] = new double [ img->GetWidth()];
		m_WvltCoeff[i] = new double [ img->GetWidth()];
	}

		
	//從設備緩存中獲取原始圖像數据
	for(y=0; y< img->GetHeight(); y++)
	{
		for( x=0; x< img->GetWidth(); x++)
		{
			BYTE *ptr = (BYTE *)img->GetPixelAddress(x, img->GetHeight()-1-y);
     		tempB=ptr[0];
			tempG=ptr[1];
			tempR=ptr[2];
			spOriginData[img->GetHeight()-1-y][x]=(double)(0.3*tempR+0.59*tempG+0.11*tempB);  //no problem //彩色影像轉灰階
			
		}
	}
	//完成一次圖像小波變換
	DWT_Four(spOriginData,spTransData0,spTransData1,img->GetHeight(),img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,4,1.414);//輸入spOriginData 輸出 spTransData1

		//允許圖像复原操作標志
	m_bFour = TRUE;
	m_bFour = m_bFour & ~m_bTribl & ~m_bTwice & ~m_bOnce & ~m_bFilter;
	
	//正規化 //判斷最大最小值
	MaxPixVal=spTransData1[0][0];
	MinPixVal=spTransData1[0][0];
	
	for( y=0; y< img->GetHeight(); y++)
	{
		for( x=0; x< img->GetWidth(); x++)
		{
			if(MaxPixVal<spTransData1[y][x])
				MaxPixVal=spTransData1[y][x];
			if(MinPixVal>spTransData1[y][x])
				MinPixVal=spTransData1[y][x];
			m_WvltCoeff[y][x] = spTransData1[y][x];
		}
	}
	Diff=MaxPixVal-MinPixVal;
	
	for(y=0; y< img->GetHeight(); y++)
	{
		for(x=0; x< img->GetWidth(); x++)
		{
			//正規化
          BYTE *ptr1 = (BYTE *)pImageTemp->GetPixelAddress(x,img->GetHeight()-1-y);

		//因為小波變換后的小波系數有可能超過255甚至更多，那么就將
		//小波系數的範圍映射到0~255區間內，以后出現類似的處理，目的都是一樣的
			fTempBufforDisp=spTransData1[img->GetHeight()-1-y][x];//((x-min)/(max-min))*255
			fTempBufforDisp-=MinPixVal;
			fTempBufforDisp*=255;
			fTempBufforDisp/=Diff;
		
			ptr1[0]= (unsigned char)fTempBufforDisp;
			ptr1[1]= (unsigned char)fTempBufforDisp;
			ptr1[2]= (unsigned char)fTempBufforDisp;
		}
	}


	fnCopyCImage(img,pImageTemp);

    delete spOriginData;
	delete spTransData0;
	delete spTransData1;
	
	OnInitialUpdate();
  
}
