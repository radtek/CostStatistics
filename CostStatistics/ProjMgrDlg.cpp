// ProjMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CostStatistics.h"
#include "ProjMgrDlg.h"
#include "GridHeaderDefine.h"
#include "ProjAddDlg.h"


// CProjMgrDlg �Ի���

IMPLEMENT_DYNAMIC(CProjMgrDlg, CDialog)

CProjMgrDlg::CProjMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjMgrDlg::IDD, pParent)
{

}

CProjMgrDlg::~CProjMgrDlg()
{
}

void CProjMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX,IDC_BTN_ADD,m_BtnAdd);
}


BEGIN_MESSAGE_MAP(CProjMgrDlg, CDialog)
	ON_WM_SIZE()
	//ON_BN_CLICKED(IDC_BTN_ADD, &CProjMgrDlg::OnBnClickedBtnAdd)
	ON_COMMAND(ID_ADD, &CProjMgrDlg::OnAdd)
	ON_COMMAND(ID_IMPORT, &CProjMgrDlg::OnImport)
END_MESSAGE_MAP()


// CProjMgrDlg ��Ϣ�������

BOOL CProjMgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_Font.CreateFont(12, 
		0, 
		0, 
		0, 
		FW_NORMAL, 
		FALSE, 
		FALSE, 
		0, 
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, 
		"����");

	CreateGrid();
	InitHeader();

	Repaint();

	/*m_BtnAdd.SetIcon(IDI_ICON_ADD);
	m_BtnAdd.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(204,232,207)); 
	m_BtnAdd.DrawBorder(FALSE); */

	//ShowWindow(SW_MAXIMIZE);

	m_pMenu = new CMenu() ; 
	m_pMenu->LoadMenu(IDR_MENU_PROJMGR); 
	SetMenu(m_pMenu);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CProjMgrDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	Repaint();
}


