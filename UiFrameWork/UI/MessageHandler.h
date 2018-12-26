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

public: // ��Ӧ�û�����¿ؼ�
	virtual void OnInitThingWnd(CThingWnd* pThingWnd) { }
	virtual void OnInitPersonWnd(CPersonWnd* pPersonWnd) { }
		
public: // ��Ӧ�ؼ����ݱ���
	virtual void OnSaveThingWnd(CThingWnd* pThingWnd) { }
	virtual void OnSavePersonWnd(CPersonWnd* pPersonWnd) { }

	
public: // ��Ӧ�ؼ����ݼ���, true�ɸ���Ĭ����Ϊ, false��voidĬ����Ϊ�Ա�ִ��.
	virtual void OnLoadThingWnd(CThingWnd* pThingWnd, ThingData& cfg) { }
	virtual void OnLoadPersonWnd(CPersonWnd* pPersonWnd, PersonData& cfg) { }

public: // ��Ӧ�ؼ���Ϊ
	virtual void OnToolbarClick(CToolbarEh* toolbar, int id) { }
	virtual void OnTreeListCtrlSelChange(CTreeListItem* pItem){}
	virtual void OnTreeListCtrlItemRClick(CTreeListItem * pItem){}
//����
	virtual CFont* OnToolbarGetFont() { return NULL; }
	virtual CFont* OnCtrlGetFont() { return NULL; }
	virtual DWORD OnToolbarTextColor(){return RGB(0,0,0)/*return RGB(0,0,120)*/;}
public: // ȫ������
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

