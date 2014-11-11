#pragma once


// CDSetValues 對話方塊

class CDSetValues : public CDialog
{
	DECLARE_DYNAMIC(CDSetValues)

public:
	CDSetValues(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CDSetValues();

// 對話方塊資料
	enum { IDD = IDD_SET_VALUES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	int m_int_1;
	int m_int_2;
	int m_int_3;
	int m_int_4;
	int m_int_5;
	int m_int_6;
	int m_int_7;
	int m_int_8;
	int m_int_9;
	double m_double_1;
	double m_double_2;
	double m_double_3;
	double m_double_4;
	double m_double_5;
	BOOL m_BOOL_1;
	BOOL m_BOOL_2;
	BOOL m_BOOL_3;
	BOOL m_BOOL_4;
	BOOL m_BOOL_5;
	CString m_CString_1;
	CString m_CString_2;
	CString m_CString_3;
};


// CDFileNew 對話方塊

class CDFileNew : public CDialog
{
	DECLARE_DYNAMIC(CDFileNew)

public:
	CDFileNew(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CDFileNew();

// 對話方塊資料
	enum { IDD = IDD_FILE_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()

public:
	int m_nSelect;
	int m_nWidth;
	int m_nHeight;
	CString m_sPath;

public:
	afx_msg void OnBnClickedBrowse();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnBnClickedFileNewRadio1();
	afx_msg void OnBnClickedFileNewRadio2();
	afx_msg void OnEnChangeFileNewEdit1();
};
