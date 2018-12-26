#include "stdafx.h"

HBITMAP TransparentBmp(HBITMAP hbmpSrc, COLORREF bkgndColor, COLORREF maskColor)
{
	CBitmap memBmp;
	CBitmap tmpBmp;
	tmpBmp.Attach(hbmpSrc);
	BITMAP bitMap;
	tmpBmp.GetBitmap(&bitMap);

	CClientDC dc(NULL);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, bitMap.bmWidth, bitMap.bmHeight);
	dcMem.SelectObject(&memBmp);
	dcMem.FillSolidRect(0,0,bitMap.bmWidth,bitMap.bmHeight, bkgndColor);

	CDC dcMemSrc;
	dcMemSrc.CreateCompatibleDC(&dc);		
	dcMemSrc.SelectObject(&tmpBmp);
	dcMem.TransparentBlt(0,0, bitMap.bmWidth, bitMap.bmHeight, &dcMemSrc, 0,0, bitMap.bmWidth,bitMap.bmHeight, maskColor);

	return (HBITMAP)memBmp.Detach();
}

CString GetModulePath()
{
	TCHAR *pChar;
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);

	pChar = _tcsrchr(path,_T('\\'));
	if (pChar) *pChar = _T('\0');

	return CString(path);
}

BOOL MyMoveWindow(
    _In_ HWND hWnd,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_ BOOL bRepaint)
{
    return ::MoveWindow(
        hWnd,
        X,
        Y,
        nWidth,
        nHeight,
        bRepaint
        );
}

BOOL MyMoveWindow(HWND hWnd, LPCRECT lpRect, BOOL bRepaint)
{
    return MyMoveWindow(
        hWnd,
        lpRect->left,
        lpRect->top, 
        lpRect->right - lpRect->left,
        lpRect->bottom - lpRect->top,
        bRepaint
        );
}

void MyOutputDebugStringA(char const* pszFormat, ...)
{
    //ATLASSERT(AtlIsValidString(pszFormat));

    CStringA strMsg;
    va_list argList;
    va_start(argList, pszFormat);
    strMsg.FormatV(pszFormat, argList);
    va_end(argList);

    OutputDebugStringA(strMsg);
}

void MyOutputDebugString(TCHAR const* pszFormat, ...)
{
    ATLASSERT(AtlIsValidString(pszFormat));

    CString strMsg;
    va_list argList;
    va_start(argList, pszFormat);
    strMsg.FormatV(pszFormat, argList);
    va_end(argList);

    OutputDebugString(strMsg);
}
