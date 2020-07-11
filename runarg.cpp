#include <dialog.h>


void runargdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BOOL runargdlg::OnInitDialog()
{
    runargdlg::ShowWindow(SW_SHOW);
    runargdlg::RedrawWindow();
    runargdlg::CenterWindow();
	CDialog::OnInitDialog();
	if (runargdlg::IsWindowVisible() != 0)
	{
		path = (CMFCEditBrowseCtrl*)GetDlgItem(IDC_PATH);
		admin = (CButton*)GetDlgItem(IDC_ADMIN);
		if (strcmp(pathprg, "\0"))
		{
			SetDlgItemTextA(this->m_hWnd, IDC_PATH, pathprg);
			path->EnableWindow(FALSE);
		}
	}
	return FALSE;
}
BEGIN_MESSAGE_MAP(runargdlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &runargdlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_INSTALL, &runargdlg::OnBnClickedInstall)
	ON_BN_CLICKED(IDC_UNINSTALL, &runargdlg::OnBnClickedUninstall)
END_MESSAGE_MAP()
BEGIN_MESSAGE_MAP(runargapp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()
void runargdlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	} else
	{
		CDialog::OnPaint();
	}
}
runargdlg::runargdlg(CWnd* pParent)
	: CDialog(IDD_DIALOG1, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}
HCURSOR runargdlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BOOL runargapp::InitInstance()
{
	
	AfxInitRichEdit2();
	CWinApp::InitInstance();
	return FALSE;
}
runargapp::runargapp()
{
	runargapp::InitApplication();
	runargapp::InitInstance();
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		printf("fail\n");
	}
    cmd = CommandLineToArgvA(GetCommandLineA(), &argc);
    if (argc > 1)
    {
        sprintf_s(pathprg, "%s\0", cmd[1]);
    }
	runargdlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		SHELLEXECUTEINFOA shExInfo = { 0 };
		if (check == 1)
		{
			shExInfo.lpVerb = "runas";
		} else
		{
			shExInfo.lpVerb = "";
		}
			
		shExInfo.cbSize = sizeof(shExInfo);
		shExInfo.fMask = SEE_MASK_DEFAULT;
		shExInfo.hwnd = 0;
		shExInfo.lpFile = pathprg;
		shExInfo.lpParameters = args;
		shExInfo.lpDirectory = 0;
		shExInfo.nShow = SW_SHOW;
		shExInfo.hInstApp = 0;
		if (ShellExecuteExA(&shExInfo) == TRUE)
		{
			printf_s("TRUE");
		}
		else
		{
			printf_s("FALSE");
		}
	}
	else if (nResponse == IDCANCEL)
	{
        exit(-1);
	}
	else if (nResponse == -1)
	{
		printf_s("Warning: dialog creation failed, so application is terminating unexpectedly.\n");
	}
	else if (nResponse == IDABORT)
	{
		printf_s("Warning: dialog creation failed, so application is terminating unexpectedly2.\n");
	}
	
}

runargapp theApp;
int main()
{
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return 0;
}
void runargdlg::OnBnClickedOk()
{
	GetDlgItemTextA(this->m_hWnd, IDC_PATH, pathprg, sizeof(pathprg));
	GetDlgItemTextA(this->m_hWnd, IDC_ARGS, args, sizeof(args));
	check = admin->GetCheck();
	CDialog::OnOK();
}
void runargdlg::OnBnClickedInstall()
{
	char path[500] = { '\0' };
	int error;
	sprintf_s(path, "\"%s\" \"%%1\"\0", cmd[0]);
	error = RegCrtKey(HKEY_CLASSES_ROOT, (LPSTR)"*\\Shell\\Run with args\\Command", KEY_ALL_ACCESS | KEY_WOW64_64KEY);
	RegSetKey(HKEY_CLASSES_ROOT, (LPSTR)"*\\Shell\\Run with args\\Command", REG_SZ, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, strlen(path), &path);
}
void runargdlg::OnBnClickedUninstall()
{
	RegDelnode(HKEY_CLASSES_ROOT, (LPCTSTR)L"*\\Shell\\Run with args");
}
