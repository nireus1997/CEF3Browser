#pragma once

// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "include/cef_app.h"
#include "include/wrapper/cef_message_router.h"

// Implement application-level callbacks for the browser process.
class CCefClientBrowserApp : public CefApp, public CefBrowserProcessHandler
{
public:
	CCefClientBrowserApp();

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()	OVERRIDE{ return this; }

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized
		() OVERRIDE;


private:

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CCefClientBrowserApp);
	DISALLOW_COPY_AND_ASSIGN(CCefClientBrowserApp);

};



class CCefClientRendererApp : public CefApp, public CefRenderProcessHandler
{
public:
	CCefClientRendererApp();

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() 	OVERRIDE{ return this; }


	//CefRenderProcessHandler methods
	virtual void OnWebKitInitialized() OVERRIDE;
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
	IMPLEMENT_REFCOUNTING(CCefClientRendererApp);
	DISALLOW_COPY_AND_ASSIGN(CCefClientRendererApp);
};