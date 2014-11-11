#include "stdafx.h"
#include "Functions.h"
#include <math.h>
#include <algorithm>
#include <functional>

using namespace std;

int fnCopyCImage(CImage *pDestination, CImage *pSource)
{
	if(pSource->IsNull()) return 1; // �ӷ�Ū�����~

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
*��ƴy�z�G	DWT_Once�����@���Ϲ����p�i�ܴ�										*
*��ưѼơG	short **spOriginData�G�G�����w�A���V��l���Ϲ����u					*
*			short **spTransData0�G�p�i�ܴ��t�ơA�s��@�������ܴ��Z���p�i�t��	*
*			short **spTransData1�G�p�i�ܴ��t�ơA�s��@���ƭ��ܴ��Z���p�i�t��	*
*			int   nHeight		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ������׭�			*
*			int	  nHeight_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����׭Ȫ��@�b		*
*			int   nWidth		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����e�׭�			*
*			int	  nWidth_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ��e�׭Ȫ��@�b		*
*			int   layer			�G�p�i�ܴ����h�ơA�ƭȬ�1�h						*
*			float fRadius		�G�p�i�ܴ��]�l�A�b�եήɭԤw���w�ƭȬ�1.414		*
********************************************************************************/
void DWT_Once(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)//��JspOriginData ��X spTransData1
{
	int Trans_W,				//�Ϲ����y�u����G���
		Trans_H,				//�Ϲ����y�u����G�a����
		Trans_M,				//�Ϲ��x�}���a����
		Trans_N;				//�Ϲ��x�}�����
	double Trans_Coeff0;			//�p�i�ܴ��t��
    signed short Trans_Coeff1;
	fRadius=1.414;				//�ܴ��o�i�t��
	//���Ҷ������ܴ��t�ƪ���Ȫ���
	//���ܴ�,�Ĥ@���]layer=1�ɡ^��nHeight�Y����l�Ϲ������׭�
    for(Trans_H=0; Trans_H<nHeight; Trans_H++)            
	{
		if(layer == 1)//��ܧ@�@�����p�i�ഫ
			 //layer=1�ɡAnWidth_H����l�Ϲ��e�׭Ȫ��@�b
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
                   spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]-128);		//����pixel�ȹL�j?
                    spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]-128);	
				}
	   		}
			//�Y�ܴ��h�Ƥj�_1,�h�Ȫ��˧C�W���p�i�t�� 
		if(layer > 1)//�N��O�@���H�W���p�i�ഫ //�@���H�W�N�����ܴ��o�i�Y��?
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
      //�_���ƭȩM���@�b
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])/2);	//�ۥ[���H2
			//�޿�D�ާ@�Z�ƭȥ[1
			Trans_Coeff1= ~Trans_Coeff1+1;	
			//�t�ƹw��
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//�����@�Ӱ��t�ƪ���ɳB�z

		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])/2);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]+Trans_Coeff1;
		//�����@�ө_�t�ƪ���ɳB�z
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])/4);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//���ɡA��ƨ��ƪ��ܴ�
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])/4);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}//������V���ܴ�����
	//�ݪ���V���ܴ��}�l�A���u���������ܴ��Z���p�i�t��

	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]*=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]/=fRadius;
		}
	}


	
	//�洣�ɦZ�����u�bspTransData0���AspTransData0�������u�۵M�_������
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//�C�ܴ�
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
          Trans_H =Trans_M<<1;
			//�W�aLL����
			spTransData1[Trans_M][Trans_N] = spTransData0[Trans_H][Trans_N];
			//�W�aHL����
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData0[Trans_H+1][Trans_N];
			//�W�aLH����
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N];	
			//�W�aHH����
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H+1][nWidth_H+Trans_N];
		}
		//�Ĥ@�����ɩ_�Ƨ��Шt��
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
		//�ݪ���V���ܴ� 
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
		//��ɳB�z
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]+Trans_Coeff1;

		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])/4);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]+Trans_Coeff0;
		Trans_Coeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])/4);
		//��ɳB�z
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]+Trans_Coeff0;
		//�Ĥ@�����ɰ��Ƨ��Шt��
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])/4);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]+Trans_Coeff0;
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])/4);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]+Trans_Coeff0;
		}
	}
	//�s��p�i�t�ơALL�W�a���t�ƶi��T�ȼW�j�B�z�A�䥦���W�W�a���t�ƫh�d�z��T��
	
	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
               // if(spTransData1[Trans_M][Trans_N/2]>255) spTransData1[Trans_M][Trans_N/2]=255;//�]���������g�L�ۥ[���H�G�άO�۰Ű��H�G���L�{ �i��|���p���I������ �|���~�t��-1��256������ 
				//if(spTransData1[Trans_M][Trans_N/2]<0)   spTransData1[Trans_M][Trans_N/2]=0;
				//if(spTransData1[Trans_M+nHeight_H][Trans_N]>255)spTransData1[Trans_M+nHeight_H][Trans_N]=255;
				//if(spTransData1[Trans_M+nHeight_H][Trans_N]<0)   spTransData1[Trans_M+nHeight_H][Trans_N]=0;

			spTransData1[Trans_M][Trans_N]*=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]/=fRadius;
		}
	}

}
/********************************************************************************
*��ƴy�z�G	DWT_TwoLayers�����⦸�Ϲ����p�i�ܴ�									*	
*��ưѼơG	short **spOriginData�G�G�����w�A���V��l���Ϲ����u					*
*			short **spTransData0�G�p�i�ܴ��t�ơA�s��@�������ܴ��Z���p�i�t��	*
*			short **spTransData1�G�p�i�ܴ��t�ơA�s��@���ƭ��ܴ��Z���p�i�t��	*
*			int   nHeight		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ������׭�			*
*			int	  nHeight_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����׭Ȫ��@�b		*
*			int   nWidth		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����e�׭�			*
*			int	  nWidth_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ��e�׭Ȫ��@�b		*
*			int   layer			�G�p�i�ܴ����h�ơA�ƭȬ�2�h						*
*			float fRadius		�G�p�i�ܴ��]�l�A�b�եήɭԤw���w�ƭȬ�1.414		*
********************************************************************************/

