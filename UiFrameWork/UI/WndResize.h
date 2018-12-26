CRect rect;
GetWindowRect(&rect);
ScreenToClient(&rect);
CPoint pt = point;
ScreenToClient(&pt);
int nFrame=4;
rect.DeflateRect(nFrame,nFrame);
if (!rect.PtInRect(pt))
{ 
	if ((m_resizeMoveFlag & (RSM_LEFT | RSM_BOTTOM)) == (RSM_LEFT | RSM_BOTTOM)
        && pt.x<=nFrame && pt.y>=rect.bottom-nFrame)
	{
		return HTBOTTOMLEFT;
	}
    else if ((m_resizeMoveFlag & (RSM_LEFT | RSM_TOP)) == (RSM_LEFT | RSM_TOP)
        && pt.x <= nFrame && pt.y <= nFrame)
    {
		return HTTOPLEFT;
	}
	else if ((m_resizeMoveFlag & (RSM_RIGHT | RSM_TOP)) == (RSM_RIGHT | RSM_TOP)
        && pt.x>=rect.right-nFrame && pt.y<=nFrame)
	{
		return HTTOPRIGHT;
	}
	else if ((m_resizeMoveFlag & (RSM_RIGHT | RSM_BOTTOM)) == (RSM_RIGHT | RSM_BOTTOM)
        && pt.x>=rect.right-nFrame && pt.y>=rect.bottom-nFrame)
	{
		return HTBOTTOMRIGHT;
	}
	else if ((m_resizeMoveFlag & RSM_LEFT)
        && pt.x<=nFrame)
	{
		return HTLEFT;
	}
	else if ((m_resizeMoveFlag & RSM_TOP)
        && pt.y<=nFrame)
	{
		return HTTOP;
	}
	else if ((m_resizeMoveFlag & RSM_BOTTOM)
        && pt.y>=rect.bottom-nFrame)
	{
		return HTBOTTOM;
	}
	else if ((m_resizeMoveFlag & RSM_RIGHT)
        && pt.x>=rect.right-nFrame)
	{
		return HTRIGHT;
	}
}
