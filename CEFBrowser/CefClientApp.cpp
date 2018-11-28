// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#include "stdafx.h"
#include "CefClientApp.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "CefClientHandler.h"


namespace {

	// When using the Views framework this object provides the delegate
	// implementation for the CefWindow that hosts the Views-based browser.
	class SimpleWindowDelegate : public CefWindowDelegate
	{
	public:
		explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
			: browser_view_(browser_view) {}

		void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE
		{
			// Add the browser view and show the window.
			window->AddChildView(browser_view_);
			window->Show();

			// Give keyboard focus to the browser view.
			browser_view_->RequestFocus();
		}

		void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE
		{
			browser_view_ = NULL;
		}

		bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE
		{
			// Allow the window to close if the browser says it's OK.
			CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
			if (browser)
				return browser->GetHost()->TryCloseBrowser();
			return true;
		}

	private:
		CefRefPtr<CefBrowserView> browser_view_;

		IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
		DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
	};

}  // namespace

CCefClientBrowserApp::CCefClientBrowserApp() {}

void CCefClientBrowserApp::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();
}

CCefClientRendererApp::CCefClientRendererApp()
{

}

void CCefClientRendererApp::OnWebKitInitialized()
{

}

void CCefClientRendererApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	CefRefPtr<CefV8Value> object = context->GetGlobal();
}

void CCefClientRendererApp::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{

}

bool CCefClientRendererApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	return true;
}