void DWT_Two(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	double **pData, **pTran0, **pTran1;
	//�Ϲ����ݩʰѼ�
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//��o���u�Ŷ������w
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//�Ϲ��ݩʰѼƽ��
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//�Q�δ`�������⦸�p�i�ܴ�
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
*��ƴy�z�G	DWT_TriLayers�����T���Ϲ����p�i�ܴ�									*	
*��ưѼơG	short **spOriginData�G�G�����w�A���V��l���Ϲ����u					*
*			short **spTransData0�G�p�i�ܴ��t�ơA�s��@�������ܴ��Z���p�i�t��	*
*			short **spTransData1�G�p�i�ܴ��t�ơA�s��@���ƭ��ܴ��Z���p�i�t��	*
*			int   nHeight		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ������׭�			*
*			int	  nHeight_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����׭Ȫ��@�b		*
*			int   nWidth		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����e�׭�			*
*			int	  nWidth_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ��e�׭Ȫ��@�b		*
*			int   layer			�G�p�i�ܴ����h��,�ƭȬ�3�h						*
*			float fRadius		�G�p�i�ܴ��]�l�A�b�եήɭԤw���w�ƭȬ�1.414		*
********************************************************************************/
void DWT_Tri(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	double **pData, **pTran0, **pTran1;
		//�Ϲ����ݩʰѼ�
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//��o���u�Ŷ������w
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
//�Ϲ��ݩʰѼƽ��
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
//�Q�δ`�������⦸�p�i�ܴ�
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}
}
/********************************************************************************
*��ƴy�z�G	DWT_FourLayers�����|���Ϲ����p�i�ܴ�									*	
*��ưѼơG	short **spOriginData�G�G�����w�A���V��l���Ϲ����u					*
*			short **spTransData0�G�p�i�ܴ��t�ơA�s��@�������ܴ��Z���p�i�t��	*
*			short **spTransData1�G�p�i�ܴ��t�ơA�s��@���ƭ��ܴ��Z���p�i�t��	*
*			int   nHeight		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ������׭�			*
*			int	  nHeight_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����׭Ȫ��@�b		*
*			int   nWidth		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����e�׭�			*
*			int	  nWidth_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ��e�׭Ȫ��@�b		*
*			int   layer			�G�p�i�ܴ����h��,�ƭȬ�3�h						*
*			float fRadius		�G�p�i�ܴ��]�l�A�b�եήɭԤw���w�ƭȬ�1.414		*
********************************************************************************/
void DWT_Four(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	double **pData, **pTran0, **pTran1;
		//�Ϲ����ݩʰѼ�
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//��o���u�Ŷ������w
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
//�Ϲ��ݩʰѼƽ��
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
//�Q�δ`�������⦸�p�i�ܴ�
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
*��ƴy�z�G	DWTi_Once�����@���Ϲ��p�i�ܴ����f�ܴ�								*	
*��ưѼơG	short **spData		�G�G�����w�A����u�Ŷ��s��p�i�f�ܴ��Z���Ϲ����u*
*			short **spTransData0�G�p�i�ܴ��t�ơA�s��@�������ܴ��Z���p�i�t��	*
*			short **spTransData1�G�p�i�ܴ��t�ơA�s��@���ƭ��ܴ��Z���p�i�t��	*
*			int   nHeight		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ������׭�			*
*			int	  nHeight_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����׭Ȫ��@�b		*
*			int   nWidth		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ����e�׭�			*
*			int	  nWidth_H		�G�Ϲ��ݩʰѼơA�ƭȬ���l�Ϲ��e�׭Ȫ��@�b		*
*			int   layer			�G�p�i�ܴ����h��,�ƭȬ�3�h						*
*			float fRadius		�G�p�i�ܴ��]�l�A�b�եήɭԤw���w�ƭȬ�1.414		*
********************************************************************************/

