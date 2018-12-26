#pragma once

#include <vector>
#include <map>
#include <list>
#include <set>
using namespace std;


#define WM_SUB_WND_SIZE_CHANGED						0x8001
#define WM_TOOLBAR_MESSAGE                          0x8002
#define WM_STATIC_RENAME                            0x8003
#define WM_STATIC_REPOSITION                        0x8004
#define WM_CONTROL_ACTIVED							0x8005
#define WM_LEFTWND_SHOWHIDE							0x8006
#define WM_THING_ACTIVE                             0x8007
#define WM_PERSON_ACTIVE                            0x8008
#define WM_CONTEXT_MENU                             0x8009

// For window ReSize and Move
#define RSM_LEFT                                    0x0001
#define RSM_TOP                                     0x0002
#define RSM_RIGHT                                   0x0004
#define RSM_BOTTOM                                  0x0008


enum E_WindowType
{
	e_none		= 0,
	e_person	= 1,
	e_thing		= 2,		
	e_custom
};

class AbsCtrlData
{
public:
	AbsCtrlData(){}
	virtual ~AbsCtrlData(){}
};

class CAbsCtrl
{
public:
	CAbsCtrl(){}
	virtual ~CAbsCtrl(){}

public:
	virtual AbsCtrlData* Save() = 0;
	virtual void Load(AbsCtrlData* pData) = 0;

};


// For CFileCtrl
class FileCtrlData : public AbsCtrlData
{
public:
	CString strName;
	CString strOpen;
	CString strSave;
	CString strUndo;
};

// For CTimeCtrl
class TimeCtrlData : public AbsCtrlData
{
public:
	bool month;
	bool week;
	bool day;
	COleDateTime begin;
	COleDateTime end;
	COleDateTime complete;
	CString strBegin;
	CString strEnd;
	CString strComplete;
};

// For CQuantityCtrl
class QuantityCtrlData : public AbsCtrlData
{
public:
	CString strNum;
	CString strDesc;
	CString strCur;
	list<CString> strList;
};

// For CEditEh
class EditCtrlData : public AbsCtrlData
{
public:
	CString text;
};

// For CEditBoxEh
class EditBoxCtrlData : public AbsCtrlData
{
public:
	double time;
	CString text;
};

// For LGridRTree
class LGridRTreeCtrlData : public AbsCtrlData
{
public:
};

class GridCtrlData : public AbsCtrlData
{
public:
};

class TreeGridCtrlData : public AbsCtrlData
{
public:
};

class FaceCtrlData : public AbsCtrlData
{
public:
	CString filename;
};

class GotoCtrlData : public AbsCtrlData
{
public:
	list<CString> itemList;
};

class GotoEhCtrlData : public AbsCtrlData
{
public:
	list<CString> itemList;
	CString strBegin;
	CString strComplete;
	CString strCreator;
	CString strProgress;
	COleDateTime begin;
	COleDateTime complete;
	UINT percent;
};


// For TreeListCtrl
// 可根据需求自己扩展
class TreeListCtrlData : public AbsCtrlData
{
public:
	class TreeCol
	{
	public:
		CString name;
		int width;
		TreeCol()
			: width(0)
		{
		}

		TreeCol(LPCTSTR lpName, int nWidth)
			: name(lpName)
			, width(nWidth)
		{
		}

	};

	class TreeNode
	{
	public:
		CString name;
		vector<TreeNode> children;

		TreeNode()
		{
		}
		TreeNode(LPCTSTR lpName)
			: name(lpName)
		{
		}

		~TreeNode()
		{
		}

	};

	TreeListCtrlData()
	{
	}

	vector<TreeCol> cols;
	TreeNode root;
};


enum E_ToolbarBtnType
{
	e_TextBtn = 0,
	e_ImageBtn = 1,
	e_TwoStatesImageBtn = 2,
	e_TextStatic = 3
};

enum E_ButtonState
{
	e_BtnStateError = -1,
	e_BtnState1 = 0,
	e_BtnState2 = 1,
};


class ToolbarBtnData
{
public:
	E_ToolbarBtnType type;
	E_ButtonState btnState;
	bool show;

	UINT id;
	CString text;
	CString image1;
	CString image2;

	ToolbarBtnData()
	{
		type = e_TextBtn;
		btnState = e_BtnState1;
		show = true;
		id = 0;
	}

	ToolbarBtnData(
		UINT nBtnId,
		CString strText = _T(""),
		E_ToolbarBtnType btnType = e_TextBtn,
		CString strImgFile1 = _T(""),
		CString strImgFile2 = _T(""),
		E_ButtonState imgType = e_BtnState1,
		bool bShow = true
	)
		: id(nBtnId)
		, btnState(imgType)
		, show(bShow)
		, type(btnType)
		, text(strText)
		, image1(strImgFile1)
		, image2(strImgFile2)
	{
	}

};

class ToolbarData
{
public:
	vector<ToolbarBtnData> buttons;
    ToolbarData() {}
	//TODO: and other
private:
    ToolbarData(const ToolbarData& rhs) {}
    ToolbarData& operator=(const ToolbarData& rhs) {}
};

class CtrlData
{
public:
	CString name;
	E_WindowType type;
	int customType;
	CSize size;
	AbsCtrlData * data;

	CtrlData()
		: type(e_none)
		, size(0,0)
		, data(NULL)
		,customType(0)
	{
	}

	~CtrlData()
	{
		if (data)
		{
			delete data;
			data = NULL;
		}
	}
private:
    CtrlData(const CtrlData& rhs) {}
    CtrlData& operator=(const CtrlData& rhs) {}
};

class PersonData
{
public:
	CString name;
	int idPerson;
	BOOL bShowToolbar;
	ToolbarData toolbar;
	vector<CtrlData*> ctrls;

	PersonData():bShowToolbar(TRUE),idPerson(0) {}

	~PersonData()
	{
		for (vector<CtrlData*>::iterator it = ctrls.begin(); it != ctrls.end(); ++it)
		{
			delete (*it);
		}
	}
private:
    PersonData(const PersonData& rhs) {}
    PersonData& operator=(const PersonData& rhs) {}
};

class ThingData
{
public:
	CString name;
	ToolbarData toolbar;
	vector<PersonData*> persons;
	BOOL bShowToolbar;

	ThingData():bShowToolbar(TRUE) {
	}


	~ThingData()
	{
		for (vector<PersonData*>::iterator it = persons.begin(); it != persons.end(); ++it)
		{
			delete (*it);
		}
	}
private:
    ThingData(const ThingData& rhs) {}
    ThingData& operator=(const ThingData& rhs) {}
};

class RightWndData
{
public:
	ToolbarData toolbar;
	vector<ThingData*> things;

    RightWndData() {}

	~RightWndData()
	{
		for (vector<ThingData*>::iterator it = things.begin(); it != things.end(); ++it)
		{
			delete (*it);
		}
	}
private:
    RightWndData(const RightWndData& rhs) {}
    RightWndData& operator=(const RightWndData& rhs) {}
};

// RightWndData
//     ToolbarData
//     vector<ThingData*>
//         ToolbarData
//         vector<PersonData*>
//             ToolbarData
//             vector<CtrlData*>
//                 edit
//                 tree
//                 ...


#define ZT_MFC 1

#if ZT_MFC

//typedef class zt_CView ZTCView;
//typedef class zt_CScrollView ZTCScrollView;
#define ZTCView zt_CView
#define ZTCScrollView zt_CScrollView

#else

//typedef class CView ZTCView;
//typedef class CScrollView ZTCScrollView;
#define ZTCView CView
#define ZTCScrollView CScrollView

#endif
