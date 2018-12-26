#pragma once

class IGridCtrlPrintSupport
{
public:
	virtual void PrintGrid(CString title) = 0;
	virtual void OnGridPrint(CDC* pDC, CPrintInfo* pInfo) = 0;
	virtual void OnGridBeginPrinting(CDC* pDC, CPrintInfo* pInfo) = 0;
	virtual void OnGridEndPrinting(CDC* pDC, CPrintInfo* pInfo) = 0;
};

extern "C"
{
	void cdecl GridCtrlPrintPreview(IGridCtrlPrintSupport* pGrid,const char* title);
	void cdecl GridCtrlPrint(IGridCtrlPrintSupport* pGrid, const char* title);
}