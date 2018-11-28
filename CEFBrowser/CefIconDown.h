#pragma once
/*
	11/28

	只支持png。。。。。svg不行

*/

class CCEFBrowserUI;
class CCefIconDownCallBack : public CefDownloadImageCallback
{
public:
	CCefIconDownCallBack(CCEFBrowserUI* pBrowserUI)
	{
		m_pBrowserUI = pBrowserUI;
	}
	virtual void OnDownloadImageFinished(const CefString& image_url,
	int http_status_code,
	CefRefPtr<CefImage> image) OVERRIDE;

private:
	CCEFBrowserUI* m_pBrowserUI;
	IMPLEMENT_REFCOUNTING(CCefIconDownCallBack);
};