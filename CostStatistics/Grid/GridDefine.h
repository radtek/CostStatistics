#pragma once

#pragma region 栅格常量
#define HEADER_ROW_INDEX                               0			//标题行
#define SEARCH_ROW_INDEX                               1			//搜索行
#define BUTTON_COL_INDEX                               0			//按纽列

#define SYXK_POS_TREE_ROW_HEIGHT                       24           //树行高
#define SYXK_POS_PAGE_CTRL_WIDTH                       170          // PageCtrl控件的宽度
#define SYXK_POS_STD_ROW_HEI                           23			//一般表行高
#pragma endregion  


#pragma region 颜色值
#define DISABLE_COLOR			                        RGB(165, 165, 165)
#define GRID_BK_COLOR			                        RGB(255, 255, 255)
#define GRID_FIX_BK_COLOR		                        RGB(230, 240, 255)

#define CLR_GRID_LINE                                   RGB(230, 230, 230)
#define CLR_GRID_HEAD_BK                                RGB(170, 213, 243)//RGB(255, 235, 156),RGB(170, 213, 243)
#define CLR_GRID_HEAD_FG                                RGB(11,  91,  151) 
#define CLR_GRID_SEARCH_ROW_BK                          RGB(255, 255, 255) //RGB(67,  205, 128)
#define CLR_GRID_BODY_FG                                RGB(11,  91,  151)
#define CLR_WHITE                                       RGB(255, 255, 255)          // 白色
#define CLR_BLACK                                       RGB(0,   0,   0  )          // 黑色
#define CLR_RED                                         RGB(255, 0,   0  )          // 红色
#define CLR_GREEN                                       RGB(0,   255, 0  )          // 绿色
#define CLR_BLUE                                        RGB(0,   0,   255)          // 蓝色
#define CLR_YELLOW                                      RGB(255, 255, 0  )          // 黄色
#define CLR_PANE_BK                                     RGB(202, 225, 243)          // 主界面主要背景色
#define CLR_SEL_HILIGHT                                 RGB(176, 224, 230)
#define CLR_GRAY                                        RGB(192, 192, 192)          //灰色
#define CLR_WEAKYELLOW                                  RGB(255, 255, 100)          //淡黄色
#define CLR_WEAKRED                                     RGB(250, 128, 114)          //淡红色  
#define CLR_REDDISH										RGB( 150 , 0 , 0)			//淡红色 
#define CLR_LIGHTGREEN									RGB( 0 , 150 , 0)			//淡绿色 
#pragma endregion


namespace GDS
{
	struct GridColHeadAttr
	{
		int			colIdx;		//列序号
		CString		name;		//列名
		int			colLen;     //长度
		CString     field;      //读取数据库字段别名

	};

	static	GridColHeadAttr GeneratGCHA( int idx , const CString &n , int len , const CString &fn = "" )
	{
		GridColHeadAttr s;
		s.colIdx = idx;
		s.name = n;
		s.colLen = len;
		s.field = fn;
		return s;
	}

	typedef struct 
	{
		int x;
		int y;
	} ORDERSTRUCT_POSITION;

};


struct IVTabItem
{
	virtual CString		GetTitle()	= 0;
	virtual CDialog *	GetPage()	= 0;
	virtual void		Print()		= 0;
	virtual void		Export()	= 0;
	virtual BOOL        CreatePage( CWnd * p ) = 0;
	virtual BOOL		Show( int nCmdShow ) = 0;     
	virtual void		Move( LPCRECT rect ) = 0;      
};
