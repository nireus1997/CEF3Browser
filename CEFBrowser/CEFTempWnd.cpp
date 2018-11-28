#include "stdafx.h"
#include "CEFTempWnd.h"



CCEFTempWnd::CCEFTempWnd()
{

}

CCEFTempWnd::~CCEFTempWnd()
{

}

void CCEFTempWnd::Init()
{
	if (m_hWnd == NULL)
	{
		RECT rc = { 0, 0, 1, 1 };
		Create(NULL, NULL, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, rc);
	}
}

LPCTSTR CCEFTempWnd::GetWindowClassName() const
{
	return _T("CCEFTempWnd");
}
