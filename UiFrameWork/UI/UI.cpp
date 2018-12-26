
// UI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "UI.h"
#include "ZTUIMainFrame.h"
#include "ZTUI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIApp

BEGIN_MESSAGE_MAP(CUIApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CUIApp::OnAppAbout)
END_MESSAGE_MAP()


// CUIApp construction

CUIApp::CUIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CUIApp object

CUIApp theApp;


// CUIApp initialization

BOOL CUIApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CZTUIMainFrame* pFrame = new CZTUIMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_ZTUI_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();


	g_ztui->m_pFrame = pFrame;
	
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

void CUIApp::OnAppAbout()
{
}

BOOL CUIApp::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN)
	{
		TRACE("xx Click xx \n");
	}

	return CWinApp::PreTranslateMessage(pMsg);
}
