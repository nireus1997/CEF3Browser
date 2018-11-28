#include "stdafx.h"
#include "BrowserManage.h"
#include "CEFTempWnd.h"
#include "EventManage.h"
CBrowserManage* CBrowserManage::m_pInstance = new CBrowserManage();
CBrowserManage* CBrowserManage::GetInstance()
{
	return m_pInstance;
}

void CBrowserManage::AddBrowser(CefRefPtr<CefBrowser> browser)
{
	m_sLock.lock();

	browser_list_.push_back(browser);

	m_sLock.unlock();
}

void CBrowserManage::DelBrowser(CefRefPtr<CefBrowser> browser)
{
	m_sLock.lock();

	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		if ((*bit)->IsSame(browser)) {
			browser_list_.erase(bit);
			break;
		}
	}

	m_sLock.unlock();
}

void CBrowserManage::CloseAllBrowsers(bool force_close)
{
	if (browser_list_.empty())
		return;

	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
	{
		CCEFTempWnd* tempWnd = new CCEFTempWnd();
		tempWnd->Init();
		SetParent((*it)->GetHost()->GetWindowHandle(), tempWnd->GetHWND());
		(*it)->GetHost()->CloseBrowser(force_close);
	}
}

bool CBrowserManage::IsAllBrowserClosed()
{
	return (browser_list_.empty()) ? true : false;
}

void CBrowserManage::HandleBrowserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, CDuiString pBrowserUIName)
{
	if (pBrowserUIName.Find(COMMON_NAME_BROWSER_SEARCH) != -1)
	{
		m_pSearchPage->HandleBrowserMessage(uMsg, wParam, lParam);
	}
}
