#include "stdafx.h"
#include "ImgApp.h"
#include "Dialog.h"

#include <atlimage.h>
using namespace std;

// CDSetValues 對話方塊
//=============================================================================

IMPLEMENT_DYNAMIC(CDSetValues, CDialog)

CDSetValues::CDSetValues(CWnd* pParent /*=NULL*/)
	: CDialog(CDSetValues::IDD, pParent)
	, m_int_1(0)
	, m_int_2(0)
	, m_int_3(0)
	, m_int_4(0)
	, m_int_5(0)
	, m_int_6(0)
	, m_int_7(0)
	, m_int_8(0)
	, m_int_9(0)
	, m_double_1(0)
	, m_double_2(0)
	, m_double_3(0)
	, m_double_4(0)
	, m_double_5(0)
	, m_BOOL_1(FALSE)
	, m_BOOL_2(FALSE)
	, m_BOOL_3(FALSE)
	, m_BOOL_4(FALSE)
	, m_BOOL_5(FALSE)
	, m_CString_1(_T(""))
	, m_CString_2(_T(""))
	, m_CString_3(_T(""))
{

}

CDSetValues::~CDSetValues()
{
}

void CDSetValues::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_INT1, m_int_1);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_INT2, m_int_2);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_INT3, m_int_3);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_INT4, m_int_4);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_INT5, m_int_5);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_INT6, m_int_6);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_INT7, m_int_7);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_INT8, m_int_8);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_INT9, m_int_9);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_DOUBLE1, m_double_1);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_DOUBLE2, m_double_2);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_DOUBLE3, m_double_3);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_DOUBLE4, m_double_4);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_DOUBLE5, m_double_5);
	DDX_Check(pDX, IDC_CHECK_SET_VALUES_BOOL1, m_BOOL_1);
	DDX_Check(pDX, IDC_CHECK_SET_VALUES_BOOL2, m_BOOL_2);
	DDX_Check(pDX, IDC_CHECK_SET_VALUES_BOOL3, m_BOOL_3);
	DDX_Check(pDX, IDC_CHECK_SET_VALUES_BOOL4, m_BOOL_4);
	DDX_Check(pDX, IDC_CHECK_SET_VALUES_BOOL5, m_BOOL_5);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_CSTRING1, m_CString_1);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_CSTRING2, m_CString_2);
	DDX_Text(pDX, IDC_EDIT_SET_VALUES_CSTRING3, m_CString_3);
}


BEGIN_MESSAGE_MAP(CDSetValues, CDialog)
END_MESSAGE_MAP()


// CDFileNew 對話方塊
//=============================================================================

IMPLEMENT_DYNAMIC(CDFileNew, CDialog)

CDFileNew::CDFileNew(CWnd* pParent /*=NULL*/)
	: CDialog(CDFileNew::IDD, pParent)
{
	m_nSelect = 1;
	m_nWidth = 100;
	m_nHeight = 100;
	m_sPath = "";
}

CDFileNew::~CDFileNew()
{
}

void CDFileNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_NEW_EDIT1, m_nWidth);
	DDX_Text(pDX, IDC_FILE_NEW_EDIT2, m_nHeight);
	DDX_Text(pDX, IDC_FILE_NEW_EDIT3, m_sPath);
}


BEGIN_MESSAGE_MAP(CDFileNew, CDialog)
	ON_BN_CLICKED(IDC_FILE_NEW_BUTTON1, &CDFileNew::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_FILE_NEW_RADIO1, &CDFileNew::OnBnClickedFileNewRadio1)
	ON_BN_CLICKED(IDC_FILE_NEW_RADIO2, &CDFileNew::OnBnClickedFileNewRadio2)
	ON_EN_CHANGE(IDC_FILE_NEW_EDIT1, &CDFileNew::OnEnChangeFileNewEdit1)
END_MESSAGE_MAP()


// CDFileNew 訊息處理常式

void CDFileNew::OnBnClickedBrowse()
{
	// TODO: 開新檔案 -> 瀏覽
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;
	CImage *pImgTemp = NULL;

	hResult = pImgTemp->GetImporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if(FAILED(hResult)) return; // 副檔名讀取失敗

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	hResult = (int)dlg.DoModal();
	if(FAILED(hResult)) return; // 對話方塊執行失敗

	m_sPath = dlg.GetPathName();
	OnInitDialog();
}

afx_msg BOOL CDFileNew::OnInitDialog()
{
	CButton* radio;
	switch(m_nSelect){
		case 2:
			radio = (CButton*)GetDlgItem(IDC_FILE_NEW_RADIO2);
			radio->SetCheck(BST_CHECKED);
			break;
		default:
			radio = (CButton*)GetDlgItem(IDC_FILE_NEW_RADIO1);
			radio->SetCheck(BST_CHECKED);
			break;
	}

	return CDialog::OnInitDialog();
}

void CDFileNew::OnBnClickedFileNewRadio1()
{
	m_nSelect = 1;

	CEdit* edit1 = (CEdit*)GetDlgItem(IDC_FILE_NEW_EDIT1);
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_FILE_NEW_EDIT2);
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_FILE_NEW_EDIT3);
	CButton *button1 = (CButton*)GetDlgItem(IDC_FILE_NEW_BUTTON1);
	edit1->EnableWindow(1);
	edit2->EnableWindow(1);
	edit3->EnableWindow(0);
	button1->EnableWindow(0);
}

void CDFileNew::OnBnClickedFileNewRadio2()
{
	m_nSelect = 2;

	CEdit* edit1 = (CEdit*)GetDlgItem(IDC_FILE_NEW_EDIT1);
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_FILE_NEW_EDIT2);
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_FILE_NEW_EDIT3);
	CButton *button1 = (CButton*)GetDlgItem(IDC_FILE_NEW_BUTTON1);
	edit1->EnableWindow(0);
	edit2->EnableWindow(0);
	edit3->EnableWindow(1);
	button1->EnableWindow(1);
}

void CDFileNew::OnEnChangeFileNewEdit1()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialog::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}