void DWTi_Once(double **spData, double **spTransData0, double **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	
	int Trans_W,				//�Ϲ����y�u����G���
		Trans_H,				//�Ϲ����y�u����G�a����
		Trans_M,				//�Ϲ��x�}�����
		Trans_N;				//�Ϲ��x�}���a����
	double WvltCoeff0;			//�p�i�ܴ��t��
    signed short WvltCoeff1;

	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]/=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]*=fRadius;
		}
	}
	//�f�ܴ��O�@�Ӱf�L�{�A�ҥH�{�q�ݪ���V�}�l
	//�ݪ���V���f�ܴ��]���ɦ۵M�_�����ǱƦC�^�ATrans_N���Ϲ��x�}���a����
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
	
		//���Ƨ��Ъ��p�i�t�ưf�ܴ��ɪ���ɳB�z
		WvltCoeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])/4);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]-WvltCoeff0;
		WvltCoeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])/4);
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]-WvltCoeff0;
		//�ݪ���V�f�ܴ����ĤG�����Ƨ��Фp�i�t�ƪ��f�ܴ�
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])/4);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]-WvltCoeff0;
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])/4);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]-WvltCoeff0;
		}
		//�ĤG���_�Ƨ��Фp�i�t�ƪ��f�ܴ�
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			WvltCoeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])/2);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]-WvltCoeff1;
			WvltCoeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])/2);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]-WvltCoeff1;
		}
		//�_�Ƨ��Фp�i�t�ưf�ܴ��ɪ���ɳB�z
		WvltCoeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])/2);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]-WvltCoeff1;
		WvltCoeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])/2);
		WvltCoeff1=~WvltCoeff1+1;
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]-WvltCoeff1;
		//���Ҷ������ܴ��t�ƪ���Ȫ��˪��f�ާ@
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			spTransData0[Trans_H][Trans_N] = spTransData1[Trans_M][Trans_N];
			spTransData0[Trans_H+1][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N];
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N];
			spTransData0[Trans_H+1][nWidth_H+Trans_N]= spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N];
		}
	}

	//�h���p�i�ܴ����W�a�t�ƪ��o�i�v��
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]/=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]*=fRadius;
		}
	}
	//�U���i�������V���f�ܴ�
	//������V���f�ܴ�
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		//���Ƨ��Фp�i�t�ưf�ܴ��ɪ���ɳB�z
		WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])/4);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]-WvltCoeff0;
		//�ĤG�����Ƨ��Фp�i�t�ƪ��f�ܴ�
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])/4);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]-WvltCoeff0;
		}
		//�ĤG���_�Ƨ��Фp�i�t�ƪ��f�ܴ�
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			WvltCoeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])/2);
			WvltCoeff1=~WvltCoeff1+1;
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]-WvltCoeff1;
		}
			//�_�Ƨ��Фp�i�t�ưf�ܴ��ɪ���ɳB�z
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
					spTransData0[Trans_H][Trans_N]=spTransData0[Trans_H][Trans_N]+128;//�쥻�O�Ψӭ��W�p�i�ܴ��]�l
					spTransData0[Trans_H][nWidth_H+Trans_N]=spTransData0[Trans_H][nWidth_H+Trans_N]+128;
				}
				//if(spTransData0[Trans_H][Trans_N]>255) spTransData0[Trans_H][Trans_N]=255;
				//if(spTransData0[Trans_H][Trans_N]<0)   spTransData0[Trans_H][Trans_N]=0;
				//if(spTransData0[Trans_H][nWidth_H+Trans_N]>255) spTransData0[Trans_H][nWidth_H+Trans_N]=255;//�]���x�}�j�p�w�Q�ŧi �ҥH�@�⦸
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
	
	int Trans_W,				//�Ϲ����y�u����G���
		Trans_H,				//�Ϲ����y�u����G�a����
		Trans_M,				//�Ϲ��x�}�����
		Trans_N;				//�Ϲ��x�}���a����
	double WvltCoeff0;			//�p�i�ܴ��t��
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