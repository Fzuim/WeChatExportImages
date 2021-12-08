
// ExportImagesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ExportImages.h"
#include "ExportImagesDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IMAGE_PATH_KEYSTR _T("\\FileStorage\\Image")
// CExportImagesDlg 对话框



CExportImagesDlg::CExportImagesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExportImagesDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bStopExport = FALSE;
	m_hExportThread = NULL;
	m_cstrSavePath = _T("");
	m_cstrExportPath = _T("");
	m_cstrFilePath = _T("");
}

void CExportImagesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExportImagesDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEL1, &CExportImagesDlg::OnBnClickedBtnSel1)
	ON_BN_CLICKED(IDC_BTN_SEL2, &CExportImagesDlg::OnBnClickedBtnSel2)
	ON_BN_CLICKED(IDC_BTN_START, &CExportImagesDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CExportImagesDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_SEL3, &CExportImagesDlg::OnBnClickedBtnSel3)
END_MESSAGE_MAP()


// CExportImagesDlg 消息处理程序

BOOL CExportImagesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExportImagesDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExportImagesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CExportImagesDlg::OnBnClickedBtnSel1()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR szPath[MAX_PATH * 2] = { 0 };
	memset(szPath, 0, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("请选择微信文件默认保存目录");
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		CString cstrPath(szPath);
		if (cstrPath.Right(1) == _T("\\")) //!<处理最后个字符是\的bug
		{
			m_cstrSavePath = cstrPath.Left(cstrPath.GetLength() - 1);
		}
		else
		{
			m_cstrSavePath = cstrPath;
		}

		GetDlgItem(IDC_EDIT_SPATH)->SetWindowText(m_cstrSavePath);
	}
}

void CExportImagesDlg::OnBnClickedBtnSel2()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR szPath[MAX_PATH * 2] = { 0 };
	memset(szPath, 0, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("请选择导出图片保存目录");
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		CString cstrPath(szPath);
		if (cstrPath.Right(1) == _T("\\")) //!<处理最后个字符是\的bug
		{
			m_cstrExportPath = cstrPath.Left(cstrPath.GetLength() - 1);
		}
		else
		{
			m_cstrExportPath = cstrPath;
		}

		GetDlgItem(IDC_EDIT_EPATH)->SetWindowText(m_cstrExportPath);
	}
}

///	@brief 导出线程执行体
///	@param 
///	@return 
DWORD WINAPI ExportFunc(LPVOID v_lpParam)
{
	CExportImagesDlg* pThis = (CExportImagesDlg*)v_lpParam;
	pThis->doExport();

	return 0;
}

void CExportImagesDlg::OnBnClickedBtnStart()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_SPATH)->GetWindowText(m_cstrSavePath);
	GetDlgItem(IDC_EDIT_EPATH)->GetWindowText(m_cstrExportPath);
	GetDlgItem(IDC_EDIT_FILE)->GetWindowText(m_cstrFilePath);

	if (_T("") == m_cstrFilePath && (_T("") == m_cstrSavePath || _T("") == m_cstrExportPath))
	{
		AfxMessageBox(_T("请选择相关文件位置!"));
		return;
	}

	m_bStopExport = FALSE;
	m_hExportThread = ::CreateThread(NULL, 0, ExportFunc, this, 0, NULL);

	if (NULL != m_hExportThread)
	{
		CloseHandle(m_hExportThread);
		m_hExportThread = NULL;
	}

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
}

void CExportImagesDlg::OnBnClickedBtnStop()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bStopExport = TRUE;
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}

void CExportImagesDlg::doExport()
{
	if (_T("") != m_cstrFilePath)
	{
		DatConverImage(m_cstrFilePath, m_cstrFilePath);
	}
	else 
	{
		CString cstrImagePath = m_cstrSavePath + IMAGE_PATH_KEYSTR;

		FindFilesInDir(cstrImagePath);

		GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

		CString cstrInfo = _T("");
		cstrInfo.Format(_T("导出完成，请打开文件夹[%s]查看！"), m_cstrExportPath);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(cstrInfo);
	}

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	AfxMessageBox(_T("导出完成！"));
}

///	@brief 目录文件遍历
///	@param v_cstrPath 目录
void CExportImagesDlg::FindFilesInDir(const CString& v_cstrPath)
{
	DWORD dwFileAttr = 0;
	CString cstrDir = _T("");
	CString cstrSonPath = _T("");
	CString cstrImagePath = _T("");
	CString cstrTmp = (_T("\\") == v_cstrPath.Right(1)) ? v_cstrPath.Left(v_cstrPath.GetLength() - 1) : v_cstrPath;
	CString cstrConverInfo = _T("");

	cstrDir.Format(_T("%s\\*.*"), cstrTmp);

	WIN32_FIND_DATA fileinfo;
	HANDLE hFind = ::FindFirstFileW(cstrDir, &fileinfo);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if (m_bStopExport) break;

		if (INVALID_FILE_ATTRIBUTES == fileinfo.dwFileAttributes)
		{
		}
		else if ((fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (_tcscmp(_T("."), fileinfo.cFileName) && _tcscmp(_T(".."), fileinfo.cFileName))
			{ //子目录
				cstrSonPath.Format(_T("%s\\%s"), cstrTmp, fileinfo.cFileName);

				FindFilesInDir(cstrSonPath);
			}
		}
		else
		{
			cstrSonPath.Format(_T("%s\\%s"), cstrTmp, fileinfo.cFileName);

			// 文件导出解析，dat文件转换成image
			if (0 == _tcsicmp(_T(".dat"), GetFileExt(cstrSonPath)))
			{
				int iPos = cstrTmp.Find(IMAGE_PATH_KEYSTR);
				if (-1 != iPos)
				{
					cstrImagePath.Format(_T("%s\\%s"), m_cstrExportPath, cstrTmp.Right(cstrTmp.GetLength() - iPos - _tcslen(IMAGE_PATH_KEYSTR) - 1));
					CreateDirectory(cstrImagePath, NULL);

					cstrImagePath += _T("\\");
					cstrImagePath.Append(fileinfo.cFileName);
				}
				else
				{
					cstrImagePath.Format(_T("%s\\%s"), m_cstrExportPath, fileinfo.cFileName);
				}

				cstrConverInfo.Format(_T("导出:%s"), cstrSonPath);
				GetDlgItem(IDC_STATIC_INFO)->SetWindowText(cstrConverInfo);
				DatConverImage(cstrSonPath, cstrImagePath);
			}
		}
	} while (FindNextFile(hFind, &fileinfo));

	FindClose(hFind);
	hFind = NULL;
}

