#ifndef _ASSMEBLE_GRID_H_
#define _ASSMEBLE_GRID_H_


class CAssmbleGrid
{
public:
	CAssmbleGrid();
	~CAssmbleGrid();
public:
	static void ChangeGridHeader(int nType,CGridCtrl& grid);

	static void MergeCellEx(int nMergeRows,CString strL1Title,CString strL2Title,CString strL3Title,CGridCtrl& grid,int nStartCol = -1);
protected:
private:
	static void SetMerge(int nStartRow,int nEndRow,int nStartCol,int nEndCol,CString strTitle,CGridCtrl& grid);
	static void ResetGridHeader(std::map<int, GDS::GridColHeadAttr>& mapHeader,CGridCtrl& grid);
	static void InitGrid(int nCol,CGridCtrl& grid);
	static void SummaryOfCostGridHead(CGridCtrl& grid);//成本收入汇总
private:
	static int m_nType;
	static int m_nColIndex;
};

#endif