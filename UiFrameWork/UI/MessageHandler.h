#pragma once

#include "util.h"


class ThingData;
class PersonData;
class AbsCtrlData;
class CToolbarEh;
class CTreeListItem;
class CThingWnd;
class CPersonWnd;

class CMessageHandler
{
public:
	CMessageHandler(void);
	virtual ~CMessageHandler(void);

public: // 响应用户添加新控件
	virtual void OnInitThingWnd(CThingWnd* pThingWnd) { }
	virtual void OnInitPersonWnd(CPersonWnd* pPersonWnd) { }
		
public: // 响应控件数据保存
	virtual void OnSaveThingWnd(CThingWnd* pThingWnd) { }
	virtual void OnSavePersonWnd(CPersonWnd* pPersonWnd) { }

	
public: // 响应控件数据加载, true可覆盖默认行为, false和void默认行为仍被执行.
	virtual void OnLoadThingWnd(CThingWnd* pThingWnd, ThingData& cfg) { }
	virtual void OnLoadPersonWnd(CPersonWnd* pPersonWnd, PersonData& cfg) { }

public: // 响应控件行为
	virtual void OnToolbarClick(CToolbarEh* toolbar, int id) { }
	virtual void OnTreeListCtrlSelChange(CTreeListItem* pItem){}
	virtual void OnTreeListCtrlItemRClick(CTreeListItem * pItem){}
//字体
	virtual CFont* OnToolbarGetFont() { return NULL; }
	virtual CFont* OnCtrlGetFont() { return NULL; }
	virtual DWORD OnToolbarTextColor(){return RGB(0,0,0)/*return RGB(0,0,120)*/;}
public: // 全局配置
	virtual COLORREF OnGetRightWndBackColor() { return RGB(180, 180, 240); }

	virtual COLORREF OnGetThingWndBackColor() { return RGB(170, 228, 242); }
	virtual COLORREF OnGetThingWndToolbarBackColor() { return RGB(216,216,216); }
	virtual COLORREF OnGetThingWndToolbarHighlightColor() { return RGB(216,180,180); }

	virtual COLORREF OnGetPersonWndBackColor() { return RGB(246, 248, 247); }
	virtual COLORREF OnGetPersonWndToolbarBackColor() { return RGB(231,231,246); }
	virtual COLORREF OnGetPersonWndToolbarHighlightColor() { return RGB(170,231,170); }

	virtual COLORREF OnGetCtrlBackColor() { return RGB(246, 248, 247); }
	virtual COLORREF OnGetCtrlBorderColor() { return RGB(0, 105, 255); }

	virtual CString OnGetSmallPictureDir()
	{
		static CString strSmallPictureDir;
		if (strSmallPictureDir.IsEmpty())
		{
			strSmallPictureDir = GetModulePath() + _T("\\Picture\\");
		}
		return strSmallPictureDir;
	}

	virtual CString OnGetFaceGifDir()
	{
		static CString strFaceGifDir;
		if (strFaceGifDir.IsEmpty())
		{
			strFaceGifDir = GetModulePath() + _T("\\Face\\");

		}
		return strFaceGifDir;
	}

	


};

