#include "stdafx.h"
#include "LeftTreeInfo.h"


LeftTreeInfo::LeftTreeInfo()
{

}


LeftTreeInfo::~LeftTreeInfo()
{

}

void LeftTreeInfo::loadLeftTree(CTreeListCtrl * pCtrl)
{
	CTreeListItem* pRoot = NULL;
	CTreeListItem* pL2 = NULL;
	CTreeListItem* pL3 = NULL;

	pCtrl->InsertColumn("");

	pRoot = pCtrl->InsertItem("汇总信息");
	pL2 = pCtrl->InsertItem("材料费");
	pL2 = pCtrl->InsertItem("劳务费");
	pL2 = pCtrl->InsertItem("运输费");


	pRoot = pCtrl->InsertItem("职工薪酬统计");
	pL2 = pCtrl->InsertItem("人工费合计",pRoot);
	pCtrl->InsertItem("职工薪酬",pL2);
	pL3 = pCtrl->InsertItem("直接人工汇总",pL2);
	pCtrl->InsertItem("电器调试车间",pL3);
	pCtrl->InsertItem("组装车间",pL3);
	pCtrl->InsertItem("铆焊车间",pL3);
	pCtrl->InsertItem("液压车间",pL3);
	pCtrl->InsertItem("流体车间",pL3);
	pCtrl->InsertItem("机加工车间",pL3);
	pCtrl->InsertItem("综合班",pL3);
	pCtrl->InsertItem("物资中心",pL3);
	pCtrl->InsertItem("电控柜制作车间",pL3);

	pCtrl->InsertItem("制造人工分摊",pL2);
	pCtrl->InsertItem("后勤人工分摊",pL2);
	pCtrl->InsertItem("项目人工费分摊",pL2);


    pL2 = pCtrl->InsertItem("制造费用统计表",pRoot);
	pCtrl->InsertItem("直接制造费用",pL2);
	pCtrl->InsertItem("间接制造费用",pL2);
	pCtrl->InsertItem("项目部费用",pL2);

	pL2 = pCtrl->InsertItem("制造费用统计表",pRoot);
	pCtrl->InsertItem("天津",pL2);
	pCtrl->InsertItem("顺德",pL2);
	pCtrl->InsertItem("成都",pL2);
	pCtrl->InsertItem("长沙",pL2);
	pCtrl->InsertItem("郑州",pL2);
	pCtrl->Invalidate(TRUE);
}