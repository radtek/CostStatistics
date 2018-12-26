/**********************************************************************************************************************
	GridCellAttachments.h

	作者：quzhenghzong@163.com 2017-10-03

	GridCellAttachments是一种单元格类型。
	单元格内的每行显示为一个附件超链接，点击可以打开附件。
	

	为增加此单元格类型，增加了2个源码文件：
		GridCellAttachments.h
		GridCellAttachments.cpp
	此单元格类型是对已有单元格类型的补充，这种补充是无侵入式的，未对其余单元格类型造成影
	响。除上述增加的源码外，没有对其余单元格类型的源码进行修改，也未对程序的框架及基础代码进行修改。


	使用方法可参考CGridCtrlDemoDlg::OnCellAttachments方法。首先设置某个单元格类型为此类型，然后设置单元格文字即可。
	单元格文字可以通过换行符号（\n）分隔为多个行，每行文字使用星号（*）分隔为“附件路径”与“显示文字”2个部分。
	可以参见下文对变量 ELEMENT_DELIMITER 的描述。

	比如单元文字为以下内容（注意中间有一个空行）:
		"D:\\temp\\111.jpg*图片111\n\nD:\\temp\\222.jpg*图片222"
	则可以提取出2个附件连接：
		第一个文件为"D:\\temp\\111.jpg"，显示文字为"图片111"
		第二个文件为"D:\\temp\\222.jpg"，显示文字为"图片222"
	超链接为显示为蓝色文字带有下划线，点击超链接文字的时候，如果附件存在，则可以打开。


	为了测试，对测试用的对话框程序进行了修改，只增加了一个checkbox，
	另外增加此控件的点击消息CGridCtrlDemoDlg::OnCellAttachments。

	在测试过程中发现，此测试程序的鼠标点击消息似乎有问题，第一次点击单元格的时候可以相应点击消息
		OnClick(CRect cellRect, CPoint PointCellRelative)
	但如果单元格处于选中状态时候，则不再响应OnClick消息。这可能是有意为之或程序源码本身存在的问题，与此新增的单元格类型
	无关，其余单元格也存在同样问题。
***************************************************************************************************************************/

//////////////////////////////////////////////////////////////////////
#include	<afxtempl.h>
#include	"GridCell.h"

#ifndef	__TREE_GRID_CELL_ATTACHMENTS_H__
#define	__TREE_GRID_CELL_ATTACHMENTS_H__

//元素信息的分隔符。每行元素为“{path}[*[title]]”的格式。
//其中：
//	路径path是必须有的；
//	路径path后如果无内容，则显示title取路径path的文字；
//	路径path后可以连接一个星号。如果星号后无内容，则显示title也取路径path的文字；如果星号后有内容，则为title。
//	由于路径path是文件路径，因此使用一个不能作为文件名文字的星号作为分隔符。
const	LPCTSTR	ELEMENT_DELIMITER	=	_T("*");	
//附件元素信息
typedef struct {
	CString	szRawText;	//原始文字
	CString	szTitle;	//显示标题
	CString	szPath;		//文件路径
}	ElementInfo;


class CGridCellAttachments : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellAttachments)

public:
	CGridCellAttachments();
	virtual ~CGridCellAttachments();

    virtual BOOL	Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
    virtual BOOL	Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual BOOL	GetTipTextRect( LPRECT ) {	return FALSE;	}	//重载以便不调用ShowTitleTip方法，从而不出现Tip窗口。在Tip窗口中无法进行打开超链接的操作。
    virtual void	SetText(LPCTSTR szText);                      

	void SetAutoLaunchUrl(BOOL bLaunch = TRUE) { m_bLaunchUrl = bLaunch;	}
	BOOL GetAutoLaunchUrl() { return m_bLaunchUrl;	}

protected:
    virtual BOOL OnSetCursor();
    virtual void OnClick(CRect cellRect, CPoint PointCellRelative);

    BOOL	IsOverHyperlink(CPoint& pt, CString& strURL);

	/**
	 * 把原始格式的单元格字符串解析为附件元素。
	 * @param tElementArray 出参，接收解析得到的附件元素
	 * @return	是否成功
	 */
	BOOL	ParseElement();
protected:
#ifndef _WIN32_WCE
    static HCURSOR g_hLinkCursor;		// Hyperlink mouse cursor
	HCURSOR GetHandCursor();
#endif
	
protected:
	COLORREF	m_clrUrl;
	BOOL		m_bLaunchUrl;
    CRect		m_Rect;
	int m_nRow;
	int m_nCol;

	CString		m_szRawText;		//原始文字
	CTypedPtrArray<CPtrArray,ElementInfo*> m_ElementArray;	//从原始文字解析出来的元素信息
};

#endif	//__TREE_GRID_CELL_ATTACHMENTS_H__
