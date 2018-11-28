#pragma once

#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"

class CCefClientHandler : public CefClient, public CefLifeSpanHandler, public CefLoadHandler, public CefDragHandler, public CefContextMenuHandler, public CefRequestHandler, public CefDisplayHandler
{
public:
	CCefClientHandler();
	~CCefClientHandler();

public:
	// CefClient methods.
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE{ return this; }
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

	// CefLifeSpanHandler methods  virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings, bool* no_javascript_access)  OVERRIDE;

	void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefDisplayHandler methods:
	void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;
	virtual void OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls) OVERRIDE;

	//CefLifeSpanHandler methods
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) OVERRIDE;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) OVERRIDE;

	//CefDragHandler methods
	virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) OVERRIDE{ return true; }

	//CefContextMenuHandler methods
	virtual bool RunContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback) OVERRIDE{ return true; }

	// CefRequestHandler methods:
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect) OVERRIDE;
	//virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) OVERRIDE;
	virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) OVERRIDE;

	// Member accessors.
	CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }
	void SetParentWndHwnd(HWND hwnd) { hWnd_ = hwnd; }
	int GetBrowserID() { return m_BrowserId; }
	bool IsClosing() { return m_bIsClosing; }

private:
	CefRefPtr<CefBrowser> m_Browser;
	int m_BrowserId;
	int m_BrowserCount;
	bool m_bIsClosing;

	HWND hWnd_;	

	IMPLEMENT_REFCOUNTING(CCefClientHandler);
};