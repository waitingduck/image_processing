// ImgApp.h : ImgApp ���ε{�����D���Y��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"       // �D�n�Ÿ�


// CImgAppApp:
// �аѾ\��@�����O�� ImgApp.cpp
//

class CImgAppApp : public CWinApp
{
public:
	CImgAppApp();


// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImgAppApp theApp;