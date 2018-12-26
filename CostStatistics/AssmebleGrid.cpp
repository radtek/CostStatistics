#include "stdafx.h"
#include "AssmebleGrid.h"


int CAssmbleGrid::m_nType = 0;
int CAssmbleGrid::m_nColIndex = 0;

CAssmbleGrid::CAssmbleGrid()
{

}

CAssmbleGrid::~CAssmbleGrid()
{

}


void CAssmbleGrid::ResetGridHeader(std::map<int, GDS::GridColHeadAttr>& mapHeader,CGridCtrl& grid)
{
	grid.DeleteAllItems();
	grid.Invalidate();

	int nCol = mapHeader.size();
	if( nCol == 0 )
	{
		return;
	}

	grid.SetEditable(TRUE);
	grid.SetRowCount(1);
	grid.SetColumnCount(nCol);
	grid.SetFixedRowCount(1);
	grid.SetFreezedRowCount(1);
	grid.SetFreezedColumnCount(1);
	grid.SetRowHeight(0, SYXK_POS_STD_ROW_HEI + 1);

	int nPriviligeCnt = 0;
	for (int col = 0; col < nCol; ++col)
	{
		grid.SetItemText(0, col, mapHeader[col].name );
		grid.SetColumnWidth(col, mapHeader[col].colLen );
		grid.SetItemBkColour(0, col, CLR_GRID_HEAD_BK);
		grid.SetItemFgColour(0, col, CLR_GRID_HEAD_FG);
		grid.SetItemFormat(HEADER_ROW_INDEX,col,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}
}


void CAssmbleGrid::SetMerge(int nStartRow,int nEndRow,int nStartCol,int nEndCol,CString strTitle,CGridCtrl& grid)
{
	grid.MergeCells(CCellRange( nStartRow, nStartCol, nEndRow,nEndCol));
	grid.SetItemText(nStartRow,nStartCol,strTitle);

	grid.SetColumnWidth(nStartCol,strTitle.GetLength()* 15); //设置宽度
	grid.GetCell(nStartRow,nStartCol)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX); //设置居中显示
}


void CAssmbleGrid::MergeCellEx(int nMergeRows,CString strL1Title,CString strL2Title,CString strL3Title,CGridCtrl& grid,int nStartCol /*= -1*/)
{
	std::vector<CString> vL1Title;
	std::vector<CString> vL2Title;
	std::vector<CString> vL3Title;

	std::vector<CString> vL3Tmp;

	CCommon::StringToVector(strL1Title,",",vL1Title);
	CCommon::StringToVector(strL2Title,",",vL2Title);

	int nLevel1Col = vL1Title.size();
	int nLevel2Col = vL2Title.size();
	int nLevel3Col = 0;

	CCommon::StringToVector(strL3Title,":",vL3Tmp);
	CString strL3Tmp = "";
	if(vL3Tmp.size() == 2 )
	{
		strL3Tmp = vL3Tmp.at(0);
		CCommon::StringToVector(vL3Tmp.at(1),",",vL3Title);
	}
	nLevel3Col = vL3Title.size();

	if( nStartCol != -1 )
	{
		m_nColIndex = nStartCol;
	}

	int nStartColTmp = m_nColIndex;

	if( nMergeRows == 2 )
	{

	}
	if( nMergeRows == 3 )
	{
		int nMergeTotalColIndex = 0;

		if( nLevel1Col > 0 )
		{
			if( nLevel2Col == 0 && nLevel3Col == 0 )
			{
				nMergeTotalColIndex = nStartColTmp + nLevel1Col - 1;
			}
			else if( nLevel2Col > 0 && nLevel3Col == 0 )
			{
				nMergeTotalColIndex = nStartColTmp + nLevel2Col - 1;
			}
			else if( nLevel2Col > 0 && nLevel3Col > 0 )
			{
				nMergeTotalColIndex = nStartColTmp + nLevel2Col + nLevel3Col - 2;
			}
		}

		if( nLevel1Col == 1 && (nLevel2Col > 0 || nLevel3Col > 0) )  //二级，三级
		{
			//第一行
			grid.MergeCells(CCellRange( 0, nStartColTmp, 0,nMergeTotalColIndex));
			grid.SetItemText(0,nStartColTmp,strL1Title);

			//第二行
			int nIndex = 0;
			int nCurIndex = nStartColTmp;
			for(int i = nStartColTmp;i < (nStartColTmp + nLevel2Col);i++)
			{
				CString strTTmp = vL2Title.at(nIndex++);

				if( strTTmp.Compare(strL3Tmp) == 0 )  //合并第二行单元格
				{
					SetMerge(1,1,i,i + nLevel3Col - 1,strTTmp,grid);
					nCurIndex = nCurIndex + nLevel3Col;
				}
				else //插入第二行数据
				{
					SetMerge(1,2,nCurIndex,nCurIndex,strTTmp,grid);
					nCurIndex++;
				}
			}

			//第三行
			if( nLevel3Col > 0 )
			{
				nIndex = 0;

				for(int i = nStartColTmp;i < (nStartColTmp + nLevel2Col );i++)
				{
					CString strTTmp = vL2Title.at(nIndex++);

					if( strTTmp.Compare(strL3Tmp) == 0 )
					{
						int nIndex2 = 0;
						for(int j = i;j< (i + nLevel3Col);j++)
						{
							SetMerge(2,2,j,j,vL3Title.at(nIndex2++),grid);
						}
					}
				}
			}

			m_nColIndex = nMergeTotalColIndex + 1;
		}
		else //一级
		{
			int nIndex = 0;
			for(int i = nStartColTmp;i< (nStartColTmp + nLevel1Col);i++)
			{
				SetMerge(0,2,i,i,vL1Title.at(nIndex++),grid);
			}

			m_nColIndex = nMergeTotalColIndex + 1;
		}
	}
}


