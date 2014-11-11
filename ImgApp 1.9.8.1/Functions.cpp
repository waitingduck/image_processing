#include "stdafx.h"
#include "Functions.h"
#include <math.h>
#include <algorithm>
#include <functional>

using namespace std;

int fnCopyCImage(CImage *pDestination, CImage *pSource)
{
	if(pSource->IsNull()) return 1; // 來源讀取錯誤

	int nBPP = pSource->GetBPP();
	int nWidth = pSource->GetWidth();
	int nHeight = pSource->GetHeight();
	int nBytes = nBPP / 8;
	CImage *img = new CImage;
	img->Create(nWidth, nHeight, nBPP, 0);

	if(nBPP % 8 == 0){
		for(int x = 0; x < nWidth; x++)
			for(int y = 0; y < nHeight; y++){
				BYTE *pSrc = (BYTE *) pSource->GetPixelAddress(x, y);
				BYTE *pDest = (BYTE *) img->GetPixelAddress(x, y);
				for(int i = 0; i < nBytes; i++)
					pDest[i] = pSrc[i];
			}
	}
	else{
		for(int x = 0; x < nWidth; x++)
			for(int y = 0; y < nHeight; y++)
				img->SetPixel(x, y, pSource->GetPixel(x, y));
	}

	*pDestination = *img;

	return 0;
}


bool fnCheckCImageIs24Bits(CImage *pImage)
{
	if(pImage->GetBPP() == 24)
		return true;
	else{
		fnFixImage24Bits(pImage);
		return false;
	}
}


int fnFixImage24Bits(CImage *pImage)
{
	int nBPP = pImage->GetBPP();
	if(nBPP == 24) return 0;

	CImage *img = new CImage;

	img->Create(pImage->GetWidth(), pImage->GetHeight(), 24, 0);
	for(int i = 0; i < pImage->GetWidth(); i++)
		for(int j = 0; j < pImage->GetHeight(); j++)
			img->SetPixel(i, j, pImage->GetPixel(i, j));

	*pImage = *img;

	return 1;
}


bool fnIsEqualImage(CImage *pImageA, CImage *pImageB)
{
	if(pImageA->GetWidth() != pImageB->GetWidth()) return false;
	if(pImageA->GetHeight() != pImageB->GetHeight()) return false;
	if(pImageA->GetBPP() != pImageB->GetBPP()) return false;

	int nWidth = pImageB->GetWidth();
	int nHeight = pImageB->GetHeight();

	int nBytes = pImageA->GetBPP() / 8;

	if(pImageA->GetBPP() % 8 == 0){
		for(int x = 0; x < nWidth; x++)
			for(int y = 0; y < nHeight; y++){
				BYTE *pImgA = (BYTE *) pImageA->GetPixelAddress(x, y);
				BYTE *pImgB = (BYTE *) pImageB->GetPixelAddress(x, y);
				for(int i = 0; i < nBytes; i++)
					if(pImgA[i] != pImgB[i]) return false;
			}
	}
	else{
		for(int x = 0; x < nWidth; x++)
			for(int y = 0; y < nHeight; y++)
				if(pImageA->GetPixel(x, y) != pImageB->GetPixel(x, y))
					return false;
	}

	return true;
}


int fnGray(CImage *pImage)
{
	fnCheckCImageIs24Bits(pImage);

	for(int i = 0; i < pImage->GetWidth(); i++)
		for(int j = 0; j < pImage->GetHeight(); j++){
			BYTE *pPixel = (BYTE *) pImage->GetPixelAddress(i, j);
			BYTE nGray = (pPixel[0] + pPixel[1] + pPixel[2]) / 3;
			pPixel[0] = pPixel[1] = pPixel[2] = nGray;
		}

	return 0;
}


int fnNegative(CImage *pImage)
{
	fnCheckCImageIs24Bits(pImage);

	for(int i = 0; i < pImage->GetWidth(); i++)
		for(int j = 0; j < pImage->GetHeight(); j++){
			BYTE *pPixel = (BYTE *) pImage->GetPixelAddress(i, j);
			pPixel[0] = 255 - pPixel[0];
			pPixel[1] = 255 - pPixel[1];
			pPixel[2] = 255 - pPixel[2];
		}

	return 0;
}


