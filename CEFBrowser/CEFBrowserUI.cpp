#include "stdafx.h"
#include "CEFBrowserUI.h"
#include "CefClientHandler.h"
#include "CEFTempWnd.h"
#include "BrowserManage.h"
#include "CefIconDown.h"

CCEFBrowserWnd::CCEFBrowserWnd()
{
	m_pOwner = NULL;
	m_bWndClose = false;
}

CCEFBrowserWnd::~CCEFBrowserWnd()
{	
	m_pOwner = NULL;
}

void CCEFBrowserWnd::Init(CCEFBrowserUI* pOwner)
{
	m_pOwner = pOwner;

	if (m_hWnd == NULL)
	{
		RECT rc = m_pOwner->GetPos();
		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_CHILD, 0, rc);
	}
}

LPCTSTR CCEFBrowserWnd::GetWindowClassName() const
{
	return _T("CCEFBrowserWnd");
}

void CCEFBrowserWnd::OnFinalMessage(HWND hWnd)
{
	delete this;
}

LRESULT CCEFBrowserWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		bHandled = FALSE;
		m_bWndClose = true;
		break;
	}
	case WM_DESTROY:
		bHandled = FALSE;
		break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	lRes = HandleBrowserMessage(uMsg, wParam, lParam, bHandled);
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CCEFBrowserWnd::HandleBrowserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	if (uMsg >= CEF_ON_LOAD_START && uMsg <= CEF_DO_CLOSE)
	{
		if (!m_bWndClose)
		{
			CDuiString pBrowserUIName = m_pOwner->GetName();

			switch (uMsg)
			{
			case CEF_ON_LOAD_START:
				break;
			case CEF_ON_LOAD_END:
				break;
			case CEF_ON_TITLE_CHANGE:
				break;
			case CEF_ON_ICON_CHANGE:{
				CefString* icon_url = (CefString *)(lParam);
				this->m_pOwner->m_handler->GetBrowser()->GetHost()->DownloadImage(*icon_url, false, 0, false, m_pOwner->m_pIconDownCallBack);

				delete icon_url;
				icon_url = nullptr;
				break;
			}
			case CEF_ON_BEFORE_POPUP:
				break;
			case CEF_ON_AFETER_CREATE:
				this->m_pOwner->SetPos(m_pOwner->GetPos());
				break;
			case CEF_DO_CLOSE:
				break;
			case CEF_ON_BEFORE_CLOSE:
				break;
			default:
				break;
			}

			CBrowserManage::GetInstance()->HandleBrowserMessage(uMsg, wParam, lParam, pBrowserUIName);
		}

	}
	return 0;
}

CCEFBrowserUI::CCEFBrowserUI()
{
	m_pBrowserWnd = NULL;
	m_handler = NULL;
	m_sURL = _T("www.websec.site");
	m_sTitle = _T("");
}

CCEFBrowserUI::~CCEFBrowserUI()
{
	if (m_pBrowserWnd != NULL)
	{
		m_pBrowserWnd->Close();
	}
}

LPCTSTR CCEFBrowserUI::GetClass() const
{
	return _T("CEF3BrowserUI");
}

LPVOID CCEFBrowserUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("CEF3Browser")) == 0) return static_cast<CCEFBrowserUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CCEFBrowserUI::DoInit()
{
	m_pBrowserWnd = new CCEFBrowserWnd();
	if (m_pBrowserWnd)
	{
		m_pBrowserWnd->Init(this);
		m_pBrowserWnd->ShowWindow(true);
	}

	m_handler = new CCefClientHandler();
	m_handler->SetParentWndHwnd(m_pBrowserWnd->GetHWND());

	wstring url(m_sURL.GetData());
	CefString cef_str(url);
	cef_str = url;
	cef_str.FromWString(cef_str);

	NewBrowserPage(cef_str);


	m_pIconDownCallBack = new CCefIconDownCallBack(this);
}

void CCEFBrowserUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("URL")) == 0)
	{
		m_sURL = pstrValue;
	}
	__super::SetAttribute(pstrName, pstrValue);
}

void CCEFBrowserUI::SetPos(RECT rc)
{
	__super::SetPos(rc);
	
	if (m_pBrowserWnd != NULL)
	{
		::SetWindowPos(m_pBrowserWnd->GetHWND(), NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);

		if (m_handler->GetBrowser() != NULL)
			::MoveWindow(m_handler->GetBrowser()->GetHost()->GetWindowHandle(), 0, 0, rc.right - rc.left, rc.bottom - rc.top, true);
	}
}

void CCEFBrowserUI::SetVisible(bool bVisible)
{
	__super::SetVisible(bVisible);

	SetInternVisible(bVisible);
}

void CCEFBrowserUI::SetInternVisible(bool bVisible /*= true*/)
{
	if (m_pBrowserWnd != NULL)
	{
		::ShowWindow(m_pBrowserWnd->GetHWND(), bVisible ? SW_SHOW : SW_HIDE);
		if (m_handler->GetBrowser() != NULL)
		{
			::ShowWindow(m_handler->GetBrowser()->GetHost()->GetWindowHandle(), bVisible ? SW_SHOW : SW_HIDE);
		}
	}
	__super::SetInternVisible(bVisible);

}

void CCEFBrowserUI::CaclBrowserPos(RECT &rc)
{
	rc.right = rc.right - rc.left;
	rc.bottom = rc.bottom - rc.top;
	rc.left = 0;
	rc.top = 0;
}

void CCEFBrowserUI::NewBrowserPage(const CefString& Url)
{
	CefWindowInfo window_info;
	window_info.SetAsChild(m_pBrowserWnd->GetHWND(), this->GetPos());

	CefBrowserSettings browser_settings;

	BOOL i = CefBrowserHost::CreateBrowser(window_info, m_handler, Url, browser_settings, NULL);
}

void CCEFBrowserUI::DelBrowserPage()
{
	CefRefPtr<CefBrowser> browser = m_handler->GetBrowser();
	CCEFTempWnd* tempWnd = new CCEFTempWnd();
	SetParent(browser->GetHost()->GetWindowHandle(), tempWnd->GetHWND());

	if (m_handler.get() && !m_handler->IsClosing())
	{
		if (browser.get())
		{
			browser->GetHost()->CloseBrowser(false);
			return;
		}
	}
}