void CProjMgrDlg::CreateGrid()
{
	m_Grid.Create(CRect(0, 0, 0, 0), this, GRID_PROJMGR_ID,WS_CHILD | WS_TABSTOP | WS_VISIBLE);       
	m_Grid.SetEditable(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetBkColor(CLR_WHITE);
	m_Grid.SetGridLineColor(CLR_GRID_LINE);
	m_Grid.SetFont(&m_Font);
	m_Grid.SetDoubleBuffering();
	m_Grid.SetListMode();
	m_Grid.EnableAutoWordbreak(false);
	m_Grid.SetHeaderSort(TRUE);
}

void CProjMgrDlg::InitHeader()
{
	BOOL bEditable = FALSE;

	m_mapHeader.clear();

	m_mapHeader[ IDX_PROJMGR_SEQ	] = GDS::GeneratGCHA( IDX_PROJMGR_SEQ,	TEXT("���"),60 );
	m_mapHeader[ IDX_PROJMGR_NAME	] = GDS::GeneratGCHA( IDX_PROJMGR_NAME,	TEXT(" ��Ŀ��"),80 );
	m_mapHeader[ IDX_PROJMGR_TYPE	] = GDS::GeneratGCHA( IDX_PROJMGR_TYPE,	TEXT("����"),60 );
	m_mapHeader[ IDX_PROJMGR_MOLD	] = GDS::GeneratGCHA( IDX_PROJMGR_MOLD,	TEXT("��Ŀ����"),80 );
	m_mapHeader[ IDX_PROJMGR_DIAMETER] = GDS::GeneratGCHA( IDX_PROJMGR_DIAMETER,	TEXT("����ֱ��(��)"),120 );
	m_mapHeader[ IDX_PROJMGR_SPECIFICATION	] = GDS::GeneratGCHA( IDX_PROJMGR_SPECIFICATION,	TEXT("�ܹ����"),100 );
	m_mapHeader[ IDX_PROJMGR_CUSTOMERNAE	] = GDS::GeneratGCHA( IDX_PROJMGR_CUSTOMERNAE,	TEXT("�ͻ���λ"),120 );
	m_mapHeader[ IDX_PROJMGR_USEPLACE] = GDS::GeneratGCHA( IDX_PROJMGR_USEPLACE,	TEXT("ʹ�ù���"),120 );
	m_mapHeader[ IDX_PROJMGR_DELIVERYPLACE	] = GDS::GeneratGCHA( IDX_PROJMGR_DELIVERYPLACE,	TEXT("�����ص�"),150 );
	m_mapHeader[ IDX_PROJMGR_STARTDATE] = GDS::GeneratGCHA( IDX_PROJMGR_STARTDATE,	TEXT("��������"),80 );
	m_mapHeader[ IDX_PROJMGR_COMPLETEDATE	] = GDS::GeneratGCHA( IDX_PROJMGR_COMPLETEDATE,	TEXT("��ͬ����"),80 );
	m_mapHeader[ IDX_PROJMGR_ADJUSTDATE] = GDS::GeneratGCHA( IDX_PROJMGR_ADJUSTDATE,	TEXT("�ͻ���������"),120 );
	m_mapHeader[ IDX_PROJMGR_PLANCHECKDATE	] = GDS::GeneratGCHA( IDX_PROJMGR_PLANCHECKDATE,	TEXT("�ƻ��Լ�����"),120 );
	m_mapHeader[ IDX_PROJMGR_REALCHECKDATE	] = GDS::GeneratGCHA( IDX_PROJMGR_REALCHECKDATE,	TEXT("ʵ���Լ�����"),120 );
	m_mapHeader[ IDX_PROJMGR_REALOUTDATE	] = GDS::GeneratGCHA( IDX_PROJMGR_REALOUTDATE,	TEXT("��������"),120 );
	m_mapHeader[ IDX_PROJMGR_MANAGER] = GDS::GeneratGCHA( IDX_PROJMGR_MANAGER,	TEXT("��Ŀ����"),100 );
	m_mapHeader[ IDX_PROJMGR_PROJCHARGE] = GDS::GeneratGCHA( IDX_PROJMGR_PROJCHARGE,	TEXT("��Ŀ������"),100 );
	m_mapHeader[ IDX_PROJMGR_QUALITYCHARGE	] = GDS::GeneratGCHA( IDX_PROJMGR_QUALITYCHARGE,	TEXT("�ʼ���"),100 );
	m_mapHeader[ IDX_PROJMGR_CUTTERUNIT] = GDS::GeneratGCHA( IDX_PROJMGR_CUTTERUNIT,	TEXT("����������λ"),150 );
	m_mapHeader[ IDX_PROJMGR_BODYUNIT] = GDS::GeneratGCHA( IDX_PROJMGR_BODYUNIT,	TEXT("���峧��"),150 );
	m_mapHeader[ IDX_PROJMGR_PREASSEMBLY	] = GDS::GeneratGCHA( IDX_PROJMGR_PREASSEMBLY,	TEXT("Ԥ��װ"),100 );
	m_mapHeader[ IDX_PROJMGR_ASSEMBLYLOCATION] = GDS::GeneratGCHA( IDX_PROJMGR_ASSEMBLYLOCATION,	TEXT("��װ�ص�"),100 );
	m_mapHeader[ IDX_PROJMGR_CONTRACTINCOME	] = GDS::GeneratGCHA( IDX_PROJMGR_CONTRACTINCOME,	TEXT("��ͬ����"),120 );
	m_mapHeader[ IDX_PROJMGR_SPLICE	] = GDS::GeneratGCHA( IDX_PROJMGR_SPLICE,	TEXT("�½���ʽ"),80 );
	m_mapHeader[ IDX_PROJMGR_DRIVETYPE] = GDS::GeneratGCHA( IDX_PROJMGR_DRIVETYPE,	TEXT("������ʽ"),80 );
	m_mapHeader[ IDX_PROJMGR_WORKSTATE] = GDS::GeneratGCHA( IDX_PROJMGR_WORKSTATE,	TEXT("״̬"),80 );
	m_mapHeader[ IDX_PROJMGR_CREATOR] = GDS::GeneratGCHA( IDX_PROJMGR_CREATOR,	TEXT("������"),100 );
	m_mapHeader[ IDX_PROJMGR_CREATETIME] = GDS::GeneratGCHA( IDX_PROJMGR_CREATETIME,	TEXT("��������"),150 );
	


	int nCol = m_mapHeader.size();


	m_Grid.SetEditable(TRUE);
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(nCol);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFreezedRowCount(1);
	m_Grid.SetFreezedColumnCount(1);
	m_Grid.SetRowHeight(0, SYXK_POS_STD_ROW_HEI + 1);


	int nPriviligeCnt = 0;
	std::vector<int> vecCol;
	for (int col = 0; col < nCol; ++col)
	{
		m_Grid.SetItemText(0, col, m_mapHeader[col].name );

		m_Grid.SetItemText(0, col, m_mapHeader[col].name );
		m_Grid.SetColumnWidth(col, m_mapHeader[col].colLen );
		m_Grid.SetItemBkColour(0, col, CLR_GRID_HEAD_BK);
		m_Grid.SetItemFgColour(0, col, CLR_GRID_HEAD_FG);
		m_Grid.SetItemFormat(HEADER_ROW_INDEX,col,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//if( col != 0 )
		{
			vecCol.push_back(col);
		}
	}

	//����������
	m_Grid.SetTextSortColumns(vecCol);
}

void CProjMgrDlg::Repaint()
{
	CRect rect;
	GetClientRect(&rect);

	if ( m_Grid.GetSafeHwnd() != NULL )
	{
		m_Grid.MoveWindow(0,90,rect.Width(),rect.Height() - 92);
	}
}


void CProjMgrDlg::OnAdd()
{
	// TODO: �ڴ���������������
	CProjAddDlg dlg;
	dlg.DoModal();
}

void CProjMgrDlg::OnImport()
{
	// TODO: �ڴ���������������
}
