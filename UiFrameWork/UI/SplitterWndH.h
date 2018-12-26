#pragma once

class CSplitterWndH : public CSplitterWnd  
{
public:
	CSplitterWndH();
	virtual ~CSplitterWndH();

	void HideColumn(int nCol);
	void ShowColumn();
	bool HaveHiddenColumn() {return( -1 != m_nHiddenCol);}
	int GetHideColWidth() const { return m_nHideColWidth; }

protected: 
	int m_nHiddenCol;   
	int m_nHideColWidth;
};

