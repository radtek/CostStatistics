#pragma once

#include <vector>
#include <map>
#include <list>
#include <set>
#include <afxole.h>
#include <afxwin.h>
#include <atlimage.h>
#include "Common.h"
using namespace std;

#include "ZTUIDef.h"
#include "MessageHandler.h"
#include "RightWnd.h"
#include "LeftWnd.h"

#define g_ztui ZTUI::GetInstance()

class CZTUIMainFrame;


class ZTUI
{
public:
	virtual ~ZTUI(void);
	static ZTUI* GetInstance();

private:
	ZTUI(void);

public:
	CFrameWnd* InitFrame(CWnd * pParent = NULL);

	void SetLeftWndWidth(int w);
	int GetLeftWndWidth();
	void ShowHideLeftWnd();


protected:

public:
	CZTUIMainFrame * m_pFrame; // ZTUIÖ÷´°¿Ú
	CRightWnd* m_pRightWnd;
    CLeftWnd* m_pLeftWnd;
    CMessageHandler* m_pMsgHandler;

};
