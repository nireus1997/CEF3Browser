#include "stdafx.h"
#include "CefClientHandler.h"

#include "include/wrapper/cef_helpers.h"
#include "BrowserManage.h"

// Handle messages in the browser process.
class MessageHandler : public CefMessageRouterBrowserSide::Handler {
public:
	explicit MessageHandler(HWND hwnd) { m_hMainHwnd = hwnd; }

	// Called due to cefQuery execution in message_router.html.
	bool OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id, const CefString& request, bool persistent, CefRefPtr<Callback> callback) OVERRIDE
	{
		// Only handle messages from the startup URL.
		const std::string& url = frame->GetURL();

		const std::string& message_name = request;
		if (message_name.find("AddPerson") == 0)
		{
			//// Reverse the string and return.
			//std::string result = message_name.substr(sizeof("MessageRouterTest"));
			//std::reverse(result.begin(), result.end());

			::PostMessage(m_hMainHwnd, SOME_JS_CALL_BACK_MSG, 0, 0);
			//browser->GetHost()->
			callback->Success(_T(""));
			return true;
		}

		return false;
	}

private:
	HWND m_hMainHwnd;
	DISALLOW_COPY_AND_ASSIGN(MessageHandler);
};

CCefClientHandler::CCefClientHandler()
{
	m_Browser = NULL;
	m_BrowserId = 0;
	m_BrowserCount = 0;
	m_bIsClosing = false;
	hWnd_ = NULL;
}

CCefClientHandler::~CCefClientHandler()
{

}


bool CCefClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	return true;
}

bool CCefClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access)
{
	CEF_REQUIRE_UI_THREAD();

	CefString* strTargetURL = new CefString(target_url);
	CefString url = "www.websec.site";
	if (url != target_url)
		::PostMessage(hWnd_, CEF_ON_BEFORE_POPUP, (WPARAM)0, (LPARAM)strTargetURL);

	//不允许创建弹出窗口
	return true;
}

void CCefClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (!m_Browser.get())   
	{
		m_Browser = browser;
		m_BrowserId = browser->GetIdentifier();
	}

	CBrowserManage::GetInstance()->AddBrowser(browser);
	::PostMessage(hWnd_, CEF_ON_AFETER_CREATE, m_BrowserId, 0);
	m_BrowserCount++;
}

bool CCefClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_BrowserId == browser->GetIdentifier())
	{
		m_bIsClosing = true;
	}

	CBrowserManage::GetInstance()->DelBrowser(browser);
	return false;
}

void CCefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_BrowserId == browser->GetIdentifier())
	{
		m_Browser = NULL;
	}

	CBrowserManage::GetInstance()->DelBrowser(browser);
	::PostMessage(hWnd_, CEF_ON_BEFORE_CLOSE, m_BrowserId, 0);
}

void CCefClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	CEF_REQUIRE_UI_THREAD();

	CefString* strTitle = new CefString(title);
	::PostMessage(hWnd_, CEF_ON_TITLE_CHANGE, m_BrowserId, (LPARAM)strTitle);
}

void CCefClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
{
	CEF_REQUIRE_UI_THREAD();

	CefString* strAddress = new CefString(url);
	::PostMessage(hWnd_, CEF_ON_URL_CHANGE, m_BrowserId, (LPARAM)strAddress);
}

void CCefClientHandler::OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls)
{	
	if (icon_urls.size() == 0) return;

	CefString* icon_url = new CefString(icon_urls.at(0));

	::PostMessage(hWnd_, CEF_ON_ICON_CHANGE, m_BrowserId, (LPARAM)icon_url);
}

void CCefClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type)
{
	CEF_REQUIRE_UI_THREAD();
	::PostMessage(hWnd_, CEF_ON_LOAD_START, m_BrowserId, 0);
}

void CCefClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	CEF_REQUIRE_UI_THREAD();

	::PostMessage(hWnd_, CEF_ON_LOAD_END, m_BrowserId, 0); 
}

void CCefClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED)
		return;

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</h2></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

bool CCefClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
{
	return false;
}

void CCefClientHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status)
{
}
