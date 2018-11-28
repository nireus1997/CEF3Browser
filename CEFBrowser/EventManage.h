#pragma once

class CCEFBrowserUI;

class CEventManage
{
public:
	CEventManage();
	~CEventManage();
private:
	CHorizontalLayoutUI* m_pHLayoutTitleBar;
	CTabLayoutUI *m_pTabBrowser;	
	CButtonUI* m_pBtnNewLabel;

	CButtonUI* m_pBtnGoBack;
	CButtonUI* m_pBtnGoForward;
	CButtonUI* m_pBtnReload;

	CEditUI* m_pEditUrl;


public:
	void Init(CPaintManagerUI* pPaintManager);
	void Notify(TNotifyUI& msg);

public:
	void HandleBrowserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:	
	void NewTabBrowserPage(CefString& targetURL);			
	void NewBrowserLabel(int nBrowserID);					
	void DelTabBrowserPage(int nBrowserID);					
	void DelBrowserLabel(CButtonUI* pCloseBtn);				
	void HideLoginSuccessBrowser(int nBrowserID);			
public:
	void SetTitle(int nBrowserID, CDuiString sTitle);

	CCEFBrowserUI* GetSpecifiedBrowser(int nBrowserID);
	CefRefPtr<CefBrowser> GetCurSelBrowser();
public:
	void OnClickBtnNewLabel();
public:
	void UpdataGoBackorForwardStatus();
	void OnClickBtnGoBack();
	void OnClickBtnGoForward();
	void OnClickBtnReload();
	void OnEnterKeyDown();
	void UpdataUrlAddress();

	class COptionTag
	{
	public:
		int m_nBrowserID;
		int m_nIndexID;
		COptionTag::COptionTag(int nBrowserID, int nIndexID) 
		:m_nBrowserID(nBrowserID), m_nIndexID(nIndexID)
		{

		};
	};
};