void CAssmbleGrid::InitGrid(int nCol,CGridCtrl& grid)
{
	grid.DeleteAllItems();
	grid.Invalidate();
	grid.SetEditable(TRUE);
	grid.SetRowCount(1);
	grid.SetColumnCount(nCol);
	grid.SetFixedRowCount(1);
	grid.SetFreezedRowCount(1);
	grid.SetFreezedColumnCount(1);
	grid.SetRowHeight(0, SYXK_POS_STD_ROW_HEI + 1);

}

void CAssmbleGrid::ChangeGridHeader(int nType,CGridCtrl& grid)
{
	m_nType = nType;

	std::map<int, GDS::GridColHeadAttr> mapHeader;
	mapHeader.clear();

	if( m_nType == 0 ) //默认页码
	{
		/*int nCol = 52;
		InitGrid(nCol,grid);
		SummaryOfCostGridHead(grid);*/

		grid.DeleteAllItems();
		grid.Invalidate();

		for ( int i = 0 ; i < 21 ; i++ )
		{
			grid.InsertColumn("");
		}
		grid.InsertRow("");
		grid.InsertRow("");

		CAssmbleGrid::MergeCellEx(3,"序号,项目号,分类,合同收入,制造成本,毛利润","","",grid,0);
		CAssmbleGrid::MergeCellEx(3,"毛利率","实际,目标,增(+)减(-)","",grid);
		CAssmbleGrid::MergeCellEx(3,"直接生产成本","小计,工时,职工薪酬,材料费,劳务费","职工薪酬:小计,工时费,人工费",grid);
		CAssmbleGrid::MergeCellEx(3,"制造费用","小计,运输费,驻外基地,直接费用,间接费用","",grid);

		grid.SetFixedRowCount(0);
		grid.SetFixedColumnCount(0);
		grid.SetRowHeight(0,40);
		grid.SetGridLineColor(RGB(30, 30, 30));
		grid.SetEditable(TRUE);

		for ( int i = 0 ;i < grid.GetColumnCount() ; i++)
		{
			for ( int j = 0 ; j < 3 ; j++)
			{
				grid.SetItemState(j,i,GVNI_READONLY);
			}
		}

		return;
	}
	else if( m_nType == 1 )
	{
		
	}
	else if( m_nType == 11 ) //材料费
	{	
		grid.DeleteAllItems();
		grid.Invalidate();

		for ( int i = 0 ; i < 17 ; i++ )
		{
			grid.InsertColumn("");
		}
		grid.InsertRow("");
		grid.InsertRow("");

		CAssmbleGrid::MergeCellEx(3,"序号,项目号,分类","","",grid,0);
		CAssmbleGrid::MergeCellEx(3,"主材","定额,实际,差价","",grid);
		CAssmbleGrid::MergeCellEx(3,"辅材","定额,实际,差价","",grid);
		CAssmbleGrid::MergeCellEx(3,"耗材","定额,实际,差价","",grid);
		CAssmbleGrid::MergeCellEx(3,"总计","","",grid);
		CAssmbleGrid::MergeCellEx(3,"材料费","合计,材料,设计变更","",grid);
		CAssmbleGrid::MergeCellEx(3,"售后费","","",grid);

		grid.SetFixedRowCount(0);
		grid.SetFixedColumnCount(0);
		grid.SetRowHeight(0,40);
		grid.SetGridLineColor(RGB(30, 30, 30));
		grid.SetEditable(TRUE);

		for ( int i = 0 ;i < grid.GetColumnCount() ; i++)
		{
			for ( int j = 0 ; j < 3 ; j++)
			{
				grid.SetItemState(j,i,GVNI_READONLY);
			}
		}

		return;
	}
	else if( m_nType == 12 ) //劳务费
	{
		grid.DeleteAllItems();
		grid.Invalidate();

		for ( int i = 0 ; i < 58 ; i++ )
		{
			grid.InsertColumn("");
		}
		grid.InsertRow("");
		grid.InsertRow("");

		CAssmbleGrid::MergeCellEx(3,"序号,项目号,分类,总计","","",grid,0);
		CAssmbleGrid::MergeCellEx(3,"整机组装","预算,总装(含预组装),总装(不含预组装),小计,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"预组装","预算,小计,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"螺机","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"拼装机","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"流体总成","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"液压胶管","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"配电柜","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"刀盘拼点","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"刀盘焊接","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"刀盘油漆","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"包装","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"装卸","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"油漆","","",grid);
		CAssmbleGrid::MergeCellEx(3,"其他","","",grid);


		grid.SetFixedRowCount(0);
		grid.SetFixedColumnCount(0);
		grid.SetRowHeight(0,40);
		grid.SetGridLineColor(RGB(30, 30, 30));
		grid.SetEditable(TRUE);

		for ( int i = 0 ;i < grid.GetColumnCount() ; i++)
		{
			for ( int j = 0 ; j < 3 ; j++)
			{
				grid.SetItemState(j,i,GVNI_READONLY);
			}
		}

		return;
	}
	else if( m_nType == 13 ) //运输费
	{
		grid.DeleteAllItems();
		grid.Invalidate();

		for ( int i = 0 ; i < 37 ; i++ )
		{
			grid.InsertColumn("");
		}
		grid.InsertRow("");
		grid.InsertRow("");

		CAssmbleGrid::MergeCellEx(3,"序号,项目号,分类,总计","","",grid,0);
		CAssmbleGrid::MergeCellEx(3,"整机","预算,小计,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"盾体","预算,小计,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"管片拼装机","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"主驱动总成","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"主驱动","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"刀盘","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"管片机总成","","",grid);
		CAssmbleGrid::MergeCellEx(3,"倒运费","预算,实际,差额,零星","",grid);
		CAssmbleGrid::MergeCellEx(3,"其他部件","","",grid);
		CAssmbleGrid::MergeCellEx(3,"吊装费","","",grid);
		

		grid.SetFixedRowCount(0);
		grid.SetFixedColumnCount(0);
		grid.SetRowHeight(0,40);
		grid.SetGridLineColor(RGB(30, 30, 30));
		grid.SetEditable(TRUE);

		for ( int i = 0 ;i < grid.GetColumnCount() ; i++)
		{
			for ( int j = 0 ; j < 3 ; j++)
			{
				grid.SetItemState(j,i,GVNI_READONLY);
			}
		}

		return;
	}
	else if( m_nType == 2 )
	{
		mapHeader[ 1	] = GDS::GeneratGCHA( 1,			TEXT("000"),60 );
		mapHeader[ 2	] = GDS::GeneratGCHA( 2,			TEXT("111"),	70  );
		mapHeader[ 3	] = GDS::GeneratGCHA( 3,			TEXT("222"),	170  );
		mapHeader[ 4	] = GDS::GeneratGCHA( 4,			TEXT("333"),	170  );
		mapHeader[ 5	] = GDS::GeneratGCHA( 5,			TEXT("444"),	170  );
		mapHeader[ 6	] = GDS::GeneratGCHA( 6,			TEXT("555"),	170  );
		mapHeader[ 7	] = GDS::GeneratGCHA( 7,			TEXT("666"),	170  );
	}
	else if( m_nType == 4 ) //职工薪酬
	{
		grid.DeleteAllItems();
		grid.Invalidate();

		for ( int i = 0 ; i < 17 ; i++ )
		{
			grid.InsertColumn("");
		}
		grid.InsertRow("");
		grid.InsertRow("");

		CAssmbleGrid::MergeCellEx(3,"序号,项目号,分类","","",grid,0);
		CAssmbleGrid::MergeCellEx(3,"合计,人工费,工时费","","",grid);
		CAssmbleGrid::MergeCellEx(3,"保险统筹","基本养老保险,补充养老保险,基本医疗保险,大额互助医疗保险,失业保险,工伤保险,生育保险,住房公积金,福利费,工会,职工教育","",grid);


		grid.SetFixedRowCount(0);
		grid.SetFixedColumnCount(0);
		grid.SetRowHeight(0,40);
		grid.SetGridLineColor(RGB(30, 30, 30));
		grid.SetEditable(TRUE);

		for ( int i = 0 ;i < grid.GetColumnCount() ; i++)
		{
			for ( int j = 0 ; j < 3 ; j++)
			{
				grid.SetItemState(j,i,GVNI_READONLY);
			}
		}

		return;
	}
	else if( m_nType == 41 )
	{

	}
	else if( m_nType == 411 || //直接人工
		m_nType == 412 ||
		m_nType == 413 ||
		m_nType == 414 ||
		m_nType == 415 ||
		m_nType == 416 ||
		m_nType == 417 ||
		m_nType == 418 ||
		m_nType == 419 )
	{
		grid.DeleteAllItems();
		grid.Invalidate();

		for ( int i = 0 ; i < 22 ; i++ )
		{
			grid.InsertColumn("");
		}
		grid.InsertRow("");
		grid.InsertRow("");

		CAssmbleGrid::MergeCellEx(3,"序号,项目号,分类","","",grid,0);
		CAssmbleGrid::MergeCellEx(3,"合计,人工费,工时费","","",grid);
		CAssmbleGrid::MergeCellEx(3,"保险统筹","基本养老保险,补充养老保险,基本医疗保险,大额互助医疗保险,失业保险,工伤保险,生育保险,住房公积金,福利费,工会,职工教育,分摊后工时,小计,预算,预算分配,零星工时","",grid);


		grid.SetFixedRowCount(0);
		grid.SetFixedColumnCount(0);
		grid.SetRowHeight(0,40);
		grid.SetGridLineColor(RGB(30, 30, 30));
		grid.SetEditable(TRUE);

		for ( int i = 0 ;i < grid.GetColumnCount() ; i++)
		{
			for ( int j = 0 ; j < 3 ; j++)
			{
				grid.SetItemState(j,i,GVNI_READONLY);
			}
		}

		return;
	}
	else if( m_nType == 42 )
	{

	}
	else if( m_nType == 43 )
	{

	}
	else if( m_nType == 44 )
	{

	}
	else if( m_nType == 7 ) //制造费用
	{

	}
	else if( m_nType == 71 )//直接制造费用
	{
		grid.DeleteAllItems();
		grid.Invalidate();

		for ( int i = 0 ; i < 20 ; i++ )
		{
			grid.InsertColumn("");
		}
		grid.InsertRow("");
		grid.InsertRow("");

		CAssmbleGrid::MergeCellEx(3,"序号,项目号,分类,总计","","",grid,0);
		CAssmbleGrid::MergeCellEx(3,"合计,材料费,电费,水费,折旧费,修理费,办公费,差旅费,运输费,劳务费,保险费,试验检验费,安全生产费,租赁费,低值易耗品,其他","","",grid);


		grid.SetFixedRowCount(0);
		grid.SetFixedColumnCount(0);
		grid.SetRowHeight(0,40);
		grid.SetGridLineColor(RGB(30, 30, 30));
		grid.SetEditable(TRUE);

		for ( int i = 0 ;i < grid.GetColumnCount() ; i++)
		{
			for ( int j = 0 ; j < 3 ; j++)
			{
				grid.SetItemState(j,i,GVNI_READONLY);
			}
		}

		return;
	}
	else if( m_nType == 72 )//间接制造费用
	{
		grid.DeleteAllItems();
		grid.Invalidate();

		for ( int i = 0 ; i < 20 ; i++ )
		{
			grid.InsertColumn("");
		}
		grid.InsertRow("");
		grid.InsertRow("");

		CAssmbleGrid::MergeCellEx(3,"序号,项目号,分类,总计","","",grid,0);
		CAssmbleGrid::MergeCellEx(3,"合计,材料费,电费,水费,折旧费,修理费,办公费,差旅费,运输费,劳务费,保险费,试验检验费,安全生产费,租赁费,低值易耗品,其他","","",grid);


		grid.SetFixedRowCount(0);
		grid.SetFixedColumnCount(0);
		grid.SetRowHeight(0,40);
		grid.SetGridLineColor(RGB(30, 30, 30));
		grid.SetEditable(TRUE);

		for ( int i = 0 ;i < grid.GetColumnCount() ; i++)
		{
			for ( int j = 0 ; j < 3 ; j++)
			{
				grid.SetItemState(j,i,GVNI_READONLY);
			}
		}

		return;
	}
	else if( m_nType == 73 )//项目部费用
	{
		grid.DeleteAllItems();
		grid.Invalidate();

		for ( int i = 0 ; i < 20 ; i++ )
		{
			grid.InsertColumn("");
		}
		grid.InsertRow("");
		grid.InsertRow("");

		CAssmbleGrid::MergeCellEx(3,"序号,项目号,分类,总计","","",grid,0);
		CAssmbleGrid::MergeCellEx(3,"合计,材料费,电费,水费,折旧费,修理费,办公费,差旅费,运输费,劳务费,保险费,试验检验费,安全生产费,租赁费,低值易耗品,其他","","",grid);


		grid.SetFixedRowCount(0);
		grid.SetFixedColumnCount(0);
		grid.SetRowHeight(0,40);
		grid.SetGridLineColor(RGB(30, 30, 30));
		grid.SetEditable(TRUE);

		for ( int i = 0 ;i < grid.GetColumnCount() ; i++)
		{
			for ( int j = 0 ; j < 3 ; j++)
			{
				grid.SetItemState(j,i,GVNI_READONLY);
			}
		}

		return;
	}

	ResetGridHeader(mapHeader,grid);
}

