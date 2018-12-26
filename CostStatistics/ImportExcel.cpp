#include "stdafx.h"
#include "ImportExcel.h"
#include "libxl/include_cpp/libxl.h"


ImportExcel::ImportExcel()
{

}

ImportExcel::~ImportExcel()
{

}

void ImportExcel::ImportStaffSalary()
{
	CString path;
	CFileDialog dlg(TRUE);
	if(dlg.DoModal()==IDOK) 
	{
       path = dlg.GetPathName();
	}
	//path.MakeLower();
 	CString ext;
 	int pos = path.ReverseFind('.');
 	if(pos>0)
 	{
 		ext = path.Mid(pos+1);
 		ext.Trim();
 	}
 	if(ext != "xls" && ext != "xlsx")
 	{
 		AfxMessageBox("文件格式无效");
 		return;
 	}
 	CString suf = path.Right(path.GetLength()-path.ReverseFind('.')-1);
 	libxl::Book* book = xlCreateXMLBook();
	AfxMessageBox("导入成功");
// 	if ( suf == "xls")
// 	{
// 		book = xlCreateBook();
// 	}
// 	const TCHAR * name = _T("hchen");
// 	const TCHAR* key  = _T("windows-2e26280a0cc4ef0969b4686aa7tbu9hd");
// 	book->setKey(name,  key); 
// 
// 	if (book)
// 	{
// 		if (book->load(path))
// 		{
// 			libxl::Sheet* sheet = book->getSheet(0);
// 			if (sheet)
// 			{
// 				int row = sheet->lastRow();
// 				int col = sheet->lastCol();
// 				CString firstLabel = sheet->readStr(0,0);
// 
// 				if(col<10 || firstLabel.Find("部件号")<0 )
// 				{
// 					AfxMessageBox("Excel表格格式与标准不相符");
// 					return;
// 				}
// 				CString countstr = sheet->readStr(0,5);
// 				if ( strstr(countstr,"部件数量") == NULL )
// 				{
// 					AfxMessageBox("部件数量不在G列，请调整明细表格式");
// 					return;
// 				}
// 				else 
// 				{
// 					
// 				}
// 		   }
// 		book->release();
// 	 }
//    }
}

void ImportExcel::ImportWorkShopTotal()
{
	CString path;
	CFileDialog dlg(TRUE);
	if(dlg.DoModal()==IDOK) 
	{
		path = dlg.GetPathName();
	}
	//path.MakeLower();
	CString ext;
	int pos = path.ReverseFind('.');
	if(pos>0)
	{
		ext = path.Mid(pos+1);
		ext.Trim();
	}
	if(ext != "xls" && ext != "xlsx")
	{
		AfxMessageBox("文件格式无效");
		return;
	}
	CString suf = path.Right(path.GetLength()-path.ReverseFind('.')-1);
	libxl::Book* book = xlCreateXMLBook();
	AfxMessageBox("导入成功");
}

void ImportExcel::ImportCostTotal()
{
	CString path;
	CFileDialog dlg(TRUE);
	if(dlg.DoModal()==IDOK) 
	{
		path = dlg.GetPathName();
	}
	//path.MakeLower();
	CString ext;
	int pos = path.ReverseFind('.');
	if(pos>0)
	{
		ext = path.Mid(pos+1);
		ext.Trim();
	}
	if(ext != "xls" && ext != "xlsx")
	{
		AfxMessageBox("文件格式无效");
		return;
	}
	CString suf = path.Right(path.GetLength()-path.ReverseFind('.')-1);
	libxl::Book* book = xlCreateXMLBook();
	AfxMessageBox("导入成功");
}

void ImportExcel::ImportOverSeaProduce()
{
	CString path;
	CFileDialog dlg(TRUE);
	if(dlg.DoModal()==IDOK) 
	{
		path = dlg.GetPathName();
	}
	//path.MakeLower();
	CString ext;
	int pos = path.ReverseFind('.');
	if(pos>0)
	{
		ext = path.Mid(pos+1);
		ext.Trim();
	}
	if(ext != "xls" && ext != "xlsx")
	{
		AfxMessageBox("文件格式无效");
		return;
	}
	CString suf = path.Right(path.GetLength()-path.ReverseFind('.')-1);
	libxl::Book* book = xlCreateXMLBook();
	AfxMessageBox("导入成功");
}