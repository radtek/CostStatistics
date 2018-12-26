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
 		AfxMessageBox("�ļ���ʽ��Ч");
 		return;
 	}
 	CString suf = path.Right(path.GetLength()-path.ReverseFind('.')-1);
 	libxl::Book* book = xlCreateXMLBook();
	AfxMessageBox("����ɹ�");
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
// 				if(col<10 || firstLabel.Find("������")<0 )
// 				{
// 					AfxMessageBox("Excel����ʽ���׼�����");
// 					return;
// 				}
// 				CString countstr = sheet->readStr(0,5);
// 				if ( strstr(countstr,"��������") == NULL )
// 				{
// 					AfxMessageBox("������������G�У��������ϸ���ʽ");
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
		AfxMessageBox("�ļ���ʽ��Ч");
		return;
	}
	CString suf = path.Right(path.GetLength()-path.ReverseFind('.')-1);
	libxl::Book* book = xlCreateXMLBook();
	AfxMessageBox("����ɹ�");
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
		AfxMessageBox("�ļ���ʽ��Ч");
		return;
	}
	CString suf = path.Right(path.GetLength()-path.ReverseFind('.')-1);
	libxl::Book* book = xlCreateXMLBook();
	AfxMessageBox("����ɹ�");
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
		AfxMessageBox("�ļ���ʽ��Ч");
		return;
	}
	CString suf = path.Right(path.GetLength()-path.ReverseFind('.')-1);
	libxl::Book* book = xlCreateXMLBook();
	AfxMessageBox("����ɹ�");
}