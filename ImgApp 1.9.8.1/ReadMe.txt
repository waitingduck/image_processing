================================================================================
    MFC �{���w : ImgApp �M�׷��[
===============================================================================

���ε{�����F�w�g���z�إߤF�o�� ImgApp ���ε{���C�o�����ε{��
���ȥܽd�F�ϥ� MFC ���򥻷����A�]�O�z���g���ε{�����_�I�C

�o���ɮת����e�K�n�A�]�t�U�Ӻc�� ImgApp ���ε{�����ɮסC

ImgApp.vcproj
    �o�O�ϥ����ε{�����F�Ҳ��ͤ� VC++ �M�ת��D�n�M���ɡC 
    ���]�t�����ɮ׮ɪ� Visual C++ ������T�A�H�Ψϥ����ε{�����F�����ɮת�
    �L�{���A��������x�B�պA�M�M�ץ\�઺������T�C

ImgApp.h
    �o�O���ε{�����D�n���Y�ɡC���]�t��L�M�ׯS�w�����Y (�]�A Resource.h)�A
    �ëŧi CImgAppApp ���ε{�����O�C

ImgApp.cpp
    �o�O�D�n�����ε{����l�{���ɡA���]�t���ε{�����O CImgAppApp�C

ImgApp.rc
    �o���M��]�t�F�Ҧ��{���n�ϥΪ� Microsoft Windows �귽�C�䤤�]�A�F�x�s
    �b RES �l�ؿ��U���ϥܡB�I�}�ϩM��СC�z�i�H�����b Microsoft Visual C++ 
    ���s��o���ɮסC�z���M�׸귽�b 1028�C

res\ImgApp.ico
    �o�ӹϥ��ɬO�ΨӰ������ε{�����ϥܡC�o�ӹϥܥ]�t�b�D�n�귽��
    ImgApp.rc ���C

res\ImgApp.rc2
    �o���ɮץ]�t Microsoft Visual C++ �s��L�k�s�誺�귽�C�z���ӱN�Ҧ��귽
    �s�边�L�k�s�誺�귽���o���ɮסC

/////////////////////////////////////////////////////////////////////////////

�D�ج[����:
    �M�קt���з� MFC �����C

MainFrm.h, MainFrm.cpp
    �o���ɮץ]�t�ج[���O CMainFrame�A�Ӯج[���O�l�ͦ� 
    CMDIFrameWnd�A�ñ���Ҧ� MDI �ج[�\��C

res\Toolbar.bmp
    �o���I�}���ɬO�Ψӫإߨñ���ܦb�u��C���v���C
    ��l�u��C�M���A�C�O�b CMainFrame ���O���غc���C
    �z�i�H�ϥθ귽�s�边�A�s��o�Ӥu��C�I�}�ϡA�ӥB�A�z�]�i�H��s
    ImgApp.rc �� IDR_MAINFRAME TOOLBAR �}�C�A�Y�i
    �s�W�u��C�����s�C
/////////////////////////////////////////////////////////////////////////////

�l�ج[����:

ChildFrm.h�BChildFrm.cpp
    �o���ɮשw�q�ù�@ CChildFrame ���O�A�o�����O�䴩 MDI 
    ���ε{�����l�����C

/////////////////////////////////////////////////////////////////////////////

���ε{�����F�إߤ�������M�˵�:

ImgAppDoc.h�BImgAppDoc.cpp - ���
    �o���ɮץ]�t�z�� CImgAppDoc ���O�C�s��o���ɮסA�Y�i�s�W�z���S
    �����ơA�ù�@�ɮ��x�s�M���J (�z�L CImgAppDoc::Serialize)�C

ImgAppView.h�BImgAppView.cpp - ����˵�
    �o���ɮץ]�t�z�� CImgAppView ���O�C
    CImgAppView ����O�Ψ��˵� CImgAppDoc ����C

res\ImgAppDoc.ico
    �o�ӹϥ��ɬO�ΨӰ��� CImgAppDoc ���O�� MDI �l�������ϥܡC
    �o�ӹϥܬO�]�t�b�D�n�귽�� ImgApp.rc ���C




/////////////////////////////////////////////////////////////////////////////

��L�\��:

ActiveX ���
    ���ε{���䴩 ActiveX ������ϥΡC

�C�L�M�w���C�L�䴩
    ���ε{�����F�q MFC �{���w�I�s CView ���O���������禡�A���ͤF�i�H�B�z
    �C�L�B�C�L�]�w�M�w���C�L�R�O���{���X�C

/////////////////////////////////////////////////////////////////////////////

��L�з��ɮ�:

StdAfx.h�BStdAfx.cpp
    �o���ɮ׬O�Ψӫظm�W�� ImgApp.pch ������sĶ���Y (PCH) �ɡA
    �H�ΦW�� StdAfx.obj ������sĶ���O�ɡC

Resource.h
    �o�O�зǼ��Y�ɡA���w�q�s���귽 ID�C
    Microsoft Visual C++ �|Ū���M��s�o���ɮסC

ImgApp.manifest
	���ε{����T�M���ɮ׬O Windows XP �ΨӦb�æs�ե󪺯S�w�����W�A�y�z
	���ε{�����̩ۨʡC���J���|�Q�γo����T�A�q�ե�֨����J�A���ե�A
	�Ϊ̱q���ε{�����J�p�K���_�C���ε{����T�M��i��ΨӰ����~��  
	.manifest �ɪ��ല�o�A�o�� .manifest �ɪ��w�˦�m�A�M���ε{���i������
	����Ƨ��ۦP�A�Ϊ̡A���]�i��H�귽���Φ��]�t�b�i�����ɤ��C 
/////////////////////////////////////////////////////////////////////////////

��L�`�N�ƶ�:

���ε{�����F�ϥ� "TODO:" �ӫ��ܱz���ӥ[�J�Φۭq����l�{���X�����C

�p�G�z�����ε{���ϥ� MFC ���@�� DLL�A�z�N�ݭn�ല�o MFC DLL�C
�p�G�z�����ε{���P�@�~�t�Ϊ��a�ϳ]�w���P�A�z�]�����ല�o������
��a�y�t�Ƹ귽 MFC80XXX.DLL�C�p�G�ݭn�o�ǥD�D���ԲӸ�T�A�Ьd
�\ MSDN ������ല�o Visual C++ ���ε{�������`�C

/////////////////////////////////////////////////////////////////////////////
