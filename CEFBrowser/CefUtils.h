#pragma once
#include "stdafx.h"
enum ProcessType {
	PROCESS_TYPE_BROWSER,
	PROCESS_TYPE_RENDERER,
	PROCESS_TYPE_OTHER,
};
const char kProcessType[] = "type";
const char kRendererProcess[] = "renderer";


CefRefPtr<CefCommandLine> CreateCommandLine(const CefMainArgs& main_args);
ProcessType GetProcessType(const CefRefPtr<CefCommandLine>& command_line);
CefString Dui2Cef(CDuiString str);
CDuiString Cef2Dui(CefString str);