///	@brief dat文件转换成jpg/gif/png文件格式
///	@param v_cstrDatPath dat文件路径
///	@param v_cstrImagePath 导出图片路径
///	@note dat文件内容为图片文件根据某个值进行异或，这个值直接通过异或jpg/png/gif的文件头两位即可获取
void CExportImagesDlg::DatConverImage(const CString& v_cstrDatPath, const CString& v_cstrImagePath)
{
	BYTE byJPG1 = 0xFF;
	BYTE byJPG2 = 0xD8;
	BYTE byGIF1 = 0x47;
	BYTE byGIF2 = 0x49;
	BYTE byPNG1 = 0x89;
	BYTE byPNG2 = 0x50;

	HANDLE hDatFile = INVALID_HANDLE_VALUE;
	HANDLE hImageFile = INVALID_HANDLE_VALUE;

	CString cstrImageFile(v_cstrImagePath);

	do 
	{
		hDatFile = CreateFile(v_cstrDatPath, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (INVALID_HANDLE_VALUE == hDatFile)
			break;

		// 读取内容前2个字节
		BYTE byBuf[64*1024] = { 0 };
		DWORD dwReadLen = 0;
		BOOL bRet = ReadFile(hDatFile, byBuf, 2, &dwReadLen, NULL);
		if (!bRet || 2 != dwReadLen)
			break;

		// 开始异或判断
		BYTE byJ1 = byJPG1 ^ byBuf[0];
		BYTE byJ2 = byJPG2 ^ byBuf[1];
		BYTE byG1 = byGIF1 ^ byBuf[0];
		BYTE byG2 = byGIF2 ^ byBuf[1];
		BYTE byP1 = byPNG1 ^ byBuf[0];
		BYTE byP2 = byPNG2 ^ byBuf[1];

		// 判断异或值
		BYTE byXOR = 0;
		if (byJ1 == byJ2)
		{
			cstrImageFile += _T(".jpg");
			byXOR = byJ1;
		}
		else if (byG1 == byG2)
		{
			cstrImageFile += _T(".gif");
			byXOR = byG1;
		}
		else if (byP1 == byP2)
		{
			cstrImageFile += _T(".png");
			byXOR = byP1;
		}
		else
			break;

		SetFilePointer(hDatFile, 0, NULL, FILE_BEGIN); // 设置到文件头开始
		hImageFile = CreateFile(cstrImageFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if (INVALID_HANDLE_VALUE == hImageFile)
			break;

		DWORD dwWriteLen = 0;
		do 
		{
			dwReadLen = 0;
			bRet = ReadFile(hDatFile, byBuf, 64 * 1024, &dwReadLen, NULL);
			if (!bRet)
				break;

			XOR(byBuf, dwReadLen, byXOR);

			bRet = WriteFile(hImageFile, byBuf, dwReadLen, &dwWriteLen, NULL);
			if (!bRet || dwReadLen != dwWriteLen)
				break;

		} while (dwReadLen == 64*1024);
	} while (FALSE);

	if (INVALID_HANDLE_VALUE != hDatFile)
	{
		CloseHandle(hDatFile);
		hDatFile = INVALID_HANDLE_VALUE;
	}

	if (INVALID_HANDLE_VALUE != hImageFile)
	{
		CloseHandle(hImageFile);
		hImageFile = INVALID_HANDLE_VALUE;
	}
}

CString CExportImagesDlg::GetFileExt(const CString& v_cstrFileName)
{
	int iLen = v_cstrFileName.GetLength();
	CString cstrTemp = _T("");

	for (int i = iLen - 1; i >= 0; --i)
	{
		if (_T('.') == v_cstrFileName.GetAt(i))
		{
			cstrTemp = v_cstrFileName.Right(iLen - i);
			break;
		}
		else if (_T('\\') == v_cstrFileName.GetAt(i))
		{
			break;
		}
	}

	return cstrTemp;
}

void CExportImagesDlg::XOR(BYTE* v_pbyBuf, DWORD v_dwBufLen, BYTE byXOR)
{
	for (int i = 0; i < v_dwBufLen; i++)
	{
		v_pbyBuf[i] ^= byXOR;
	}
}


void CExportImagesDlg::OnBnClickedBtnSel3()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, _T("Target Files (*.*)|*.*|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		POSITION Position = dlgFile.GetStartPosition();
		if (Position != NULL)
		{
			GetDlgItem(IDC_EDIT_FILE)->SetWindowText(dlgFile.GetNextPathName(Position));
		}
	}
}
