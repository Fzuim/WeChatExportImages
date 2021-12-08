
// ExportImagesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ExportImages.h"
#include "ExportImagesDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IMAGE_PATH_KEYSTR _T("\\FileStorage\\Image")
// CExportImagesDlg �Ի���



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


// CExportImagesDlg ��Ϣ�������

BOOL CExportImagesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CExportImagesDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CExportImagesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CExportImagesDlg::OnBnClickedBtnSel1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath[MAX_PATH * 2] = { 0 };
	memset(szPath, 0, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("��ѡ��΢���ļ�Ĭ�ϱ���Ŀ¼");
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		CString cstrPath(szPath);
		if (cstrPath.Right(1) == _T("\\")) //!<���������ַ���\��bug
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath[MAX_PATH * 2] = { 0 };
	memset(szPath, 0, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("��ѡ�񵼳�ͼƬ����Ŀ¼");
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		CString cstrPath(szPath);
		if (cstrPath.Right(1) == _T("\\")) //!<���������ַ���\��bug
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

///	@brief �����߳�ִ����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT_SPATH)->GetWindowText(m_cstrSavePath);
	GetDlgItem(IDC_EDIT_EPATH)->GetWindowText(m_cstrExportPath);
	GetDlgItem(IDC_EDIT_FILE)->GetWindowText(m_cstrFilePath);

	if (_T("") == m_cstrFilePath && (_T("") == m_cstrSavePath || _T("") == m_cstrExportPath))
	{
		AfxMessageBox(_T("��ѡ������ļ�λ��!"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		cstrInfo.Format(_T("������ɣ�����ļ���[%s]�鿴��"), m_cstrExportPath);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(cstrInfo);
	}

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	AfxMessageBox(_T("������ɣ�"));
}

///	@brief Ŀ¼�ļ�����
///	@param v_cstrPath Ŀ¼
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
			{ //��Ŀ¼
				cstrSonPath.Format(_T("%s\\%s"), cstrTmp, fileinfo.cFileName);

				FindFilesInDir(cstrSonPath);
			}
		}
		else
		{
			cstrSonPath.Format(_T("%s\\%s"), cstrTmp, fileinfo.cFileName);

			// �ļ�����������dat�ļ�ת����image
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

				cstrConverInfo.Format(_T("����:%s"), cstrSonPath);
				GetDlgItem(IDC_STATIC_INFO)->SetWindowText(cstrConverInfo);
				DatConverImage(cstrSonPath, cstrImagePath);
			}
		}
	} while (FindNextFile(hFind, &fileinfo));

	FindClose(hFind);
	hFind = NULL;
}

///	@brief dat�ļ�ת����jpg/gif/png�ļ���ʽ
///	@param v_cstrDatPath dat�ļ�·��
///	@param v_cstrImagePath ����ͼƬ·��
///	@note dat�ļ�����ΪͼƬ�ļ�����ĳ��ֵ����������ֱֵ��ͨ�����jpg/png/gif���ļ�ͷ��λ���ɻ�ȡ
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

		// ��ȡ����ǰ2���ֽ�
		BYTE byBuf[64*1024] = { 0 };
		DWORD dwReadLen = 0;
		BOOL bRet = ReadFile(hDatFile, byBuf, 2, &dwReadLen, NULL);
		if (!bRet || 2 != dwReadLen)
			break;

		// ��ʼ����ж�
		BYTE byJ1 = byJPG1 ^ byBuf[0];
		BYTE byJ2 = byJPG2 ^ byBuf[1];
		BYTE byG1 = byGIF1 ^ byBuf[0];
		BYTE byG2 = byGIF2 ^ byBuf[1];
		BYTE byP1 = byPNG1 ^ byBuf[0];
		BYTE byP2 = byPNG2 ^ byBuf[1];

		// �ж����ֵ
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

		SetFilePointer(hDatFile, 0, NULL, FILE_BEGIN); // ���õ��ļ�ͷ��ʼ
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
