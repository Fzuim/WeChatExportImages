
// ExportImagesDlg.h : ͷ�ļ�
//

#pragma once


// CExportImagesDlg �Ի���
class CExportImagesDlg : public CDialogEx
{
// ����
public:
	CExportImagesDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EXPORTIMAGES_DIALOG };
	void doExport();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void FindFilesInDir(const CString& v_cstrPath);
	void DatConverImage(const CString& v_cstrDatPath, const CString& v_cstrImagePath);
	CString GetFileExt(const CString& v_cstrFileName);
	void XOR(BYTE* v_pbyBuf, DWORD v_dwBufLen, BYTE byXOR);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
