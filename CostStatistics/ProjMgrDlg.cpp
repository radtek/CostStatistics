// ProjMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CostStatistics.h"
#include "ProjMgrDlg.h"
#include "GridHeaderDefine.h"
#include "ProjAddDlg.h"


// CProjMgrDlg 对话框

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


// CProjMgrDlg 消息处理程序

BOOL CProjMgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

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
		"宋体");

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
	// 异常: OCX 属性页应返回 FALSE
}



void CProjMgrDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

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

	m_mapHeader[ IDX_PROJMGR_SEQ	] = GDS::GeneratGCHA( IDX_PROJMGR_SEQ,	TEXT("序号"),60 );
	m_mapHeader[ IDX_PROJMGR_NAME	] = GDS::GeneratGCHA( IDX_PROJMGR_NAME,	TEXT(" 项目号"),80 );
	m_mapHeader[ IDX_PROJMGR_TYPE	] = GDS::GeneratGCHA( IDX_PROJMGR_TYPE,	TEXT("分类"),60 );
	m_mapHeader[ IDX_PROJMGR_MOLD	] = GDS::GeneratGCHA( IDX_PROJMGR_MOLD,	TEXT("项目类型"),80 );
	m_mapHeader[ IDX_PROJMGR_DIAMETER] = GDS::GeneratGCHA( IDX_PROJMGR_DIAMETER,	TEXT("开挖直径(米)"),120 );
	m_mapHeader[ IDX_PROJMGR_SPECIFICATION	] = GDS::GeneratGCHA( IDX_PROJMGR_SPECIFICATION,	TEXT("盾构规格"),100 );
	m_mapHeader[ IDX_PROJMGR_CUSTOMERNAE	] = GDS::GeneratGCHA( IDX_PROJMGR_CUSTOMERNAE,	TEXT("客户单位"),120 );
	m_mapHeader[ IDX_PROJMGR_USEPLACE] = GDS::GeneratGCHA( IDX_PROJMGR_USEPLACE,	TEXT("使用工地"),120 );
	m_mapHeader[ IDX_PROJMGR_DELIVERYPLACE	] = GDS::GeneratGCHA( IDX_PROJMGR_DELIVERYPLACE,	TEXT("交货地点"),150 );
	m_mapHeader[ IDX_PROJMGR_STARTDATE] = GDS::GeneratGCHA( IDX_PROJMGR_STARTDATE,	TEXT("工作任务"),80 );
	m_mapHeader[ IDX_PROJMGR_COMPLETEDATE	] = GDS::GeneratGCHA( IDX_PROJMGR_COMPLETEDATE,	TEXT("合同工期"),80 );
	m_mapHeader[ IDX_PROJMGR_ADJUSTDATE] = GDS::GeneratGCHA( IDX_PROJMGR_ADJUSTDATE,	TEXT("客户调整日期"),120 );
	m_mapHeader[ IDX_PROJMGR_PLANCHECKDATE	] = GDS::GeneratGCHA( IDX_PROJMGR_PLANCHECKDATE,	TEXT("计划自检日期"),120 );
	m_mapHeader[ IDX_PROJMGR_REALCHECKDATE	] = GDS::GeneratGCHA( IDX_PROJMGR_REALCHECKDATE,	TEXT("实际自检日期"),120 );
	m_mapHeader[ IDX_PROJMGR_REALOUTDATE	] = GDS::GeneratGCHA( IDX_PROJMGR_REALOUTDATE,	TEXT("出厂日期"),120 );
	m_mapHeader[ IDX_PROJMGR_MANAGER] = GDS::GeneratGCHA( IDX_PROJMGR_MANAGER,	TEXT("项目经理"),100 );
	m_mapHeader[ IDX_PROJMGR_PROJCHARGE] = GDS::GeneratGCHA( IDX_PROJMGR_PROJCHARGE,	TEXT("项目负责人"),100 );
	m_mapHeader[ IDX_PROJMGR_QUALITYCHARGE	] = GDS::GeneratGCHA( IDX_PROJMGR_QUALITYCHARGE,	TEXT("质检人"),100 );
	m_mapHeader[ IDX_PROJMGR_CUTTERUNIT] = GDS::GeneratGCHA( IDX_PROJMGR_CUTTERUNIT,	TEXT("刀盘制作单位"),150 );
	m_mapHeader[ IDX_PROJMGR_BODYUNIT] = GDS::GeneratGCHA( IDX_PROJMGR_BODYUNIT,	TEXT("盾体厂家"),150 );
	m_mapHeader[ IDX_PROJMGR_PREASSEMBLY	] = GDS::GeneratGCHA( IDX_PROJMGR_PREASSEMBLY,	TEXT("预组装"),100 );
	m_mapHeader[ IDX_PROJMGR_ASSEMBLYLOCATION] = GDS::GeneratGCHA( IDX_PROJMGR_ASSEMBLYLOCATION,	TEXT("总装地点"),100 );
	m_mapHeader[ IDX_PROJMGR_CONTRACTINCOME	] = GDS::GeneratGCHA( IDX_PROJMGR_CONTRACTINCOME,	TEXT("合同收入"),120 );
	m_mapHeader[ IDX_PROJMGR_SPLICE	] = GDS::GeneratGCHA( IDX_PROJMGR_SPLICE,	TEXT("铰接形式"),80 );
	m_mapHeader[ IDX_PROJMGR_DRIVETYPE] = GDS::GeneratGCHA( IDX_PROJMGR_DRIVETYPE,	TEXT("驱动方式"),80 );
	m_mapHeader[ IDX_PROJMGR_WORKSTATE] = GDS::GeneratGCHA( IDX_PROJMGR_WORKSTATE,	TEXT("状态"),80 );
	m_mapHeader[ IDX_PROJMGR_CREATOR] = GDS::GeneratGCHA( IDX_PROJMGR_CREATOR,	TEXT("创建人"),100 );
	m_mapHeader[ IDX_PROJMGR_CREATETIME] = GDS::GeneratGCHA( IDX_PROJMGR_CREATETIME,	TEXT("创建日期"),150 );
	


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

	//设置排序列
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
	// TODO: 在此添加命令处理程序代码
	CProjAddDlg dlg;
	dlg.DoModal();
}

void CProjMgrDlg::OnImport()
{
	// TODO: 在此添加命令处理程序代码
}
