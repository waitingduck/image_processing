#include "stdafx.h"
#include "ImgApp.h"
#include "ImgAppDoc.h"
#include "ImgAppView.h"
#include "ColorSpace.h"
#include <math.h>
#include <functional>
#include <algorithm>
using namespace std;
int neighborgetp(int,int,int,int);
int ifover(int);
void IDWT(double **spData, double **spTransData0, double **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
/*
	CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	OnInitialUpdate();
*/
void CImgAppView::OnTestFunction0()
{
	// TODO: 灰階
	CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();

	CImage *pImage = pDoc->m_pImage;
    BYTE **pmi = new BYTE* [pImage->GetWidth()];
	

	for(int i = 0; i < pImage->GetWidth(); i++)
		for(int j = 0; j < pImage->GetHeight(); j++){
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			BYTE gray = (ptr[0] + ptr[1] + ptr[2]) / 3;
			ptr[0] = gray; // B
			ptr[1] = gray; // G
			ptr[2] = gray; // R
		}

	OnInitialUpdate();
}

void CImgAppView::OnTestFunction1()
{
	// TODO: 對話盒
	CDSetValues dlg;
	dlg.DoModal();
	for(int i = 0; i < dlg.m_int_1; i++){
		CString str;
		str.Format((CString)"i = %d", i);
		::AfxMessageBox(str);
	}

}

void CImgAppView::OnTestFunction2()
{
	// TODO: 灰度線性轉換
	CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();

	CImage *pImage = pDoc->m_pImage;
    BYTE **pmi = new BYTE* [pImage->GetWidth()];
	
	int x1=120,x2=180,y1=70,y2=230;
	for(int i = 0; i < pImage->GetWidth(); i++)
		for(int j = 0; j < pImage->GetHeight(); j++){
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			BYTE gray = ((ptr[0] + ptr[1] + ptr[2]) / 3);
			if(gray<x1)
			{gray=y1/x1*gray;}
			else if(gray>=x1 && gray<x2)
			{gray=(y2-y1)/(x2-x1)*(gray-x1+y1);}
			else if(gray>=x2)
			{gray=(255-y2)/(255-x2)*(gray-x2)+y2;}
			ptr[0] = gray; // B
			ptr[1] = gray; // G
			ptr[2] = gray; // R
		}

	OnInitialUpdate();
}

void CImgAppView::OnTestFunction3()
{
	// TODO: 負片
	CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();

	fnNegative(pDoc->m_pImage);
	
	OnInitialUpdate();
}

void CImgAppView::OnTestFunction4()
{
	// TODO: 在此加入您的命令處理常式程式碼
	int width,height;
	int setcolor;
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

	for(int j = 0; j < height; j++)         
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			BYTE gray = (ptr[0] + ptr[1] + ptr[2]) / 3;
			bmp[i][j]=gray;
		}
	}

	for(int j = 0; j < height; j++)         
	{
		for(int i = 0; i < width; i++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			if(i <= width/2)
			{
				if(2*i+1 <= width)
				{
					setcolor=(bmp[2*i][j] + bmp[2*i+1][j])/2;
					setcolor=ifover(setcolor);
				}
				ptr[0] = setcolor; // B    
				ptr[1] = setcolor; // G
				ptr[2] = setcolor; // R
			}
			else
			{
				if(i-width/2 <= width)
				{
					setcolor=(bmp[(i-width/2-1)*2][j] - bmp[(i-width/2)*2+1][j])/2;
					setcolor=ifover(setcolor);
				}
				else
				{
					setcolor=bmp[(i-width/2-1)*2][j];
				}
				ptr[0] = setcolor; // B    
				ptr[1] = setcolor; // G
				ptr[2] = setcolor; // R
			}
		}
	}
	
	for (int a=0;a<width;a++)
	{
		delete [] bmp[a];
	}
	delete [] bmp;

	OnInitialUpdate();
}