int fnGetRChannel(CImage *pDestination, CImage *pSource)
{
	fnCopyCImage(pDestination, pSource);

	for(int i = 0; i < pSource->GetWidth(); i++)
		for(int j = 0; j < pSource->GetHeight(); j++){
			BYTE *pDest = (BYTE *) pDestination->GetPixelAddress(i, j);
			BYTE *pSrc = (BYTE *) pSource->GetPixelAddress(i, j);
			pDest[2] = pSrc[2];
			pDest[0] = pDest[1] = 0;
		}

	return 0;
}


int fnGetGChannel(CImage *pDestination, CImage *pSource)
{
	fnCopyCImage(pDestination, pSource);

	for(int i = 0; i < pSource->GetWidth(); i++)
		for(int j = 0; j < pSource->GetHeight(); j++){
			BYTE *pDest = (BYTE *) pDestination->GetPixelAddress(i, j);
			BYTE *pSrc = (BYTE *) pSource->GetPixelAddress(i, j);
			pDest[1] = pSrc[1];
			pDest[0] = pDest[2] = 0;
		}

	return 0;
}


int fnGetBChannel(CImage *pDestination, CImage *pSource)
{
	fnCopyCImage(pDestination, pSource);

	for(int i = 0; i < pSource->GetWidth(); i++)
		for(int j = 0; j < pSource->GetHeight(); j++){
			BYTE *pDest = (BYTE *) pDestination->GetPixelAddress(i, j);
			BYTE *pSrc = (BYTE *) pSource->GetPixelAddress(i, j);
			pDest[0] = pSrc[0];
			pDest[1] = pDest[2] = 0;
		}

	return 0;
}


char *fnCStringToChar(CString str)
{
	char *charString = new char[str.GetLength() + 1];
	wcstombs_s(NULL, charString, str.GetLength() + 1, str, _TRUNCATE);
	return charString;
}


void fnSetStatusBar(CString str)
{
	((CMainFrame*)(::AfxGetMainWnd()))->m_wndStatusBar.SetPaneText(0, str, 1);
}


void fnSetStatusBar(CString str, int index)
{
	((CMainFrame*)(::AfxGetMainWnd()))->m_wndStatusBar.SetPaneText(index, str, 1);
}

