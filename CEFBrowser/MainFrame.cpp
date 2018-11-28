#include "stdafx.h"
#include "MainFrame.h"
#include "Resource.h"
#include "CEFBrowserUI.h"
#include "BrowserManage.h"
#include "EventManage.h"

#define WMU_NOTIFY WM_USER+1

CMainFrame::CMainFrame()
{
	m_pEventManage = new CEventManage();
}

CMainFrame::~CMainFrame()
{
}

DuiLib::CDuiString CMainFrame::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("Res\\");
}

DuiLib::CDuiString CMainFrame::GetSkinFile()
{
	return _T("");
}

LPCTSTR CMainFrame::GetWindowClassName(void) const
{
	return _T("MainFrame");
}

void CMainFrame::InitWindow()
{
	SetIcon(IDI_ICON2);
	m_pEventManage->Init(&m_PaintManager);
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	delete this;
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	bool bHandle = FALSE;

	if (msg.sType != DUI_MSGTYPE_KILLFOCUS || msg.sType != DUI_MSGTYPE_SETFOCUS)
	{
		m_pEventManage->Notify(msg);
	}


	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("Btn_Setting"))
		{
			CPoint point(0, 0);
			GetCursorPos(&point);

			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menu.xml"), point, &m_PaintManager, &m_MenuCheckInfo);

			// 先获取到根项，然后就可以使用rootMenu插到到菜单内的任意子菜单项，然后做添加删除操作
			CMenuUI* rootMenu = pMenu->GetMenuUI();
			if (rootMenu != NULL)
			{
				CMenuElementUI* pNew = new CMenuElementUI;
				pNew->SetName(_T("Menu_Dynamic"));
				pNew->SetText(_T("动态一级菜单"));


				CMenuElementUI* pSubNew = new CMenuElementUI;
				pSubNew->SetText(_T("动态二级菜单"));
				pSubNew->SetName(_T("Menu_Dynamic"));


				rootMenu->Add(pNew);


				CMenuElementUI* pNew2 = new CMenuElementUI;
				pNew2->SetName(_T("Menu_Dynamic"));
				pNew2->SetText(_T("动态一级菜单2"));
				rootMenu->AddAt(pNew2, 2);
			}

			// 动态添加后重新设置菜单的大小
			pMenu->ResizeMenu();
		}
	}

	__super::Notify(msg);
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:
		lRes = OnCreate(uMsg, wParam, lParam, bHandled);
		break;
	case WM_CLOSE:
		lRes = OnClose(uMsg, wParam, lParam, bHandled);
		break;
	case WM_DESTROY:
		lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCACTIVATE:
		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCCALCSIZE:
		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCPAINT:
		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCHITTEST:
		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SIZE:
		lRes = OnSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_GETMINMAXINFO:
		lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SYSCOMMAND:
		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled);
		break;
	case WM_MOVE:
		lRes = OnMove(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SHOWWINDOW:
		lRes = OnVisibleChanged(uMsg, wParam, lParam, bHandled);
		break;
	case WMU_NOTIFY:
		lRes = OnTrayIcon(uMsg, wParam, lParam, bHandled);
		break;
	case WM_KEYDOWN:		
		lRes = OnKeyDown(uMsg, wParam, lParam, bHandled);
		break;
	case WM_COMMAND:
		lRes = OnMenuMessage(uMsg, wParam, lParam, bHandled);
		bHandled = FALSE;
		break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;

	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lRes;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::OnClose(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	::ShowWindow(m_hWnd, SW_HIDE);
	bHandled = TRUE;

	return 0;
}

LRESULT CMainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);


	m_PaintManager.Init(m_hWnd);

	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(MAIN_XML, (UINT)0, this, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");

	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	InitWindow();
	if (m_PaintManager.HasWindowShadow())
	{
		m_wShadow = new WindowShadow(m_hWnd);
		if (m_wShadow)
		{
			m_wShadow->Create(m_hWnd, _T("shadow"), WS_POPUP | WS_SYSMENU, 0 ^ WS_EX_NOACTIVATE ^ WS_EX_TOOLWINDOW);
			m_wShadow->ShowWindow(true, false);
		}
	}


	HINSTANCE  hInstance = (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE);
	InstallIcon(hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2)), IDI_ICON2);
	return 0;
}

LRESULT CMainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0))
	{
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++;
		rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	if (m_wShadow != NULL && m_PaintManager.HasWindowShadow())
	{
		::SendMessage(m_wShadow->GetHWND(), WM_SIZE, wParam, lParam);
	}


	bHandled = FALSE;

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE)
	{
		::ShowWindow(m_hWnd, SW_HIDE);
		//::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT CMainFrame::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_PaintManager.HasWindowShadow() && m_wShadow != NULL && ::IsWindow(m_wShadow->GetHWND()))
	{
		RECT rcWnd = { 0 };
		::GetWindowRect(m_hWnd, &rcWnd);
		rcWnd.bottom += 7;
		rcWnd.left -= 5;
		rcWnd.right += 5;
		rcWnd.top -= 3;
		::SetWindowPos(*m_wShadow, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top,
			SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
	}
	return 0;
}

LRESULT CMainFrame::OnVisibleChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_wShadow != NULL && m_PaintManager.HasWindowShadow())
	{
		::ShowWindow(m_wShadow->GetHWND(), wParam);
	}
	return 0;
}

DuiLib::CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("CEF3Browser")) == 0)
	{
		return new CCEFBrowserUI();
	}
	return NULL;
}

bool CMainFrame::InstallIcon(HINSTANCE hInstance, HICON hIcon, UINT nID)
{
	m_NotifyMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_CEFBROWSER));
	m_NotifyMenu = GetSubMenu(m_NotifyMenu, 0);

	m_nid.hWnd = this->GetHWND();
	m_nid.uID = nID;
	m_nid.hIcon = hIcon;
	m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_STATE;
	m_nid.uCallbackMessage = WMU_NOTIFY;
	m_nid.dwStateMask = NIS_HIDDEN;
	wcscpy_s(m_nid.szTip, FRAME_TITLE);
	// Install 
	bool bRet = Shell_NotifyIcon(NIM_ADD, &m_nid) ? true : false;
	// Done 
	return bRet;
}

bool CMainFrame::UnInstallIcon()
{
	bool bRet = Shell_NotifyIcon(NIM_DELETE, &m_nid) ? true : false;
	return bRet;
}

LRESULT CMainFrame::OnTrayIcon(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (wParam != m_nid.uID)
	{
		return 0;
	}
	POINT point;

	if (lParam == WM_RBUTTONDOWN)//托盘消息中lParam接受鼠标的行为
	{
		GetCursorPos(&point);
		SetForegroundWindow(GetHWND());
		TrackPopupMenu(m_NotifyMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, GetHWND(), NULL);
	}
	else if (lParam == WM_LBUTTONDOWN)
	{
		//TO DO
		::ShowWindow(*this, true);
	}
	return 0;
}

LRESULT CMainFrame::OnMenuMessage(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	switch (LOWORD(wParam))
	{
	case IDM_EXIT:
		CheckIfAllBrowserClosed();
		UnInstallIcon();
		::PostQuitMessage(0L);
		break;
	default:
		break;
	}
	return 0;
}

bool CMainFrame::CheckIfAllBrowserClosed()
{
	CBrowserManage* ins = CBrowserManage::GetInstance();
	if (!ins->IsAllBrowserClosed())
	{
		ins->CloseAllBrowsers(false);
	}

	while (!ins->IsAllBrowserClosed())
	{
		Sleep(10);
	}
	return true;
}

