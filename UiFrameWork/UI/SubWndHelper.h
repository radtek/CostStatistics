#pragma once


class WndInfo
{
public:
	CWnd* pWnd;
	E_WindowType eType;
	int customType;
	bool bAutoWidth;
	bool bNeedDel;

	WndInfo()
		: pWnd(NULL)
		, eType(E_WindowType(0))
		, bAutoWidth(true)
		, bNeedDel(true)
		,customType(0)
	{
	}

	WndInfo(CWnd* wnd, E_WindowType wndType, bool autoWidth, bool needDel,int cstmType)
		: pWnd(wnd)
		, eType(wndType)
		, bAutoWidth(autoWidth)
		, bNeedDel(needDel)
		,customType(cstmType)
	{
	}

};

class CSubWndHelper
{
public:
	CSubWndHelper();
	virtual ~CSubWndHelper();
	void RemoveAll();
	void AddSubWnd(CWnd* pWnd, E_WindowType type, int cstmType, bool bAutoWidth = true, bool bNeedDel = true);
	void RemoveSubWnd(const CWnd* pWnd);
	void MoveSubWnd(const CWnd* pWnd, int step);
	void MoveSubWndToTop(const CWnd* pWnd);
	void RepositionY();
    void RepositionX(int cx);
	UINT GetHight();
	int MinHight() const { return m_nMinHight; }
	void MinHight(int val) { m_nMinHight = val; }
	bool Min() const { return m_bMin; }
	void Min(bool val) { m_bMin = val; }
	void Active(CWnd* pWnd);
	CWnd* FindByName(CString name);
	
public:
	int m_yMargin;
	CWnd * m_pOwnerWnd;
	list<WndInfo> m_subWndList;
	POINT m_beginPoint;
	int m_nMinHight;
	bool m_bMin;

};