/********************************************************************************
*函數描述：	DWT_Once完成一次圖像的小波變換										*
*函數參數：	short **spOriginData：二維指針，指向原始的圖像數据					*
*			short **spTransData0：小波變換系數，存放一次水平變換后的小波系數	*
*			short **spTransData1：小波變換系數，存放一次數值變換后的小波系數	*
*			int   nHeight		：圖像屬性參數，數值為原始圖像的高度值			*
*			int	  nHeight_H		：圖像屬性參數，數值為原始圖像高度值的一半		*
*			int   nWidth		：圖像屬性參數，數值為原始圖像的寬度值			*
*			int	  nWidth_H		：圖像屬性參數，數值為原始圖像寬度值的一半		*
*			int   layer			：小波變換的層數，數值為1層						*
*			float fRadius		：小波變換因子，在調用時候已指定數值為1.414		*
********************************************************************************/
void DWT_Once(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)//輸入spOriginData 輸出 spTransData1
{
	int Trans_W,				//圖像掃描線控制：橫坐標
		Trans_H,				//圖像掃描線控制：縱坐標
		Trans_M,				//圖像矩陣的縱坐標
		Trans_N;				//圖像矩陣的橫坐標
	double Trans_Coeff0;			//小波變換系數
    signed short Trans_Coeff1;
	fRadius=1.414;				//變換濾波系數
	//本模塊完成變換系數的賦值采樣
	//行變換,第一次（layer=1時）時nHeight即為原始圖像的高度值
    for(Trans_H=0; Trans_H<nHeight; Trans_H++)            
	{
		if(layer == 1)//表示作一階的小波轉換
			 //layer=1時，nWidth_H為原始圖像寬度值的一半
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)          
			{
				Trans_W=Trans_N<<1;//
	            if (fRadius==2)
				{
					spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]);
                    spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]);
				}
	            else
				{
                   spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]-128);		//防止pixel值過大?
                    spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]-128);	
				}
	   		}
			//若變換層數大于1,則僅采樣低頻的小波系數 
		if(layer > 1)//代表是一階以上的小波轉換 //一階以上就不用變換濾波係數?
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_W=Trans_N<<1;
			spTransData0[Trans_H][Trans_N] = spTransData1[Trans_H][Trans_W];
            spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_H][Trans_W+1];
		}
	}
	
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
      //奇偶數值和的一半
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])/2);	//相加除以2
			//邏輯非操作后數值加1
			Trans_Coeff1= ~Trans_Coeff1+1;	
			//系數預測
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//完成一個偶系數的邊界處理

		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])/2);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]+Trans_Coeff1;
		//完成一個奇系數的邊界處理
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])/4);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//提升，整數到整數的變換
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])/4);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}//水平方向的變換結束
	//豎直方向的變換開始，數据源未水平變換后的小波系數

	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]*=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]/=fRadius;
		}
	}


	
	//行提升后的數据在spTransData0中，spTransData0中的數据自然奇偶有序
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//列變換
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
          Trans_H =Trans_M<<1;
			//頻帶LL部分
			spTransData1[Trans_M][Trans_N] = spTransData0[Trans_H][Trans_N];
			//頻帶HL部分
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData0[Trans_H+1][Trans_N];
			//頻帶LH部分
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N];	
			//頻帶HH部分
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H+1][nWidth_H+Trans_N];
		}
		//第一次提升奇數坐標系數
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
		//豎直方向的變換 
			Trans_Coeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])/2);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]+Trans_Coeff1;
			Trans_Coeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])/2);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+Trans_Coeff1;
		}
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])/2);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]+Trans_Coeff1;
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])/2);
		Trans_Coeff1=~Trans_Coeff1+1;
		//邊界處理
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]+Trans_Coeff1;

		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])/4);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]+Trans_Coeff0;
		Trans_Coeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])/4);
		//邊界處理
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]+Trans_Coeff0;
		//第一次提升偶數坐標系數
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])/4);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]+Trans_Coeff0;
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])/4);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]+Trans_Coeff0;
		}
	}
	//存放小波系數，LL頻帶的系數進行幅值增強處理，其它高頻頻帶的系數則削弱其幅值
	
	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
               // if(spTransData1[Trans_M][Trans_N/2]>255) spTransData1[Trans_M][Trans_N/2]=255;//因為中間有經過相加除以二或是相剪除以二的過程 可能會有小數點的產生 會有誤差到-1或256的情形 
				//if(spTransData1[Trans_M][Trans_N/2]<0)   spTransData1[Trans_M][Trans_N/2]=0;
				//if(spTransData1[Trans_M+nHeight_H][Trans_N]>255)spTransData1[Trans_M+nHeight_H][Trans_N]=255;
				//if(spTransData1[Trans_M+nHeight_H][Trans_N]<0)   spTransData1[Trans_M+nHeight_H][Trans_N]=0;

			spTransData1[Trans_M][Trans_N]*=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]/=fRadius;
		}
	}

}
/********************************************************************************
*函數描述：	DWT_TwoLayers完成兩次圖像的小波變換									*	
*函數參數：	short **spOriginData：二維指針，指向原始的圖像數据					*
*			short **spTransData0：小波變換系數，存放一次水平變換后的小波系數	*
*			short **spTransData1：小波變換系數，存放一次數值變換后的小波系數	*
*			int   nHeight		：圖像屬性參數，數值為原始圖像的高度值			*
*			int	  nHeight_H		：圖像屬性參數，數值為原始圖像高度值的一半		*
*			int   nWidth		：圖像屬性參數，數值為原始圖像的寬度值			*
*			int	  nWidth_H		：圖像屬性參數，數值為原始圖像寬度值的一半		*
*			int   layer			：小波變換的層數，數值為2層						*
*			float fRadius		：小波變換因子，在調用時候已指定數值為1.414		*
********************************************************************************/

void DWT_Two(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	double **pData, **pTran0, **pTran1;
	//圖像的屬性參數
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//獲得數据空間的指針
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//圖像屬性參數賦值
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//利用循環完成兩次小波變換
	int hodd[2][6]={{0},{0}},wodd[2][6]={{0},{0}};
	hodd[0][0]=iHeight;
	wodd[0][0]=iWidth;

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
	}
	
	for(int i=0;i<5;i++)
	{
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

	for(i=1; i<=layer; i++) 
	{
		DWT_Once(pData,pTran0,pTran1,hodd[0][i]*2,hodd[0][i],wodd[0][i]*2,wodd[0][i],i,fr);
		/*DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;*/
	}

}

