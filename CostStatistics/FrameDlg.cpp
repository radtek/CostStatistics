// FrameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FrameDlg.h"
#include "FrameLeftDlg.h"
#include "FrameRightDlg.h"
#include "CostStatistics.h"


#define  TREE_ID_CODE   1
#define  TREE_ID_TYPE   2

// CFrameDlg �Ի���

IMPLEMENT_DYNAMIC(CFrameDlg, CDialog)

CFrameDlg::CFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrameDlg::IDD, pParent)
{
	m_pZTUIMainFrame = NULL;
}

CFrameDlg::~CFrameDlg()
{
}

void CFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFrameDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	
END_MESSAGE_MAP()



// CFrameDlg ��Ϣ�������

BOOL CFrameDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô���ͼ��
	HICON m_hIcon;//����һ��icon���
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_LOGO/*IDR_MAINFRAME*/);//������Դ�ļ�IDΪIDR_MAINFRAME����Դ
	SetIcon(m_hIcon, TRUE); 
	SetIcon(m_hIcon, FALSE);
	
	g_ztui->m_pMsgHandler = new CMyMessageHandler();
	m_pZTUIMainFrame = g_ztui->InitFrame(this);
	SetCursor(theApp.LoadStandardCursor(IDC_ARROW ));
	
	InitMyChildWnd(); 
	g_ztui->SetLeftWndWidth(LEFT_TREE_WIDTH);

	//��ʼ�����ڱ���
	CString strTitle = "�ɱ�ͳ��ϵͳ";
	SetMyWindowText(strTitle);
	ShowWindow(SW_MAXIMIZE);

	::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOMOVE);
	::SetWindowPos(this->GetSafeHwnd(),HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOMOVE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CFrameDlg::InitMyChildWnd()
{
	//��ര��
	CFrameLeftDlg *pLeft = new CFrameLeftDlg;
	pLeft->Create(CFrameLeftDlg::IDD);
	g_ztui->m_pLeftWnd->AppendWnd(pLeft);

	//�Ҳര��
	CFrameRightDlg *pRight = new CFrameRightDlg;
	pRight->Create(CFrameRightDlg::IDD);
	g_ztui->m_pRightWnd->AddCustomWnd(pRight);

	//��������
	InitLeftToolbar();
	InitRightToolbar();
}

void CFrameDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ( m_pZTUIMainFrame != NULL && m_pZTUIMainFrame->GetSafeHwnd() )
	{
		m_pZTUIMainFrame->MoveWindow(0, 0, cx, cy);
		g_ztui->m_pRightWnd->Refresh();
	}
}

void CFrameDlg::InitLeftToolbar()
{
	ToolbarData  toolbar;
	toolbar.buttons.clear();
	int buttonId = 3000;

	g_ztui->m_pLeftWnd->m_pToolbar->Load(toolbar);
	g_ztui->m_pLeftWnd->m_pToolbar->m_bTabMode=true;
}

void CFrameDlg::InitRightToolbar()
{
	ToolbarData  toolbar;
	toolbar.buttons.clear();
	int buttonId = 10000;

	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"ϵͳ",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"�˺Ź���",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"Ȩ������",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"���ݵ���",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"��Ŀ����",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"��������",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"����",e_TextStatic));

	g_ztui->m_pRightWnd->m_pToolbar->Load(toolbar);
	g_ztui->m_pRightWnd->m_pToolbar->m_bTabMode = true;
}


void CFrameDlg::SetMyWindowText(CString strTitle)
{
	if( this->GetSafeHwnd() == NULL )
	{
		return;
	}

	CString strMyTitle = strTitle;
	this->SetWindowText(strMyTitle);
}


BOOL CFrameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if( pMsg->message == WM_KEYDOWN )
	{
		switch(pMsg-> wParam) 
		{ 
		case   VK_RETURN://���λس� 
			return   TRUE;    
		case   VK_ESCAPE://����Esc 
			return   TRUE;  
		} 
	}

	return __super::PreTranslateMessage(pMsg);
}

void CFrameDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if( ::MessageBox(this->GetSafeHwnd(),"ȷ��Ҫ�˳�ϵͳ��","��ʾ��",MB_YESNO|MB_ICONQUESTION) == IDNO )
	{
		return;
	}
	
	//ǿ���˳�����
	HANDLE hProcess = GetCurrentProcess();
	TerminateProcess(hProcess, 0);

	__super::OnClose();
}