void CImgAppView::OnTestFunction5()
{
	// TODO: 在此加入您的命令處理常式程式碼
CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	
	CImage *pImage = pDoc->m_pImage;
	BYTE masiker[100][3];

	for(int i = 0; i < pImage->GetWidth(); i++)
	{
		for(int j = 0; j < pImage->GetHeight(); j++)
		{
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			if(j%30==0&&i%10==0)
			{
				masiker[j/30][0]=ptr[0];
				masiker[j/30][1]=ptr[1];
				masiker[j/30][2]=ptr[2];
			}
			ptr[0] = masiker[j/30][0]; // B
			ptr[1] = masiker[j/30][1]; //G
			ptr[2] = masiker[j/30][2]; // R
			
		}
	}
	OnInitialUpdate();

}

void CImgAppView::OnTestFunction6()
{
	// TODO: 模糊
CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	for(int a=0;a<11;a++){
	CImage *pImage = pDoc->m_pImage;
	BYTE **pmi = new BYTE* [pImage->GetWidth()];
	
	for (int a=0;a<pImage->GetWidth();a++)
	{
		pmi[a]= new BYTE[pImage->GetHeight()];
	}
	for(int j = 0; j < pImage->GetHeight(); j++)
		for(int i = 0; i < pImage->GetWidth(); i++){
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			BYTE gray = (ptr[0] + ptr[1] + ptr[2]) / 3;
			pmi[i][j]=gray;
		}
	for(int j = 1; j < pImage->GetHeight()-1; j++)
		for(int i = 1; i < pImage->GetWidth()-1; i++){
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			ptr[0]=(pmi[i-1][j-1]+pmi[i-1][j]+pmi[i-1][j+1]+pmi[i][j-1]+pmi[i][j]+pmi[i][j+1]+pmi[i+1][j-1]+pmi[i+1][j]+pmi[i+1][j+1])/9;
			ptr[1]=ptr[0];
			ptr[2]=ptr[0];
		}
	}
	OnInitialUpdate();

}
int    cmp(const void *a, const void *b)
{    return  (*(double*) a - *(double*) b);        }


void CImgAppView::OnTestFunction7()
{
	// TODO: 在此加入您的命令處理常式程式碼
	int arr_size;
CImgAppDoc* pDoc = GetDocument();
	pDoc->AddHistory();
	
	CImage *pImage = pDoc->m_pImage;
double xx;
	double *pmi = new double [pImage->GetWidth()*pImage->GetHeight()];
	

double *Data = new double [pImage->GetWidth()*pImage->GetHeight()];

	for(int h=0; h<pImage->GetHeight(); h++)
		{
			for(int w=0; w<pImage->GetWidth(); w++)
			{
				BYTE *ptr = (BYTE *)pImage->GetPixelAddress(w, h);
				Data[pImage->GetWidth()*h+w]=((double)(ptr[0] + ptr[1] + ptr[2])) / 3;
			}
		}
	qsort(Data,pImage->GetWidth()*pImage->GetHeight(),sizeof(double), cmp);

		for(int j = 1; j < pImage->GetHeight()-1; j++){
		for(int i = 1; i < pImage->GetWidth()-1; i++){
			BYTE *ptr = (BYTE *)pImage->GetPixelAddress(i, j);
			ptr[0]=Data[pImage->GetWidth()*j+i];
			ptr[1]=Data[pImage->GetWidth()*j+i];
			ptr[2]=Data[pImage->GetWidth()*j+i];
			xx=Data[pImage->GetWidth()*j+i];
		}
	}
	OnInitialUpdate();

}

#include <queue>
void CImgAppView::OnTestFunction8()
{
	OnInitialUpdate();
}


