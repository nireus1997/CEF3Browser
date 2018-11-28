#include "StdAfx.h"
#include "WindowShadow.h"

#include <memory>

WindowShadow::WindowShadow(HWND hwd)
{
	m_hParent = hwd;
}

void WindowShadow::Notify(TNotifyUI& msg)
{

}

LRESULT WindowShadow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static std::auto_ptr<MessageMap> customMessageMap(InitMessageMap());

	BOOL bHandled = TRUE;
	LRESULT lRes = 0;

	if (customMessageMap->find(uMsg) != customMessageMap->end())
	{
		CustomMsgHandler handler = (*customMessageMap)[uMsg];
		lRes = (this->*handler)(wParam, lParam, bHandled);

		if (bHandled) return lRes;
	}

	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

WindowShadow::MessageMap* WindowShadow::InitMessageMap()
{
	MessageMap* map = new MessageMap;

	(*map)[WM_CREATE] = &WindowShadow::OnCreate;
	(*map)[WM_DESTROY] = &WindowShadow::OnDestory;
	(*map)[WM_ERASEBKGND] = &WindowShadow::OnErasebkgnd;
	(*map)[WM_SIZE] = &WindowShadow::OnSize;
	(*map)[WM_NCPAINT] = &WindowShadow::OnNcPaint;
	(*map)[WM_NCACTIVATE] = &WindowShadow::OnNcActive;
	(*map)[WM_NCCALCSIZE] = &WindowShadow::OnNcCalSize;
	(*map)[WM_NCHITTEST] = &WindowShadow::OnNcHitTest;
	(*map)[WM_SYSCOMMAND] = &WindowShadow::OnSysCommand;
	(*map)[WM_GETMINMAXINFO] = &WindowShadow::OnGetMinMaxInfo;

	return map;
}

HRESULT WindowShadow::OnCreate(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("shadow.xml"), (UINT)0, NULL, &m_pm);
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	return 0;
}

HRESULT WindowShadow::OnDestory(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//::PostQuitMessage(0L);
	return 0;
}

HRESULT WindowShadow::OnErasebkgnd(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 1;
}

HRESULT WindowShadow::OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0))
	{
		RECT rcWnd;
		//::GetWindowRect(*this, &rcWnd);
		::GetWindowRect(m_hParent, &rcWnd);
		::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);
		rcWnd.right++;
		rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right,
			rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
	RECT rt = { 0 };
	::GetWindowRect(m_hParent, &rt);
	rt.bottom += 7;
	rt.left -= 5;
	rt.right += 5;
	rt.top -= 3;
	// 使用MoveWindow函数
	//char szBuffer[MAX_PATH] ={0};
	//sprintf(szBuffer,"x:%d   y:%d   w:%d   h:%d\r\n",rt.left,rt.top,rt.right,rt.bottom);
	//OutputDebugStringA(szBuffer);
	MoveWindow(GetHWND(), rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, TRUE);
	bHandled = FALSE;
	return 0;
}

HRESULT WindowShadow::OnNcPaint(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

HRESULT WindowShadow::OnNcActive(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

HRESULT WindowShadow::OnNcCalSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

HRESULT WindowShadow::OnNcHitTest(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (!::IsZoomed(*this))
	{
		RECT rcSizeBox = m_pm.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
	}

	RECT rcCaption = m_pm.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left
		&& pt.x < rcClient.right - rcCaption.right
		&& pt.y >= rcCaption.top
		&& pt.y < rcCaption.bottom)
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));

		if (pControl
			&& _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0
			&& _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0)
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT WindowShadow::OnSysCommand(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		Close();
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(WM_SYSCOMMAND, wParam, lParam);

	return 1L;
}

LRESULT WindowShadow::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	RECT rcWork = oMonitor.rcWork;
	::OffsetRect(&rcWork, -rcWork.left, -rcWork.top);
	//rcWork.Offset(-rcWork.left, -rcWork.top);

	/// 窗口最大化时裁剪阴影所占区域
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left - 5;
	lpMMI->ptMaxPosition.y = rcWork.top - 3;
	lpMMI->ptMaxSize.x = rcWork.right + 10;
	lpMMI->ptMaxSize.y = rcWork.bottom + 10;

	bHandled = FALSE;
	return 0;
}