/********************************************************************************
*函數描述：	DWT_TriLayers完成三次圖像的小波變換									*	
*函數參數：	short **spOriginData：二維指針，指向原始的圖像數据					*
*			short **spTransData0：小波變換系數，存放一次水平變換后的小波系數	*
*			short **spTransData1：小波變換系數，存放一次數值變換后的小波系數	*
*			int   nHeight		：圖像屬性參數，數值為原始圖像的高度值			*
*			int	  nHeight_H		：圖像屬性參數，數值為原始圖像高度值的一半		*
*			int   nWidth		：圖像屬性參數，數值為原始圖像的寬度值			*
*			int	  nWidth_H		：圖像屬性參數，數值為原始圖像寬度值的一半		*
*			int   layer			：小波變換的層數,數值為3層						*
*			float fRadius		：小波變換因子，在調用時候已指定數值為1.414		*
********************************************************************************/
void DWT_Tri(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	double **pData, **pTran0, **pTran1;
		//圖像的屬性參數
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//獲得數据空間的指針
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
//圖像屬性參數賦值
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
//利用循環完成兩次小波變換
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}
}
/********************************************************************************
*函數描述：	DWT_FourLayers完成四次圖像的小波變換									*	
*函數參數：	short **spOriginData：二維指針，指向原始的圖像數据					*
*			short **spTransData0：小波變換系數，存放一次水平變換后的小波系數	*
*			short **spTransData1：小波變換系數，存放一次數值變換后的小波系數	*
*			int   nHeight		：圖像屬性參數，數值為原始圖像的高度值			*
*			int	  nHeight_H		：圖像屬性參數，數值為原始圖像高度值的一半		*
*			int   nWidth		：圖像屬性參數，數值為原始圖像的寬度值			*
*			int	  nWidth_H		：圖像屬性參數，數值為原始圖像寬度值的一半		*
*			int   layer			：小波變換的層數,數值為3層						*
*			float fRadius		：小波變換因子，在調用時候已指定數值為1.414		*
********************************************************************************/
void DWT_Four(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	double **pData, **pTran0, **pTran1;
		//圖像的屬性參數
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//獲得數据空間的指針
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
//圖像屬性參數賦值
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
//利用循環完成兩次小波變換
	int hodd[2][6]={{0},{0}},wodd[2][6]={{0},{0}};
	hodd[0][0]=iHeight;
	wodd[0][0]=iWidth;

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
	for(i=1; i<=layer; i++) 
	{
		DWT_Once(pData,pTran0,pTran1,hodd[0][i]*2,hodd[0][i],wodd[0][i]*2,wodd[0][i],i,fr);
		/*DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;*/
	}

}
/********************************************************************************
*函數描述：	DWTi_Once完成一次圖像小波變換的逆變換								*	
*函數參數：	short **spData		：二維指針，其數据空間存放小波逆變換后的圖像數据*
*			short **spTransData0：小波變換系數，存放一次水平變換后的小波系數	*
*			short **spTransData1：小波變換系數，存放一次數值變換后的小波系數	*
*			int   nHeight		：圖像屬性參數，數值為原始圖像的高度值			*
*			int	  nHeight_H		：圖像屬性參數，數值為原始圖像高度值的一半		*
*			int   nWidth		：圖像屬性參數，數值為原始圖像的寬度值			*
*			int	  nWidth_H		：圖像屬性參數，數值為原始圖像寬度值的一半		*
*			int   layer			：小波變換的層數,數值為3層						*
*			float fRadius		：小波變換因子，在調用時候已指定數值為1.414		*
********************************************************************************/

