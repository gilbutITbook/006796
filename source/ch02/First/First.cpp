
// First.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "First.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "FirstDoc.h"
#include "FirstView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstApp
// ���α׷� ��ü�� �����ϴ� Ŭ���� �̴�.
// ���α׷� ���۽� ���� �ϳ��� �ν��Ͻ��� �����ȴ�.
// ��ť��Ʈ ���ø�(Document Template), ���� ������ ������ ���� �����Ѵ�.

BEGIN_MESSAGE_MAP(CFirstApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CFirstApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CFirstApp ����
// ������ �Լ�
// Ư���� �۾��� ����.
CFirstApp::CFirstApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("First.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CFirstApp ��ü�Դϴ�.

CFirstApp theApp;


// CFirstApp �ʱ�ȭ

BOOL CFirstApp::InitInstance() //���α׷��� �����ϴµ� �ʿ��� ��� �۾�����
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.

	// 
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	// 



	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	// ���� ���α׷��� ���� ���ø��� ����մϴ�.  ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	// ��ť��Ʈ ���ø��� �����ϴ� �ڵ�
	// ��ť��Ʈ ���ø� ������ ���ؼ��� ���ϵ� ������ Ŭ����, ��ť��Ʈ Ŭ����, �� Ŭ���� �ʿ�
	// �̵�Ŭ�������� ���� ���ҽ� ���̵� �ʿ�.

	
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_FirstTYPE,
		RUNTIME_CLASS(CFirstDoc),
		RUNTIME_CLASS(CChildFrame), // ����� ���� MDI �ڽ� �������Դϴ�.
		RUNTIME_CLASS(CFirstView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ������ ��ť��Ʈ ���ø��� AddDocTemplate�Լ��� �̿��ؼ� ���

	// �� MDI ������ â�� ����ϴ�.
	// ���� ������ �����츦 �����ϰ�, �̸� ȭ�鿡 ��Ÿ��

	//���α׷��� ���� ������ ����� CMainFrameŬ������ ���
	// new �����ڸ� �̿��ؼ� CMainFrame Ŭ������ �ν��Ͻ��� ��������
	CMainFrame* pMainFrame = new CMainFrame;
	//LoadFrame �Լ����� IDR_MAINFRAME���ҽ� ������ �̿��ؼ� ������ �����츦 ����
	
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	// ���� ���� ���� ������ ������� ShowWindow�Լ��� �̿��ؼ� ȭ�鿡 ��Ÿ��
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CFirstApp::ExitInstance()//���α׷� ����� �������� ����Ǵ� �Լ�
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CFirstApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
// �ܼ��� ��ȭ ���ڸ� �߿� �ش�. ����.......����

void CFirstApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CFirstApp �޽��� ó����