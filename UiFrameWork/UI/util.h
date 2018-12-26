#pragma once


HBITMAP TransparentBmp(HBITMAP hbmpSrc, COLORREF bkgndColor, COLORREF maskColor);
CString GetModulePath();
BOOL MyMoveWindow(
    _In_ HWND hWnd,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_ BOOL bRepaint
    );

BOOL MyMoveWindow(HWND hWnd, LPCRECT lpRect, BOOL bRepaint);

void MyOutputDebugStringA(char const* pszFormat, ...);
void MyOutputDebugString(TCHAR const* pszFormat, ...);