void CImgAppView::OnTestFunction9()
{
	// TODO: 在此加入您的命令處理常式程式碼
	 CImgAppDoc* pDoc = GetDocument();
      pDoc->AddHistory();

      CImage *img = pDoc->m_pImage;//所開的圖設成*img

	  
	  CImage *pImageTemp = new CImage;//開一張新的畫布 pImageTemp
	  fnCopyCImage(pImageTemp, img);//先把img內的值暫存到plmgTemp中

	int x,y;
	unsigned char tempR, tempG, tempB ;
	double transRL,transGA,transBB;
	float fTempBufforDisp;//正規化差值
	double MaxPixVal,MinPixVal,Diff;//正規化用

	 double **spOriginDataRL, **spOriginDataGA, **spOriginDataBB, **spTransData0, **spTransData1;//動態宣告
	
	  //分配圖像小波變換所用的數据空間	
	spOriginDataRL = new double* [img->GetHeight()];
	spOriginDataGA = new double* [img->GetHeight()];
	spOriginDataBB = new double* [img->GetHeight()];
	spTransData0 = new double* [img->GetHeight()];
	spTransData1 = new double* [img->GetHeight()];
	m_WvltCoeff	= new double * [img->GetHeight()];

	for(int i = 0; i < img->GetHeight(); i ++)
	{
		spOriginDataRL[i] = new double [ img->GetWidth()];
		spOriginDataGA[i] = new double [ img->GetWidth()];
		spOriginDataBB[i] = new double [ img->GetWidth()];
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
			RGB_Lab(tempR, tempG, tempB, transRL,transGA, transBB);     //轉成CIELAB
			spOriginDataRL[img->GetHeight()-1-y][x]=(double)(transRL);  
			spOriginDataGA[img->GetHeight()-1-y][x]=(double)(transGA);  
			spOriginDataBB[img->GetHeight()-1-y][x]=(double)(transBB);  
		}
	}
	
	//對L做二階小波
	DWT_Two(spOriginDataRL,spTransData0,spTransData1,img->GetHeight(),img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,2,1.414);//輸入spOriginData 輸出 spTransData1
	m_bTwice = TRUE;
	m_bTwice = ~m_bFour & ~m_bTribl & m_bTwice & ~m_bOnce & ~m_bFilter;
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
	IDWT(spOriginDataRL, spTransData0, m_WvltCoeff, img->GetHeight(), img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,2,1);
	m_bTwice = FALSE;
	
	DWT_Four(spOriginDataGA,spTransData0,spTransData1,img->GetHeight(),img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,4,1.414);//輸入spOriginData 輸出 spTransData1
	m_bFour = TRUE;
	m_bFour = m_bFour & ~m_bTribl & ~m_bTwice & ~m_bOnce & ~m_bFilter;
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
	IDWT(spOriginDataGA, spTransData0, m_WvltCoeff, img->GetHeight(), img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,4,1);
	m_bFour = FALSE;
	

	DWT_Four(spOriginDataBB,spTransData0,spTransData1,img->GetHeight(),img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,4,1.414);//輸入spOriginData 輸出 spTransData1
	m_bFour = TRUE;
	m_bFour = m_bFour & ~m_bTribl & ~m_bTwice & ~m_bOnce & ~m_bFilter;
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
	IDWT(spOriginDataBB, spTransData0, m_WvltCoeff, img->GetHeight(), img->GetHeight()/2,img->GetWidth(),img->GetWidth()/2,4,1);
	m_bFour = FALSE;

	//將圖像數据放入顯示緩存中

	for(y=0; y<(int)img->GetHeight(); y++)
	{
		for(x=0; x<(int)img->GetWidth(); x++)
		{
			BYTE *ptr1 = (BYTE *)pImageTemp->GetPixelAddress(x,img->GetHeight()-1-y);

			Lab_RGB(spOriginDataRL[img->GetHeight()-1-y][x], spOriginDataGA[img->GetHeight()-1-y][x], spOriginDataBB[img->GetHeight()-1-y][x], tempR, tempG, tempB);
			ptr1[0]= (unsigned char)tempB;
			ptr1[1]= (unsigned char)tempG;
			ptr1[2]= (unsigned char)tempR;
		}
	}
       fnCopyCImage(img,pImageTemp);

	delete spOriginDataRL;
	delete spOriginDataGA;
	delete spOriginDataBB;
	delete spTransData0;
	delete spTransData1;
	delete m_WvltCoeff;	

	OnInitialUpdate();

}

int ifover(int color)
{
	if(color > 255)
	{
		color = 255;
	}
	else if(color < 0)
	{
		color = -color;
	}
	return color;
}
