// ImgAppDoc.cpp : CImgAppDoc ���O����@
//

#include "stdafx.h"
#include "ImgApp.h"
#include "ImgAppDoc.h"
#include "Dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgAppDoc

IMPLEMENT_DYNCREATE(CImgAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CImgAppDoc, CDocument)
END_MESSAGE_MAP()


// CImgAppDoc �غc/�Ѻc

static bool bIsFirstDocument = true;

CImgAppDoc::CImgAppDoc()
{
	m_pHistory = new ImageHistory;
	m_pImage = m_pHistory->m_pInstantHistory->m_pImage;

	if(bIsFirstDocument){
		m_pImage->Create(100, 100, 24, 0);
		fnNegative(m_pImage);
		bIsFirstDocument = false;
	}
	else{
		CDFileNew dlg;
		dlg.DoModal();
		switch(dlg.m_nSelect){
			case 2:
				if(m_pImage->Load(dlg.m_sPath) >= 0) break;
				else;
			default:
				m_pImage->Create(dlg.m_nWidth, dlg.m_nHeight, 24, 0);
				fnNegative(m_pImage);
				break;
		}
	}

	
}

CImgAppDoc::~CImgAppDoc()
{
}

BOOL CImgAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �b���[�J���s��l�Ƶ{���X
	// (SDI ���|���Φ����)

	return TRUE;
}




// CImgAppDoc �ǦC��

void CImgAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �b���[�J�x�s�{���X
	}
	else
	{
		// TODO: �b���[�J���J�{���X
	}
}


// CImgAppDoc �E�_

#ifdef _DEBUG
void CImgAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImgAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImgAppDoc �R�O

int CImgAppDoc::AddHistory()
{
	int nResult = m_pHistory->Add();
	if(nResult != 0) return nResult;
	else m_pImage = this->m_pHistory->m_pInstantHistory->m_pImage;
	return 0;
}