#pragma once
#include <mutex>  
#include<list>
using namespace std;
class CEventManage;
class CBrowserManage
{
protected:
	CBrowserManage(){}

private:
	static CBrowserManage* m_pInstance;
public:
	static CBrowserManage* GetInstance();

private:
	typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
	BrowserList browser_list_;

	mutex m_sLock;

private:
	CEventManage* m_pSearchPage = nullptr;

public:
	void SetSearchPage(CEventManage* pSearchPage) { m_pSearchPage = pSearchPage; }
public:
	void AddBrowser(CefRefPtr<CefBrowser> browser);
	void DelBrowser(CefRefPtr<CefBrowser> browser);
	void CloseAllBrowsers(bool force_close);
	bool IsAllBrowserClosed();
public:
	void HandleBrowserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, CDuiString pBrowserUIName);
};