void DWTi_Once(double **spData, double **spTransData0, double **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	
	int Trans_W,				//圖像掃描線控制：橫坐標
		Trans_H,				//圖像掃描線控制：縱坐標
		Trans_M,				//圖像矩陣的橫坐標
		Trans_N;				//圖像矩陣的縱坐標
	double WvltCoeff0;			//小波變換系數
    signed short WvltCoeff1;

	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]/=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]*=fRadius;
		}
	}
	//逆變換是一個逆過程，所以現從豎直方向開始
	//豎直方向的逆變換（此時自然奇偶有序排列），Trans_N為圖像矩陣的縱坐標
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
	
		//偶數坐標的小波系數逆變換時的邊界處理
		WvltCoeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])/4);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]-WvltCoeff0;
		WvltCoeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])/4);
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]-WvltCoeff0;
		//豎直方向逆變換的第二次偶數坐標小波系數的逆變換
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])/4);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]-WvltCoeff0;
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])/4);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]-WvltCoeff0;
		}
		//第二次奇數坐標小波系數的逆變換
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			WvltCoeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])/2);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]-WvltCoeff1;
			WvltCoeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])/2);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]-WvltCoeff1;
		}
		//奇數坐標小波系數逆變換時的邊界處理
		WvltCoeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])/2);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]-WvltCoeff1;
		WvltCoeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])/2);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]-WvltCoeff1;
		//本模塊完成變換系數的賦值采樣的逆操作
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			spTransData0[Trans_H][Trans_N] = spTransData1[Trans_M][Trans_N];
			spTransData0[Trans_H+1][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N];
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N];
			spTransData0[Trans_H+1][nWidth_H+Trans_N]= spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N];
		}
	}

	//去除小波變換中頻帶系數的濾波影像
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]/=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]*=fRadius;
		}
	}
	//下面進行水平方向的逆變換
	//水平方向的逆變換
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		//偶數坐標小波系數逆變換時的邊界處理
		WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])/4);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]-WvltCoeff0;
		//第二次偶數坐標小波系數的逆變換
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])/4);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]-WvltCoeff0;
		}
		//第二次奇數坐標小波系數的逆變換
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			WvltCoeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])/2);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]-WvltCoeff1;
		}
			//奇數坐標小波系數逆變換時的邊界處理
		WvltCoeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])/2);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]-WvltCoeff1;
		
		if(layer > 1)
		{
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
			{
			Trans_W =Trans_N<<1;
			spTransData1[Trans_H][Trans_W] = spTransData0[Trans_H][Trans_N];
			spTransData1[Trans_H][Trans_W+1] = spTransData0[Trans_H][nWidth_H+Trans_N];
			}
		}
		if(layer == 1)
		{
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
			{
				Trans_W =Trans_N<<1;
				if(fRadius!=2)
				{
					spTransData0[Trans_H][Trans_N]=spTransData0[Trans_H][Trans_N]+128;//原本是用來乘上小波變換因子
					spTransData0[Trans_H][nWidth_H+Trans_N]=spTransData0[Trans_H][nWidth_H+Trans_N]+128;
				}
				//if(spTransData0[Trans_H][Trans_N]>255) spTransData0[Trans_H][Trans_N]=255;
				//if(spTransData0[Trans_H][Trans_N]<0)   spTransData0[Trans_H][Trans_N]=0;
				//if(spTransData0[Trans_H][nWidth_H+Trans_N]>255) spTransData0[Trans_H][nWidth_H+Trans_N]=255;//因為矩陣大小已被宣告 所以作兩次
				//if(spTransData0[Trans_H][nWidth_H+Trans_N]<0)   spTransData0[Trans_H][nWidth_H+Trans_N]=0;
				//   spData[Trans_H][Trans_W] = (unsigned char)spTransData0[Trans_H][Trans_N];
				spData[Trans_H][Trans_W] = spTransData0[Trans_H][Trans_N];
				//    spData[Trans_H][Trans_W+1] = (unsigned char)spTransData0[Trans_H][nWidth_H+Trans_N];
				spData[Trans_H][Trans_W+1] =spTransData0[Trans_H][nWidth_H+Trans_N];
			}
		}

	}


}

double iweight(double x,double y,double sigma ,double num)
{
	double ans;
	double temp;
	double a,b;
	if(x<y)
	{
		temp=x;
		x=y;
		y=temp;
	}
	a=(double)x;
	b=(double)y;
	ans=num*( exp( ( -(a-b) * (a-b) )/ (2*sigma*sigma) ) );
	return ans;
}


