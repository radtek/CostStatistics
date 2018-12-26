#pragma once

#include "zt_mfc/zt_view.h"

class CGridCtrl;

// CGridView view

class CGridView : public ZTCScrollView
{
	DECLARE_DYNCREATE(CGridView)

protected:
	CGridView();           // protected constructor used by dynamic creation
	virtual ~CGridView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();     // first time after construct

public:
	CGridCtrl* m_pGridCtrl;

private:
	void CreateGridCtrl();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


