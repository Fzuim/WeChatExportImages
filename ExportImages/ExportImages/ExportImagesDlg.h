
// ExportImagesDlg.h : 头文件
//

#pragma once


// CExportImagesDlg 对话框
class CExportImagesDlg : public CDialogEx
{
// 构造
public:
	CExportImagesDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EXPORTIMAGES_DIALOG };
	void doExport();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void FindFilesInDir(const CString& v_cstrPath);
	void DatConverImage(const CString& v_cstrDatPath, const CString& v_cstrImagePath);
	CString GetFileExt(const CString& v_cstrFileName);
	void XOR(BYTE* v_pbyBuf, DWORD v_dwBufLen, BYTE byXOR);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnSel1();
	afx_msg void OnBnClickedBtnSel2();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bStopExport;
	HANDLE m_hExportThread;

	CString m_cstrSavePath;
	CString m_cstrExportPath;
};