void bi_filter(double **spData, double **spTransData0, double **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, double Isigma, float fRadius)
{
	
	int Trans_W,				//圖像掃描線控制：橫坐標
		Trans_H,				//圖像掃描線控制：縱坐標
		Trans_M,				//圖像矩陣的橫坐標
		Trans_N;				//圖像矩陣的縱坐標
	double WvltCoeff0;			//小波變換系數
    signed short WvltCoeff1;
	int width,height;
	int count=0;
	double sum,tempsum,intensity,num,sigma=Isigma,weightsum;
	num=1/(sigma*sqrt(2*3.1415926535897));
	BYTE door=25;
	double weight[15][15]={{  0.000000,  0.000000,  0.000002,  0.000010,  0.000029,  0.000062,  0.000099,  0.000115,  0.000099,  0.000062,  0.000029,  0.000010,  0.000002,  0.000000,  0.000000},
						   {  0.000000,  0.000003,  0.000018,  0.000073,  0.000214,  0.000462,  0.000734,  0.000857,  0.000734,  0.000462,  0.000214,  0.000073,  0.000018,  0.000003,  0.000000},
						   {  0.000002,  0.000018,  0.000099,  0.000396,  0.001167,  0.002524,  0.004010,  0.004679,  0.004010,  0.002524,  0.001167,  0.000396,  0.000099,  0.000018,  0.000002},
						   {  0.000010,  0.000073,  0.000396,  0.001588,  0.004679,  0.010121,  0.016080,  0.018763,  0.016080,  0.010121,  0.004679,  0.001588,  0.000396,  0.000073,  0.000010},
					   	   {  0.000029,  0.000214,  0.001167,  0.004679,  0.013780,  0.029810,  0.047362,  0.055265,  0.047362,  0.029810,  0.013780,  0.004679,  0.001167,  0.000214,  0.000029},
						   {  0.000062,  0.000462,  0.002524,  0.010121,  0.029810,  0.064487,  0.102455,  0.119551,  0.102455,  0.064487,  0.029810,  0.010121,  0.002524,  0.000462,  0.000062},
						   {  0.000099,  0.000734,  0.004010,  0.016080,  0.047362,  0.102455,  0.162778,  0.189940,  0.162778,  0.102455,  0.047362,  0.016080,  0.004010,  0.000734,  0.000099},
						   {  0.000115,  0.000857,  0.004679,  0.018763,  0.055265,  0.119551,  0.189940,  0.221635,  0.189940,  0.119551,  0.055265,  0.018763,  0.004679,  0.000857,  0.000115},
						   {  0.000099,  0.000734,  0.004010,  0.016080,  0.047362,  0.102455,  0.162778,  0.189940,  0.162778,  0.102455,  0.047362,  0.016080,  0.004010,  0.000734,  0.000099},
						   {  0.000062,  0.000462,  0.002524,  0.010121,  0.029810,  0.064487,  0.102455,  0.119551,  0.102455,  0.064487,  0.029810,  0.010121,  0.002524,  0.000462,  0.000062},
						   {  0.000029,  0.000214,  0.001167,  0.004679,  0.013780,  0.029810,  0.047362,  0.055265,  0.047362,  0.029810,  0.013780,  0.004679,  0.001167,  0.000214,  0.000029},
						   {  0.000010,  0.000073,  0.000396,  0.001588,  0.004679,  0.010121,  0.016080,  0.018763,  0.016080,  0.010121,  0.004679,  0.001588,  0.000396,  0.000073,  0.000010},
						   {  0.000002,  0.000018,  0.000099,  0.000396,  0.001167,  0.002524,  0.004010,  0.004679,  0.004010,  0.002524,  0.001167,  0.000396,  0.000099,  0.000018,  0.000002},
						   {  0.000000,  0.000003,  0.000018,  0.000073,  0.000214,  0.000462,  0.000734,  0.000857,  0.000734,  0.000462,  0.000214,  0.000073,  0.000018,  0.000003,  0.000000},
						   {  0.000000,  0.000000,  0.000002,  0.000010,  0.000029,  0.000062,  0.000099,  0.000115,  0.000099,  0.000062,  0.000029,  0.000010,  0.000002,  0.000000,  0.000000}
						 };
	

	double **bmp = new double* [nHeight_H];
	
	for (int a=0;a<nHeight_H;a++)
	{
		bmp[a]= new double[nWidth_H];
	}
	

	for(Trans_H=0; Trans_H<nHeight_H; Trans_H++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			//Trans_W =Trans_N<<1;
			Trans_W =Trans_N;
			bmp[Trans_H][Trans_W]=spData[Trans_H][Trans_W];
			//bmp[Trans_H][Trans_W+1]=spData[Trans_H][Trans_W+1];
		}
		
	}

		for(Trans_H=0; Trans_H<nHeight_H; Trans_H++)
		{
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
			{
				
				sum=0;
				tempsum=0;
				weightsum=0;
				for(int a=-7;a<=7;a++)
				{
					for(int b=-7;b<=7;b++)
					{
						if((Trans_H+a>=0 && Trans_N+b>=0) && (Trans_H+a<nHeight_H && Trans_N+b<nWidth_H))
						{
							intensity=(double)bmp[Trans_H+a][Trans_N+b];
							tempsum=iweight(bmp[Trans_H][Trans_N],bmp[Trans_H+a][Trans_N+b],sigma,num);
							//tempsum=iweight(bmp[Trans_H][Trans_N],bmp[Trans_H+a][Trans_N+b],Lsigma,num);
							weightsum=weightsum+(weight[a+7][b+7] *tempsum);
							sum=sum+( intensity *weight[a+7][b+7] *tempsum);
						}					
					}
				}
				spData[Trans_H][Trans_N]=sum/weightsum;
			}
		}
	
	

	for (int a=0;a<nHeight_H;a++)
	{
		delete [] bmp[a];
	}
	delete [] bmp;
}
int    cmp0(const void *a, const void *b)
{    return  (int)(*(double*) a - *(double*) b);        }
double varsigma(double **spData, int nHeight_H, int nWidth_H)
{
	double sigma;
	int count=-1;
	
	double *Data = new double [nWidth_H*nHeight_H];

	for(int h=0; h<nHeight_H; h++)
		{
			for(int w=0; w<nWidth_H; w++)
			{
				Data[nWidth_H*h+w]=((double)abs(spData[h+nHeight_H][w+nWidth_H]));
			}
		}
	qsort(Data,nWidth_H*nHeight_H,sizeof(double), cmp0);
	
	sigma= Data[nWidth_H*nHeight_H/2]/0.6745;

	
	return sigma;
}

