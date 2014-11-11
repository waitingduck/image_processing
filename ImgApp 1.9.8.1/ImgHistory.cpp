#include "stdafx.h"
#include "ImgHistory.h"
#include "Functions.h"

// ImageHistory ========================================================
ImageHistory::ImageHistory()
{
	m_pInstantHistory = new ImageHistoryEntity;
}


int ImageHistory::Add()
{
	int nResult = m_pInstantHistory->AddHistoryEntity();
	if(nResult != 0) return nResult;
	else m_pInstantHistory = m_pInstantHistory->m_pNext;
	return 0;
}

int ImageHistory::GetCount()
{
	int nCount = 1;
	ImageHistoryEntity *pPtr = m_pInstantHistory;
	while(pPtr->m_pLast != pPtr) pPtr = pPtr->m_pLast;
	while(pPtr->m_pNext != pPtr){
		pPtr = pPtr->m_pNext;
		nCount++;
	}
	return nCount;
}


// ImageHistoryEntity ==================================================
ImageHistoryEntity::ImageHistoryEntity()
{
	m_pImage = new CImage;
	m_pLast = this;
	m_pNext = this;
}


int ImageHistoryEntity::AddHistoryEntity()
{
	if(m_pNext != this) m_pNext->DeleteHistoryEntity(); // 不是最後一個步驟

	ImageHistoryEntity *pNewHistoryEntity = new ImageHistoryEntity;
	pNewHistoryEntity->m_pLast = this;
	m_pNext = pNewHistoryEntity;

	int nResult = fnCopyCImage(pNewHistoryEntity->m_pImage, m_pImage);
	pNewHistoryEntity = NULL;

	return nResult;
}


int ImageHistoryEntity::DeleteHistoryEntity()
{
	if(m_pNext != this) m_pNext->DeleteHistoryEntity(); // 不是最後一個步驟
	else{
		delete m_pImage;
		m_pLast->m_pNext = m_pLast;
		m_pLast = m_pNext = NULL;
		delete this;
	}
	return 0;
}
