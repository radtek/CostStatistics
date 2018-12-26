/**********************************************************************************************************************
	GridCellAttachments.h

	���ߣ�quzhenghzong@163.com 2017-10-03

	GridCellAttachments��һ�ֵ�Ԫ�����͡�
	��Ԫ���ڵ�ÿ����ʾΪһ�����������ӣ�������Դ򿪸�����
	

	Ϊ���Ӵ˵�Ԫ�����ͣ�������2��Դ���ļ���
		GridCellAttachments.h
		GridCellAttachments.cpp
	�˵�Ԫ�������Ƕ����е�Ԫ�����͵Ĳ��䣬���ֲ�����������ʽ�ģ�δ�����൥Ԫ���������Ӱ
	�졣���������ӵ�Դ���⣬û�ж����൥Ԫ�����͵�Դ������޸ģ�Ҳδ�Գ���Ŀ�ܼ�������������޸ġ�


	ʹ�÷����ɲο�CGridCtrlDemoDlg::OnCellAttachments��������������ĳ����Ԫ������Ϊ�����ͣ�Ȼ�����õ�Ԫ�����ּ��ɡ�
	��Ԫ�����ֿ���ͨ�����з��ţ�\n���ָ�Ϊ����У�ÿ������ʹ���Ǻţ�*���ָ�Ϊ������·�����롰��ʾ���֡�2�����֡�
	���Բμ����ĶԱ��� ELEMENT_DELIMITER ��������

	���絥Ԫ����Ϊ�������ݣ�ע���м���һ�����У�:
		"D:\\temp\\111.jpg*ͼƬ111\n\nD:\\temp\\222.jpg*ͼƬ222"
	�������ȡ��2���������ӣ�
		��һ���ļ�Ϊ"D:\\temp\\111.jpg"����ʾ����Ϊ"ͼƬ111"
		�ڶ����ļ�Ϊ"D:\\temp\\222.jpg"����ʾ����Ϊ"ͼƬ222"
	������Ϊ��ʾΪ��ɫ���ִ����»��ߣ�������������ֵ�ʱ������������ڣ�����Դ򿪡�


	Ϊ�˲��ԣ��Բ����õĶԻ������������޸ģ�ֻ������һ��checkbox��
	�������Ӵ˿ؼ��ĵ����ϢCGridCtrlDemoDlg::OnCellAttachments��

	�ڲ��Թ����з��֣��˲��Գ�����������Ϣ�ƺ������⣬��һ�ε����Ԫ���ʱ�������Ӧ�����Ϣ
		OnClick(CRect cellRect, CPoint PointCellRelative)
	�������Ԫ����ѡ��״̬ʱ��������ӦOnClick��Ϣ�������������Ϊ֮�����Դ�뱾����ڵ����⣬��������ĵ�Ԫ������
	�޹أ����൥Ԫ��Ҳ����ͬ�����⡣
***************************************************************************************************************************/

//////////////////////////////////////////////////////////////////////
#include	<afxtempl.h>
#include	"GridCell.h"

#ifndef	__TREE_GRID_CELL_ATTACHMENTS_H__
#define	__TREE_GRID_CELL_ATTACHMENTS_H__

//Ԫ����Ϣ�ķָ�����ÿ��Ԫ��Ϊ��{path}[*[title]]���ĸ�ʽ��
//���У�
//	·��path�Ǳ����еģ�
//	·��path����������ݣ�����ʾtitleȡ·��path�����֣�
//	·��path���������һ���Ǻš�����Ǻź������ݣ�����ʾtitleҲȡ·��path�����֣�����Ǻź������ݣ���Ϊtitle��
//	����·��path���ļ�·�������ʹ��һ��������Ϊ�ļ������ֵ��Ǻ���Ϊ�ָ�����
const	LPCTSTR	ELEMENT_DELIMITER	=	_T("*");	
//����Ԫ����Ϣ
typedef struct {
	CString	szRawText;	//ԭʼ����
	CString	szTitle;	//��ʾ����
	CString	szPath;		//�ļ�·��
}	ElementInfo;


class CGridCellAttachments : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellAttachments)

public:
	CGridCellAttachments();
	virtual ~CGridCellAttachments();

    virtual BOOL	Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
    virtual BOOL	Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual BOOL	GetTipTextRect( LPRECT ) {	return FALSE;	}	//�����Ա㲻����ShowTitleTip�������Ӷ�������Tip���ڡ���Tip�������޷����д򿪳����ӵĲ�����
    virtual void	SetText(LPCTSTR szText);                      

	void SetAutoLaunchUrl(BOOL bLaunch = TRUE) { m_bLaunchUrl = bLaunch;	}
	BOOL GetAutoLaunchUrl() { return m_bLaunchUrl;	}

protected:
    virtual BOOL OnSetCursor();
    virtual void OnClick(CRect cellRect, CPoint PointCellRelative);

    BOOL	IsOverHyperlink(CPoint& pt, CString& strURL);

	/**
	 * ��ԭʼ��ʽ�ĵ�Ԫ���ַ�������Ϊ����Ԫ�ء�
	 * @param tElementArray ���Σ����ս����õ��ĸ���Ԫ��
	 * @return	�Ƿ�ɹ�
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

	CString		m_szRawText;		//ԭʼ����
	CTypedPtrArray<CPtrArray,ElementInfo*> m_ElementArray;	//��ԭʼ���ֽ���������Ԫ����Ϣ
};

#endif	//__TREE_GRID_CELL_ATTACHMENTS_H__