void thresholding(double **spData, int nHeight_H, int nWidth_H, double sigma)
{
	double localsigma[225];
	int sigmacount=0;
	double Lsigma,finalsigma;
	
	for(int h=0; h<nHeight_H*2; h++)
		{
			for(int w=0; w<nWidth_H*2; w++)
			{
				if(h>nHeight_H || w>nWidth_H)
				{
					for(int a=-7;a<=7;a++)
					{
						for(int b=-7;b<=7;b++)
						{
							if((h+a>nHeight_H || w+b>nWidth_H) && (h+a<nHeight_H*2 && w+b<nWidth_H*2) && (h+a>=0 && w+b>=0))
							{
								localsigma[sigmacount]=(double)spData[h+a][w+b];
								sigmacount++;
							}					
						}
					}
				sigmacount=0;
				Lsigma=getlocalsigma(localsigma,sigmacount);
				finalsigma=abs(sigma-Lsigma);
					if(spData[h][w]>-finalsigma && spData[h][w]<0)
					{
						spData[h][w]=0;
					}
					else if(spData[h][w]<-finalsigma)
					{
						spData[h][w]=spData[h][w]+finalsigma;
					}	
					else if(0<spData[h][w] && spData[h][w]<finalsigma)
					{
						spData[h][w]=0;
					}
					else if(spData[h][w]>finalsigma)
					{
						spData[h][w]=spData[h][w]-finalsigma;
					}
				}
			}
		}
}

void lastthresholding(double **spData, int nHeight_H, int nWidth_H, double sigma)
{
	for(int h=0; h<nHeight_H; h++)
		{
			for(int w=0; w<nWidth_H; w++)
			{
					if(spData[h][w]>-sigma && spData[h][w]<0)
					{
						//spData[h][w]=0;
					}
					else if(spData[h][w]<-sigma)
					{
						spData[h][w]=spData[h][w]+sigma;
					}	
					else if(0<spData[h][w] && spData[h][w]<sigma)
					{
						//spData[h][w]=0;
					}
					else if(spData[h][w]>sigma)
					{
						spData[h][w]=spData[h][w]-sigma;
					}
				
			}
		}
}

double getlocalsigma(double *spData,int max)
{
	double sigma;
	int count=-1;
	
	double *Data = new double [max];

	for(int h=0; h<max; h++)
		{
			Data[h]=((double)abs(spData[h]));
		}
	qsort(Data,max,sizeof(double), cmp0);
	
	sigma= (Data[max/2]/0.6745)*5;

	
	return sigma;
}