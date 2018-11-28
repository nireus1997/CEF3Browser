#pragma once
class CCefClientHandler;
class CCEFBrowserUI;
class CCefIconDownCallBack;
class CCEFBrowserWnd : public CWindowWnd
{
public:
	CCEFBrowserWnd();
	~CCEFBrowserWnd();

public:
	void Init(CCEFBrowserUI* pOwner);

public:
	LPCTSTR GetWindowClassName() const;

	void OnFinalMessage(HWND hWnd);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleBrowserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CCEFBrowserUI* m_pOwner;
	bool m_bWndClose;
};

class CCEFBrowserUI : public CControlUI
{
	friend class CCEFBrowserWnd;
public:
	CCEFBrowserUI();
	~CCEFBrowserUI();

public:
	LPCTSTR	GetClass() const override;
	LPVOID	GetInterface(LPCTSTR pstrName) override;

	virtual void DoInit() override;
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	virtual void SetPos(RECT rc) override;
	void SetVisible(bool bVisible) override;
	void SetInternVisible(bool bVisible = true) override;

public:
	void CaclBrowserPos(RECT &rc);
	void NewBrowserPage(const CefString& Url);
	void DelBrowserPage();
public:
	CefRefPtr<CCefIconDownCallBack> m_pIconDownCallBack;
public:
	CDuiString GetBrowserTitle()	{return m_sTitle;}
	void SetBrowserTitle(CDuiString sTitle)	{ m_sTitle = sTitle; }

	CCEFBrowserWnd* m_pBrowserWnd;
	CefRefPtr<CCefClientHandler> m_handler;
	
	CDuiString m_sURL;
	CDuiString m_sTitle;
private:
	CControlUI* m_pIcon = nullptr;
public:
	void SetIcon(CControlUI* pControl) { m_pIcon = pControl; }
	CControlUI* GetIcon() { return m_pIcon; }
};