void CAssmbleGrid::SummaryOfCostGridHead(CGridCtrl& grid)
{
	grid.InsertRow("");
	grid.InsertRow("");

	for ( int i = 0 ; i < grid.GetColumnCount() ; i++ )
	{
		grid.SetColumnWidth(i,50);
	}
	grid.SetColumnWidth(1,100);
	grid.SetItemText(0,0,"序号");
	grid.MergeCells(CCellRange( 0, 0, 2,0));
	grid.SetItemText(0,1,"项目编号");
	grid.MergeCells(CCellRange( 0, 1, 2,1));
	grid.SetItemText(0,2,"合同收入");
	grid.MergeCells(CCellRange( 0, 2, 2,2));
	grid.SetItemText(0,3,"财务");
	grid.MergeCells(CCellRange( 0, 3, 2,3));
	grid.SetItemText(0,4,"制造成本");
	grid.MergeCells(CCellRange( 0, 4, 2,4));
	grid.SetItemText(0,5,"毛利润");
	grid.MergeCells(CCellRange( 0, 5, 2,5));
	grid.SetItemText(0,6,"毛利率");
	grid.MergeCells(CCellRange( 0, 6, 0,10));
	grid.SetItemText(1,6,"实际");
 	grid.MergeCells(CCellRange( 1, 6, 2,6));
	grid.SetItemText(1,7,"目标");
	grid.MergeCells(CCellRange( 1, 7, 2,7));
	grid.SetItemText(1,8,"财务");
	grid.MergeCells(CCellRange( 1, 8, 2,8));
	grid.SetItemText(1,9,"与财务比");
	grid.MergeCells(CCellRange( 1, 9, 2,9));
	grid.SetItemText(1,10,"与目标差异");
	grid.MergeCells(CCellRange( 1, 10, 2,10));
	grid.SetItemText(0,11,"直接生产成本");
	grid.MergeCells(CCellRange( 0, 11, 0,17));
	grid.SetItemText(1,11,"小计");
	grid.MergeCells(CCellRange( 1, 11, 2,11));
	grid.SetItemText(1,12,"工时");
	grid.MergeCells(CCellRange( 1, 12, 2,12));
	grid.SetItemText(1,13,"职工薪酬");
	grid.MergeCells(CCellRange( 1, 13, 1,15));
	grid.SetItemText(2,13,"小计");
	grid.MergeCells(CCellRange( 2, 13, 2,13));
	grid.SetItemText(2,14,"工时费");
	grid.MergeCells(CCellRange( 2, 14, 2,14));
	grid.SetItemText(2,15,"人工费");
	grid.MergeCells(CCellRange( 2, 15, 2,15));
	grid.SetItemText(2,15,"人工费");
	grid.MergeCells(CCellRange( 2, 15, 2,15));
	
	grid.SetFixedRowCount(0);
	grid.SetFixedColumnCount(0);
	grid.SetRowHeight(0,40);
	grid.SetEditable(TRUE);
}

